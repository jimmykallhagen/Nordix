 # **Nordix** **_-_** A system for enthusiasts, by enthusiasts.

 Performance-focused Arch Linux with tools to empower new users and showcase Linux's true potential. To share the so-called
 **"Linux Power User"** experience to everyone and the goal to build a community for us who have systems as a hobby.
- The world's first "Non Purpose Operating System"

---

### **Nordix follows the law of performance** *-->*
> **"Don't put trash in the CPU's memory pipeline."**<br>
> _Jimmy Källhagen_
> 
- This is the philosophy I had when I built Nordix.

---

## **Nordix Performance**
- Aims to provide the most optimized system attainable 
- Performance first, no compromise - But
- Performance should not negatively affect function or appearance
- Everything in the system is carefully weighed between performance and function


---

## "Enterprise" for regular PC 
It's like endless super features and systems exist in Linux
Free for everyone to use, but still hidden behind the big enterprise companies.

 **Nordix will change this**
- ISO handle zfs raid stripe, mirror, slog, l2arc, special vdevs.
- Fully optimazed zfs.conf - 27 tuned kernel parameters
- 21 dataset - Per-directory recordsize and compression optimization 
- Compressed ARC - effectively multiplies cache capacity by compression ratio
- Automated snapshots
- ZFSBootMenu — usually seen in enterprise deployments, delivered out of the box"
- Delivered in a way that everyone can enjoy this

### **ARC** _-_ _The Seacret to Performance_
ZFS has a more server-like cache behavior by default, with prioritizing hot and cold data
I have instead adjusted zfs ARC to cache and release cache in instances, this provides a fairly powerful setup for desktops.
Test on 64gb ram shows a hitrate of 97%

[Info on Nordix zfs tuning](/zfs-config/README-zfs.md)

#### **Virtual Machine**
Nordix will offer VM solutions like no one has ever seen before.
With the help of ZFS and a little new thinking, I can guarantee that VM has never been so good before

- GUI for a guided setup with z-vol, ready-made templates for the common operating systems
- Run Windows on ZFS without needing a Virtio drive, ZVOL offers the solution to give Windows a "real" nvme to install ntfs on.<br>
The only difference is that it is a zfs block, so you get ARC caching, checksums, snapshots on Windows.

- VM should be so good, and easily accessible that you never ever need anything other than Nordix

- [ ] I have developed a system where I use a zvol to share files between host and VM. without having to mess with<br>
samba, ssh, virtio file system, iso or similar. just put your files in a folder, run the process, then done

#### Docker
- Nordix comes with zfs drivers and options that makes docker use zfs
Containers were actually invented by Sun Microcontroller and have good compatibility with ZFS.

---

### **Nordix Graceful Shutdown**

Nordix uses very aggressive memory and cache settings.<br>
I haven't noticed any problems myself, but I consider it responsible to handle this correctly

[**Info - Graceful Shutdown**](nordix-graceful-shutdown/README.md) 

---
  
## To do
Everything takes so much time, once you start documenting everything,
This list is not complete, as I will have to spend as much time writing the list as fixing it
- [ ] So much
- [ ] VM setup, need to be fully written
- [ ] ISO finns men behöver en helt ombyggnad.

--- 

## **My Entusiast View on Nordix and Yggdrasil**

**Nordix is ​​many things**
- It is rebellious, it is the battleship from the north.
- Show the world what linux can actually do, a warrior to spread justice and freedom
- It is a steatmenet 

- _But...._

## **The purpose of truth - a desire to find friends**

###  **I have a vision of a platform to unite enthusiasts**

- There are many enthusiasts who spend all their time on their projects, thousands of hours.
- Projects that are so good that you can't believe it's true!
- Sadly, many of these projects will never find a place in a mainstream Linux distro.

### **Nordix is ​​what I'm building with the vision to change this**

Instead of projects ending up in a corner on aur and never getting a fair chance. 
I would like to create a platform where we can all share our common interests.

> **"I am not a project manager, I have built Nordix to be your host."**<br>
>  *Jimmy Källhagen - Nordix*

I don't want other people's projects to be swallowed up by Nordix and the developers to be hidden and forgotten.

- I therefore want everyone who wants to participate with their projects to keep their own repos.
- I want everyone to continue to be project leaders for their own projects, the only difference is that they have been given a home "Nordix"
- Instead, I plan to list each project participant's project first in the Nordix repo, with thanks for the instagtsen and link to the original repo
- I have a vision of us creating our own platform where we can discuss our interests.
---

> ### **However, I can say that I am deeply serious about Nordix and I also expect those who want to participate to be so too, I demand a certain standard. Nordix is ​​a very serious _"Non purpose OS"_**

