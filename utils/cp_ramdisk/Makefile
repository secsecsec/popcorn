.PHONY: all clean

LIB = ../lib
INCLUDE = ../lib/include

all: cp_ramdisk

cp_ramdisk: cp_ramdisk.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB) -o $@ $^ -lpopcorn
	mkdir -p ../bin
	cp $@ ../bin

clean:
	rm -f cp_ramdisk
