////////////////////////////////////////////////////////////////////////////////
//
//    Generic Unicode Text data loader/saver
//
//  (c) Copyright 2003 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include"kernel.h"
#include"dms.h"
#include"debug.h"
#include"stdlib.h"
#include"utext.h"
////////////////////////////////////////////////////////////////////////////////
l_bool UTextSave ( PFile file, l_ptr  Data, l_ulong  Size, l_ulong *EndOffset ) {
	Size = UTextLen((l_utext)Data)*2;
	FileWrite( Data, Size, 1, file);
	if ( EndOffset ) FileGetPos(file,EndOffset);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool UTextLoad ( PFile file, l_ptr *Data, l_ulong *Size, l_ulong *EndOffset ) {
	*Data = (void*)malloc((*Size)+2);
	if ( !(*Data) ) return false;
	FileRead( *Data, *Size, 1, file);
	if ( EndOffset ) FileGetPos(file,EndOffset);
	*((short*)(((l_ulong)*Data)+*Size)) = 0x0000;
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool UTextSaveToFile   ( l_text file, l_ptr  Data, l_ulong  Size ) {
	l_bool result;
	PFile f = FileOpen(file,"wb");
	if ( !f ) return false;
	result = UTextSave(f,Data,Size,NULL);
	FileClose(f);
	return result;
}
////////////////////////////////////////////////////////////////////////////////
l_bool UTextLoadFromFile ( l_text file, l_ptr *Data, l_ulong *Size ) {
	l_bool result;
	PFile f = FileOpen(file,"rb");
	if ( !f ) return false;
	*Size = FileLength(f);
	if ( ((*Size)%2) ) (*Size)--;
	result = UTextLoad(f,Data,Size,NULL);
	FileClose(f);
	return result;
}
////////////////////////////////////////////////////////////////////////////////
void DmsInstallGenericUText ( void ) {

	InstallNewCodec( TYPE_UTEXT, 
	                   0,
	                   "Generic Unicode Text",
	                   NULL,
	                   true,
	                   &UTextSaveToFile,
	                   &UTextLoadFromFile,
	                   &UTextSave,
	                   &UTextLoad,
	                   NULL );
	
}
////////////////////////////////////////////////////////////////////////////////
