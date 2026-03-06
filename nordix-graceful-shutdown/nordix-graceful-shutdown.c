/*===================================================================*
 * SPDX-License-Identifier: GPL-3.0-or-later                         *
 * Copyright (c) 2025 Jimmy Källhagen                                *
 * Part of Yggdrasil - Nordix desktop environment                    *
 * Nordix and Yggdrasil are registered trademarks of Jimmy Källhagen *
 *===================================================================*/

/*
 * nordix-graceful-shutdown.c
 *
 * Daemon for Nordix – an Arch Linux distro with ZFS on root and
 * aggressive cache settings (Linux page cache + ZFS ARC).
 *
 * Runs as a systemd socket-activated service (root).  Listens on a
 * Unix socket for commands from the unprivileged client:
 *
 *   "poweroff"  – graceful shutdown + power off
 *   "reboot"    – graceful shutdown + reboot
 *
 * The daemon performs the full shutdown sequence and survives user
 * logout because it is a system service.
 *
 * Shutdown sequence:
 *   1.  sync + zpool sync           (flush RAM → disk)
 *   2.  Check scrub/resilver/trim   (popup: wait / pause / cancel)
 *   3.  Check running VMs           (popup: continue / cancel)
 *   4.  Check active downloads      (popup: wait / ignore / cancel)
 *   5.  Stop important services     (docker, podman, databases, etc.)
 *   6.  Stop AppImages + unmount FUSE
 *   7.  sync + zpool sync again
 *   8.  Logout user
 *   9.  Unmount steam children
 *  10.  Unmount .local children
 *  11.  Unmount home children
 *  12.  Unmount root children
 *  13.  zfs umount -af              (force-unmount any stragglers)
 *  14.  poweroff / reboot
 *
 * Build:
 *   gcc -O2 -Wall -o nordix-graceful-shutdown nordix-graceful-shutdown.c
 *
 * Install:
 *   cp nordix-graceful-shutdown /usr/lib/nordix/
 *   cp nordix-graceful-shutdown.socket /etc/systemd/system/
 *   cp nordix-graceful-shutdown.service /etc/systemd/system/
 *   systemctl daemon-reload
 *   systemctl enable nordix-graceful-shutdown.socket
 *   systemctl start  nordix-graceful-shutdown.socket
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <errno.h>

/* ── paths to the popup helper scripts ── */
#define POPUP_SCRUB_TRIM   "/usr/lib/nordix/nordix-shutdown-scrub.py"
#define POPUP_VM           "/usr/lib/nordix/nordix-shutdown-vm.py"
#define POPUP_DOWNLOAD     "/usr/lib/nordix/nordix-shutdown-download.py"

/* socket path – must match the .socket unit */
#define SOCKET_PATH        "/run/nordix-graceful-shutdown.sock"

/* connection speed in Mbit/s – change to match the system */
#define CONNECTION_MBIT    100.0
#define DOWNLOAD_THRESHOLD 0.40   /* 40 % of connection speed */

#define MAX_CMD 1024

/* ─────────────────────── helpers ─────────────────────── */

static void run(const char *cmd)
{
    (void)system(cmd);
}

static void sleep_ms(long ms)
{
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };
    nanosleep(&ts, NULL);
}

static void sleep_sec(int s)
{
    struct timespec ts = { s, 0 };
    nanosleep(&ts, NULL);
}

/*
 * Run a popup script.  Returns 0 to continue shutdown, 1 to abort.
 *
 * DISPLAY and XAUTHORITY / WAYLAND_DISPLAY are forwarded from the
 * client so that the GUI popup appears on the user's screen.
 */
static int run_popup(const char *script)
{
    char cmd[MAX_CMD];
    snprintf(cmd, sizeof(cmd), "/usr/bin/python3 %s", script);
    int rc = system(cmd);
    if (rc == -1)
        return 0;   /* popup failed to launch – continue shutdown */
    return WIFEXITED(rc) ? WEXITSTATUS(rc) : 0;
}

