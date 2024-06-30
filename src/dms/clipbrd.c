////////////////////////////////////////////////////////////////////////////////
//
//  Clipboard - Core File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "clipbrd.h"

l_ulong	AppVersion = ULONG_ID(0, 0, 0, 1);
l_char	AppName[] = "Clipboard";
l_uid	nUID = "clipbrd";

TClipboardItem Clipboard;


void ClipboardPut ( l_ulong Type, void *Data, l_bool Duplicate ) {
	if ( Duplicate )
		Clipboard.Data = DuplicateData2(Type,Data);
	else
		Clipboard.Data = Data;
	Clipboard.Type = Type;
}

void *ClipboardGet ( l_ulong Type ) {
	if ( Clipboard.Type != Type ) return 0;
	return Clipboard.Data;
}

void ClipboardFree ( void ) {
	if ( Clipboard.Data ) FreeData2(Clipboard.Type,Clipboard.Data);
	Clipboard.Type = 0;
}

l_int LibMain( int argc, l_text *argv )
{
	
	APPEXPORT(ClipboardPut);
	APPEXPORT(ClipboardGet);
	APPEXPORT(ClipboardFree);

	return true;
}

void Close(void)
{
	
	ClipboardFree();
	
}
