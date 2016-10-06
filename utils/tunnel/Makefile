.PHONY: all clean

all = tunnel
LIB = ../lib
INCLUDE = ../lib/include

tunnel: tunnel.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB) -o $@ $^ -lpthread
	mkdir -p ../bin
	cp $@ ../bin

clean:
	rm -f tunnel
