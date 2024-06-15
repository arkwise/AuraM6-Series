CC := gcc
AR := ar
CP := copy
RM := del

PREFIX := C:/djgpp

CFLAGS := -Isrc -Wno-deprecated-declarations
LDFLAGS := src/icon.res
LDLIBS := -L. -lalgif -lalleg

default: libalgif.a algif_examples

%.o: %.c
	$(CC) -c -o $@ $<

libalgif.a: src/algif.o src/gif.o src/lzw.o
	$(AR) -rcs $@ $^

algif_examples: load_gif load_animation load_raw_animation save_raw_animation

load_gif: examples/load_gif/main.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) $(LDLIBS)

load_animation: examples/load_animation/main.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) $(LDLIBS)

load_raw_animation: examples/load_raw_animation/main.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) $(LDLIBS)

save_raw_animation: examples/save_raw_animation/main.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) $(LDLIBS)

install:
	$(CP) libalgif.a $(PREFIX)/lib
	$(CP) src/algif.h $(PREFIX)/include

uninstall:
	$(RM) $(PREFIX)/lib/libalgif.a
	$(RM) $(PREFIX)/include/algif.h
