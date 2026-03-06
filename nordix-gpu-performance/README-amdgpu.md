# Nordix AMD GPU Configuration

**File:** `/etc/modprobe.d/amdgpu.conf`  
**Part of:** [Nordix](https://github.com/jimmykallhagen/Nordix)  
**Author:** Jimmy Källhagen

## Overview

Optimized kernel module parameters for the AMD `amdgpu` driver, targeting desktop performance and stability on modern AMD GPUs.

### Supported Hardware

- AMD Radeon RX 400+ (Polaris / GCN 4.0)
- AMD Radeon RX Vega series
- AMD Radeon RX 5000 series (RDNA 1)
- AMD Radeon RX 6000 series (RDNA 2)
- AMD Radeon RX 7000 series (RDNA 3)
- AMD Radeon RX 8000 series (RDNA 4)

## Installation

```bash
sudo cp amdgpu.conf /etc/modprobe.d/amdgpu.conf
sudo mkinitcpio -P
sudo reboot
```

## Configuration Summary

| Parameter | Value | Description |
|-----------|-------|-------------|
| `vm_block_size` | 14 | Large page table blocks for reduced lookup overhead |
| `vm_fragment_size` | 9 | Balanced fragment granularity for mixed workloads |
| `vm_size` | 512 | 512 GB virtual address space per process |
| `vm_update_mode` | 2 | Hybrid CPU+SDMA page table updates |
| `scheduler` | 2 | Priority-based GPU job scheduling |
| `sched_job_policy` | 2 | Favor interactive/shorter jobs |
| `gpu_recovery` | 1 | Auto-reset GPU on hang |
| `lockup_timeout` | 10000 | 10 second timeout before declaring hang |
| `ppfeaturemask` | 0xffffffff | All power/clock features unlocked |
| `dcfeaturemask` | 0xffffffff | All display features unlocked |
| `dc` | 1 | Modern display engine (DCN) enabled |
| `exp_hw_support` | 1 | Allow probing unreleased GPUs |
| `noretry` | 1 | No GPU page fault retries (low latency) |
| `gttsize` | 2048 | 2 GB system RAM for GPU staging |
| `runpm` | 0 | Runtime power-off disabled (desktop stability) |
| `npt` | 0 | Nested page tables disabled (bare-metal) |
| `audio` | 1 | HDMI/DP audio enabled |
| `debug` | 0 | Debug logging disabled |

---

## Key Features

### Virtual Memory Optimization
Tuned for large VRAM pools (8 GB+) with reduced TLB misses and faster buffer allocations. Ideal for gaming, 3D rendering, video editing, and machine learning workloads.

### Priority-Based Scheduling
Interactive tasks (gaming, UI) are prioritized over batch workloads, ensuring responsive desktop performance.

### Full Feature Unlock
All power management, overclocking, and display features are unlocked via `ppfeaturemask` and `dcfeaturemask`.

### Desktop-First Philosophy
Runtime power management (`runpm`) is disabled for maximum stability on always-on desktop systems.

## Legacy GPU Support

For Southern Islands (GCN 1.0) and Sea Islands (GCN 2.x) GPUs, uncomment these lines in the config:

```conf
options amdgpu si_support=1 cik_support=1
options radeon si_support=0 cik_support=0
```

**Applicable GPUs:**
- Southern Islands: HD 7700-7900, R7 250/260, R9 270/280
- Sea Islands: R7 260X, R9 290/290X, R7 360, R9 380

## Useful Commands

```bash
# Check current parameter value
cat /sys/module/amdgpu/parameters/ppfeaturemask

# List all parameters
for f in /sys/module/amdgpu/parameters/*; do
  echo "$(basename $f) = $(cat $f 2>/dev/null)"
done

# Monitor GPU temperature
watch -n1 sensors | grep -A5 amdgpu

# Check VRAM usage
cat /sys/class/drm/card0/device/mem_info_vram_used
cat /sys/class/drm/card0/device/mem_info_vram_total

# Check FreeSync status
for c in /sys/class/drm/card0-*/vrr_capable; do
  echo "$c: $(cat $c)"
done
```

---

## Troubleshooting

**Display corruption or GPU hangs:**  
Reduce `vm_block_size` to 12 and test.

**Out-of-memory errors in dmesg:**  
Lower `vm_block_size` or reduce `gttsize`.

**FreeSync not working:**  
Verify `dcfeaturemask=0xffffffff` is active and check `vrr_capable` status.

---

## License

* SPDX-License-Identifier: GPL-3.0-or-later                         
* Copyright (c) 2025 Jimmy Källhagen                                
* Part of Nordix - https://github.com/jimmykallhagen/Nordix             
* Nordix and Yggdrasil are trademarks of Jimmy Källhagen
