<div style="text-align:center"><img src="https://github.com/jimmykallhagen/nordix-fanart/blob/main/nordix-magic/nordix-is-magic.png" /></div>
# **Nordix**

**Enterprise-grade Linux | Linux Power User Experience For Everyone.**

# Vision

**_Nordix represents over 3000 hours of focused engineering work - more than a year of full-time effort._**

I started Nordix because I believe the most powerful features in Linux should not be locked behind years of experience. Technologies like ZFS, boot environments, automatic snapshots, hardware-optimized kernel tuning, and zvol-backed virtual machines exist today, but setting them up correctly requires deep technical knowledge and dozens of hours of research.

That is the problem Nordix solves. Not by simplifying the technology, but by doing the engineering work up front so the user gets the full benefit without the complexity.

Every configuration in Nordix is built with intent. ZFS ARC parameters are tuned per RAM tier with documented rationale for each value. Dataset hierarchies are designed around real desktop usage patterns, separating game files from configs from caches so that snapshots and rollbacks are surgical, not destructive. VM templates are benchmarked across different block sizes and caching strategies to find the actual optimal setup, not just the commonly repeated advice.

My interest is in building systems where enterprise-grade functionality becomes invisible infrastructure, it is just there, working, for everyone. That is what drives this project, and it is the kind of work I want to do professionally.

> _One of my goals is to create a community where you can gather all enthusiasts and get a home for everyone's projects, where Nordix acts as a platform like a host instead of a project leader._
>
> _My idea is that you should be able to continue to have your own repos and retain ownership - if you only want Nordix to link these projects at the top of this repo, that works too._

- _**Jimmy Källhagen**_

---

- Nordix is not safe because it is careful
> Most systems manage risk by being conservative. Nordix manages risk by making the consequences of failure irrelevant.
> Run bleeding edge kernels. Test breaking changes. Experiment freely with your system. If something goes wrong --> reboot and roll back to an automatically taken snapshot via ZFSBootMenu. The entire process takes seconds or create a clone of a previous snapshot --> boot clone mount your system and fix the errors in chroot, you can also fix the errors directly from zfbootmenu automatic chroot function

- Want to isolate an experiment completely?
> Create a clone of your root dataset directly from ZFSBootMenu and boot into it.
> Your actual installation remains untouched while you test whatever you want inside the clone.
> This is not a recovery feature. It is the intended workflow and it is faster, simpler and your exact system environment unlike dockers.

Nordix inverts the traditional distribution logic. Instead of protecting you from risk, Nordix gives you the tools to make risk irrelevant.

---

**Nordix** is a complete Arch Linux distribution built on ZFS. It takes the kind of system architecture, data integrity, and performance tuning normally found in enterprise environments, and makes it available to anyone, including people who have never used Linux before.

