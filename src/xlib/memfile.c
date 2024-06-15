////////////////////////////////////////////////////////////////////////////////
//
//  Memory files - Core File
//
//	(c) Copyright 2004 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include"kernel.h"
#include"memfile.h"

l_ulong	AppVersion = ULONG_ID(0, 1, 0, 0);
l_char	AppName[] = "Memory files";
l_uid	nUID = "memfile";
l_uid NeededLibs[] = { "" };

////////////////////////////////////////////////////////////////////////////////
PFileDriver MemFileDriver;
PDrive MemFileDrive;
////////////////////////////////////////////////////////////////////////////////
PFile   MemFileFromBinData ( PBinData b ) {

	PFile f = malloc(sizeof(TFile));
	if ( !f ) return NULL;

	memset(f,0,sizeof(TFile));
	
	f->Drive = MemFileDrive;

	f->FileSize = b->Size;
	f->BufferPos = 0;
	f->Pos = 0;
	
	f->BufferSize = ((b->Size/1024)+1)*1024;
	f->Buffer = malloc(f->BufferSize);

	if ( !f->Buffer ) {
		free(f);
		return NULL;
	}
	memcpy(f->Buffer,b->Data,b->Size);

	return f;
}
////////////////////////////////////////////////////////////////////////////////
PFile   MemFileFromFreeBinData ( PBinData b ) {

	PFile f = malloc(sizeof(TFile));
	if ( !f ) return NULL;

	memset(f,0,sizeof(TFile));
	
	f->Drive = MemFileDrive;

	f->FileSize = b->Size;
	f->BufferPos = 0;
	f->Pos = 0;
	
	f->BufferSize = b->Size;
	f->Buffer = b->Data;
	
	free(b);

	return f;
}

