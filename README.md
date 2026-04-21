

# ![nordix-icon](/icons/hicolor/128x128/apps/nordix.png) **Nordix**

**Enterprise-grade Linux, accessible to everyone.**

Nordix is a complete Arch Linux distribution built on ZFS. It takes the kind of system architecture, data integrity, and performance tuning normally found in enterprise environments, and makes it available to anyone, including people who have never used Linux before.

Nordix is the first "Non pourpuse oprating system"
- [**Waht is a "Non porpuse operating system?**](https://github.com/jimmykallhagen/Nordix/blob/main/non-purpose-operating-system.md)
---

## Vision

I started Nordix because I believe the most powerful features in Linux should not be locked behind years of experience. Technologies like ZFS, boot environments, automatic snapshots, hardware-optimized kernel tuning, and zvol-backed virtual machines exist today, but setting them up correctly requires deep technical knowledge and dozens of hours of research.

That is the problem Nordix solves. Not by simplifying the technology, but by doing the engineering work up front so the user gets the full benefit without the complexity.

Every configuration in Nordix is built with intent. ZFS ARC parameters are tuned per RAM tier with documented rationale for each value. Dataset hierarchies are designed around real desktop usage patterns, separating game files from configs from caches so that snapshots and rollbacks are surgical, not destructive. VM templates are benchmarked across different block sizes and caching strategies to find the actual optimal setup, not just the commonly repeated advice.

My interest is in building systems where enterprise-grade functionality becomes invisible infrastructure, it is just there, working, for everyone. That is what drives this project, and it is the kind of work I want to do professionally.


>_One of my goals is to create a community where you can gather all enthusiasts and get a home for everyone's projects, where Nordix acts as a platform like a host instead of a project leader._
>_My idea is that you should be able to continue to have your own repos and maintain your honor and if you then only want Nordix to link these projects at the top of this repo_

- **Jimmy Källhagen**

---

## What Nordix Is

Nordix is a brand name for a family of systems. The first and primary release is **Nordix Yggdrasil**.

---

## Yggdrasil Desktop Environment

Yggdrasil is a full desktop environment built on Hyprland. It is designed so that you can manage your entire Arch Linux system, every setting, every service, every tool, from a graphical interface without touching a single dotfile.

This is not a Hyprland rice or a config collection. It is a complete desktop environment.

Future Nordix editions will expand to additional desktop environments including KDE Plasma, GNOME, Cinnamon, and COSMIC. Yggdrasil is the flagship.

---

## ZFS - Made Accessible

Nordix is ZFS only. Every installation runs on a carefully designed ZFS setup that includes:

- **Automatic snapshots** with intelligent policies, ephemeral data like caches and shader files are excluded, while system state, configs, documents, and game saves are protected
- **GUI tools** for managing snapshots, rollbacks, and pool health without opening a terminal
- **Purpose-tuned dataset hierarchy**  over 25 datasets with workload specific record sizes, compression algorithms, checksum policies, and security flags
- **Desktop-optimized ARC configuration** - five RAM-tier profiles (8 GB through 128 GB) that shift ZFS from server defaults to aggressive desktop caching with compressed ARC, tuned I/O parallelism, and NVMe aware allocation
- **ZFSBootMenu** for boot environment management
- **Full installer** offering stripe, mirror, RAIDZ, special vdev, and L2ARC configurations

The goal is that anyone can run ZFS with all of its data integrity, snapshot, and performance capabilities, without needing to understand it first.

---

## Performance

Nordix is performance focused. The system ships with hardware specific optimized configurations for CPU scheduling, memory management, I/O behavior, and GPU tuning. Multiple kernel tuning profiles are available for different workloads. These are not generic tweaks each parameter is documented with its default value, the Nordix value, and the reasoning behind the change.

---

## Virtual Machines

Nordix includes guided VM setup with ZFS zvol backed storage, benchmarked volblocksize recommendations, and optimized virsh templates for different hardware configurations (AMD/Intel, DDR4/DDR5). The documentation covers not just how to set up a VM, but why each configuration choice matters building understanding, not just following steps.

---

## Who Nordix Is For

Nordix is for anyone who wants a powerful, well engineered Linux system without having to build it themselves. It is designed with enthusiasts in mind, but built so that newcomers can use it from day one. 

> _A Linux power user experience for everyone_

---

## Project Status

Nordix is in active development. Yggdrasil is the current focus.

---

## Licensing
Nordix is licensed under the **GNU General Public License, version 3 or later**

`SPDX-License-Identifier: GPL-3.0-or-later`
`Copyright (c) 2025- The Nordix Authors`
 
Nordix was licensed under [PolyForm Noncommercial 1.0.0](https://polyformproject.org/licenses/noncommercial/1.0.0/). But i have started to change it to GPL v3.

---

## About

Nordix is created and maintained by **Jimmy Källhagen**.

- GitHub: [github.com/jimmykallhagen](https://github.com/jimmykallhagen)
