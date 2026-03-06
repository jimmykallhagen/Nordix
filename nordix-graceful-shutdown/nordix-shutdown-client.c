/*===================================================================*
 * SPDX-License-Identifier: GPL-3.0-or-later                         *
 * Copyright (c) 2025 Jimmy Källhagen                                *
 * Part of Yggdrasil - Nordix desktop environment                    *
 * Nordix and Yggdrasil are registered trademarks of Jimmy Källhagen *
 *===================================================================*/

/*
 * nordix-shutdown-client.c
 *
 * Lightweight client that sends a shutdown or reboot request to
 * the nordix-graceful-shutdown daemon via its Unix socket.
 *
 * Forwards the username and display environment so the daemon can
 * show popup dialogs on the user's screen.
 *
 * Usage:
 *   nordix-shutdown-client --poweroff
 *   nordix-shutdown-client --reboot
 *
 * Build:
 *   gcc -O2 -Wall -o nordix-shutdown-client nordix-shutdown-client.c
 *
 * Install:
 *   cp nordix-shutdown-client /usr/bin/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/run/nordix-graceful-shutdown.sock"

static void usage(const char *prog)
{
    fprintf(stderr, "Usage: %s [--poweroff | --reboot]\n", prog);
    fprintf(stderr, "  --poweroff   Shut down the system (default)\n");
    fprintf(stderr, "  --reboot     Reboot the system\n");
}

int main(int argc, char *argv[])
{
    const char *action = "poweroff";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--reboot") == 0)
            action = "reboot";
        else if (strcmp(argv[i], "--poweroff") == 0)
            action = "poweroff";
        else {
            usage(argv[0]);
            return 1;
        }
    }

    /* gather environment for popup display */
    const char *user    = getenv("USER");
    const char *display = getenv("DISPLAY");
    const char *xauth   = getenv("XAUTHORITY");
    const char *wl_disp = getenv("WAYLAND_DISPLAY");

    /* use "-" as placeholder for missing values */
    if (!user    || !*user)    user    = "-";
    if (!display || !*display) display = "-";
    if (!xauth   || !*xauth)  xauth   = "-";
    if (!wl_disp || !*wl_disp) wl_disp = "-";

    /* build message */
    char msg[1024];
    snprintf(msg, sizeof(msg), "%s %s %s %s %s",
             action, user, display, xauth, wl_disp);

    /* connect to daemon socket */
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "Could not connect to nordix-graceful-shutdown daemon.\n");
        fprintf(stderr, "Is the service running?  Try: systemctl start nordix-graceful-shutdown.socket\n");
        close(fd);
        return 1;
    }

    /* send command */
    write(fd, msg, strlen(msg));

    /* read response */
    char reply[64] = {0};
    ssize_t n = read(fd, reply, sizeof(reply) - 1);
    close(fd);

    if (n > 0) {
        reply[strcspn(reply, "\r\n")] = '\0';
        if (strcmp(reply, "ok") == 0) {
            printf("Nordix graceful %s initiated.\n", action);
            return 0;
        } else if (strcmp(reply, "error") == 0) {
            fprintf(stderr, "Daemon rejected the request.\n");
            return 1;
        }
    }

    printf("Request sent.\n");
    return 0;
}