////////////////////////////////////////////////////////////////////////////////
PFile   MemFileNewEmpty ( void ) {

	PFile f = malloc(sizeof(TFile));
	if ( !f ) return NULL;

	memset(f,0,sizeof(TFile));
	
	f->Drive = MemFileDrive;

	f->FileSize = 0;
	f->BufferPos = 0;
	f->Pos = 0;
	
	f->BufferSize = 1024;
	f->Buffer = malloc(f->BufferSize);
	if ( !f->Buffer ) {
		free(f);
		return NULL;
	}

	return f;
}
////////////////////////////////////////////////////////////////////////////////
void    MemFileClose ( PFile f ) {
	if ( f->Buffer ) free(f->Buffer);
	free(f);
}
////////////////////////////////////////////////////////////////////////////////
PBinData MemFileCloseToBinData ( PFile f ) {
	PBinData o = NewBinData(f->Buffer,f->FileSize);
	free(f);
	return o;
}
////////////////////////////////////////////////////////////////////////////////
l_int   MemFileRead ( PFile f, void *d, l_ulong s ) {
	l_ulong toRead;
	
	if ( (s + f->Pos) > f->FileSize ) 
		toRead = f->FileSize-f->Pos;
	else
		toRead = s;
	memcpy(d,f->Buffer+f->Pos,s);
	
	f->Pos += toRead;
	
	DebugMessage("Read %d bytes, filesize : %d, pos : %d, buffer : %d",toRead,f->FileSize,f->Pos,f->BufferSize);
	
	return toRead;
}
////////////////////////////////////////////////////////////////////////////////
l_int   MemFileWrite ( PFile f, void *d, l_ulong s ) {
	
	if ( !f->Buffer ) return 0;

	if ( s > (f->BufferSize - f->Pos) ) {
		f->BufferSize = (((s + f->Pos)/1024)+1)*1024;
		f->Buffer = realloc(f->Buffer,f->BufferSize);
		
		if ( !f->Buffer ) return 0;
		
		DebugMessage("Buffer size changed to %d, to store %d bytes",f->BufferSize,s);
	}

	if ( s > (f->FileSize - f->Pos) ) f->FileSize = f->Pos+s;
	
	memcpy(f->Buffer+f->Pos,d,s);

	f->Pos += s;
	
	DebugMessage("Write %d bytes, filesize : %d, pos : %d, buffer : %d",s,f->FileSize,f->Pos,f->BufferSize);
	
	return s;
}
////////////////////////////////////////////////////////////////////////////////
l_int	MemFileGetPos ( PFile f, l_ulong *Pos ) {

	if ( Pos ) *Pos = f->Pos;
	
}
////////////////////////////////////////////////////////////////////////////////
l_int	MemFileSeek ( PFile f, l_int Type, l_long Pos ) {
	l_ulong newPos;
	
	if ( Type == SEEK_SET ) newPos = Pos;
	if ( Type == SEEK_CUR ) newPos = Pos + f->Pos;
	if ( Type == SEEK_END ) newPos = f->FileSize + Pos;
	
	if ( newPos > f->BufferSize ) {
		f->BufferSize = ((Pos/1024)+1)*1024;
		f->Buffer = realloc(f->Buffer,f->BufferSize);
		
		if ( !f->Buffer ) return -1;
		
		DebugMessage("Buffer size changed to %d, to reach %d",f->BufferSize,Pos);
	}
	
	f->Pos = newPos;
	if ( f->Pos > f->FileSize ) f->FileSize = f->Pos;
	
	DebugMessage("Moved, filesize : %d, pos : %d, buffer : %d",f->FileSize,f->Pos,f->BufferSize);
	
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
l_int	MemFileEOF ( PFile f ) {

	if ( f->Pos == f->FileSize ) return 0;
	return 1;

}
////////////////////////////////////////////////////////////////////////////////
l_int	MemFileFlush ( PFile f ) {

	return 0;

}
////////////////////////////////////////////////////////////////////////////////
PBinData FileExtractToBinData ( PFile f ) {
	l_ulong Size = 0;
	l_ulong OldPos = 0;
	void *Data;
	
	MemFileGetPos(f,&OldPos);
	
	FileSeek(f, 0, SEEK_END);
	MemFileGetPos(f,&Size);
	
	FileSeek(f, 0, SEEK_SET);
	
	DebugMessage("Size = %d",Size);
	
	Data = malloc(Size);
	if ( !Data ) return NULL;
	
	Size = FileRead(Data,1,Size,f);
	
	DebugMessage("Size = %d",Size);
	
	FileSeek (f, OldPos, SEEK_SET);
	
	return NewBinData(Data,Size);
}
////////////////////////////////////////////////////////////////////////////////
l_bool LibMain ( int argc, l_text *argv )
{
	MemFileDriver = malloc(sizeof(TFileDriver));
	memset(MemFileDriver,0,sizeof(TFileDriver));
	
	MemFileDriver->DriverName	= TextDup("mem drive driver");
	MemFileDriver->FileClose	= &MemFileClose;
	MemFileDriver->FileRead		= &MemFileRead;
	MemFileDriver->FileWrite	= &MemFileWrite;
	MemFileDriver->FileGetPos	= &MemFileGetPos;
	MemFileDriver->FileSeek		= &MemFileSeek;
	MemFileDriver->FileFlush	= &MemFileFlush;
	MemFileDriver->FileEOF		= &MemFileEOF;
	
	MemFileDrive = malloc(sizeof(TDrive));
	memset(MemFileDrive,0,sizeof(TDrive));

	MemFileDrive->Name = TextDup("&__memory");
	MemFileDrive->Type = FT_RAM;
	MemFileDrive->Driver = MemFileDriver;
	
	APPEXPORT(MemFileNewEmpty);
	APPEXPORT(FileExtractToBinData);
	APPEXPORT(MemFileCloseToBinData);
	APPEXPORT(MemFileFromBinData);
	APPEXPORT(MemFileFromFreeBinData);
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{

}
////////////////////////////////////////////////////////////////////////////////

