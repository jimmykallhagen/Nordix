# Nordix NVIDIA GPU Configuration

**File:** `/etc/modprobe.d/nvidia.conf`  
**Part of:** [Nordix](https://github.com/jimmykallhagen/Nordix)  
**Author:** Jimmy Källhagen

## Overview

Optimized kernel module parameters for the proprietary NVIDIA driver, designed for Wayland compatibility and desktop performance.

### Supported Hardware

- NVIDIA GeForce GTX 900 series (Maxwell 2.0+)
- NVIDIA GeForce GTX 10 series (Pascal)
- NVIDIA GeForce RTX 20 series (Turing)
- NVIDIA GeForce RTX 30 series (Ampere)
- NVIDIA GeForce RTX 40 series (Ada Lovelace)
- NVIDIA GeForce RTX 50 series (Blackwell)

> **Best suited for Turing (RTX 20xx) and newer**

---

## Installation

```bash
sudo cp nvidia.conf /etc/modprobe.d/nvidia.conf
sudo mkinitcpio -P
sudo reboot
```

### Required Systemd Services

For suspend/resume support, enable these services:

```bash
sudo systemctl enable nvidia-suspend.service
sudo systemctl enable nvidia-resume.service
sudo systemctl enable nvidia-hibernate.service
```

## Configuration Summary

| Module | Parameter | Value | Description |
|--------------|-----------|-------|-------------|
| `nvidia_drm` | `modeset` | 1 | Kernel Mode Setting (required for Wayland) |
| `nvidia_drm` | `fbdev` | 1 | Framebuffer console support |
| `nvidia` | `NVreg_PreserveVideoMemoryAllocations` | 1 | Preserve VRAM on suspend/resume |
| `nvidia` | `NVreg_UsePageAttributeTable` | 1 | Use PAT for memory management |
| `nvidia` | `NVreg_DynamicPowerManagement` | 0x02 | Fine-grained runtime D3 power management |
| `nvidia` | `NVreg_TemporaryFilePath` | /tmp | Fast tmpfs for VRAM backup |
| `nvidia` | `NVreg_EnableGpuFirmware` | 0 | GSP firmware disabled |

---

## Key Features

### Wayland Support

Full Wayland compatibility with:
- **KMS (modeset=1):** Required for all Wayland compositors (Hyprland, Sway, KDE Plasma 6, GNOME)
- **Framebuffer console (fbdev=1):** Working TTY/virtual terminals under Wayland
- **VRAM preservation:** Sessions survive sleep without crashes

### Suspend/Resume

VRAM contents are saved to system RAM before suspend and restored on resume, preventing:
- Black screens after wake
- Application crashes
- Compositor restarts

### Power Management

Fine-grained RTD3 (Runtime D3) power management for laptops:
- GPU powers down when idle
- Automatic wake on demand
- Significant battery savings on hybrid graphics systems

---

### GSP Firmware

GSP (GPU System Processor) firmware is **disabled** by default for maximum stability. The GSP offloads some driver tasks to the GPU but can cause issues on certain hardware.

> Enable GSP (`NVreg_EnableGpuFirmware=1`) only if you need specific features or experience better stability with it.

## Modules Overview

| Module | Purpose |
|--------|---------|
| `nvidia` | Core GPU driver (NVreg_ parameters) |
| `nvidia_drm` | DRM/KMS interface for display |
| `nvidia_uvm` | Unified Virtual Memory for CUDA |

---

## Useful Commands

```bash
# Show all loaded NVIDIA parameters
cat /proc/driver/nvidia/params

# Check driver version
cat /proc/driver/nvidia/version

# Check KMS status
cat /sys/module/nvidia_drm/parameters/modeset

# Check fbdev status
cat /sys/module/nvidia_drm/parameters/fbdev

# Check GSP firmware status
nvidia-smi -q | grep "GSP Firmware"

# Check GPU power state
cat /proc/driver/nvidia/gpus/*/power

# Check Resizable BAR status
nvidia-smi -q | grep -i "bar"

# Monitor GPU
watch -n1 nvidia-smi

# List available module parameters
modinfo -p nvidia
modinfo -p nvidia_drm
modinfo -p nvidia_uvm
```

---

## Troubleshooting

**Black screen on Wayland:**  
Ensure `modeset=1` is set and initramfs is rebuilt.

**No console on TTY switch:**  
Verify `fbdev=1` is active (requires driver 545+).

**Crash after suspend (laptop):**  
Disable VRAM preservation: `NVreg_PreserveVideoMemoryAllocations=0`

**KDE 6 + Wayland freeze after wake:**  
Uncomment `NVreg_InitializeSystemMemoryAllocations=0`

**CUDA not working:**  
Ensure `nvidia_uvm` module is loaded: `lsmod | grep nvidia_uvm`

---

## Optional Parameters

The config file includes commented-out options for advanced tuning:

| Parameter | Description |
|-----------|-------------|
| `NVreg_InitializeSystemMemoryAllocations` | Disable memory zeroing for slight performance gain |
| `NVreg_EnableMSI` | Message Signaled Interrupts (usually auto-detected) |
| `NVreg_RegistryDwords` | Advanced registry overrides (e.g., `RMIntrLockingMode=1`) |
| `NVreg_EnablePCIeGen3` | Force PCIe Gen3 speed |
| `uvm_disable_hmm` | Disable Heterogeneous Memory Management in CUDA |

## Notes

- All `NVreg_` parameters are **case-sensitive**
- Changes require initramfs rebuild and reboot
- Always have `nomodeset` bootloader fallback ready

---

## License

* SPDX-License-Identifier: GPL-3.0-or-later                         
* Copyright (c) 2025 Jimmy Källhagen                                
* Part of **Yggdrasil - Nordix Desktop Environment**                    
* Nordix and Yggdrasil are trademarks of Jimmy Källhagen
