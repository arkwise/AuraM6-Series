////////////////////////////////////////////////////////////////////////////////
//
//    Generic Binary data loader/saver
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include"stdio.h"
#include"dms.h"
#include"debug.h"
#include"stdlib.h"
////////////////////////////////////////////////////////////////////////////////
l_bool BinarySave ( PFile file, l_ptr  Data, l_ulong  Size, l_ulong *EndOffset ) {
	FileWrite( Data, Size, 1, file);
	if ( EndOffset ) FileGetPos(file,EndOffset);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool BinaryLoad ( PFile file, l_ptr *Data, l_ulong *Size, l_ulong *EndOffset ) {
	*Data = (void*)malloc(*Size);
	if ( !(*Data) ) return false;
	FileRead( *Data, *Size, 1, file);
	if ( EndOffset ) FileGetPos(file,EndOffset);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool BinarySaveToFile   ( l_text file, l_ptr  Data, l_ulong  Size ) {
	l_bool result;
	PFile f = FileOpen(file,"wb");
	if ( !f ) return false;
	result = BinarySave(f,Data,Size,NULL);
	FileClose(f);
	return result;
}
////////////////////////////////////////////////////////////////////////////////
l_bool BinaryLoadFromFile ( l_text file, l_ptr *Data, l_ulong *Size ) {
	l_bool result;
	PFile f = FileOpen(file,"rb");
	if ( !f ) return false;
	*Size = FileLength(f);
	result = BinaryLoad(f,Data,Size,NULL);
	FileClose(f);
	return result;
}
////////////////////////////////////////////////////////////////////////////////
void DmsInstallGenericBinary ( void ) {

	InstallNewCodec( TYPE_UNKNOW_BINARY, 
	                   0,
	                   "Generic Binary",
	                   NULL,
	                   true,
	                   &BinarySaveToFile,
	                   &BinaryLoadFromFile,
	                   &BinarySave,
	                   &BinaryLoad,
	                   NULL );
	
}
////////////////////////////////////////////////////////////////////////////////
