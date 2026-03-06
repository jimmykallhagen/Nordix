# Nordix Intel GPU Configuration

**File:** `/etc/modprobe.d/intel.conf`  
**Part of:** [Nordix](https://github.com/jimmykallhagen/Nordix)  
**Author:** Jimmy Källhagen

## Overview

Optimized kernel module parameters for Intel graphics drivers, covering both integrated GPUs (i915) and discrete Arc GPUs (xe).

### Supported Hardware

**i915 Driver (Integrated GPUs):**
- Intel HD Graphics (Gen3–Gen8)
- Intel UHD Graphics (Gen9–Gen12)
- Intel Iris / Iris Plus / Iris Xe (Gen11–Gen12)
- 3rd Gen through 13th Gen Intel Core processors

**xe Driver (Discrete GPUs):**
- Intel Arc A380, A580, A750, A770
- Future Intel discrete and integrated GPUs (Lunar Lake+)

## Installation

```bash
sudo cp intel.conf /etc/modprobe.d/intel.conf
sudo mkinitcpio -P
sudo reboot
```
---

## Configuration Summary

### i915 (Integrated Graphics)

| Parameter | Value | Description |
|-----------|-------|-------------|
| `enable_guc` | 3 | GuC submission + HuC loading enabled |
| `enable_fbc` | 1 | Framebuffer compression (power saving) |
| `enable_psr` | 1 | Panel Self Refresh (laptop power saving) |
| `fastboot` | 1 | Flicker-free boot transition |
| `nuclear_pageflip` | 1 | Atomic modesetting for Wayland |

### xe (Arc Discrete GPUs)

The xe driver auto-configures most settings. Use `force_probe` only if your Arc GPU is not automatically detected.

## Key Features

### GuC/HuC Firmware

Enables both the Graphics Micro Controller (GuC) for improved scheduling and the HEVC Micro Controller (HuC) for hardware-authenticated media decode:

- **GuC submission:** Offloads GPU job scheduling from CPU to GPU
- **HuC loading:** Required for DRM-protected HEVC/H.265 playback

> **Note:** On Gen9 (Skylake, Kaby Lake), GuC submission may cause instability. Use `enable_guc=2` (HuC only) if you experience crashes.

### Power Saving Features

- **Frame Buffer Compression (FBC):** Reduces memory bandwidth by compressing the framebuffer
- **Panel Self Refresh (PSR):** Allows laptop displays to refresh from local memory when content is static

### Wayland Support

- **Atomic modesetting:** Required for modern Wayland compositors
- **Fastboot:** Preserves firmware framebuffer for flicker-free transitions

## Firmware Requirements

Ensure firmware files are installed:

```bash
# Arch Linux
sudo pacman -S linux-firmware

# Check available firmware
ls /usr/lib/firmware/i915/
ls /usr/lib/firmware/xe/
```

## Useful Commands

```bash
# Check current parameter value
cat /sys/module/i915/parameters/enable_guc

# List all parameters
for f in /sys/module/i915/parameters/*; do
  echo "$(basename $f) = $(cat $f 2>/dev/null)"
done

# List available parameters with descriptions
modinfo -p i915
modinfo -p xe

# Check which driver is in use
lspci -k | grep -A3 VGA

# Monitor GPU usage
sudo intel_gpu_top

# Check GuC/HuC firmware status
sudo cat /sys/kernel/debug/dri/0/gt/uc/guc_info
sudo cat /sys/kernel/debug/dri/0/gt/uc/huc_info

# Check Adaptive Sync status
for c in /sys/class/drm/card0-*/vrr_capable; do
  echo "$c: $(cat $c)"
done
```

---

## Troubleshooting

**Crashes on Skylake/Kaby Lake:**  
Change `enable_guc=3` to `enable_guc=2` (HuC only, no GuC submission).

**PSR flickering on laptop:**  
Change `enable_psr=1` to `enable_psr=0` or try `enable_psr=2` for PSR2.

**Arc GPU not detected:**  
Uncomment `force_probe=*` in the xe section, or specify your GPU's PCI ID.

**Black screen on boot:**  
Add `nomodeset` to kernel parameters as a fallback, then debug one parameter at a time.

## xe Driver Notes

The xe driver is included in mainline Linux kernels starting from 6.8. It becomes the default for Intel Arc GPUs in kernel 6.10+. 

To find your GPU's PCI ID for force probing:
```bash
lspci -nn | grep -i vga
# Look for [8086:XXXX] — use the XXXX part
```

---

## License

* SPDX-License-Identifier: GPL-3.0-or-later                         
* Copyright (c) 2025 Jimmy Källhagen                                
* Part of **Yggdrasil - Nordix Desktop Environment**                    
* Nordix and Yggdrasil are trademarks of Jimmy Källhagen
