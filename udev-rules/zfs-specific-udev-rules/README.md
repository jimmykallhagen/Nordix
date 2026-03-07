# **60-ioschedulers.rules** - _ZFS the smart file system_

Normally, it is the kennel that is responsible for ensuring that file systems and storage are handled correctly in a system.

This is different when it comes to zfs, you can think that you now have two systems in your system, and that zfs should now control the file system and storage itself, and act like a helper to your kernel instead.

Therefore, we need to make sure that ZFS is allowed to handle these things itself.

### HDD
ACTION=="add|change", KERNEL=="sd[a-z]*", ATTR{queue/rotational}=="1", \
    ATTR{queue/scheduler}="none"

### SSD
ACTION=="add|change", KERNEL=="sd[a-z]*|mmcblk[0-9]*", ATTR{queue/rotational}=="0", \
    ATTR{queue/scheduler}="none"

### NVMe SSD
ACTION=="add|change", KERNEL=="nvme[0-9]*", ATTR{queue/rotational}=="0", \
    ATTR{queue/scheduler}="none"
