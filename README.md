---
<p align="center">
  <img src="https://github.com/jimmykallhagen/nordix-fanart/blob/main/nx-tux/nordix-NX-tux1.png">
</p>

---
![nordix-icon](/icons/hicolor/128x128/apps/nordix.png)
# **Nordix Linux**
**Nordix brings Enterprise-grade Linux to Enthusiasts - Who Deserve More Than "Good Enough"**
> **A "Linux Power-User" Experience For Everyone.**
---

## What is Nordix Linux?
 - Opinionated, Arch Linux-based operating system
 - ZFS-only
 - ZFSBootMenu as default
 - Strong focus on performance
 - Minimalistic, yet without sacrificing either function or the visual aspect
 - Custom-configured root system for optimal performance and functionality
 - Dual desktop system

 > **Nordix design philosophy:** _Don't put trash in the CPU's memory pipeline._

---

### What is Nordix designed to be?
Nordix is not designed to suit everyone, nor for the enterprise, nor for weak computers, nor for old computers, nor for work.
Although it is entirely possible to use Nordix with good results in these categories, that is not its intended purpose.

Nordix is designed to be an operating system for enthusiasts-for those who treat computers as a hobby, and who enjoy spending their money on powerful hardware.
This is the system designed to give their hardware the justice it deserves.

That said, the Nordix installer offers pre-optimized setups even for systems with as little as 8GB of RAM; it is perfectly functional, though you get the full "Nordix" experience on more powerful systems.

> _**"Nordix follows the law of performance."**_

---

### Nordix ZFS setup
  - Beyond the root dataset, there are 21 additional datasets mounted for the system.
  - Auto snapshots
  - Smart snapshot layout for all datasets-to save space, but also, importantly, to provide a secure rollback option.
  - All datasets have been tested and measured using custom-developed tools in order to offer the most optimal record sizes and compression methods possible.
  - ARC carefully tuned to shift ZFS from its default server-like behavior to better suit a modern desktop, making the distinction between hot and cold data far less noticeable in Nordix.

Read more about how Nordix tunes the ZFS system here: </br>
[Placeholder - How Nordix Tunes ZFS](soon/after/the/installer/is/finished)

**Also worth mentioning**
ZFS offers various parameters that show HDDs in a whole new light!

  > Yes! It works perfectly fine to run Nordix on a zpool consisting of HDDs.
  > A video demonstration of running Nordix on an HDD and Special vdevs with low latency will be available later.

Read more about how Nordix set ZFS parameters and why here: </br>
[Placeholder - Nordix ZFS parameters and What They Mean for HDDs](soon/after/the/installer/is/finished)

---

## Fearless Experimentation - Nordix's Got Your Back!
Nordix is not safe because it is careful. Most systems manage risk by being conservative. Nordix manages risk by making the consequences of failure irrelevant.
  - Run a custom kernel?
  - Test breaking changes?
  - Experiment freely with your system?

**If something goes wrong:**
  - Reboot into ZFSBootMenu.
  - Roll back to an automatically taken snapshot.

**The entire process takes seconds-or create a clone of a previous snapshot.**
If you don't want to roll back, several other options are available:
  - Fix the errors directly using ZFSBootMenu's automatic chroot function.
  - Fix the errors directly using ZFSBootMenu's automatic rescue shell.
  - Check zpool status.
  - Edit the kernel command line.

**Want to isolate an experiment completely?**
> Create a clone of your root dataset directly from ZFSBootMenu and boot into it.
> Your actual installation remains untouched while you test whatever you want inside the clone.
> This is not a recovery feature. It is the intended workflow-and it is faster, simpler, and gives you your exact system environment, unlike Docker containers.

> Clone your system (using zero extra disk space), mount it, and use `arch-chroot`-instantly giving you a powerful container. This is the exact same core mechanism behind Solaris Zones.

Nordix inverts traditional distribution logic. Instead of protecting you from risk, Nordix gives you the tools to make risk irrelevant.

---
---

