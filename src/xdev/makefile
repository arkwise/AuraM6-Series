CFLAGS   = -O3 -g -funroll-loops -I../Source/include -Iinclude

OBJ_LIST = \
obj/main.o \
obj/devwdg.o \
obj/devclass.o \
obj/about.o \
obj/menuedit.o \
obj/msgman.o \
obj/viewprop.o

obj/%.o: src/%.c
	gcc $(CFLAGS) -c $< -o $@

all : ../Source/bin/apps/illkirch.app
	@echo Codename Illkirch has been built.

../Source/bin/applications/illkirch.app: $(OBJ_LIST)
	../Source/dynldlnk.exe -Rillkirch.res ../Source/bin/applications/illkirch.app $(OBJ_LIST)
