.PHONY: all clean

obj =  obj/popcorn.o obj/bits.o
CLFAGS += -I ./include

all: libpopcorn.a

libpopcorn.a: $(obj)
	ar rcs $@ $^

obj/%.o: src/%.c
	mkdir -p obj
	gcc $(CFLAGS) -I ./include -c -o $@ $<

clean:
	rm -rf obj