/* ─────────── 1 & 7: sync page cache + ZFS ARC ─────────── */

static int sync_all(void)
{
    printf("[nordix-shutdown] syncing linux page cache...\n");
    sync();

    printf("[nordix-shutdown] syncing ZFS ARC...\n");
    FILE *fp = popen("zpool list -H -o name 2>/dev/null", "r");
    if (!fp) {
        perror("popen zpool list");
        return 1;
    }

    char pool[256];
    int ret = 0;
    while (fgets(pool, sizeof(pool), fp)) {
        pool[strcspn(pool, "\n")] = '\0';
        if (pool[0] == '\0')
            continue;

        char cmd[512];
        snprintf(cmd, sizeof(cmd), "zpool sync %s", pool);
        printf("  syncing pool: %s\n", pool);
        fflush(stdout);

        if (system(cmd) != 0) {
            fprintf(stderr, "  [WARN] zpool sync failed for: %s\n", pool);
            ret = 1;
        }
    }
    pclose(fp);
    return ret;
}

/* ─────────── 2: scrub / resilver / trim check ─────────── */

typedef enum {
    ZSTATE_NONE,
    ZSTATE_SCRUB,
    ZSTATE_RESILVER,
    ZSTATE_TRIM
} ZpoolState;

static ZpoolState check_zpool_activity(void)
{
    FILE *fp = popen("zpool status 2>/dev/null", "r");
    if (!fp)
        return ZSTATE_NONE;

    char line[512];
    ZpoolState state = ZSTATE_NONE;

    while (fgets(line, sizeof(line), fp)) {
        char *p = line;
        while (*p == ' ' || *p == '\t')
            p++;

        if (strncmp(p, "scan:", 5) == 0) {
            if (strstr(p, "scrub in progress"))
                state = ZSTATE_SCRUB;
            else if (strstr(p, "resilver in progress"))
                state = ZSTATE_RESILVER;
        } else if (strncmp(p, "trim:", 5) == 0 || strncmp(p, "action:", 7) == 0) {
            if (strstr(p, "in progress") || strstr(p, "TRIM in progress"))
                state = ZSTATE_TRIM;
        }
    }
    pclose(fp);
    return state;
}

static int handle_scrub_trim(void)
{
    ZpoolState s = check_zpool_activity();
    if (s == ZSTATE_NONE) {
        printf("[nordix-shutdown] No scrub/resilver/trim in progress.\n");
        return 0;
    }

    const char *label = "unknown";
    switch (s) {
    case ZSTATE_SCRUB:    label = "scrub";    break;
    case ZSTATE_RESILVER: label = "resilver"; break;
    case ZSTATE_TRIM:     label = "trim";     break;
    default: break;
    }
    printf("[nordix-shutdown] ZFS %s in progress – showing popup.\n", label);

    setenv("NORDIX_ZFS_OP", label, 1);
    int rc = run_popup(POPUP_SCRUB_TRIM);
    return (rc == 1) ? 1 : 0;
}

/* ─────────── 3: VM check ─────────── */

static int any_vm_running(void)
{
    FILE *fp = popen("virsh list --state-running --name 2>/dev/null", "r");
    if (!fp)
        return 0;

    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        if (line[0] != '\0') {
            found = 1;
            break;
        }
    }
    pclose(fp);
    return found;
}

static int handle_vm_check(void)
{
    if (!any_vm_running()) {
        printf("[nordix-shutdown] No VMs running.\n");
        return 0;
    }
    printf("[nordix-shutdown] VM(s) detected – showing popup.\n");
    int rc = run_popup(POPUP_VM);
    return (rc == 1) ? 1 : 0;
}

/* ─────────── 4: download speed check ─────────── */

