.PHONY: all clean

all = mpart
LIB = ../lib
INCLUDE = ../lib/include

mpart: mpart.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB) -o $@ $^ -lpopcorn
	mkdir -p ../bin
	cp $@ ../bin

clean:
	rm -f mpart