## You can read more about the ZFS and the ZFSBootMenu here:
[OpenZFS Github](https://github.com/openzfs) </br>
[OpenZFS Documentation](https://openzfs.github.io/openzfs-docs/) </br>
[ZFSBootmenu Documentation](https://docs.zfsbootmenu.org/en/v3.1.x/) </br>
[ZFSBootmenu Github](https://github.com/zbm-dev) </br>

---
---

## Modular Desktop Architecture – Zero Bloat, Total Control

Nordix rejects monolithic desktop environments like GNOME and KDE Plasma-not because your hardware can't handle them, but because tangled dependency chains and rigid configurations get in the way of performance and freedom.

Instead, Nordix is built on a **strictly modular Unix philosophy**:
* **Pure Compositors:** Choose between **Umbriel** (high-speed tiling/scrolling) or **Labwc** (bulletproof stacking).
* **Decoupled Shell:** The entire desktop experience runs on top via **Noctalia Shell**-launched as a single, independent process (`noctalia &`).
* **Radical Simplicity:** Want to swap out the shell, run a custom bar, or strip it down to bare metal? Just comment out one line. No broken systemd targets, no hidden daemons, no friction.

 ![Umbriel.png](icons/umbriel.svg)
#### **Umbriel**
Complete tiling window manager  that can handle both scrolling, master and dwiling lauot. 
This is next-level quality code! You can’t help but be impressed!

  [<kbd> <br>**READ MORE ABOUT UMBRIEL HERE **<br> </kbd>](https://github.com/noctalia-dev/umbriel)</br>

 ![labwc.png](icons/labwc.png)
#### **Labwc**
*The friend that never lets you down – it is highly efficient, high-performing, and exceptionally stable
When nothing else works, labwc does`
 
  [<kbd> <br>**READ MORE ABOUT LABWC HERE**<br> </kbd>](https://github.com/labwc/labwc)</br>

![noctalia](icons/noctalia.svg)
#### **Noctalia**
Noctalia is the shell that takes both labwc and umbrial to full desktop sessions.
Offers a high degree of customization, comes with built-in support for hundreds of plugins, dynamic themes that span your entire desktop, and much more;
it delivers a sophisticated, modern look that truly stands out-a next-level Linux desktop experience.

  [<kbd> <br>**READ MORE ABOUT NOCTALIA HERE**<br> </kbd>](https://github.com/noctalia-dev/noctalia)</br>

#### **Both Noctalia and Umbriel is developed by the Noctalia-Dev**
For more info about Noctalia and Umbriel:
[**--https://noctalia.dev--**](https://noctalia.dev)


> Two Equal Workflows: Start with Labwc for a familiar, bulletproof stacking desktop, or jump straight into Umbriel for high-speed tiling and scrolling.
> Switch environments in seconds right from the login screen without losing your setup.

---


### Labwc – Bulletproof Stacking in a Modern Suit

> *"Loved for its lightweight speed and rock-solid foundation, Labwc has become the go-to choice for projects migrating from X11. Nordix takes a fresh approach-presenting Labwc in a modern suit few would ever associate with it."*

Powered by **Noctalia Shell**, it gives you a traditional stacking workflow with the exact same tools, themes, and zero-bloat modularity as Umbriel.

![Nordix Labwc Desktop](screenshots/din-labwc-skarmdump.png)


---

### Umbriel – Built to Be Worthy of the Foundation Beneath It

> *"Beneath the surface lies raw Nordic ZFS engineering. On top sits Umbriel-a newcomer that already demonstrates the quality and craftsmanship needed to earn its place above ZFS."*

![Nordix Umbriel  Desktop](screenshots/din-umbriel-skarmdump.png)



---

---
 # **VISION**
---

**_Nordix represents over 4,000 hours of focused engineering work, testing, and learning-more than 1.5 years of full-time effort._**

I started Nordix because I believe the most powerful features in Linux should not be locked behind years of experience.
Technologies like ZFS, boot environments, automatic snapshots, hardware-optimized kernel tuning, zvol-backed virtual machines, and advanced container systems exist today, but setting them up correctly requires deep technical knowledge and dozens of hours of research.
That is the problem Nordix solves-not by simplifying the technology, but by doing the engineering work up front so the user gets the full benefit without the complexity.

> Every configuration in Nordix is built with intent. ZFS ARC parameters are tuned per RAM tier with documented rationale for each value. Dataset hierarchies are designed around real desktop usage patterns, separating game files from configs and caches so that snapshots and rollbacks are surgical, not destructive. VM templates are benchmarked across different block sizes and caching strategies to find the actual optimal setup, not just commonly repeated advice.

My interest is in building systems where enterprise-grade functionality becomes invisible infrastructure-it is just there, working, for everyone. That is what drives this project, and it is the kind of work I want to do professionally.

---

## Core Contributors: A Vision of a Truly Enthusiast-Driven System

My own passion and primary focus is the core system-specifically **ZFS architecture, root optimization, and low-level performance**.
Rather than trying to build every single user-space component alone, I want to partner with a small group of skilled, independent enthusiasts who can take true ownership of the areas where they excel.
By this, I do not mean having 20 different projects competing for the same part of the system, nor am I looking to build a massive crowd of casual contributors.

**What I am looking for:**
- A few highly capable people who can take independent responsibility for specific parts of Nordix (such as desktop components, tooling, or packaging).

If you have deep technical knowledge in a specific domain and can work independently, you can own that responsibility in Nordix.

> Many great projects sit hidden in a corner of the AUR or a remote Git repository.
> There is pride in building something great-and you shouldn't have a distro take the credit while you are forgotten as just another name in a crowd of contributors.

If you have built a strong project that fits Nordix's philosophy and want to step in as the maintainer for that part of the system, bring it with you. You remain the leader of your project, and Nordix will showcase and link directly to your repository at the top of Nordix's main GitHub page. Your pride and ownership will never be taken away from you.

> I personally want to show the world just how great an operating system built by a few passionate, skilled engineers can be.

---

### General Contributions
In addition to core maintainers, everyone is welcome to contribute regardless of experience level-including, but not limited to:
  - Report bugs
  - Help the core contributors
  - Propose ideas
  - Test and benchmark different setups
  - Publish YouTube videos
  - Create apps
  - Create scripts and wrappers to improve usability
  - Fine-tune and polish the UI
  - Build and maintain packages
  - Write guides and documentation
  - Spread the word to grow the community

**Looking Ahead (GUI Development):**
A native GUI is needed to bring all ZFS features together in a practical, user-friendly interface so that ZFS truly feels like an integrated part of the desktop.
- I am currently learning `Rust` and believe `egui` is a strong choice for building these GUIs.

---

## About
Once the Nordix installer is complete, the Nordix main GIT repository will be moved to:
`https://gitlab.com/Nordix-Linux`


**Nordix is created by:**
- Jimmy Källhagen

**Core maintainers:**
- Jimmy Källhagen

**Contributors**

---

## **Nordix Repo's**

![nordix-icon](/icons/hicolor/128x128/apps/nordix.png)

  [<kbd> <br> **NORDIX INSTALLER** <br> </kbd>](https://github.com/jimmykallhagen/nordix-installer)</br>

  [<kbd> <br> **NORDIX ZFS SETUP/CONFIGURATION** <br> </kbd>](https://github.com/jimmykallhagen/nordix-zfs)</br>

  [<kbd> <br> **NORDIX PERFORMANCE: GPU, CPU, MEMORY, KERNEL** <br> </kbd>](https://github.com/jimmykallhagen/nordix-performance)</br>

  [<kbd> <br> **NORDIX GRACEFUL SHUTDOWN** <br> </kbd>](https://github.com/jimmykallhagen/nordix-graceful-shutdown)</br>

  [<kbd> <br> **NORDIX CUSTOM COMPILE "SYSTEMX"** <br> </kbd>](https://github.com/jimmykallhagen/nordix-systemx)</br>

  [<kbd> <br> **NORDIX OPTIMIZED ANANICY-CCP** <br> </kbd>](https://github.com/jimmykallhagen/nordix-optimized-ananicy-cpp)</br>

  [<kbd> <br> **NORDIX ZFS DESTROY SNAPSHOTS** <br> </kbd>](https://github.com/jimmykallhagen/zfs-destroy-snapshots)</br>

  [<kbd> <br> **NORDIX PACMAN WRAPPER BULK REMOVE** <br> </kbd>](https://github.com/jimmykallhagen/pacman-bulk-Rns)</br>


---
 ![nordix-tools-icon](https://github.com/jimmykallhagen/Nordix/blob/main/icons/hicolor/128x128/apps/nordix-term.png)

  [<kbd> <br> **NORDIX PACKAGE SEARCH** <br> </kbd>](https://github.com/jimmykallhagen/nordix-package-search)</br>

  [<kbd> <br> **NORDIX TOOLS** <br> </kbd>](https://github.com/jimmykallhagen/nordix-tools)</br>

  [<kbd> <br> **NORDIX CLI-TOOLS** <br> </kbd>](https://github.com/jimmykallhagen/nordix-cli)</br>

  [<kbd> <br> **NORDIX CLI-ZFS** <br> </kbd>](https://github.com/jimmykallhagen/nordix-cli-zfs)</br>

  [<kbd> <br> **NORDIX CLI-ARCH** <br> </kbd>](https://github.com/jimmykallhagen/nordix-cli-arch)</br>


---

  [<kbd> <br> **NORDIX CHEATSHEET** <br> </kbd>](https://github.com/jimmykallhagen/nordix-cheatsheet)</br>

---
![HHD-oooH-HHD](https://github.com/jimmykallhagen/nordix-zfs/blob/main/nordix-hhd.png)

  [<kbd> <br> **NORDIX ZGUIDE TO ZFS** <br> </kbd>](https://github.com/jimmykallhagen/nordix-ZGuide_guide-to-zfs)</br>

  [<kbd> <br> **NORDIX-VIRTUAL MACHINES AND ZFS** <br> </kbd>](https://github.com/jimmykallhagen/nordix-vm)</br>

  [<kbd> <br> **NORDIX: A THEORETICAL FRAMEWORK FOR ZFS_BACKEND MEMORY EXTENSION IN LOCAL AI INFERENCE** <br> </kbd>](https://github.com/jimmykallhagen/nordix-ai-memory)</br>

---

## Licensing
Nordix is licensed under the **GNU General Public License, version 3 or later**

`SPDX-License-Identifier: GPL-3.0-or-later`</br>
`Copyright (c) 2025- The Nordix Authors`
 
---