static const char *find_default_iface(void)
{
    static char iface[64];
    FILE *fp = popen("ip route show default 2>/dev/null | awk '{print $5; exit}'", "r");
    if (!fp)
        return NULL;
    if (!fgets(iface, sizeof(iface), fp)) {
        pclose(fp);
        return NULL;
    }
    pclose(fp);
    iface[strcspn(iface, "\n")] = '\0';
    return (iface[0] != '\0') ? iface : NULL;
}

static long long read_rx_bytes(const char *iface)
{
    char path[256];
    snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/rx_bytes", iface);
    FILE *fp = fopen(path, "r");
    if (!fp)
        return -1;
    long long b = -1;
    if (fscanf(fp, "%lld", &b) != 1)
        b = -1;
    fclose(fp);
    return b;
}

static double measure_download_mbit(const char *iface)
{
    struct timespec t1, t2;
    long long rx1 = read_rx_bytes(iface);
    if (rx1 < 0)
        return -1.0;

    clock_gettime(CLOCK_MONOTONIC, &t1);
    usleep(500000);
    clock_gettime(CLOCK_MONOTONIC, &t2);

    long long rx2 = read_rx_bytes(iface);
    if (rx2 < 0)
        return -1.0;

    double elapsed = (t2.tv_sec - t1.tv_sec) +
                     (t2.tv_nsec - t1.tv_nsec) / 1e9;
    return ((rx2 - rx1) / elapsed) * 8.0 / 1e6;
}

static int handle_download_check(void)
{
    const char *iface = find_default_iface();
    if (!iface) {
        printf("[nordix-shutdown] Could not detect network interface, skipping.\n");
        return 0;
    }

    double speed = measure_download_mbit(iface);
    double threshold = CONNECTION_MBIT * DOWNLOAD_THRESHOLD;

    if (speed < 0) {
        printf("[nordix-shutdown] Speed measurement failed, skipping.\n");
        return 0;
    }

    printf("[nordix-shutdown] Download speed: %.1f Mbit/s  (threshold: %.1f)\n",
           speed, threshold);

    if (speed <= threshold)
        return 0;

    printf("[nordix-shutdown] Active download detected – showing popup.\n");
    char env_buf[64];
    snprintf(env_buf, sizeof(env_buf), "%.1f", speed);
    setenv("NORDIX_DL_SPEED", env_buf, 1);

    int rc = run_popup(POPUP_DOWNLOAD);
    return (rc == 1) ? 1 : 0;
}

/* ─────────── 5: stop important services ─────────── */

static void stop_docker_containers(void)
{
    FILE *fp = popen("docker ps -q 2>/dev/null", "r");
    if (!fp)
        return;
    char id[128];
    while (fgets(id, sizeof(id), fp)) {
        id[strcspn(id, "\n")] = '\0';
        if (*id == '\0')
            continue;
        char cmd[192];
        snprintf(cmd, sizeof(cmd), "docker stop '%s' 2>/dev/null", id);
        run(cmd);
    }
    pclose(fp);
}

static void unmount_revokefs(void)
{
    FILE *fp = popen("findmnt -n -o TARGET,FSTYPE 2>/dev/null | grep revokefs | awk '{print $1}'", "r");
    if (!fp)
        return;
    char mnt[512];
    while (fgets(mnt, sizeof(mnt), fp)) {
        mnt[strcspn(mnt, "\n")] = '\0';
        if (*mnt == '\0')
            continue;
        char cmd[600];
        snprintf(cmd, sizeof(cmd),
                 "fusermount -u '%s' 2>/dev/null || umount -l '%s' 2>/dev/null",
                 mnt, mnt);
        run(cmd);
    }
    pclose(fp);
}

