
// copyright Antonio Barbalace, SSRG, VT, 2013
// initial implementation copyright Ben Shelton, SSRG, VT, 2013

/*
 * this program will be integrated in kexec (user/kernel code)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "bootparam.h"
#include "popcorn.h"

#define PAGE_SIZE 4096

#define MAX_RAMDISK_SIZE	UINT32_MAX //ramdisk_size type is uint32

//input: ramdisk file ramdisk img_addr
static bool check_ramdisk_size(unsigned long ramdisk_size) {
	if(ramdisk_size > MAX_RAMDISK_SIZE) {
		return false;
	}

	return true;
}

int main(int argc, char *argv[])
{
	int _ret;
	void* ramdisk_phys_addr;
	uint64_t ramdisk_size, size_read;
	char _len;

	if (argc != 3) {
		printf("Usage: %s ADDR FILE\n"
				"Copies the entire FILE at physical memory ADDR.\n",
				argv[0]);
		return 1;
	}

	/*check each arguments */
	ramdisk_phys_addr = (void*)(unsigned long)strtoul(argv[1], 0, 0);
	if(ramdisk_phys_addr == 0) {
		perror("conversion error or physical address 0\n");
		return 1;
	}

	/* ramdisk must be page aligned */
	if((unsigned long)ramdisk_phys_addr & (PAGE_SIZE -1)) {
		printf("error ramdisk is not page aligned (page offset 0x%lx)\n",
				((unsigned long)ramdisk_phys_addr & (PAGE_SIZE -1)));
		return 1;
	}
	/* check if the physical address is assigned to the current kernel */
	//TODO

	///////////////////////////////////////////////////////////////////////////////

	/* Open the file with the ramdisk and determine its size */
	FILE *ramdisk_file = fopen(argv[2], "rb");
	if (!ramdisk_file) {
		printf("error fopen %s\n", argv[2]);
		return 1;
	}
	fseek(ramdisk_file, 0, SEEK_END);
	ramdisk_size = ftell(ramdisk_file);
	fseek(ramdisk_file, 0, SEEK_SET);

	/* check if the ramdisk is too big */
	if(!check_ramdisk_size(ramdisk_size)) {
		fclose(ramdisk_file);
		printf("Error RAM Disk is too big\n");
		return 1;
	}

	/* Open /dev/mem to write the ramdisk in */
	int mem_fd = open_mem(O_RDWR | O_SYNC);
	if (mem_fd < 0) {
		perror("Error opening ");
		fclose(ramdisk_file);
		return 1;
	}
	void* ramdisk_virt_addr = mmap(0, ramdisk_size,
			PROT_READ | PROT_WRITE, MAP_SHARED,
			mem_fd, (unsigned long)ramdisk_phys_addr);
	if(ramdisk_virt_addr == MAP_FAILED) {
		close_mem(mem_fd);
		fclose(ramdisk_file);
		return 1;
	}

	/* Read the ramdisk into memory */
	size_read = fread(ramdisk_virt_addr, 1, ramdisk_size, ramdisk_file);
	if(size_read != ramdisk_size) {
		printf("Failed to read the entire ramdisk into memory!\n");
		munmap(ramdisk_virt_addr, ramdisk_size);
		close_mem(mem_fd);
		fclose(ramdisk_file);
		return 1;
	}
	fclose(ramdisk_file);

	_ret = munmap(ramdisk_virt_addr, ramdisk_size);
	if(_ret) {
		printf("Error. Failed to munmap (%d)\n", _ret);
		close_mem(mem_fd);
		fclose(ramdisk_file);
		return 1;
	}
	printf("SUCCESS writing %p size %lu\n",
			 ramdisk_phys_addr, ramdisk_size);

	/* load the boot parameters */
	struct boot_params* boot_params = map_boot_param(mem_fd);
	if(!boot_params) {
		close_mem(mem_fd);
		return 1;
	}

	boot_params->hdr.ramdisk_image =
		(unsigned long)ramdisk_phys_addr & (unsigned long)(__u32)~0U;
	boot_params->hdr.ramdisk_shift =
		(unsigned long)ramdisk_phys_addr >> (sizeof(__u32) * 8);
	boot_params->hdr.ramdisk_size = ramdisk_size;
	boot_params->hdr.ramdisk_magic = 0xdf;

	printf("ramdisk _image 0x%8.8x _shift 0x%8.8x _size %d _magic 0x%x\n",
			boot_params->hdr.ramdisk_image, boot_params->hdr.ramdisk_shift,
			boot_params->hdr.ramdisk_size, boot_params->hdr.ramdisk_magic);
	unmap_boot_param(boot_params);
	close_mem(mem_fd);

	return 0;
}
