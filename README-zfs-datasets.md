# Nordix ZFS Dataset Configuration

**Part of:** [Yggdrasil - Nordix Desktop Environment](https://github.com/yourusername/nordix)  
**License:** GPL-3.0-or-later  
**Author:** Jimmy Källhagen

## Overview

This document describes the Nordix ZFS dataset layout — a carefully designed hierarchy that separates system, user, and application data into purpose-optimized datasets. This architecture enables **surgical rollbacks**, **workload-specific tuning**, and **efficient snapshot management**.

## Why Separate Datasets?

Traditional filesystems treat all data equally. ZFS datasets allow you to:

### 1. Granular Rollbacks
When you rollback a dataset, **only that dataset is affected**. By separating `/home` from `~/.config`, you can:
- Rollback your desktop configuration without losing downloaded files
- Restore your system root without affecting application data in `/var/lib`
- Undo game settings changes without re-downloading 100 GB of game files

### 2. Workload-Specific Optimization
Different data has different access patterns:

| Data Type | Access Pattern | Optimal Settings |
|-----------|---------------|------------------|
| Game files | Large sequential reads | `recordsize=1M`, `primarycache=all` |
| Logs | Append-only writes | `sync=disabled`, `compression=zstd-10` |
| Temp files | Ephemeral, high churn | `checksum=off`, `compression=lz4` |
| Documents | Small random I/O | `recordsize=32K`, `copies=2` |

### 3. Snapshot Efficiency
Datasets that change frequently (caches, logs, temp) are excluded from snapshots, which:
- Reduces snapshot size dramatically
- Speeds up snapshot creation and deletion
- Prevents rollbacks from restoring stale cache data

### 4. Security Isolation
Each dataset can have its own security properties:
- `exec=off` on data directories (Pictures, Music, Downloads)
- `setuid=off` to prevent privilege escalation
- `devices=off` to block device file creation

---

## Pool Configuration

```bash
zpool create -f \
  -o ashift=13 \
  -o autotrim=on \
  -O dnodesize=auto \
  -O redundant_metadata=most \
  nordix "${ROOT_DISK}"
```

| Property | Value | Purpose |
|----------|-------|---------|
| `ashift=13` | 8K sectors | Optimal for modern NVMe SSDs (use 12 for 4K sector drives) |
| `autotrim=on` | Automatic TRIM | Maintains SSD performance and longevity |
| `dnodesize=auto` | Variable metadata | Efficient extended attribute storage |
| `redundant_metadata=most` | Extra metadata copies | Protects all metadata blocks with redundant copies |

### Metadata Redundancy

The `redundant_metadata=most` property is inherited by **all datasets** and provides:

- **Extra metadata copies** — ZFS stores additional copies of indirect blocks, dnode blocks, and other metadata
- **Corruption recovery** — If a metadata block becomes corrupted, ZFS can reconstruct it from the redundant copy
- **Single-disk protection** — On a single-disk system (no mirror/raidz), this is the only way to protect metadata
- **Minimal overhead** — Metadata is typically <1% of total data, so the space cost is negligible

---

## Dataset Hierarchy

```
nordix (pool)
├── ROOT
│   └── default          ← System root (/)     [SNAPSHOTS: YES]
├── varcache             ← /var/cache          [SNAPSHOTS: NO]
├── varlog               ← /var/log            [SNAPSHOTS: NO]
├── varlib               ← /var/lib            [SNAPSHOTS: YES]
├── vartmp               ← /var/tmp            [SNAPSHOTS: NO]
├── tmp                  ← /tmp                [SNAPSHOTS: NO]
├── opt                  ← /opt                [SNAPSHOTS: YES]
└── home                 ← /home               [SNAPSHOTS: YES]
    ├── cache            ← ~/.cache            [SNAPSHOTS: NO]
    ├── config           ← ~/.config           [SNAPSHOTS: YES]
    ├── local            ← ~/.local            [SNAPSHOTS: YES]
    │   ├── lutris       ← Lutris data         [SNAPSHOTS: YES]
    │   └── steam        ← Steam root          [SNAPSHOTS: YES]
    │       ├── game     ← Game installs       [SNAPSHOTS: NO]
    │       ├── proton   ← Proton versions     [SNAPSHOTS: YES]
    │       └── shadercache                    [SNAPSHOTS: NO]
    ├── games            ← ~/Games             [SNAPSHOTS: YES]
    ├── wine-prefix      ← ~/Wine-prefix       [SNAPSHOTS: YES]
    ├── documents        ← ~/Documents         [SNAPSHOTS: YES]
    ├── pictures         ← ~/Pictures          [SNAPSHOTS: YES]
    ├── videos           ← ~/Videos            [SNAPSHOTS: YES]
    ├── music            ← ~/Music             [SNAPSHOTS: YES]
    └── downloads        ← ~/Download          [SNAPSHOTS: NO]
```

---

## Dataset Details

### System Root (`nordix/ROOT/default`)

The bootable system root with maximum data integrity and hardening.

```bash
zfs create -o mountpoint=/ \
  -o compression=zstd-5 \
  -o recordsize=128K \
  -o copies=2 \
  -o checksum=fletcher4 \
  -o xattr=sa \
  -o acltype=posixacl \
  nordix/ROOT/default
```

| Property | Value | Rationale |
|----------|-------|-----------|
| `compression=zstd-5` | Balanced | Good ratio without high CPU cost |
| `recordsize=128K` | Default | Mixed workload (binaries, configs, libs) |
| `copies=2` | Redundancy + Performance | See below |
| `xattr=sa` | System attributes | Store xattrs in dnodes, not hidden files |
| `acltype=posixacl` | POSIX ACLs | Required for systemd and modern Linux |

#### ZFS Hardening: `copies=2`

The `copies=2` property on the root dataset is a key hardening measure that provides **both redundancy and performance**:

**Redundancy Benefits:**
- ZFS stores two independent copies of every data block on disk
- If one copy becomes corrupted (bit rot, failed write, cosmic ray), ZFS automatically uses the good copy
- Combined with `redundant_metadata=most`, this gives near-RAID1 protection on a single disk
- Protects against silent data corruption that would go undetected on traditional filesystems

**Performance Benefits:**
- ZFS can read from **either copy** — the scheduler picks whichever is faster to access
- On HDDs: If one copy is closer to the current head position, ZFS reads that one
- On SSDs: Parallel reads from different NAND cells can improve throughput
- Read-heavy workloads (boot, application loading) benefit most

**Space Trade-off:**
- System root is typically 15-30 GB
- With `copies=2`, this becomes 30-60 GB on disk
- On modern 500 GB+ SSDs, this is an acceptable trade-off for critical system data
- User data datasets use `copies=1` to avoid doubling storage for large files

**Why only on root?**
- Root contains irreplaceable system files (binaries, libraries, kernel)
- User data (documents, media) can often be recovered from backups
- Game files can be re-downloaded
- Applying `copies=2` everywhere would double total storage usage

**Snapshot behavior:** ✅ Included in auto-snapshots. Rollback restores entire system state.

---

### Cache Directories (No Snapshots)

These datasets are **excluded from snapshots** because their contents are:
- Regeneratable (can be rebuilt)
- High-churn (change constantly)
- Not valuable after rollback (stale data)

#### `/var/cache`
```bash
-o compression=zstd-1    # Light compression (high throughput)
-o recordsize=32K        # Package manager chunk size
-o sync=standard         # Durability for pacman database
-o logbias=throughput    # Optimize for streaming writes
-o exec=off              # No execution from cache
```

#### `/var/log`
```bash
-o compression=zstd-10   # Maximum compression (logs compress extremely well)
-o sync=disabled         # Async writes (logs are not critical)
-o primarycache=metadata # Don't cache log content in ARC
```

#### `/tmp` and `/var/tmp`
```bash
-o compression=lz4       # Fastest compression
-o checksum=off          # No integrity checks (ephemeral data)
-o sync=disabled         # No write barriers
-o primarycache=metadata # Minimal caching
```

**Why exclude from snapshots?**
- Rolling back `/tmp` makes no sense — it's meant to be empty
- Restoring old logs creates confusion (missing recent entries)
- Cache data becomes invalid after system changes

---

### Application Data (`/var/lib`)

Persistent application state that survives system rollbacks.

```bash
-o compression=zstd-5
-o recordsize=16K        # Small records for databases
-o sync=standard         # Data integrity
-o logbias=latency       # Responsive database operations
```

**Snapshot behavior:** ✅ Separate snapshots. Database state preserved independently of system.

**Use case:** Rollback `/` to yesterday, but keep today's PostgreSQL data.

---

### Home Directory Structure

The home directory is split into multiple datasets for maximum rollback flexibility.

#### User Home (`nordix/home`)
Base home directory with general user files.

```bash
-o compression=zstd-1
-o recordsize=64K
-o copies=1              # Single copy (important data has own datasets)
```

#### User Config (`~/.config`)
Desktop environment and application settings.

```bash
-o compression=zstd-5
-o recordsize=32K
-o exec=off              # Configs should not be executable
```

**Snapshot behavior:** ✅ Rollback broken configs without losing documents.

#### User Cache (`~/.cache`)
Regeneratable cache data (thumbnails, browser cache, build artifacts).

```bash
-o compression=zstd-5
-o recordsize=16K
-o exec=off
```

**Snapshot behavior:** ❌ Excluded. Cache rebuilds automatically.

---

### Media Directories

Optimized for large, already-compressed files (images, video, audio).

#### Pictures
```bash
-o compression=lz4       # Fast (JPG/PNG already compressed)
-o recordsize=1M         # Large sequential reads
-o primarycache=metadata # Don't waste RAM caching image data
-o secondarycache=none   # Don't use L2ARC for media
```

#### Videos
```bash
-o recordsize=4M         # Very large sequential reads
-o primarycache=metadata
```

#### Music
```bash
-o recordsize=2M         # Album-sized chunks
-o primarycache=metadata
```

**Why minimal caching?**
Media files are typically read once (playback) and don't benefit from caching. Saving ARC space for frequently-accessed system data improves overall performance.

---

### Gaming Datasets

Purpose-built for Steam, Lutris, and Wine gaming.

#### Game Installations (`nordix/home/local/steam/game`)
```bash
-o compression=zstd-4    # Moderate compression
-o recordsize=1M         # Large game assets
-o primarycache=all      # Cache frequently-loaded textures
```

**Snapshot behavior:** ❌ Excluded. Games are 50-150 GB each — snapshots would explode storage. Re-download if needed.

#### Proton/Wine Compatibility Tools
```bash
-o compression=zstd-5
-o recordsize=32K
```

**Snapshot behavior:** ✅ Rollback broken Proton versions.

#### Shader Cache
```bash
-o compression=zstd-5
-o recordsize=16K        # Small compiled shader files
-o checksum=fletcher2    # Fast integrity check
```

**Snapshot behavior:** ❌ Excluded. Shader caches rebuild automatically.

#### Save Games (`~/Games`, `~/Wine-prefix`)
```bash
-o compression=zstd-4
-o recordsize=32K-1M     # Varies by dataset
-o copies=1
```

**Snapshot behavior:** ✅ Protect your 200-hour save files!

---

## Snapshot Strategy

### What Gets Snapshotted

| Dataset | Snapshots | Reason |
|---------|-----------|--------|
| `ROOT/default` | ✅ Yes | System rollback |
| `varlib` | ✅ Yes | Application state |
| `opt` | ✅ Yes | Third-party software |
| `home` | ✅ Yes | User data |
| `home/config` | ✅ Yes | Settings recovery |
| `home/documents` | ✅ Yes | Important files |
| `home/games` | ✅ Yes | Save files |
| `home/wine-prefix` | ✅ Yes | Wine configurations |
| `home/local/steam/proton` | ✅ Yes | Compatibility tools |

### What's Excluded

| Dataset | Snapshots | Reason |
|---------|-----------|--------|
| `varcache` | ❌ No | Regeneratable |
| `varlog` | ❌ No | Historical only |
| `tmp` | ❌ No | Ephemeral |
| `vartmp` | ❌ No | Ephemeral |
| `home/cache` | ❌ No | Regeneratable |
| `home/downloads` | ❌ No | Transient files |
| `home/local/steam/game` | ❌ No | Re-downloadable, huge |
| `home/local/steam/shadercache` | ❌ No | Regeneratable |

---

## Rollback Scenarios

### Scenario 1: Broken System Update
```bash
# Rollback only the system, keep /var/lib databases intact
zfs rollback nordix/ROOT/default@before-update
```
Your MariaDB data, Docker volumes, and application state remain untouched.

### Scenario 2: Broken Desktop Config
```bash
# Rollback only ~/.config
zfs rollback nordix/home/config@yesterday
```
Your documents, downloads, and game saves are unaffected.

### Scenario 3: Corrupted Game Save
```bash
# Rollback only your Games folder
zfs rollback nordix/home/games@before-boss-fight
```
System, configs, and installed games remain current.

### Scenario 4: Bad Proton Version
```bash
# Rollback Proton compatibility tools
zfs rollback nordix/home/local/steam/proton@working-version
```
Installed games and save data unaffected.

---

## ZFS Hardening Strategy

Nordix implements multiple layers of data protection:

### Layer 1: Metadata Redundancy (All Datasets)
```
redundant_metadata=most
```
Every dataset inherits this from the pool. All metadata blocks (directory entries, file attributes, indirect blocks) are stored with redundant copies. Metadata corruption is automatically repaired.

### Layer 2: Data Redundancy (Critical Datasets)
```
copies=2 on nordix/ROOT/default
copies=2 on nordix/opt
```
Critical system data is stored twice. ZFS can recover from single-block corruption and gains read performance by choosing the faster copy.

### Layer 3: Checksums (All Datasets)
```
checksum=fletcher4
```
Every block is checksummed. ZFS detects silent corruption that would go unnoticed on ext4/NTFS/BTRFS. When combined with `copies=2` or `redundant_metadata=most`, detected corruption is automatically repaired.

### Layer 4: Security Properties
```
exec=off      # No code execution from data directories
setuid=off    # No setuid binaries
devices=off   # No device files
```
Applied to user data datasets (Pictures, Videos, Music, Downloads, cache directories) to prevent malicious files from executing.

### Protection Matrix

| Dataset | Metadata Redundancy | Data Copies | Checksum | Security Flags |
|---------|---------------------|-------------|----------|----------------|
| ROOT/default | ✅ | 2 | fletcher4 | — |
| opt | ✅ | 2 | fletcher4 | — |
| varlib | ✅ | 1 | fletcher4 | setuid=off |
| home | ✅ | 1 | fletcher4 | setuid=off, devices=off |
| home/config | ✅ | 1 | fletcher4 | exec=off, setuid=off, devices=off |
| home/pictures | ✅ | 1 | fletcher4 | exec=off, setuid=off, devices=off |
| tmp | ✅ | 1 | off | exec=off, setuid=off, devices=off |

---

## Useful Commands

```bash
# List all datasets with compression ratios
zfs list -o name,used,compressratio,mountpoint

# Show snapshot space usage
zfs list -t snapshot -o name,used,refer

# Create manual snapshot before risky operation
zfs snapshot nordix/ROOT/default@before-experiment

# Rollback to snapshot
zfs rollback nordix/ROOT/default@before-experiment

# Compare snapshot to current state
zfs diff nordix/home/config@yesterday

# Destroy old snapshots
zfs destroy nordix/ROOT/default@old-snapshot
```

---

## Performance Tuning Notes

### Record Size Selection

| Record Size | Best For |
|-------------|----------|
| 16K | Databases, small random I/O |
| 32K | Config files, mixed workloads |
| 128K | General system files |
| 1M | Game assets, large binaries |
| 4M | Video files, streaming media |

### Compression Selection

| Algorithm | Speed | Ratio | Best For |
|-----------|-------|-------|----------|
| `lz4` | Fastest | Low | Temp, media, already-compressed |
| `zstd-1` | Fast | Medium | General cache, home |
| `zstd-5` | Balanced | Good | System, documents |
| `zstd-10` | Slow | Excellent | Logs, archival |

### Cache Strategy

| Setting | Meaning |
|---------|---------|
| `primarycache=all` | Cache data + metadata in RAM (ARC) |
| `primarycache=metadata` | Cache only metadata (save RAM for other uses) |
| `secondarycache=all` | Use L2ARC (SSD cache) for data |
| `secondarycache=none` | Don't use L2ARC |

---

## License

This configuration is part of the Nordix desktop environment and is released under the GPL-3.0-or-later license.

---

*Nordix and Yggdrasil are registered trademarks of Jimmy Källhagen*