static void shutdown_services(void)
{
    printf("[nordix-shutdown] Stopping services...\n");

    run("pkill -TERM flatpak 2>/dev/null");
    sleep_ms(500);

    unmount_revokefs();
    stop_docker_containers();

    run("podman stop --all 2>/dev/null");
    run("waydroid session stop 2>/dev/null");
    run("umount -a -t nfs,nfs4 2>/dev/null");

    run("systemctl stop"
        " smb.service"
        " influxdb.service"
        " memcached.service"
        " postgresql.service"
        " mongodb.service"
        " mariadb.service"
        " keydb-sentinel.service"
        " keydb.service"
        " redis.service"
        " redis-sentinel.service"
        " nmb.service"
        " samba.service"
        " docker.service"
        " containerd.service"
        " bluetooth.service"
        " 2>/dev/null");
}

/* ─────────── 6: stop AppImages + unmount FUSE ─────────── */

static void pkill_pattern(const char *pattern, int sig)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "pgrep -f '%s' 2>/dev/null", pattern);
    FILE *fp = popen(cmd, "r");
    if (!fp)
        return;
    char pidbuf[32];
    while (fgets(pidbuf, sizeof(pidbuf), fp)) {
        pid_t pid = (pid_t)atoi(pidbuf);
        if (pid > 0)
            kill(pid, sig);
    }
    pclose(fp);
}

static void unmount_all_fuse(void)
{
    FILE *fp = popen(
        "findmnt -n -o TARGET,FSTYPE 2>/dev/null | grep fuse | grep -v fusectl | awk '{print $1}'",
        "r");
    if (!fp)
        return;
    char mnt[512];
    while (fgets(mnt, sizeof(mnt), fp)) {
        mnt[strcspn(mnt, "\n")] = '\0';
        if (*mnt == '\0')
            continue;
        char cmd[600];
        snprintf(cmd, sizeof(cmd),
                 "fusermount -u '%s' 2>/dev/null || umount -l '%s' 2>/dev/null",
                 mnt, mnt);
        run(cmd);
    }
    pclose(fp);
}

static void shutdown_appimages_fuse(void)
{
    printf("[nordix-shutdown] Stopping AppImages and FUSE mounts...\n");

    pkill_pattern("/tmp/.mount_", SIGTERM);
    run("pkill -TERM gvfsd-fuse 2>/dev/null");
    run("pkill -TERM xdg-document-portal 2>/dev/null");

    sleep_sec(2);

    pkill_pattern("/tmp/.mount_", SIGKILL);
    unmount_all_fuse();
}

/* ─────────── 8: logout user ─────────── */

static void logout_user(const char *username)
{
    printf("[nordix-shutdown] Logging out user sessions...\n");

    if (username && username[0] != '\0') {
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "loginctl terminate-user %s 2>/dev/null", username);
        printf("  %s\n", cmd);
        run(cmd);
        sleep_ms(500);
    }

    /* kill any remaining sessions on seat0 */
    run("loginctl terminate-seat seat0 2>/dev/null");
    sleep_ms(200);
}

/* ─────────── 9–12: unmount ZFS datasets ─────────── */

static int zfs_umount(const char *dataset)
{
    char cmd[MAX_CMD];
    snprintf(cmd, sizeof(cmd), "zfs umount %s 2>/dev/null", dataset);
    printf("  umount: %s  ", dataset);
    fflush(stdout);

    int status = system(cmd);
    if (status == 0) {
        printf("[OK]\n");
        return 0;
    }

    /* try force */
    snprintf(cmd, sizeof(cmd), "zfs umount -f %s 2>/dev/null", dataset);
    status = system(cmd);
    if (status == 0) {
        printf("[OK forced]\n");
        return 0;
    }

    printf("[FAIL – continuing]\n");
    return 1;
}

static void umount_list(const char *label, const char **datasets)
{
    printf("[nordix-shutdown] Unmounting %s datasets...\n", label);
    int failures = 0;
    for (int i = 0; datasets[i] != NULL; i++) {
        if (zfs_umount(datasets[i]) != 0)
            failures++;
    }
    if (failures > 0)
        printf("  [WARN] %d dataset(s) could not be unmounted – continuing.\n", failures);
}

