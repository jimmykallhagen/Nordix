# 60-ioschedulers.rules - ZFS the smart file system

## The original repo [**Here**](https://github.com/jimmykallhagen/nordix-zfs/tree/main/zfs-specific-udev-rules)

Normally, it is the kernel that is responsible for ensuring that file systems and storage are handled correctly in a system. This is different when it comes to ZFS. You can think of it as having two systems in your system — ZFS takes over control of file system and storage, while the kernel steps aside.

Therefore, we need to make sure that ZFS is allowed to handle these things itself.

Technological explanation: ZFS has its own I/O scheduler that understands the pool topology (mirrors, stripes, special vdevs). The kernel's scheduler doesn't know about this and can make suboptimal decisions. Setting none lets ZFS handle all scheduling.

### HDD
ACTION=="add|change", KERNEL=="sd[a-z]*", ATTR{queue/rotational}=="1", \
    ATTR{queue/scheduler}="none"

### SSD
ACTION=="add|change", KERNEL=="sd[a-z]*|mmcblk[0-9]*", ATTR{queue/rotational}=="0", \
    ATTR{queue/scheduler}="none"

### NVMe SSD
ACTION=="add|change", KERNEL=="nvme[0-9]*", ATTR{queue/rotational}=="0", \
    ATTR{queue/scheduler}="none"

You can find the file here:

    60-ioschedulers.rules

Install

sudo cp 60-ioschedulers.rules /etc/udev/rules.d/
sudo reboot
