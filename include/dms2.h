////////////////////////////////////////////////////////////////////////////////
//
//	Data Management System v2 - Header file
//
//	See also types.h for definitions
//
//	(c) Copyright 2004 Point Mad. All rights reserved.
//
//	Documentation: docs/dms-fms-v2-draft.txt
//
////////////////////////////////////////////////////////////////////////////////

#include "allegro.h"
#include "types.h"
#include "list.h"
#include "vfile.h"
#include "resource.h"
#include "error.h"

#ifndef _DMS2_H_INCLUDED_
#define _DMS2_H_INCLUDED_

typedef struct TFExt {
	//l_text 	Extention; -> Stored in list key
	l_ulong	CodecId;
} TFExt, *PFExt;

#define FEXT(o) ((PFExt)(o))

typedef struct TMimeDat {
	//l_text 	MimeType; -> Stored in list key
	l_ulong	CodecId;
} TMimeDat, *PMimeDat;

#define MIMEDAT(o) ((PMimeDat)(o))

typedef struct TCodec {
	l_text 	Name;
	l_ulong	CodecId;
	PList 	SubCodecs;

	PIcon Default;
	
	PIcon (*GetIcons) ( PFileInfo i );
	
	l_text DefaultExtention;
	l_text DefaultMimeType;
	l_ulong FavoriteType;
	l_ulong Flags;
} TCodec, *PCodec;

#define CDCF_FREEICONS 0x01

typedef struct TSubCodec {

	l_ulong TypeId;
	l_ulong CodecId;
	l_ulong	Flags;
	l_bool	(*SaveToFile)( l_text file, l_ptr  Data, l_ulong *ExtraSave, PErrorHandler h );
	l_bool	(*LoadFromFile)( l_text file, l_ptr *Data, l_ulong *ExtraLoad, PErrorHandler h );
	l_bool	(*Save)( PFile file, l_ptr  Data, l_ulong *EndOffset, l_ulong *ExtraSave, PErrorHandler h );
	l_bool	(*Load)( PFile file, l_ptr *Data, l_ulong *EndOffset, l_ulong *ExtraLoad, PErrorHandler h );
	l_ulong	*DefaultExtraSave;	
	l_ulong	*DefaultExtraLoad;
	l_text	ExtraSaveSetApp;
	l_text	ExtraLoadSetApp;

} TSubCodec, *PSubCodec;

typedef struct TDataTypeMan  {
	l_ulong TypeId;
	void 	(*Free) ( void* Data );
	void*   (*Duplicate) ( void*Data );
	PSubCodec DefaultSubcodec;
} TDataTypeMan, *PDataTypeMan;

#define DATATYPEMAN(o) ((PDataTypeMan)(o))
#define CODEC(o) ((PCodec)(o))
#define SUBCODEC(o) ((PSubCodec)(o))

void DMS2Init ( void );
void DMS2DeInit ( void );


extern PList FExts, MimeDats, Codecs, DatMan;
extern PSubCodec GenericText, GenericBinary, GenericUText;

PDataTypeMan	GetDataTypeMan 			( l_ulong TypeId );
void 			AddMimeType 			( l_text Mime, l_ulong CodecId );
void 			AddFileExtention 		( l_text FExt, l_ulong CodecId );
PDataTypeMan 	RegisterDataType 		( l_ulong TypeId,void(*Free)(void*),void*(*Duplicate)(void*) );
PCodec			AddCodec				( l_text Name,l_ulong CodecId,p_bitmap Icon16,p_bitmap Icon32,p_bitmap Icon48,PIcon (*GetIcons) ( PFileInfo i ),l_text DefaultExtention,l_text DefaultMimeType,l_ulong FavoriteType,l_ulong Flags );
PCodec 			GetCodec 				( l_ulong CodecId );
PCodec 			GetCodecFileExtention 	( l_text FExt );


PSubCodec AddNewSubCodec ( PCodec Codec, l_ulong TypeId, l_ulong	Flags, 
						l_bool(*SaveToFile)( l_text , l_ptr  , l_ulong *, PErrorHandler ),
						l_bool(*LoadFromFile)( l_text, l_ptr *, l_ulong *, PErrorHandler ),
						l_bool(*Save)( PFile, l_ptr, l_ulong *, l_ulong *, PErrorHandler ),
						l_bool(*Load)( PFile, l_ptr *, l_ulong *, l_ulong *, PErrorHandler ),
						l_ulong	*DefaultExtraSave,	l_ulong	*DefaultExtraLoad,
						l_text	ExtraSaveSetApp,l_text	ExtraLoadSetApp );
						
PSubCodec NewSubCodec ( l_ulong TypeId,l_ulong CodecId, l_ulong	Flags, 
						l_bool(*SaveToFile)( l_text , l_ptr  , l_ulong *, PErrorHandler ),
						l_bool(*LoadFromFile)( l_text, l_ptr *, l_ulong *, PErrorHandler ),
						l_bool(*Save)( PFile, l_ptr, l_ulong *, l_ulong *, PErrorHandler ),
						l_bool(*Load)( PFile, l_ptr *, l_ulong *, l_ulong *, PErrorHandler ),
						l_ulong	*DefaultExtraSave,	l_ulong	*DefaultExtraLoad,
						l_text	ExtraSaveSetApp,l_text	ExtraLoadSetApp );
						
void AddSubCodec ( PCodec Codec, PSubCodec o );
void SetDefaultSubCodec ( l_ulong TypeId, PSubCodec sc );

PSubCodec GetSubCodec ( l_ulong CodecId, l_ulong TypeId );
PSubCodec GetSubCodecFileExtention (  l_text FExt, l_ulong TypeId );
PSubCodec GetSubCodecMimeType (  l_text Mime, l_ulong TypeId );

l_text GetFileExtention ( l_text File );


l_bool SaveDataEx ( l_text File, void *Data, l_ulong TypeId, l_ulong CodecId, l_ulong *ExtraSave, PErrorHandler h );
l_bool SaveData2 ( l_text File, void*Data, l_ulong TypeId );

void *LoadDataEx ( l_text File, l_ulong TypeId, l_ulong CodecId, l_ulong *ExtraLoad, PErrorHandler h );
void * LoadData2 ( l_text File, l_ulong TypeId );

PBinData NewBinData ( void *Data, l_ulong Size );

void *DuplicateData2 ( l_ulong TypeId, void *Data );
void FreeData2 ( l_ulong TypeId, void *Data ) ;

PSubCodec GetDefaultSubCodec ( l_ulong TypeId );
PCodec GetCodecFileInfo ( PFileInfo f );

void DMS2InstallIcons ( void );

PIcon NewIcon ( p_bitmap i16, p_bitmap i32, p_bitmap i48 );
void FreeIcon ( PIcon i );
PIcon NewIcon2 ( p_bitmap i16, p_bitmap i32, p_bitmap i48 );
PIcon DuplicateIcon ( PIcon i );

void *DuplicateFileRef (void *Data );
void FreeBinData ( void* o );

#endif