/* step 9 – steam children (deepest first) */
static const char *steam_children[] = {
    "nordix/home/local/steam/shadercache",
    "nordix/home/local/steam/proton",
    "nordix/home/local/steam/game",
    NULL
};

/* step 10 – .local children */
static const char *local_children[] = {
    "nordix/home/local/lutris",
    "nordix/home/local/steam",
    NULL
};

/* step 11 – home children */
static const char *home_children[] = {
    "nordix/home/pictures",
    "nordix/home/videos",
    "nordix/home/music",
    "nordix/home/downloads",
    "nordix/home/documents",
    "nordix/home/games",
    "nordix/home/wine-prefix",
    "nordix/home/local",
    "nordix/home/cache",
    NULL
};

/* step 12 – root children */
static const char *root_children[] = {
    "nordix/tmp",
    "nordix/opt",
    "nordix/home",
    "nordix/varlib",
    "nordix/vartmp",
    "nordix/varlog",
    "nordix/varcache",
    "nordix/vm",
    NULL
};

/* step 13 – force unmount everything that remains */
static void zfs_force_umount_all(void)
{
    printf("[nordix-shutdown] Force unmounting all remaining ZFS datasets...\n");
    run("zfs umount -af 2>/dev/null");
}

/* step 14 – power off or reboot */
static void final_action(int reboot)
{
    if (reboot) {
        printf("[nordix-shutdown] Rebooting...\n");
        fflush(stdout);
        run("systemctl reboot");
    } else {
        printf("[nordix-shutdown] Powering off...\n");
        fflush(stdout);
        run("systemctl poweroff");
    }
}

/* ═══════════════ shutdown sequence ═══════════════ */

/*
 * Run the full graceful shutdown.
 * Returns 0 if shutdown proceeds, 1 if user cancelled via popup.
 */
static int do_shutdown(int reboot, const char *username,
                       const char *display, const char *xauthority,
                       const char *wayland_display)
{
    /*
     * Set display environment so popup GUIs appear on the user's
     * screen even though we are running as a system service.
     */
    if (display && *display)
        setenv("DISPLAY", display, 1);
    if (xauthority && *xauthority)
        setenv("XAUTHORITY", xauthority, 1);
    if (wayland_display && *wayland_display)
        setenv("WAYLAND_DISPLAY", wayland_display, 1);

    const char *action = reboot ? "Reboot" : "Shutdown";
    printf("╔══════════════════════════════════════════╗\n");
    printf("║   Nordix Graceful %-23s║\n", action);
    printf("╚══════════════════════════════════════════╝\n\n");

    /* 1. First sync */
    sync_all();

    /* 2. Scrub / resilver / trim check */
    if (handle_scrub_trim() != 0) {
        printf("[nordix-shutdown] Cancelled by user (scrub/trim).\n");
        return 1;
    }

    /* 3. VM check */
    if (handle_vm_check() != 0) {
        printf("[nordix-shutdown] Cancelled by user (VM running).\n");
        return 1;
    }

    /* 4. Download check */
    if (handle_download_check() != 0) {
        printf("[nordix-shutdown] Cancelled by user (active download).\n");
        return 1;
    }

    /* 5. Stop important services */
    shutdown_services();

    /* 6. Stop AppImages + FUSE */
    shutdown_appimages_fuse();

    /* 7. Second sync */
    sync_all();

    /* 8. Logout user */
    logout_user(username);

    /* 9–12. Unmount ZFS datasets (children first) */
    umount_list("steam", steam_children);
    umount_list(".local", local_children);
    umount_list("home", home_children);
    umount_list("root", root_children);

    /* 13. Force unmount stragglers */
    zfs_force_umount_all();

    /* 14. Power off or reboot */
    final_action(reboot);

    return 0;
}

/* ═══════════════ socket handling ═══════════════ */

