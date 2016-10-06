#CC_FLAGS=-DDEBUG
.PHONY: all clean lib cmdline_tool cp_ramdisk mpart tunnel

CFLAGS += -std=gnu99
export CFLAGS

all: lib cmdline_tool cp_ramdisk mpart tunnel

lib:
	make -C lib

cmdline_tool:
	make -C cmdline_tool

cp_ramdisk:
	make -C cp_ramdisk

mpart:
	make -C mpart

tunnel:
	make -C tunnel

clean:
	make -C lib clean
	make -C cmdline_tool clean
	make -C cp_ramdisk clean
	make -C mpart clean
	make -C tunnel clean
	rm -rf bin
	rm -rf *.args
	rm -rf *.param
