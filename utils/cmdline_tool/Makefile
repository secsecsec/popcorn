.PHONY: all clean

all = cmdline_tool
LIB = ../lib
INCLUDE = ../lib/include

cmdline_tool: cmdline_tool.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB) -o $@ $^ -lpopcorn
	mkdir -p ../bin
	cp $@ ../bin

clean:
	rm -f cmdline_tool
