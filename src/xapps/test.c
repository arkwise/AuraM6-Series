////////////////////////////////////////////////////////////////////////////////
//
//	Console tester
//
//	Copyright (c) 2004 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include"kernel.h"
#include"console.h"

l_ulong AppVersion	= ULONG_ID(0,0,1,0);
char AppName[]		= "console tester";
l_uid NeededLibs[] = { "conlib","" };

l_int Main( int arc, l_text *arv )
{
	Printf(&Me,"Hello world !\n\nPress any key...\n");
	GetKey(&Me);
	return false;	
}
