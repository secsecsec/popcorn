# <b> Popcorn Linux</b>
===============================
## <b> Install</b>
---------------------------------
Popcorn Linux is multikernel os. This System boot second kernel after master kernel.<br>
Our Test Environment as follows.<br>

<Center>Environment | <Center>Type
-------- |:-----------
Hardware | qemu-system-x86_64
OS | Ubuntu 12.04 server
Memory (RAM)| 2048MB
CPUs | 2

### 1.1 Install Kernel
<b>Download Popcorn Linux Patch File</b><br>
$ cd ~/<br>
$ git clone https://github.com/packetngin/popcorn.git<br>

<b>Install Package for Kernel Compile</b><br>
$ sudo apt-get update<br>
$ sudo apt-get install make libncurses5-dev git

<b>Download Linux 3.2.14 Kernel<br></b>
$ cd ~/<br>
$ mkdir linux<br>
$ cd linux<br>
$ wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.2.14.tar.gz<br>
$ tar -zxf linux-3.2.14.tar.gz<br>

<b>Patch Kernel Source and Compile Kernel</b><br>
$ cd ~/linux-3.2.14<br>
$ patch -p1 < ~/popcorn/PopcornLinux.patch<br>
$ sudo make menuconfig<br>
$ sudo make<br>
$ sudo make modules<br>
$ sudo make modules_install<br>
$ sudo make install<br>


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

## 2. Setup
### 2.1 Setup Boot Parameter
$ sudo shutdown -r now<br>

## 3. Boot
## 4. Boot Second Kernel
## 5. Communication
Popcorn Linux supports communication between kernel.
### 5.1 SSH
### 5.2 Virtual Serial
