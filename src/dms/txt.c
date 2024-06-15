////////////////////////////////////////////////////////////////////////////////
//
//    Generic Text data loader/saver
//
//  (c) Copyright 2003 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include"stdio.h"
#include"dms.h"
#include"debug.h"
#include"stdlib.h"
#include"text.h"
////////////////////////////////////////////////////////////////////////////////
l_bool TextSave ( PFile file, l_ptr  Data, l_ulong  Size, l_ulong *EndOffset ) {
	Size = TextLen((l_text)Data);
	FileWrite( Data, Size, 1, file);
	if ( EndOffset ) FileGetPos(file,EndOffset);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool TextLoad ( PFile file, l_ptr *Data, l_ulong *Size, l_ulong *EndOffset ) {
	*Data = (void*)malloc((*Size)+1);
	if ( !(*Data) ) return false;
	FileRead( *Data, *Size, 1, file);
	if ( EndOffset ) FileGetPos(file,EndOffset);
	*((char*)(((l_ulong)*Data)+*Size)) = 0x00;
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool TextSaveToFile   ( l_text file, l_ptr  Data, l_ulong  Size ) {
	l_bool result;
	PFile f = FileOpen(file,"wb");
	if ( !f ) return false;
	result = TextSave(f,Data,Size,NULL);
	FileClose(f);
	return result;
}
////////////////////////////////////////////////////////////////////////////////
l_bool TextLoadFromFile ( l_text file, l_ptr *Data, l_ulong *Size ) {
	l_bool result;
	PFile f = FileOpen(file,"rb");
	if ( !f ) return false;
	*Size = FileLength(f);
	result = TextLoad(f,Data,Size,NULL);
	FileClose(f);
	return result;
}
////////////////////////////////////////////////////////////////////////////////
void DmsInstallGenericText ( void ) {

	InstallNewCodec( TYPE_TEXT, 
	                   0,
	                   "Generic Text",
	                   NULL,
	                   true,
	                   &TextSaveToFile,
	                   &TextLoadFromFile,
	                   &TextSave,
	                   &TextLoad,
	                   NULL );
	
}
////////////////////////////////////////////////////////////////////////////////
