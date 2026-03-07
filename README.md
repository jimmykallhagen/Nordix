# **Nordix** **_-_** A system for enthusiasts, by enthusiasts.

Performance-focused Arch Linux with tools to empower new users and showcase Linux's true potential. To share the so-called
**"Linux Power User"** experience to everyone and the goal to build a community for us who have systems as a hobby.
- The first "Non Purpose Operating System"
- [**Info** - What is a "Non Purpose OS"](/non-purpose-operating-system.md)

---

## **I strongly reject competing with existing distros**
That's why I have chosen to build my own desktop environment "Yggdrasil"

### If you want a regular desktop environment
I recommend that you then choose one of the already established Arch-based distros<br>
- They are all very good!
- [**List of good Arch distros**](https://wiki.archlinux.org/title/Arch-based_distributions)

## You find Nordix Desktop Environment here:
- [**Yggdrasil**](https://github.com/jimmykallhagen/Nordix-Yggdrasil)

---

## **Nordix principles - Honor the developers** 
* **All credit should go to the developers**

**Nordix's base**
  - [**Arch Linux**](https://github.com/archlinux) - One of the world's largest communities
  - [**GNU Nano**](https://www.nano-editor.org/) - Default in Nordix, with colors!

**Yggdrasil** - _Nordix desktop envirorment_
  - Built on Hyprland
  - [Hyprland's development team](https://github.com/hyprwm)

**Nordix Kernel** - Nordix uses Linux Tachyon as default kernel
  - [**Linux Tachyon**](https://git.staropensource.de/StarOpenSource/Linux-Tachyon) — Fork of Intel Clear Linux kernel patchset
  - Maintained by **JeremyStarTM** and **yarost12**
  - *When Intel abandoned Clear Linux, they kept the dream alive*

> _After Intel announced the shutdown of Clear Linux on July 18th 2025, these two developers refused to let the fastest kernel patchset die.<br>
> Nordix is proud to build upon their work._
>
> *Nordix will carry on the Clear Linux legacy of performance, both in heart and soul.*

- See: [**linux-tachyon-nxbuild**](https://github.com/jimmykallhagen/nordix-custom-builds/blob/main/linux-tachyon-nxbuild.md)

  * [ ] Note - not working yet

### CachyOS has been a great inspiration
- [CachyOS](https://github.com/CachyOS) - One of the fastest distros in the world

**Nordix has borrowed from CachyOS:**
- [Udev-Rules](/udev-rules/borrowed-from-cachyos/rules-borrowed-from-cachyos.md) - _Performance and hardware rules_
- [**Kernel Config**](https://github.com/jimmykallhagen/nordix-custom-builds/blob/main/linux-tachyon-nxbuild.md) - _Base kernel configuration (modified for Nordix)_

---

## Nordix ZFS implementation is built upon:
- [**OpenZFS**](https://github.com/openzfs/zfs) — The open-source ZFS project for Linux and FreeBSD
- [**ZFSBootMenu**](https://github.com/zbm-dev/zfsbootmenu) — Bootloader for ZFS-native systems

### The legacy
- ZFS was originally developed by **Sun Microsystems** for Solaris
- After Oracle's acquisition, the open-source community forked it as **OpenZFS**
- Today it is maintained by a dedicated community of developers

> *The 10 million dollar masterpiece: ZettaByte File System*

---

## **Nordix Performance**
 **Nordix follows the law of performance**
> **"Don't put trash in the CPU's memory pipeline."**<br>
>                                 _Jimmy Källhagen_
> 
- This is the philosophy I had when I built Nordix.
- Aims to provide the most optimized system attainable
- Heavily optimized
- Everything in the system is carefully weighed between performance and function<br>

but with the ideology that performance should not negatively affect function or appearance

**GPU**
 - [**Nordix-GPU**](https://github.com/jimmykallhagen/nordix-gpu)

> _People never want to do their own testing, their own research into what is actually possible.
> But at the same time, everyone knows what doesn't work?_

> These settings are considered 'dangerous' by people who have never tried them.
> 
> I run this daily. It works.
> Try it for yourself and decide."

---

## "Enterprise" for regular PC 
It's like endless super features and systems exist in Linux.<br>
Free for everyone to use, but still hidden behind the big enterprise companies.

**Nordix will change this**
- ISO handles ZFS raid stripe, mirror, slog, l2arc, special vdevs
- Fully optimized zfs.conf - 27 tuned kernel parameters
- 21 datasets - Per-directory recordsize and compression optimization 
- Compressed ARC - effectively multiplies cache capacity by compression ratio
- Automated snapshots
- ZFSBootMenu — usually seen in enterprise deployments, delivered out of the box
 
#### Delivered in a way that everyone can enjoy this

### **ARC** _-_ _The Secret to Performance_
ZFS has a more server-like cache behavior by default, with prioritizing hot and cold data.
I have instead adjusted ZFS ARC to cache aggressively and release quickly, this provides a fairly powerful setup for desktops.
Test on 64GB RAM shows a hit rate of 97%.

**Nordix ZFS Setup
- [**Info on Nordix ZFS Dataset**](/README-zfs-datasets.md)
- [**Info on Nordix ZFS tuning**](/zfs-config/README-zfs.md)
- [**Info on Nordix ZFS udev**](/udev-rules/zfs-specific-rules/README.md)


#### **Virtual Machine**
Nordix will offer VM solutions for everyone

- Run Windows on ZFS without needing a Virtio drive, ZVOL offers the solution to give Windows a "real" NVMe to install NTFS on.<br>
The only difference is that it is a ZFS block, so you get ARC caching, checksums, snapshots on Windows.
- VM should be so good, and easily accessible.
 - ![puppetmaster](/image/puppet-master-windows.png)
### Will be finalized later after ISO is released and Nordix is fully functional and tested
- [ ] GUI for a guided setup with ZVOL, ready-made templates for the common operating systems
- [ ] I have developed a system where I use a ZVOL to share files between host and VM, without having to mess with samba, ssh, virtio file system, iso or similar. Just put your files in a folder, run the process, then done

#### Docker
- Nordix comes with ZFS drivers and options that make Docker use ZFS.
Containers were actually invented by Sun Microsystems and have good compatibility with ZFS.

- [**Docker on ZFS**](https://github.com/jimmykallhagen/nordix-zfs/blob/main/docker/README.md)

---

### **Nordix Graceful Shutdown**
Nordix uses very aggressive memory and cache settings.<br>
I haven't noticed any problems myself, but I consider it responsible to handle this correctly.

[**Info - Graceful Shutdown**]([nordix-graceful-shutdown/README.md](https://github.com/jimmykallhagen/nordix-graceful-shutdown)) 

---

## To Do
Everything takes so much time, once you start documenting everything.
This list is not complete, as I will have to spend as much time writing the list as fixing it.
- [ ] So much
- [ ] Continue expanding the lists of developers in "Nordix principles - Honor the developers"
- [ ] VM setup, needs to be fully written
- [ ] ISO exists but needs a complete overhaul
- [ ] Nordix repo for custom built packages: nordix-systemx

### Nordix SystemX 
**This is not fully developed but just a sharing of my own tests and ideas.**
- [Nordix-SystemX](README-systemx.md) - _An idea for a repo_

--- 


## **My Enthusiast View on Nordix and Yggdrasil**

- **The purpose of truth - a desire to find friends**

### **I have a vision of a platform to unite enthusiasts**

- There are many enthusiasts who spend all their time on their projects, thousands of hours.
- Projects that are so good that you can't believe it's true!
- Sadly, many of these projects will never find a place in a mainstream Linux distro.

### **Nordix is what I'm building with the vision to change this**

Instead of projects ending up in a corner on AUR and never getting a fair chance,
I would like to create a platform where we can all share our common interests.
I am not a project manager, I have built Nordix to be your host.
I don't want other people's projects to be swallowed up by Nordix and the developers to be hidden and forgotten.

- I therefore want everyone who wants to participate with their projects to keep their own repos.
- I want everyone to continue to be project leaders for their own projects, the only difference is that they have been given a home: "Nordix"
- Instead, I plan to list each project participant's project first in the Nordix repo, with thanks for the contribution and link to the original repo
- I have a vision of us creating our own platform where we can discuss our interests.

> **"Even though the project is free of charge,<br>
> we are all responsible for paying by showing respect."**<br>
> _Jimmy Källhagen – Nordix_

---

> ### **However, I can say that I am deeply serious about Nordix and I also expect those who want to participate to be so too. I expect a certain standard. Nordix is a very serious _"Non Purpose OS"_**
