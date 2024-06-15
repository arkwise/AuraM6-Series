////////////////////////////////////////////////////////////////////////////////
//
//  Codename Phoenix WIN32 gateway. Used to prevent conflicts.
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////


#include"windows.h"

int _w32_setfileattributes ( char *file, unsigned short attribs ) {
	return SetFileAttributesA(file,attribs);
}