Nordix is the first - _“Non purpose operating system”_    </br>

  [<kbd> <br> **NON PURPOSE OPERATING SYSTEM?** <br> </kbd>](https://github.com/jimmykallhagen/Nordix/blob/main/non-purpose-operating-system.md)</br>
---

## **Nordix Repo's**

![nordix-icon](/icons/hicolor/128x128/apps/nordix.png)

  [<kbd> <br> **NORDIX ZFS SETUP/CONFIGURATION** <br> </kbd>](https://github.com/jimmykallhagen/nordix-zfs)</br> 

  [<kbd> <br> **NORDIX PERFORMANCE: GPU, CPU, MEMORY, KERNEL** <br> </kbd>](https://github.com/jimmykallhagen/nordix-performance)</br> 


---
![yggdrasil-icon](https://github.com/jimmykallhagen/Nordix/blob/main/icons/hicolor/128x128/apps/yggdrasil.png)

  [<kbd> <br> **YGGDRASIL DESKTOP ENVIRONMENT** <br> </kbd>](https://github.com/jimmykallhagen/yggdrasil-DE)</br> 

  [<kbd> <br> **NORDIX HYPRLAND MIRRORS** <br> </kbd>](https://github.com/jimmykallhagen/noridx-hyprland-mirrors/tree/main)</br> 

  [<kbd> <br> **NORDIX GRACEFUL SHOTDOWN** <br> </kbd>](https://github.com/jimmykallhagen/nordix-graceful-shutdown)</br> 

  [<kbd> <br> **NORDIX FANART/WALLPAPER** <br> </kbd>](https://github.com/jimmykallhagen/nordix-fanart/blob/main/README.md)</br> 


---
 ![nordix-tools-icon](https://github.com/jimmykallhagen/Nordix/blob/main/icons/hicolor/128x128/apps/nordix-term.png)
 
  [<kbd> <br> **NORDIX PACKAGE SEARCH** <br> </kbd>](https://github.com/jimmykallhagen/nordix-package-search)</br> 

  [<kbd> <br> **NORDIX TOOLS** <br> </kbd>](https://github.com/jimmykallhagen/nordix-tools)</br> 

  [<kbd> <br> **NORDIX CLI-TOOLS** <br> </kbd>](https://github.com/jimmykallhagen/nordix-cli)</br> 

---

  [<kbd> <br> **NORDIX CHEATSHEET** <br> </kbd>](https://github.com/jimmykallhagen/nordix-cheatsheet)</br> 

---
![HHD-oooH-HHD](https://github.com/jimmykallhagen/nordix-zfs/blob/main/nordix-hhd.png)

  [<kbd> <br> **NORDIX ZGUIDE TO ZFS** <br> </kbd>](https://github.com/jimmykallhagen/nordix-ZGuide_guide-to-zfs)</br> 

  [<kbd> <br> **NORDIX-VIRTUAL MACHINES AND ZFS** <br> </kbd>](https://github.com/jimmykallhagen/nordix-vm)</br> 

  [<kbd> <br> **NORDIX: A THEORETICAL FRAMEWORK FOR ZFS_BACKEND MEMORY EXTENSION IN LOCAL AI INTERFERENCE** <br> </kbd>](https://github.com/jimmykallhagen/nordix-ai-memory)</br> 

---
_This is an ongoing project and I'm working on getting everything up on git, I have a version of the iso but it is also being rebuilt to include everything._

---


## What Nordix Is

Nordix is a brand name for a family of systems. The first and primary release is **Nordix Yggdrasil**.

---

## Yggdrasil Desktop Environment

Yggdrasil is a full desktop environment built on Hyprland. It is designed so that you can manage your entire desktop environment from a graphical interface without touching a single dotfile.

This is not a Hyprland rice or a config collection. It is a complete desktop environment.

  [<kbd> <br> **YGGDRASIL DESKTOP ENVIRONMENT** <br> </kbd>](https://github.com/jimmykallhagen/yggdrasil-DE)</br> 

---

## **Screenshots** _**:**_ Nordix Yggdrasil - _With Nordix Dynamic Theme_
1. 
![1](https://github.com/jimmykallhagen/Nordix/blob/main/screenshots/Screenshot-Sat%20Apr%2025%2004%3A11%3A59%20PM%20UTC%202026.png)
2. 
![2](https://github.com/jimmykallhagen/Nordix/blob/main/screenshots/Screenshot-Sat%20Apr%2025%2003%3A45%3A08%20PM%20UTC%202026.png)
3.
![3](https://github.com/jimmykallhagen/Nordix/blob/main/screenshots/Screenshot-Sat%20Apr%2025%2003%3A47%3A36%20PM%20UTC%202026.png)
4.
![4](https://github.com/jimmykallhagen/Nordix/blob/main/screenshots/Screenshot-Tue%20Apr%2021%2009%3A14%3A01%20PM%20UTC%202026.png)
5.
![5](https://github.com/jimmykallhagen/Nordix/blob/main/screenshots/Screenshot-Sat%20Apr%2025%2007%3A40%3A13%20PM%20UTC%202026.png)
6.
![6](https://github.com/jimmykallhagen/Nordix/blob/main/screenshots/Screenshot-Sat%20Apr%2025%2007%3A40%3A29%20PM%20UTC%202026.png)
7.
![7](https://github.com/jimmykallhagen/Nordix/blob/main/screenshots/Screenshot-Sat%20Apr%2025%2007%3A47%3A07%20PM%20UTC%202026.png)
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
 
Whether you are an experienced Linux user or a Windows power user ready to make the switch - Nordix gives you the full experience from day one.

> _A Linux power user experience for everyone_

---

## Project Status

Nordix is in active development. 
Focus right now is to get everything up on Github and be able to get Yggdrasil as a working DE to install from AUR. ISO is built and I myself have been running Nordix with this iso since August 2025, however it is now no longer relevant and this will be prioritized soon, to update it with the latest Nordix - Yggdrasil

---

## Contribute

Nordix is a meeting place for system enthusiasts, a home for those who find joy in understanding how systems work.

**Join Nordix**
  - contribute to the core system through development, testing, ideas, documentation or content creation. No prior project needed, just passion and curiosity.

**Bring your project**
  - if you have a project you believe in, you can give it a home within Nordix. You keep your name, your repo, and your leadership. Nordix is the host, not the owner. You decide if your project becomes part of Nordix or stays independent but linked. The only requirement is that contributions are wholehearted. Nordix holds a high standard, not of experience, but of passion.

We don’t have a community chat yet. When the first contributors arrive, we’ll decide together where to meet.

---

## Licensing
Nordix is licensed under the **GNU General Public License, version 3 or later**

`SPDX-License-Identifier: GPL-3.0-or-later`</br>
`Copyright (c) 2025- The Nordix Authors`
 
---

## About

Nordix is created and maintained by **Jimmy Källhagen**.

- GitHub: [github.com/jimmykallhagen](https://github.com/jimmykallhagen)
