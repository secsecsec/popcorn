# <b> Popcorn Linux</b>
===============================
## <b> Install</b>
---------------------------------
Popcorn Linux is multi-kernel OS. This System boots the second kernel after the master kernel.  
Our test environment is as follows.  

<Center>Environment | <Center>Type
-------- |:-----------
<center>Hardware | <center>qemu-system-x86_64
<center>OS | <center>Ubuntu 12.04 server
<center>Memory (RAM)| <center>2048MB
<center>Storage | <center>16GB
<center>CPUs | <center>2

### 1.1 Install Kernel
**Install Package for Kernel Compile**    
$ sudo apt-get update  
cf) Throughout this stage, you may see the error saying '/var/lib/apt/lists/~',  
then enter the command below, and restart 'apt-get update'.  
$ sudo rm -rf /var/lib/apt/lists/*  

$ sudo apt-get install make libncurses5-dev git gawk  

**Download Popcorn Linux Patch File and utils**  
$ cd ~/  
$ git clone https://github.com/packetngin/popcorn.git   

**Download Linux 3.2.14 Kernel**  
$ cd ~/  
$ mkdir linux  
$ cd linux  
$ wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.2.14.tar.gz  
$ tar -zxf linux-3.2.14.tar.gz  

**Patch Kernel Source and Compile Kernel**  
$ cd ~/linux/linux-3.2.14  
$ patch -p1 < ~/popcorn/PopcornLinux.patch  
$ sudo make menuconfig  
Load 'ben_config' file, save '.config' file and Exit.  
>1. select 'Load an Alternate Configuration File', 
>and enter the name of the configuration file, 'ben_config'.  
>2. select 'Save an Alternate Configuration File', 
>and enter the name of the configuration file, '.config'.  

$ sudo make  
$ sudo make modules  
$ sudo make modules_install  
$ sudo make install  
>cf) If you want to make faster, then use '-j2' option.(ex. sudo make -j2)

### 1.2 Install kexec
**Download kexec 2.0.3**  
$ mkdir ~/kexec  
$ cd ~/kexec  
$ wget https://www.kernel.org/pub/linux/utils/kernel/kexec/kexec-tools-2.0.3.tar.gz  
$ tar -zxf kexec-tools-2.0.3.tar.gz  

**Patch and Install kexec**  
$ cd kexec-tools-2.0.3  
$ patch -p1 < ~/popcorn/mk_kexec.patch  
$ ./configure  
$ make  
$ sudo make install  
$ sudo shutdown -r now  
Select linux-3.2.14 from grub menu  

## 2. Setup
### 2.2 Setup Boot Parameter
$ cd ~/popcorn/utils  
$ make  
$ sudo ./generate_all.sh  

**Edit Boot Parameter for Tunnel Communication**  
$ cd ~/popcorn/utils  
$ sudo vi boot_args_1.args  
Then, reduce 'mem' value by 100.(This Space will be used to tunnel communication.)  
ex) From 'mem=1792M' to 'mem=1692M'  

**Copy command line to grub configuration.**  
$ sudo vi /boot/grub/grub.cfg  
Copy vty_offset, present_map, mem from boot_args_0.args to end of 3.2.14 kernel parameter.  
ex) linux /boot/vmlinuz-3.2.14 root=UUID=a61cf2ed-9318-4ad3-89f0-9dc309101cdd ro vty_offset=0x74000000 present_mask=0 mem=896M  
$ sudo shutdown -r now
## 3. Boot
**Check resources first kernel.**  
$ cat /proc/cpuinfo  
$ cat /proc/meminfo  
## 4. Boot Second Kernel
### 4.1 Create Second Kernel Image
$ cd ~/popcorn/utils  
$ sudo ./create_elf.sh ~/linux/linux-3.2.14/vmlinux  
$ ls vmlinux.elf
### 4.2 Boot Second Kernel
**Now, Boot Second Kernel**  
$ cd ~/popcorn/utils  
$ sudo ./mklinux_boot.sh 1  
## 5. Communication
**Popcorn Linux supports communication between kernels by tap or virtual serial.**  
### 5.1 SSH
$ cd ~/popcorn/utils  
$ sudo ./tunnelize.sh  
$ ping 10.1.2.2  
$ ssh user@10.1.2.2  
password is 'password'  
$ cat /proc/cpuinfo  
$ cat /proc/meminfo  
### 5.2 Virtual Serial
$ sudo screen /dev/ttty1 38400  
**ID**: user  
**Password**: password  
