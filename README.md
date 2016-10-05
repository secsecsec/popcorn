# <b> Popcorn Linux</b>
===============================
## <b> Install</b>
---------------------------------
Popcorn Linux is multikernel OS. This System boot second kernel after master kernel.<br>
Our Test Environment as follows.<br>

<Center>Environment | <Center>Type
-------- |:-----------
<center>Hardware | <center>qemu-system-x86_64
<center>OS | <center>Ubuntu 12.04 server
<center>Memory (RAM)| <center>2048MB
<center>CPUs | <center>2

### 1.1 Install Kernel
<b>Download Popcorn Linux Patch File</b><br>
$ cd ~/<br>
$ git clone https://github.com/packetngin/popcorn.git<br>

<b>Install Package for Kernel Compile</b><br>
$ sudo apt-get update<br>
$ sudo apt-get install make libncurses5-dev git gawk

<b>Download Linux 3.2.14 Kernel<br></b>
$ cd ~/<br>
$ mkdir linux<br>
$ cd linux<br>
$ wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.2.14.tar.gz<br>
$ tar -zxf linux-3.2.14.tar.gz<br>

<b>Patch Kernel Source and Compile Kernel</b><br>
$ cd ~/linux-3.2.14<br>
$ patch -p1 < ~/popcorn/PopcornLinux.patch<br>
$ sudo make menuconfig<br><br>
<b>Load benconfig file and Exit.</b>
$ sudo make<br>
$ sudo make modules<br>
$ sudo make modules_install<br>
$ sudo make install


### 1.2 Install kexec
<b>Download kexec 2.0.3</b><br>
$ mkdir ~/kexec<br>
$ cd ~/kexec<br>
$ wget https://www.kernel.org/pub/linux/utils/kernel/kexec/kexec-tools-2.0.3.tar.gz<br>
$ tar -zxf kexec-tools-2.0.3.tar.gz<br>

<b>Patch and Install kexec</b><br>
$ cd kexec-tools-2.0.3<br>
$ patch -p1 < ~/popcorn/mk_kexec.patch<br>
$ ./configure<br>
$ make<br>
$ sudo make install<br>
$ sudo shutdown -r now<br><br>
<b>Select linux-3.2.14 from grub menu<br></b>

## 2. Setup
### 2.2 Setup Boot Parameter
$ cd ~/popcorn/popcornlinux_utils<br>
$ make<br>
$ sudo ./generate_all.sh<br><br>
<b>Copy command line to grub configuration.<br></b>
$ sudo vi /boot/grub/grub.conf<br>
## 3. Boot
$ sudo shutdown -r now<br><br>
<b>Check resources first kernel.<br></b>
$ cat /proc/cpuinfo<br>
$ cat /proc/meminfo<br>
## 4. Boot Second Kernel
### 4.1 Create Second Kernel Image
$ cd ~/popcorn/popcornlinux_utils<br>
$ sudo create_elf.sh ~/linux/linux-3.2.14/vmlinux<br>
$ ls vmlinux.elf
### 4.2 Boot Second Kernel
<b>Now, Boot Second Kernel<br></b>
$ cd ~/popcorn/popcornlinux_utils<br>
$ sudo ./mklinux_boot.sh 1<br>
## 5. Communication
<b>Popcorn Linux supports communication between kernel by tap or virtual serial.<br></b>
$ cd ~/popcorn/popcornlinux_utils<br>
$ sudo ./tunnel.sh<br>
### 5.1 SSH
$ cd ~/popcorn/popcornlinux_utils<br>
$ ./tunnelize.sh<br>
$ ssh root@10.0.10.2<br>
$ cat /proc/cpuinfo<br>
$ cat /proc/meminfo<br>
### 5.2 Virtual Serial
