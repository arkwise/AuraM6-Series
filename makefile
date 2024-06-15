# Phoenix 1.2 Makefile
# Written by Lukas Lipka and Julien Etelain

CFLAGS   = -O3 -funroll-loops -Iinclude -Idjgpp/watt32/inc -Ldjgpp/watt32/lib -lcurses
CPPFLAGS = -O3 -funroll-loops -Iinclude -Idjgpp/watt32/inc -Ldjgpp/watt32/lib
AFLAGS = -ddjgpp -fcoff

# cflag -std=c99
REMOVE = del
OBJ_DIR = obj
DYNLDLNK = dynldlnk.exe
MAIN = bin/main.exe
OBJ_LIST2 = $(OBJ_DIR)/vfile/msdos.o
LIB_LIST = lib/libz.a lib/libalttf.a -lalleg lib/libwatt.a lib/libmpeg2.a lib/judaslib.a
LIB_DYNLNK = lib/libz.a

include common.mak