/*
 * Protocol (one line from client):
 *   <action> <username> <DISPLAY> <XAUTHORITY> <WAYLAND_DISPLAY>
 *
 * action = "poweroff" or "reboot"
 * The remaining fields let us show popups on the user's display.
 * Fields may be "-" if unavailable.
 *
 * Response back to client:
 *   "ok\n"        – shutdown proceeding
 *   "cancelled\n" – user cancelled via popup
 *   "error\n"     – bad command
 */
static void handle_client(int client_fd)
{
    char buf[1024] = {0};
    ssize_t n = read(client_fd, buf, sizeof(buf) - 1);
    if (n <= 0) {
        close(client_fd);
        return;
    }
    buf[n] = '\0';

    /* strip newline */
    buf[strcspn(buf, "\r\n")] = '\0';

    /* parse fields */
    char action[32] = {0};
    char username[128] = {0};
    char display[64] = {0};
    char xauthority[256] = {0};
    char wayland_display[64] = {0};

    int fields = sscanf(buf, "%31s %127s %63s %255s %63s",
                        action, username, display, xauthority, wayland_display);

    if (fields < 1) {
        write(client_fd, "error\n", 6);
        close(client_fd);
        return;
    }

    /* treat "-" as empty */
    if (strcmp(username, "-") == 0)        username[0] = '\0';
    if (strcmp(display, "-") == 0)         display[0] = '\0';
    if (strcmp(xauthority, "-") == 0)      xauthority[0] = '\0';
    if (strcmp(wayland_display, "-") == 0) wayland_display[0] = '\0';

    int reboot;
    if (strcmp(action, "poweroff") == 0)
        reboot = 0;
    else if (strcmp(action, "reboot") == 0)
        reboot = 1;
    else {
        fprintf(stderr, "[nordix-shutdown] Unknown action: %s\n", action);
        write(client_fd, "error\n", 6);
        close(client_fd);
        return;
    }

    printf("[nordix-shutdown] Received: %s (user=%s)\n", action,
           username[0] ? username : "(none)");

    /* send immediate ack so client knows we started */
    write(client_fd, "ok\n", 3);
    close(client_fd);

    /* run the shutdown sequence (does not return on success) */
    int rc = do_shutdown(reboot, username, display, xauthority, wayland_display);
    if (rc != 0) {
        printf("[nordix-shutdown] Shutdown was cancelled. Returning to idle.\n");
    }
}

/* ═══════════════════════ main ═══════════════════════ */

int main(void)
{
    if (geteuid() != 0) {
        fprintf(stderr, "nordix-graceful-shutdown: must be run as root\n");
        return 1;
    }

    /*
     * Check for systemd socket activation.
     * If LISTEN_FDS is set, fd 3 is our pre-opened socket.
     */
    int listen_fd = -1;
    const char *listen_fds = getenv("LISTEN_FDS");

    if (listen_fds && atoi(listen_fds) >= 1) {
        /* systemd gave us fd 3 */
        listen_fd = 3;
        printf("[nordix-shutdown] Using systemd socket activation (fd 3).\n");
    } else {
        /* manual mode – create our own socket */
        listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (listen_fd < 0) {
            perror("socket");
            return 1;
        }

        struct sockaddr_un addr = {0};
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

        unlink(SOCKET_PATH);
        if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("bind");
            return 1;
        }

        /* allow all users to connect */
        chmod(SOCKET_PATH, 0666);

        if (listen(listen_fd, 2) < 0) {
            perror("listen");
            return 1;
        }

        printf("[nordix-shutdown] Listening on %s\n", SOCKET_PATH);
    }

    /* main accept loop */
    for (;;) {
        int client_fd = accept(listen_fd, NULL, NULL);
        if (client_fd < 0) {
            if (errno == EINTR)
                continue;
            perror("accept");
            continue;
        }
        handle_client(client_fd);
    }

    return 0;
}
