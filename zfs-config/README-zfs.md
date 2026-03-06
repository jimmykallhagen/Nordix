# Nordix ZFS Kernel Module Configuration

**File:** `/etc/modprobe.d/zfs.conf`  
**Part of:** [Nordix](https://github.com/jimmykallhagen/Nordix)  
**Author:** Jimmy Källhagen

## Overview

These are pre-tuned ZFS kernel module parameter files (`/etc/modprobe.d/zfs.conf`) for the Nordix Linux distribution, optimized for **desktop and gaming performance** on NVMe/SSD storage.

ZFS defaults are designed for enterprise servers with spinning disks and conservative memory usage. These configurations shift the balance toward interactive desktop responsiveness: aggressive ARC caching, high I/O parallelism, fast scrub completion, and SSD-aware allocation — while remaining stable and safe for daily use.

Each file is a complete, drop-in replacement. Pick the one that matches your system RAM and copy it to `/etc/modprobe.d/zfs.conf`.

## Available Profiles

| File             | System RAM | ARC Max | ARC Min | Effective Cache* | Aggressiveness |
|------------------|------------|---------|---------|------------------|----------------|
| `zfs-8gb.conf`   | 8 GB       | 4 GB    | 1 GB    | ~8–12 GB         | Conservative   |
| `zfs-16gb.conf`  | 16 GB      | 8 GB    | 2 GB    | ~16–24 GB        | Balanced       |
| `zfs-32gb.conf`  | 32 GB      | 20 GB   | 8 GB    | ~50–60 GB        | Performance    |
| `zfs-64gb.conf`  | 64 GB      | 37 GB   | 15 GB   | ~100–120 GB      | Aggressive     |
| `zfs-128gb.conf` | 128 GB     | 80 GB   | 30 GB   | ~200–240 GB      | Extreme        |

*\* Effective cache assumes compressed ARC enabled with ~2.5x ZSTD compression ratio.*

## Installation

```bash
# 1. Copy the appropriate file for your RAM size
sudo cp zfs-32gb.conf /etc/modprobe.d/zfs.conf

# 2. If ZFS is in your mkinitcpio MODULES=() (required for ZFS-on-root):
sudo mkinitcpio -P

# 3. Reboot
sudo reboot
```

> **Important:** If `zfs` is listed in `MODULES=()` in `/etc/mkinitcpio.conf` (which it must be for ZFS-on-root), you **must** run `mkinitcpio -P` before rebooting. Otherwise your changes are silently ignored because the old parameters are baked into the initramfs image.

## What Gets Tuned (27 Parameters)

### ARC Memory (7 parameters)

The Adaptive Replacement Cache is ZFS's in-memory read cache. Unlike a simple LRU cache, the ARC uses a patented algorithm that balances recency and frequency to maximize hit rates. On a well-tuned desktop system, 90–98% of all disk reads are served from RAM.

| Parameter                      | What it does                           | How it scales                |
|--------------------------------|----------------------------------------|------------------------------|
|`zfs_arc_max`                  | Upper bound of cache size                | 50–62% of RAM               |
|`zfs_arc_min`                  | Floor that survives memory pressure      | 25–37% of arc_max           |
|`zfs_arc_lotsfree_percent`     | When to start yielding memory            | 8% (8 GB) -> 1% (64+ GB)    |
|`zfs_arc_meta_limit_percent`   | Max metadata share of ARC                | 50% (all profiles)          |
|`zfs_arc_meta_prune`           | Metadata cleanup scan depth              | 32K (8 GB) -> 256K (128 GB) |
|`zfs_arc_meta_adjust_behavior` | Protect data blocks from metadata storms | META_ONLY (all profiles)    |
|`zfs_arc_average_blocksize`    | Hash table sizing hint                   | 64 KB                       |

### Compressed ARC (1 parameter)

Stores cached data in compressed form, effectively multiplying ARC capacity by the compression ratio. A 64 GB system with 37 GB ARC behaves like it has 100+ GB of cache. CPU overhead is negligible on modern hardware.

### I/O Queue Depths (8 parameters)

NVMe drives support tens of thousands of concurrent commands across multiple hardware queues. ZFS defaults are tuned for HDDs with queue depths of 1–10. These profiles increase parallelism proportionally to system RAM (which correlates with expected workload intensity).

| Parameter       | 8 GB | 16 GB | 32 GB | 64 GB | 128 GB |
|-----------------|------|-------|-------|-------|--------|
| Async read max  | 16   | 24    | 32    | 32    | 64     |
| Sync read max   | 16   | 24    | 32    | 32    | 64     |
| Async write max | 10   | 12    | 16    | 16    | 24     |
| Sync write max  | 5    | 6     | 8     | 8     | 10     |

### Write Management (3 parameters)

| Parameter                    | What it does                      | How it scales                  |
|------------------------------|-----------------------------------|--------------------------------|
| `zfs_dirty_data_max_percent` | RAM available for write buffering | 15% (8 GB) → 40% (64+ GB)      |
| `zfs_txg_timeout`            | Flush interval                    | 5 sec (all profiles)           |
| `zfs_immediate_write_sz`     | ZIL bypass threshold              | 64 KB (8 GB) → 128 KB (16+ GB) |

### Scrub & Resilver (4 parameters)

All throttling removed (`delay=0`, `idle=0`) for fast completion on NVMe/SSD. Scrub I/O depth scales from 3 (8 GB) to 8 (128 GB). For HDD pools, consider re-adding throttling to maintain responsiveness during multi-hour scrubs.

### I/O Aggregation (3 parameters)

Controls how aggressively ZFS merges adjacent I/O requests into single operations. Larger gap limits allow merging reads/writes that are close but not perfectly contiguous on disk.

| Parameter         | 8 GB   | 16 GB  | 32+ GB |
|-------------------|--------|--------|--------|
| Aggregation limit | 128 KB | 128 KB | 128 KB |
| Read gap limit    | 32 KB  | 48 KB  | 64 KB  |
| Write gap limit   | 8 KB   | 12 KB  | 16 KB  |

### SSD Optimization (1 parameter)

`metaslab_lba_weighting_enabled=0` disables the HDD-oriented allocation bias that prefers lower LBA addresses (outer platter edge on spinning disks). On NVMe/SSD, all addresses are equal — disabling this gives more even wear and reduced fragmentation.

## Scaling Philosophy

The profiles are not just the same file with different ARC sizes. As system RAM increases:

- **Memory pressure thresholds decrease** — more RAM means the system can absorb sudden spikes without OOM risk, so the ARC can hold onto data more aggressively.
- **I/O parallelism increases** — larger systems tend to have higher-end NVMe devices and heavier workloads that benefit from deeper command queues.
- **Write buffers grow** — more RAM means more space for dirty data accumulation, enabling better write coalescing and fewer TXG commits.
- **Metadata pruning becomes more aggressive** — larger ARC means more metadata to manage, requiring deeper prune scans to stay efficient.
- **L2ARC recommendations change** — from "strongly recommended" at 8 GB to "unnecessary" at 128 GB.

## L2ARC Guidance

All profiles include commented-out L2ARC parameters. Uncomment them if you add a dedicated cache device (`zpool add <pool> cache <device>`).

| System RAM | L2ARC Recommendation                                                         |
|------------|------------------------------------------------------------------------------|
| 8 GB       | **Strongly recommended** — 4 GB ARC cannot hold a modern desktop working set |
| 16 GB      | **Recommended** — meaningful benefit for heavy workloads                     |
| 32 GB      | **Optional** — 20 GB ARC covers most desktop workloads                       |
| 64 GB      | **Usually unnecessary** — 100+ GB effective capacity                         |
| 128 GB     | **Unnecessary** — 200+ GB effective capacity exceeds any desktop working set |

## Verification

After rebooting with the new configuration:

```bash
# Verify parameters are active
cat /sys/module/zfs/parameters/zfs_arc_max
cat /sys/module/zfs/parameters/zfs_arc_min

# Check ARC hit rate (target: >90%)
awk '/^hits/{h=$3} /^misses/{m=$3} END{
  printf "%.1f%%\n",h/(h+m)*100}' /proc/spl/kstat/zfs/arcstats

# Full ARC summary
arc_summary

# Monitor ARC in real time
watch -n1 'grep -E "^(size|c_max|c_min|hits|misses)" /proc/spl/kstat/zfs/arcstats'

# Pool health
zpool status -x

# I/O monitoring
zpool iostat -v <pool> 1
```

## For HDD Pools

These profiles are optimized for NVMe/SSD. If your pool contains spinning disks, consider these adjustments:

- `zfs_resilver_delay=2` — add throttling to maintain responsiveness
- `zfs_scrub_delay=4` — background scrub instead of full-speed
- `zfs_vdev_scrub_max_active=2` — lower scrub parallelism
- `zfs_vdev_async_read_max_active=10` — reduce read parallelism to prevent head thrashing
- `zfs_vdev_aggregation_limit=1048576` — larger merges to minimize seeking
- `metaslab_lba_weighting_enabled=1` — re-enable LBA bias (outer tracks are faster on HDDs)

## File Documentation

Every parameter in every file includes inline documentation with:

- Default value, valid range, and Nordix setting
- Detailed explanation of what the parameter does and why
- Scaling guidance for different system sizes
- Warnings about potential risks

The files are designed to be self-documenting — you should be able to understand and customize any parameter just by reading the comments.

## License
* SPDX-License-Identifier: GPL-3.0-or-later                         
* Copyright (c) 2025 Jimmy Källhagen                                
* Part of Nordix - https://github.com/jimmykallhagen/Nordix             
* Nordix and Yggdrasil are trademarks of Jimmy Källhagen
