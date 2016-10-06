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
<b>Install Package for Kernel Compile</b><br>
$ sudo apt-get update<br>
$ sudo apt-get install make libncurses5-dev git gawk

<b>Download Popcorn Linux Patch File and utils</b><br>
$ cd ~/<br>
$ git clone https://github.com/packetngin/popcorn.git<br>

<b>Download Linux 3.2.14 Kernel<br></b>
$ cd ~/<br>
$ mkdir linux<br>
$ cd linux<br>
$ wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.2.14.tar.gz<br>
$ tar -zxf linux-3.2.14.tar.gz<br>

<b>Patch Kernel Source and Compile Kernel</b><br>
$ cd ~/linux/linux-3.2.14<br>
$ patch -p1 < ~/popcorn/PopcornLinux.patch<br>
$ sudo make menuconfig<br>
Load benconfig file and Exit.<br>
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
$ sudo shutdown -r now<br>
Select linux-3.2.14 from grub menu<br>

## 2. Setup
### 2.2 Setup Boot Parameter
$ cd ~/popcorn/utils<br>
$ make<br>
$ sudo ./generate_all.sh<br><br>
<b>Edit Boot Parameter for Tunnel Communication<br></b>
$ cd ~/popcorn/utils<br>
$ sudo vi boot_args_1.args<br>
Then, Reduce mem to 100M(This Space will be used to tunnel communication.)<br>
ex) mem=1792M ==> mem=1692M<br><br>
<b>Copy command line to grub configuration.<br></b>
$ sudo vi /boot/grub/grub.conf<br>
Copy vty_offset, present_map, mem from boot_args_0.args to end of 3.2.14 kernel parameter.<br>
ex) linux /boot/vmlinuz-3.2.14 root=UUID=a61cf2ed-9318-4ad3-89f0-9dc309101cdd ro vty_offset=0x74000000 present_mask=0 mem=896M<br>
$ sudo shutdown -r now
## 3. Boot
<b>Check resources first kernel.<br></b>
$ cat /proc/cpuinfo<br>
$ cat /proc/meminfo<br>
## 4. Boot Second Kernel
### 4.1 Create Second Kernel Image
$ cd ~/popcorn/utils<br>
$ sudo create_elf.sh ~/linux/linux-3.2.14/vmlinux<br>
$ ls vmlinux.elf
### 4.2 Boot Second Kernel
<b>Now, Boot Second Kernel<br></b>
$ cd ~/popcorn/utils<br>
$ sudo ./mklinux_boot.sh 1<br>
## 5. Communication
<b>Popcorn Linux supports communication between kernel by tap or virtual serial.<br></b>
### 5.1 SSH
$ cd ~/popcorn/utils<br>
$ ./tunnelize.sh<br>
$ ping 10.1.2.2<br>
$ ssh root@10.1.2.2<br>
Root has no passward. So, Press only 'Enter Key'.
$ cat /proc/cpuinfo<br>
$ cat /proc/meminfo<br>
### 5.2 Virtual Serial
