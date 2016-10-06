// copyright Antonio Barbalace, SSRG, VT, 2013
// partially based on initial work by Ben Shelton, SSRG, VT, 2013
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "popcorn.h"

#define BUF_SIZE 2048

int read_cmdline() {
	char cmd_line_buf[BUF_SIZE];

	int ret = load_cmd_line(cmd_line_buf, BUF_SIZE);
	if(!ret) {
		printf("library error getting struct\n");
		return 1;
	}

	printf("%s\n", cmd_line_buf);
	return 0;
}

int main(int argc, char *argv[]) {
	if(argc == 1) {
		return read_cmdline();
	}

	char cmdline_ptr[BUF_SIZE];
	memset(cmdline_ptr, 0, BUF_SIZE);
	int cmdline_size = BUF_SIZE - 1;
	for(int i = 1; i < argc; i++) {
		FILE* cmdline_file = fopen(argv[i], "r");
		if(!cmdline_file) {
			printf("error fopen %s\n", argv[i]);
			return 1;
		}

		int len = 0;
		int size_read = 0;
		char buf[2048];
		while(fgets(buf, 2048 - 1, cmdline_file)) {
			if(BUF_SIZE - strlen(cmdline_ptr) > strlen(buf) + 1) {
				strcat(cmdline_ptr, " ");
				strcat(cmdline_ptr, buf);
			} else {
				fclose(cmdline_file);
				printf("Can't read buf\n");
				return 1;
			}
		}
		fclose(cmdline_file);
	}

	/* loading the command line in phys memory */
	int _ret = save_cmd_line(cmdline_ptr, strlen(cmdline_ptr));
	if(!_ret) {
		printf("Can't save cmd line\n");
		return 1;
	}

	return 0;
}
