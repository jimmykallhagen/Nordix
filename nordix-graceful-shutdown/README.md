# Nordix Graceful Shutdown

A shutdown manager for [Nordix](https://github.com/nordix) — an Arch Linux-based distro with ZFS on root and aggressive cache tuning.

## Why this exists

Nordix ships with aggressive cache settings for both the Linux page cache and the ZFS ARC to maximize desktop performance. The trade-off is that at any given moment there is significantly more uncommitted data in RAM than on a typical system.

A standard `systemctl poweroff` can lead to data loss or corruption when:

- **ZFS datasets fail to unmount** because Docker, Podman, or AppImage FUSE mounts are still holding them open, causing systemd to hit its timeout and force-kill everything.
- **Large amounts of cached writes** never make it to disk because the sync window is too short.
- **A scrub, resilver, or trim** is interrupted mid-operation.
- **A Virtual machine** is interrupted mid-operation.

This shutdown manager solves these problems by performing a careful, ordered teardown of the entire system before handing off to `systemctl poweroff` or `systemctl reboot`.

## Architecture

```
┌──────────────────────┐         Unix socket          ┌──────────────────────────┐
│  nordix-shutdown-    │  ──── /run/nordix-graceful ──│  nordix-graceful-        │
│  client              │       -shutdown.sock         │  shutdown (daemon)       │
│  (runs as user)      │                              │  (runs as root)          │
└──────────────────────┘                              └──────────┬───────────────┘
                                                                 │
                                                   ┌─────────────┼─────────────┐
                                                   │             │             │
                                                   ▼             ▼             ▼
                                                scrub.py       vm.py      download.py
                                               (GTK3 popup)  (GTK3 popup) (GTK3 popup)
```

The system consists of four components:

| Component                          | Runs as                | Purpose                                                            |
|------------------------------------|------------------------|--------------------------------------------------------------------|
| `nordix-graceful-shutdown`         | root (systemd service) | Daemon that performs the full 14-step shutdown sequence            |
| `nordix-shutdown-client`           | user                   | Lightweight client that sends `poweroff` or `reboot` to the daemon |
| `nordix-graceful-shutdown.socket`  | systemd                | Creates and manages the Unix socket                                |
| `nordix-graceful-shutdown.service` | systemd                | Manages the daemon lifecycle                                       |

The daemon runs as a systemd service, which means it **survives user logout** — critical because step 8 terminates the user session, and the remaining unmount steps must still execute.

The client forwards `DISPLAY`, `WAYLAND_DISPLAY`, and `XAUTHORITY` to the daemon so that GTK popup dialogs appear on the user's screen even though the daemon runs as root.

## Shutdown sequence

|Step|  Action                       | Details 
|----|-------------------------------|--------------------------------------------------------------------------
| 1  | **Sync**                      | Flush Linux page cache (`sync`) and ZFS ARC (`zpool sync`) for all pools
| 2  | **Check scrub/resilver/trim** | If active, show popup: wait for completion, pause, or cancel shutdown 
| 3  | **Check running VMs**         | If libvirt VMs are running, show popup: continue or cancel
| 4  | **Check active downloads**    | If download speed > 40% of connection speed, show popup: wait, ignore, or cancel
| 5  | **Stop services**             | Gracefully stop Docker containers, Podman, Flatpak, Waydroid, NFS, databases, Samba, etc.
| 6  | **Stop AppImages + FUSE**     | SIGTERM → SIGKILL AppImage processes, unmount all FUSE mounts
| 7  | **Sync again**                | Second flush to catch any data written during steps 2–6
| 8  | **Logout user**               | Terminate user session via `loginctl` 
| 9  | **Unmount Steam children**    | `nordix/home/local/steam/{shadercache,proton,game}`
| 10 | **Unmount .local children**   | `nordix/home/local/{lutris,steam}`
| 11 | **Unmount home children**     | `nordix/home/{pictures,videos,music,downloads,documents,games,wine-prefix,local,cache}`
| 12 | **Unmount root children**     | `nordix/{tmp,opt,home,varlib,vartmp,varlog,varcache,vm}`
| 13 | **Force unmount**             | `zfs umount -af` to catch any stragglers
| 14 | **Power off / reboot**        | `systemctl poweroff` or `systemctl reboot`

If any dataset fails to unmount, the sequence **continues regardless** — it is logged but never blocks shutdown.

## Popup dialogs

Three GTK3 popup scripts handle interactive decisions during shutdown. They inherit the system GTK3 theme, so they integrate with Nordix's pywal-based theming.

**Scrub/Trim/Resilver dialog** — three choices:
- *Wait* — polls `zpool status` every 5 seconds and continues automatically when done
- *Pause* — pauses the operation (user must manually restart it on next boot)
- *Cancel* — aborts shutdown entirely

**VM dialog** — two choices:
- *Continue anyway* — proceeds with shutdown (VMs will not be shut down gracefully)
- *Cancel* — aborts shutdown so the user can shut down VMs manually

**Download dialog** — three choices:
- *Wait* — polls download speed every 10 seconds, continues when it drops below threshold
- *Continue anyway* — ignores the active download
- *Cancel* — aborts shutdown

## Building

```bash
clang -march=native -mtune=native -O3 -flto=full -o nordix-graceful-shutdown nordix-graceful-shutdown.c
clang -march=native -mtune=native -O3 -flto=full -o nordix-shutdown-client nordix-shutdown-client.c
```

## Installation

```bash
# Daemon and popup scripts
sudo mkdir -p /usr/lib/nordix
sudo cp nordix-graceful-shutdown    /usr/lib/nordix/
sudo cp nordix-shutdown-scrub.py    /usr/lib/nordix/
sudo cp nordix-shutdown-vm.py       /usr/lib/nordix/
sudo cp nordix-shutdown-download.py /usr/lib/nordix/

# Client binary
sudo cp nordix-shutdown-client /usr/bin/

# systemd units
sudo cp nordix-graceful-shutdown.socket  /etc/systemd/system/
sudo cp nordix-graceful-shutdown.service /etc/systemd/system/

# Enable and start
sudo systemctl daemon-reload
sudo systemctl enable nordix-graceful-shutdown.socket
sudo systemctl start  nordix-graceful-shutdown.socket
```

## Usage

```bash
nordix-shutdown-client --poweroff    # graceful shutdown
nordix-shutdown-client --reboot      # graceful reboot
```

No `sudo` required — the client communicates with the root daemon through the socket.

To bind to a desktop keybind or panel button, point it at one of the above commands.

### Monitoring

```bash
# Follow shutdown logs in real time
journalctl -u nordix-graceful-shutdown.service -f

# Check socket status
systemctl status nordix-graceful-shutdown.socket
```

## Configuration

The following values are defined at the top of `nordix-graceful-shutdown.c` and can be adjusted before building:

| Define | Default | Description |
|----------------------|------------------------------------------------------------------------------
| `CONNECTION_MBIT`    | `100.0`                                       | Internet connection speed in Mbit/s 
| `DOWNLOAD_THRESHOLD` | `0.40`                                        | Fraction of connection speed that triggers the download popup
| `POPUP_SCRUB_TRIM`   | `/usr/lib/nordix/nordix-shutdown-scrub.py`    | Path to scrub/trim popup script
| `POPUP_VM`           | `/usr/lib/nordix/nordix-shutdown-vm.py`       | Path to VM popup script
| `POPUP_DOWNLOAD`     | `/usr/lib/nordix/nordix-shutdown-download.py` | Path to download popup script
| `SOCKET_PATH`        | `/run/nordix-graceful-shutdown.sock`          | Unix socket path

The ZFS dataset lists (steps 9–12) are defined as static arrays in the source and should be edited to match your pool layout.

## Dependencies

- ZFS (zpool, zfs commands)
- Python 3 with PyGObject and GTK3 (`python-gobject`, `gtk3`)
- systemd
- libvirt/virsh (optional — VM check is skipped if not installed)
- Docker/Podman (optional — container stop is skipped if not installed)

## License
---

* SPDX-License-Identifier: GPL-3.0-or-later                         
* Copyright (c) 2025 Jimmy Källhagen                                
* Part of **Yggdrasil - Nordix Desktop Environment**                    
* Nordix and Yggdrasil are registered trademarks of Jimmy Källhagen

---
