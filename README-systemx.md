# **Nordix plans for repo**

**I've been thinking of just trying to get a small repo,<br>
 just to get the system's foundation built for performance.<br>
 This will be relevant later on.**

 **What I show here is only to optimize zver4 to the max.<br>
  My idea is to also do this for CPU architectures: v3 and v4.<br>
  Maybe something intel specific too**

## This is tested and is woking with:
- [**Nordix C-Flags**](/CFLAGS.md)

Tests have been performed with lto full but adjustments or switching to lto thin may be needed.<br>
The idea is to also create PGO profiles in the future for kernel, zfs, compositor, and graphics drivers.

**Nordix Custombuilds - Desktop**
- hyprland-protocols-nordix
- hyprwayland-scanner-nordix
- hyprutils-nordix
- hyprgraphics-nordix
- hyprlang-nordix
- hyprcursor-nordix
- aquamarine-nordix
- xdg-desktop-portal-hyprland-nordix
- hyprwire-nordix
- hyprtoolkit-nordix
- hyprland-nordix
- hyprland-qt-support-nordix
- hyprqt6engine-nordix
- hyprpolkitagent-nordix
- hyprpicker-nordix
- hyprland-guiutils-nordix

## I haven't been able to accomplish this yet with:
- [**Nordix C-Flags**](/CFLAGS.md)
- Mesa
- Proton GE Custom

## Kernel
**I would like to be able to get this linux-taychon kernel with the same optimization**

I haven't managed to get a compile done yet with
- [**Kernel Config**](README-nordix-tachyon-kernel.md)

**However, I have performed tests with good results on the linux-tachyon kernel with less advanced optimizations.**
- clang
- -O3
- znver4
- lto thin

### Linux Tachyon is a beast of a kernel

