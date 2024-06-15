////////////////////////////////////////////////////////////////////////////////
//
//	Data Management System - Core file
//
//	See also types.h for definitions
//
//	(c) Copyright 2003 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "dms.h"
#include "resource.h"

//void DmsInstallBMP ( void );
void DmsInstallGenericBinary ( void );
void DmsInstallGenericText ( void );
void DmsInstallGenericUText ( void );

PList DmsCodecs;
PList DmsDataManagers;


////////////////////////////////////////////////////////////////////////////////
void FreeCodec ( void *o )
{
	DebugMessage ("DMS :: Unload Codec Name:'%s'", CODEC(o)->Name);
	if ( CODEC(o)->Name ) free(CODEC(o)->Name);
	if ( CODEC(o)->Extention ) free(CODEC(o)->Extention);
	free(o);
}
////////////////////////////////////////////////////////////////////////////////
void FreeDataMan ( void *o )
{
	DebugMessage ("DMS :: Unload Manager Name:'%s'", DATAMAN(o)->Name);
	if ( DATAMAN(o)->Name ) free(DATAMAN(o)->Name);
	free(o);
}
////////////////////////////////////////////////////////////////////////////////
PCodec InstallNewCodec (l_datatype   DataTypeManaged,
							l_dataencode DataEncodeManaged,
							l_text Name,
							l_text Extention,
							l_bool UseForRessource,
							l_bool (*SaveToFile)( l_text file, l_ptr  Data, l_ulong  Size ),
							l_bool (*LoadFromFile)( l_text file, l_ptr *Data, l_ulong *Size ),
							l_bool (*Save)( PFile file, l_ptr  Data, l_ulong  Size, l_ulong *EndOffset ),
							l_bool (*Load)( PFile file, l_ptr *Data, l_ulong *Size, l_ulong *EndOffset ),
							l_bool (*ExtraOptionsSet) ( PCodec o ) )
{
	PCodec o = NEW(TCodec);

	if (!o) return NULL;

	o->DataTypeManaged   = DataTypeManaged;
	o->DataEncodeManaged = DataEncodeManaged;
	o->Name              = TextDup(Name);
	o->Extention         = TextDup(Extention);
	o->SaveToFile        = SaveToFile;
	o->LoadFromFile      = LoadFromFile;
	o->Save              = Save;
	o->Load				 = Load;
	o->UseForRessource	 = UseForRessource;
	o->ExtraOptionsSet	 = ExtraOptionsSet;

	//o->Icon32			 = LoadImage("SYSTEM/ICONS/file32.bmp");
	//o->Icon16			 = LoadImage("SYSTEM/ICONS/file16.bmp");

	ListAdd(DmsCodecs, NULL, o, &FreeCodec);

	DebugMessage ("DMS :: New Codec installed (Name:'%s' Extension:'%s')", o->Name, o->Extention);

	return o;
}
////////////////////////////////////////////////////////////////////////////////
PDataMan InstallNewDataManager (l_datatype Type,l_text Name,void (*FreeData)(void*),void*(*Duplicate)(void*,l_ulong) )
{
	PDataMan o = NEW(TDataMan);
	if (!o) return NULL;
	o->Type   	= Type;
	o->Name      = TextDup(Name);
	o->FreeData  = FreeData;
	o->Duplicate = Duplicate;
	ListAdd(DmsDataManagers, NULL, o, &FreeDataMan);
	DebugMessage ("DMS :: New Manager installed (%s)", o->Name);
	return o;
}
////////////////////////////////////////////////////////////////////////////////
void UnInstallCodec ( PCodec o ) {
  PListItem i = ListFoundItem(DmsCodecs,o);
	if ( i ) ListRemoveItem(DmsCodecs,i);
}
////////////////////////////////////////////////////////////////////////////////
PCodec FoundCodecByExtention ( l_datatype DataTypeAccepted, l_text Extention ) {
	PListItem a, b;
  if ( !DmsCodecs->Last ) return NULL;
	a = b = DmsCodecs->Last->Next;
  if ( a ) do {
    if ( CODEC(a->Data)->DataTypeManaged == DataTypeAccepted &&
       ( !TextCaseCompare(CODEC(a->Data)->Extention,Extention) ||
        !CODEC(a->Data)->Extention) )
      return CODEC(a->Data);
	  a = a->Next;
  } while ( a != b );
  return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PCodec FoundCodecByEncoder ( l_datatype DataTypeAccepted, l_dataencode DataEncode ) {
	PListItem a, b;
  if ( !DmsCodecs->Last ) return NULL;
	a = b = DmsCodecs->Last->Next;
  if ( a ) do {
    if ( CODEC(a->Data)->DataTypeManaged == DataTypeAccepted &&
       ( CODEC(a->Data)->DataEncodeManaged == DataEncode ||
        !CODEC(a->Data)->DataEncodeManaged )  )
      return CODEC(a->Data);
	  a = a->Next;
  } while ( a != b );
  return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PCodec DefaultCodec ( l_datatype DataTypeAccepted ) {
	PListItem a, b;
  if ( !DmsCodecs->Last ) return NULL;
	a = b = DmsCodecs->Last;
  if ( a ) do {
    if ( CODEC(a->Data)->DataTypeManaged == DataTypeAccepted )
      return CODEC(a->Data);
	  a = a->Prev;
  } while ( a != b );
  return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PCodec DefaultRessourceCodec ( l_datatype DataTypeAccepted ) {
	PListItem a, b;
  if ( !DmsCodecs->Last ) return NULL;
	a = b = DmsCodecs->Last;
  if ( a ) do {
    if ( CODEC(a->Data)->DataTypeManaged == DataTypeAccepted  && CODEC(a->Data)->UseForRessource )
      return CODEC(a->Data);
	  a = a->Prev;
  } while ( a != b );
  return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PDataMan GetDataManager ( l_datatype Type ) {
	PListItem a, b;
  if ( !DmsDataManagers->Last ) return NULL;
	a = b = DmsDataManagers->Last->Next;
  if ( a ) do {
    if ( DATAMAN(a->Data)->Type == Type ) return DATAMAN(a->Data);
	  a = a->Next;
  } while ( a != b );
  return NULL;
}
////////////////////////////////////////////////////////////////////////////////
l_text GetFileExtention ( l_text File )
{
	l_text r = TextRChr(File,'.');
	if ( r ) return r+1;
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
l_bool SaveSizedData ( l_datatype Type, void *Data, l_ulong Size, l_text File ) {
	l_text ext = GetFileExtention(File);
	PCodec dm = NULL;
	if ( ext )
		dm = FoundCodecByExtention(Type,ext);
	else
		dm = DefaultCodec(Type);
	if ( !dm ) return false;
	return dm->SaveToFile(File,Data,Size);
}
////////////////////////////////////////////////////////////////////////////////
l_bool SaveData ( l_datatype Type, void *Data, l_text File ) {
	return SaveSizedData(Type,Data,0,File);
}

////////////////////////////////////////////////////////////////////////////////
PList CachedRes = NULL;
l_text CachedResFile = NULL;

////////////////////////////////////////////////////////////////////////////////
void* LoadSizedDataFromRessource ( l_datatype ExecptedType, l_text File, l_ulong *Size, l_text Item ) {
	PListItem a;
	if ( !CachedResFile ) {
		CachedResFile = TextDup(File);
		CachedRes = LoadRessourceFile(CachedResFile);
	} else if ( TextCompare(CachedResFile,File) ) {
		if ( CachedResFile ) free(CachedResFile);
		if ( CachedRes ) FreeList(CachedRes);
		CachedResFile = TextDup(File);
		CachedRes = LoadRessourceFile(CachedResFile);
		if ( !CachedRes ) return NULL;
	}
	a = ListKeyItem(CachedRes,Item);
	if ( !a ) return NULL;
	if ( a->DataType != ExecptedType ) return NULL;
	if ( Size ) *Size = a->DataSize;
	return DataDuplicate(a->DataType,a->Data,a->DataSize);
}
////////////////////////////////////////////////////////////////////////////////
void *LoadSizedData ( l_datatype ExecptedType, l_text File, l_ulong *Size ) {
	l_text Item;
	void *Data = 0;
	if ( Item = TextRChr(File,'#') ) {
		l_text ResFile = TextNDup(File,Item-File);
		Item++;
		Data = LoadSizedDataFromRessource(ExecptedType,ResFile,Size,Item);
		free(ResFile);
		return Data;
	} else {
		void *Data = 0;
		l_text ext = GetFileExtention(File);
		PCodec dm = NULL;
		if ( ext )
			dm = FoundCodecByExtention(ExecptedType,ext);
		else
			dm = DefaultCodec(ExecptedType);
		if ( !dm ) return NULL;
		if ( dm->LoadFromFile(File,&Data,Size) ) return Data;
	}
	
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void *LoadData ( l_datatype ExecptedType, l_text File ) {
	l_ulong Size = 0;
	return LoadSizedData(ExecptedType,File,&Size);
}
////////////////////////////////////////////////////////////////////////////////
void DataFree ( l_ulong Type, void *Data ) {
	PDataMan m = GetDataManager(Type);
	if ( m ) {
	  if ( m->FreeData ) m->FreeData(Data);
	}
}
////////////////////////////////////////////////////////////////////////////////
void *DataDuplicate ( l_ulong Type, void *Data, l_ulong Size ) {
	PDataMan m = GetDataManager(Type);
	if ( m ) {
	  if ( m->FreeData ) return m->Duplicate(Data,Size);
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void *_DuplicateData ( void *Data, l_ulong Size ) {
	void *o = malloc(Size);
	if ( !o ) return NULL;
	memcpy(o,Data,Size);
	return o;
}
////////////////////////////////////////////////////////////////////////////////
void *_DuplicateText (void *Data, l_ulong Size ) {
	return TextDup((l_text)Data);
}
////////////////////////////////////////////////////////////////////////////////
void *_DuplicateUText (void *Data, l_ulong Size ) {
	return UTextDup((l_utext)Data);
}
////////////////////////////////////////////////////////////////////////////////
void *_DuplicateImage (void *Data, l_ulong Size ) {
	BITMAP *s =( BITMAP*)Data;
	BITMAP *b = create_bitmap(s->w,s->h);
	if ( b ) blit(s,b,0,0,0,0,s->w,s->h);
	return b;
}
////////////////////////////////////////////////////////////////////////////////
void *_DuplicateFileRef (void *Data, l_ulong Size ) {
	PFileRef s = (PFileRef)Data;
	PFileRef d = malloc(sizeof(TFileRef));
	if ( !d ) return NULL;
	
	d->Drive 			= s->Drive;
	d->Size				= s->Size; 			
	d->Attributes	= s->Attributes;
	d->LocalName	= TextDup(s->LocalName);
	d->SName 			= TextDup(s->SName);
	
	return d;
}
//#define TEST
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void InitDMS ( void )
{
	#ifdef TEST
		PList Tst;
	#endif

	DmsCodecs = NewList();
	if ( !DmsCodecs ) DebugFatal("DMS::NOTENOUGHMEMORY");
	DmsDataManagers = NewList();
	if ( !DmsDataManagers ) DebugFatal("DMS::NOTENOUGHMEMORY");

	// Generic Manager (free,duplicate)
	InstallNewDataManager(TYPE_UNKNOW_BINARY,"Unknown Binary",&free,&_DuplicateData);
	InstallNewDataManager(TYPE_TEXT,"Text (ASCII/UTF-8)",&free,&_DuplicateText);
	InstallNewDataManager(TYPE_UTEXT,"Unicode Text (UTF-16)",&free,&_DuplicateUText);

	InstallNewDataManager(TYPE_IMAGE,"Image",(void*)&destroy_bitmap,&_DuplicateImage);
	InstallNewDataManager(TYPE_FILEREF,"File Refrence",(void*)&FreeFileRef,&_DuplicateFileRef);
	InstallNewDataManager(TYPE_FILEREFLIST,"File Refrence Listing",(void*)&FreeList,NULL);

	DmsInstallGenericBinary();
	DmsInstallGenericText();
	DmsInstallGenericUText();
	//DmsInstallBMP();

	SYSEXPORT(InstallNewCodec);
	SYSEXPORT(UnInstallCodec);
	SYSEXPORT(FoundCodecByEncoder);
	SYSEXPORT(FoundCodecByExtention);
	SYSEXPORT(DefaultRessourceCodec);
	SYSEXPORT(DefaultCodec);
	SYSEXPORT(GetFileExtention);
	SYSEXPORT(SaveSizedData);
	SYSEXPORT(SaveData);
	SYSEXPORT(LoadData);
	SYSEXPORT(LoadSizedData);

	SYSEXPORT(InstallNewDataManager);
	SYSEXPORT(DataFree);
	SYSEXPORT(DataDuplicate);

	SYSEXPORT(SaveRessourceFile);
	SYSEXPORT(LoadRessourceFile);
	SYSEXPORT(DMSWriteRessource);
	SYSEXPORT(DMSLoadRessource);

	#ifdef TEST
		Tst = NewList();
		ListAddItem(Tst,NewListItemEx("TEST","DATADATADATA",NULL,0,13));
		ListAddItem(Tst,NewListItemEx("TEST2","DATADATADATADATA",NULL,0,17));
		ListAddItem(Tst,NewListItemEx("TESTTxt","This is a nice and lovely text",NULL,TYPE_TEXT,0));
		SaveRessourceFile("test.res",Tst);
		FreeList(Tst);
		Tst = LoadRessourceFile("test.res");
		FreeList(Tst);
	#endif
}
////////////////////////////////////////////////////////////////////////////////
void ShutDownDMS ( void )
{
	DebugMessage ("Shutting down DMS...");
	FreeList(DmsCodecs);
	FreeList(DmsDataManagers);
	if ( CachedResFile ) free(CachedResFile);
	if ( CachedRes ) FreeList(CachedRes);
	DebugMessage ("DMS Stopped");
}
////////////////////////////////////////////////////////////////////////////////
