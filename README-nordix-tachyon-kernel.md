# Nordix Tachyon Kernel

**Part of:** [Nordix](https://github.com/jimmykallhagen/Nordix)  
**License:** GPL-2.0-only  
**Author:** Jimmy Källhagen

- [ ] Note - Not working yet

## Overview

Nordix Tachyon is a performance-optimized Linux kernel built upon:

- **[Linux Tachyon](https://git.staropensource.de/StarOpenSource/Linux-Tachyon)** — Fork of Intel Clear Linux kernel patchset
  - Maintained by **JeremyStarTM** and **yarost12**
  - *When Intel abandoned Clear Linux, they kept the dream alive*

- **[CachyOS Kernel Config](https://github.com/CachyOS/linux-cachyos)** — Used as base configuration
  - Gaming and desktop optimized defaults

> *Nordix will carry on the Clear Linux legacy of performance, both in heart and soul.*

---

## Build Configuration

### Compiler Toolchain

| Component | Value |
|-----------|-------|
| Compiler | Clang |
| Linker | LLD |
| LTO | Full |
| Target | AMD Zen 4 (znver4) |

### CFLAGS

```bash
-march=znver4 -mtune=znver4 -O3 -pipe -fno-plt
-mavx512f -mavx512dq -mavx512bw -mavx512vl -mavx512cd
-mavx512vbmi -mavx512vbmi2 -mavx512vnni
-mvaes -mvpclmulqdq
-flto=full
-fvectorize -fslp-vectorize -ftree-vectorize
-funroll-loops
-falign-functions=64 -falign-loops=64
-fno-math-errno -fno-trapping-math
-fmerge-all-constants
```

### Security vs Performance

See [CFLAGS.md](../CFLAGS.md) for security/performance trade-off options.

---

## Config Changes from CachyOS Base

The following changes were made to the CachyOS 6.19 config:

### Removed (CachyOS-specific)

| Config | Reason |
|--------|--------|
| `CONFIG_CACHY=y` → `n` | CachyOS patches not included, using Tachyon patches instead |

### ZFS Compatibility

| Config | Change | Reason |
|--------|--------|--------|
| `CONFIG_DEBUG_INFO_BTF` | `y` → `n` | Prevents ZFS DKMS build failures |
| `CONFIG_DEBUG_INFO_BTF_MODULES` | `y` → `n` | Same as above |

### Nordix Branding

| Config | Change |
|--------|--------|
| `CONFIG_DEFAULT_HOSTNAME` | `"cachyos"` → `"nordix"` |

### Performance Optimizations

| Config | Change | Reason |
|--------|--------|--------|
| `CONFIG_LTO_CLANG_THIN` | `y` → `n` | Using full LTO instead |
| `CONFIG_LTO_CLANG_FULL` | `n` → `y` | Better optimization, slower build |
| `CONFIG_NTSYNC` | `m` → `y` | Built-in for Wine/Proton performance |

---

## Features Inherited from CachyOS Config

These were already set in the CachyOS base and remain unchanged:

| Feature | Config | Value |
|---------|--------|-------|
| Zen 4 CPU | `CONFIG_MZEN4` | `y` |
| 1000 Hz Tickrate | `CONFIG_HZ_1000` | `y` |
| Full Preemption | `CONFIG_PREEMPT` | `y` |
| Tickless Kernel | `CONFIG_NO_HZ_FULL` | `y` |
| -O3 Optimization | `CONFIG_CC_OPTIMIZE_FOR_PERFORMANCE_O3` | `y` |
| sched_ext Support | `CONFIG_SCHED_CLASS_EXT` | `y` |
| POSIX ACLs | `CONFIG_FS_POSIX_ACL` | `y` |
| TMPFS ACLs | `CONFIG_TMPFS_POSIX_ACL` | `y` |
| Devtmpfs | `CONFIG_DEVTMPFS` | `y` |
| Devtmpfs Mount | `CONFIG_DEVTMPFS_MOUNT` | `y` |
| Unix Sockets | `CONFIG_UNIX` | `y` |

---

## Building

```bash
# Standard build (znver4, full LTO)
makepkg -s

# With menuconfig to customize
env _makemenuconfig=y makepkg -s

# Save final config for inspection
env _copyfinalconfig=y makepkg -s
```

### Build Requirements

- `clang`
- `llvm`
- `lld`
- `bc`
- `cpio`
- `pahole`
- `perl`
- `python`

---

## CPU Support

This kernel is optimized for **AMD Zen 4** processors:

- Ryzen 7000 series (7600, 7600X, 7700, 7700X, 7900, 7900X, 7950X)
- Ryzen 7000X3D series (7800X3D, 7900X3D, 7950X3D)
- Ryzen Pro 7000 series
- Ryzen Mobile 7040 series (Phoenix)
- Ryzen Mobile 7045 series (Dragon Range)
- Threadripper 7000 series (7960X, 7970X, 7980X)
- EPYC 9004 series (Genoa)

For other CPUs, modify `_nordix_arch` in the PKGBUILD:

```bash
env _nordix_arch=alderlake makepkg -s    # Intel 12th-14th gen
env _nordix_arch=znver5 makepkg -s       # AMD Zen 5
env _nordix_arch=x86-64-v3 makepkg -s    # Generic modern
```

---

## Nordix Principles — Honor the Developers

This kernel would not exist without:

- **JeremyStarTM** and **yarost12** — Linux Tachyon maintainers
- **CachyOS Team** — Kernel configuration base
- **Intel** — Original Clear Linux patchset (RIP 2025)
- **Linux Kernel Developers** — The foundation of everything

> **"Even though the project is free of charge,  
> we are all responsible for paying by showing respect."**  
> — *Jimmy Källhagen, Nordix*

---

## License

- SPDX-License-Identifier: GPL-2.0-only
- Copyright (c) 2025 Jimmy Källhagen
- Part of Nordix — https://github.com/jimmykallhagen/Nordix
- Nordix and Yggdrasil are trademarks of Jimmy Källhagen
