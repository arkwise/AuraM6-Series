# Phoenix 1.2 Makefile
# Written by Lukas Lipka and Julien Etelain
# test bridge version
CFLAGS   = -O3 -funroll-loops -Ic:\djgpp\include\watt -Iinclude
CPPFLAGS = -O3 -funroll-loops -Iinclude

REMOVE = del
OBJ_DIR = obj
DYNLDLNK = dynldlnk.exe
MAIN = main.exe
OBJ_LIST2 = $(OBJ_DIR)/vfile/msdos.o
LIB_LIST = lib/libz.a -lalttf -lalleg -lwatt
LIB_DYNLNK = lib/libz.a

include common.mak 