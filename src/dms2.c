////////////////////////////////////////////////////////////////////////////////
//
//      Data Management System V2 - Core file
//
//      (c) Copyright 2004 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#define DMS2

/** @defgroup dmsfms File and data management
        Libraries to manage file and data interractions
        @{
*/

/** @defgroup dms Data Management System (DMS)
        Load, save data, and get icons from files. Duplicates and free data.
        @{
*/
/**     @file
        DMS core file (provided into kernel)
*/

#include"kernel.h"
#include "string.h"

PSubCodec _GetSubCodec ( PCodec o, l_ulong TypeId );
void InitGenericSubCodecs ( void );
PList FExts, MimeDats, Codecs, DatMan;
PSubCodec GenericText = 0, GenericBinary = 0, GenericUText = 0;
////////////////////////////////////////////////////////////////////////////////
void FreeFExt ( PFExt o ) {
        //if ( o->Extention ) free(o->Extention);
        free(o);
}

void FreeMimeDat( PMimeDat o  ) {
        //if ( o->MimeType ) free(o->MimeType);
        free(o);
}

void FreeCodec2( PCodec o ) {
        if ( o->Default ) FreeIcon(o->Default);
        if ( o->Name )                          free(o->Name);
        if ( o->SubCodecs )             FreeList(o->SubCodecs);
        if ( o->DefaultExtention )      free(o->DefaultExtention);
        if ( o->DefaultMimeType )       free(o->DefaultMimeType);

        free(o);
}

void FreeSubCodec ( PSubCodec o ) {
        if ( o->ExtraSaveSetApp )       free(o->ExtraSaveSetApp);
        if ( o->ExtraLoadSetApp )       free(o->ExtraLoadSetApp);
        free(o);
}
////////////////////////////////////////////////////////////////////////////////
void AddFileExtention ( l_text FExt, l_ulong CodecId ) {
        PCodec c;
        PFExt o = malloc(sizeof(TFExt));
        if ( !o ) return;
        o->CodecId = CodecId;
        ListAdd(FExts,FExt,o,(void*)&FreeFExt);
        DebugMessage("DMS2 : File extention '%s' -> Codec %08x",FExt,CodecId);

        if ( c = GetCodec(CodecId) ) {
                if ( !c->DefaultExtention ) c->DefaultExtention = TextDup(FExt);
        }

}
////////////////////////////////////////////////////////////////////////////////
void AddMimeType ( l_text Mime, l_ulong CodecId ) {
        PCodec c;
        PMimeDat o = malloc(sizeof(TMimeDat));
        if ( !o ) return;
        o->CodecId = CodecId;
        ListAdd(MimeDats,Mime,o,(void*)&FreeMimeDat);
        if ( c = GetCodec(CodecId) ) {
                if ( !c->DefaultMimeType ) c->DefaultMimeType = TextDup(Mime);
        }
}
////////////////////////////////////////////////////////////////////////////////
PDataTypeMan _GetDataTypeMan ( l_ulong TypeId ) {
        PListItem a,b;
        PDataTypeMan o;
        if ( DatMan->Last ) {
                a = b = DatMan->Last->Next;
                do {
                        if ( DATATYPEMAN(a->Data)->TypeId == TypeId ) return DATATYPEMAN(a->Data);
                        a = a->Next;
                } while( a != b );
        }
        o = malloc(sizeof(TDataTypeMan));
        if ( !o ) return NULL;
        memset(o,0,sizeof(TDataTypeMan));
        o->TypeId               = TypeId;
        ListAdd(DatMan,NULL,o,&free);
        return o;
}
////////////////////////////////////////////////////////////////////////////////
PDataTypeMan GetDataTypeMan ( l_ulong TypeId ) {
        PListItem a,b;
        if ( !DatMan->Last ) return NULL;
        a = b = DatMan->Last->Next;
        do {
                if ( DATATYPEMAN(a->Data)->TypeId == TypeId ) return DATATYPEMAN(a->Data);
                a = a->Next;
        } while( a != b );
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PDataTypeMan RegisterDataType ( l_ulong TypeId,void(*Free)(void*),void*(*Duplicate)(void*) ) {
        PDataTypeMan o = _GetDataTypeMan(TypeId);
        if ( !o ) return NULL;
        o->Free                 = Free;
        o->Duplicate    = Duplicate;
        return o;
}
////////////////////////////////////////////////////////////////////////////////
PCodec AddCodec( l_text Name,l_ulong CodecId,p_bitmap Icon16,p_bitmap Icon32,p_bitmap Icon48,PIcon (*GetIcons) ( PFileInfo i ),l_text DefaultExtention,l_text DefaultMimeType,l_ulong FavoriteType, l_ulong Flags ) {
        PCodec o = malloc(sizeof(TCodec));
        if ( !o ) return NULL;
        memset(o,0,sizeof(TCodec));
        o->Name = TextDup(Name);
        o->CodecId = CodecId;

        if ( Icon16 || Icon32 || Icon48 ) {
                if ( Flags & CDCF_FREEICONS )
                        o->Default = NewIcon(Icon16,Icon32,Icon48);
                else
                        o->Default = NewIcon2(Icon16,Icon32,Icon48);
        }
        o->GetIcons = GetIcons;
        o->DefaultExtention = TextDup(DefaultExtention);
        o->DefaultMimeType = TextDup(DefaultMimeType);
        o->SubCodecs = NewList();
        o->FavoriteType = FavoriteType;
        o->Flags = Flags;

        ListAdd(Codecs,NULL,o,(void*)&FreeCodec2);

        if ( DefaultExtention ) AddFileExtention(DefaultExtention,CodecId);
        if ( DefaultMimeType ) AddMimeType(DefaultMimeType,CodecId);

        DebugMessage("DMS2 : New codec %08x : %s (%s,%s,%08x)",CodecId,Name,DefaultExtention,DefaultMimeType,FavoriteType);

        return o;
}
////////////////////////////////////////////////////////////////////////////////
PCodec GetCodec ( l_ulong CodecId ) {
        PListItem a,b;
        if ( !Codecs->Last ) return NULL;
        a = b = Codecs->Last->Next;
        do {
                if ( CODEC(a->Data)->CodecId == CodecId ) return CODEC(a->Data);
                a = a->Next;
        } while( a != b );
        DebugWarning("DMS2 : Codec %08x not loaded",CodecId);

        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PCodec GetCodecFileExtention ( l_text FExt ) {
        if ( FExt ) {
                PFExt o = ListKeyCase(FExts,FExt);
                if ( !o ) {
                        DebugWarning("DMS2 : No codec assosciated to file extention %s",FExt);
                        return NULL;
                }
                return GetCodec(o->CodecId);
        }
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PCodec GetCodecMimeType ( l_text Mime ) {
        PMimeDat o = ListKeyCase(MimeDats,Mime);
        if ( !o ) {
                DebugWarning("DMS2 : No codec assosciated to mime type %s",Mime);
                return NULL;
        }
        return GetCodec(o->CodecId);
}
////////////////////////////////////////////////////////////////////////////////
PCodec GetCodecFileInfo ( PFileInfo f ) {
        if ( f->FMS )
                return (PCodec)f->FMS;
        else if ( f->Type == FT_HDD )
                return GetCodecFileExtention("&hdd");
        else if ( f->Type == FT_FOLDER )
                return GetCodecFileExtention("&folder");
        else
                return GetCodecFileExtention(GetFileExtention(f->Name));
}
////////////////////////////////////////////////////////////////////////////////
PSubCodec NewSubCodec ( l_ulong TypeId,l_ulong CodecId, l_ulong Flags,
                                                l_bool(*SaveToFile)( l_text , l_ptr  , l_ulong *, PErrorHandler ),
                                                l_bool(*LoadFromFile)( l_text, l_ptr *, l_ulong *, PErrorHandler ),
                                                l_bool(*Save)( PFile, l_ptr, l_ulong *, l_ulong *, PErrorHandler ),
                                                l_bool(*Load)( PFile, l_ptr *, l_ulong *, l_ulong *, PErrorHandler ),
                                                l_ulong *DefaultExtraSave,      l_ulong *DefaultExtraLoad,
                                                l_text  ExtraSaveSetApp,l_text  ExtraLoadSetApp ) {
        PSubCodec o = malloc(sizeof(TSubCodec));
        if ( !o ) return NULL;
        memset(o,0,sizeof(TSubCodec));

        o->TypeId = TypeId;
        o->CodecId = CodecId;
        o->Flags = Flags;
        o->SaveToFile = SaveToFile;
        o->LoadFromFile = LoadFromFile;
        o->Save = Save;
        o->Load = Load;
        o->DefaultExtraSave = DefaultExtraSave;
        o->DefaultExtraLoad = DefaultExtraLoad;
        o->ExtraSaveSetApp = TextDup(ExtraSaveSetApp);
        o->ExtraLoadSetApp = TextDup(ExtraLoadSetApp);

        if ( !GetDefaultSubCodec(TypeId) && Save && Load ) SetDefaultSubCodec(TypeId,o);

        return o;
}
////////////////////////////////////////////////////////////////////////////////
PSubCodec AddNewSubCodec ( PCodec Codec, l_ulong TypeId, l_ulong        Flags,
                                                l_bool(*SaveToFile)( l_text , l_ptr  , l_ulong *, PErrorHandler ),
                                                l_bool(*LoadFromFile)( l_text, l_ptr *, l_ulong *, PErrorHandler ),
                                                l_bool(*Save)( PFile, l_ptr, l_ulong *, l_ulong *, PErrorHandler ),
                                                l_bool(*Load)( PFile, l_ptr *, l_ulong *, l_ulong *, PErrorHandler ),
                                                l_ulong *DefaultExtraSave,      l_ulong *DefaultExtraLoad,
                                                l_text  ExtraSaveSetApp,l_text  ExtraLoadSetApp ) {

        PSubCodec o;
        if ( !(o = _GetSubCodec(Codec,TypeId )) ) {
                o = NewSubCodec(TypeId,Codec->CodecId,Flags,SaveToFile,LoadFromFile,Save,Load   ,DefaultExtraSave,DefaultExtraLoad,ExtraSaveSetApp,ExtraLoadSetApp);
                ListAdd(Codec->SubCodecs,NULL,o,(void*)FreeSubCodec);
                if ( !Codec->FavoriteType ) Codec->FavoriteType = TypeId;
        } else
                DebugWarning("DMS2 : Codec %08x has already a sub codec for type %08x",Codec->CodecId,TypeId);
        return o;
}
////////////////////////////////////////////////////////////////////////////////
void AddSubCodec ( PCodec Codec, PSubCodec o ) {
        if ( _GetSubCodec(Codec,o->TypeId ) )
                DebugWarning("DMS2 : Codec %08x has already a sub codec for type %08x",Codec->CodecId,o->TypeId);
        else
                ListAdd(Codec->SubCodecs,NULL,o,NULL);
        if ( !Codec->FavoriteType ) Codec->FavoriteType = o->TypeId;
}
////////////////////////////////////////////////////////////////////////////////
void SetDefaultSubCodec ( l_ulong TypeId, PSubCodec sc ) {
        PDataTypeMan o = _GetDataTypeMan(TypeId);
        if ( o )
                o->DefaultSubcodec = sc;
        else
                DebugError("DMS2 : Unable to set default sub codec for type %08x",TypeId);
}
////////////////////////////////////////////////////////////////////////////////
PSubCodec _GetSubCodec ( PCodec o, l_ulong TypeId ) {
        PListItem a,b;
        if ( !o->SubCodecs->Last ) return NULL;
        a = b = o->SubCodecs->Last->Next;
        do {
                if ( SUBCODEC(a->Data)->TypeId == TypeId ) return SUBCODEC(a->Data);
                a = a->Next;
        } while( a != b );

        DebugWarning("DMS2 : Codec %x do not support data type %x",o->CodecId,TypeId);

        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PSubCodec GetSubCodec ( l_ulong CodecId, l_ulong TypeId ) {
        PCodec o = GetCodec(CodecId);
        PSubCodec sc = 0;
        PDataTypeMan dtm;
        if ( o ) sc = _GetSubCodec(o,TypeId);
        if ( !sc ) {
                dtm = GetDataTypeMan(TypeId);
                if ( dtm ) {
                        sc = dtm->DefaultSubcodec;
                        if ( sc )
                                DebugWarning("DMS2 : Use default sub-codec instaed");
                        else
                                DebugError("DMS2 : No subcodec found for data type %x",TypeId);
                } else
                        DebugError("DMS2 : No subcodec found for data type %x",TypeId);
        }
        return sc;
}
////////////////////////////////////////////////////////////////////////////////
PSubCodec GetSubCodecFileExtention (  l_text FExt, l_ulong TypeId ) {
        PCodec o = GetCodecFileExtention(FExt);
        PSubCodec sc = 0;
        PDataTypeMan dtm;
        if ( o ) sc = _GetSubCodec(o,TypeId);
        if ( !sc ) {
                dtm = GetDataTypeMan(TypeId);
                if ( dtm ) {
                        sc = dtm->DefaultSubcodec;
                        if ( sc )
                                DebugWarning("DMS2 : Use default sub-codec instaed");
                        else
                                DebugError("DMS2 : No subcodec found for data type %x",TypeId);
                } else
                        DebugError("DMS2 : No subcodec found for data type %x",TypeId);
        }
        return sc;
}
////////////////////////////////////////////////////////////////////////////////
PSubCodec GetSubCodecMimeType (  l_text Mime, l_ulong TypeId ) {
        PCodec o = GetCodecMimeType(Mime);
        PSubCodec sc = 0;
        PDataTypeMan dtm;
        if ( o ) sc = _GetSubCodec(o,TypeId);
        if ( !sc ) {
                dtm = GetDataTypeMan(TypeId);
                if ( dtm ) {
                        sc = dtm->DefaultSubcodec;
                        if ( sc )
                                DebugWarning("DMS2 : Use default sub-codec instaed");
                        else
                                DebugError("DMS2 : No subcodec found for data type %x",TypeId);
                } else
                        DebugError("DMS2 : No subcodec found for data type %x",TypeId);
        }
        return sc;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Found the sub-codec for the specified file and data type.
        Determines the codec using the file extention.
        @param File File path
        @param TypeId Data type identifier
        @return Pointer to sub-codec, or null if not none corresponding
*/
PSubCodec GetSubCodecFile ( l_text File, l_ulong TypeId ) {
        return GetSubCodecFileExtention(GetFileExtention(File),TypeId);
}
////////////////////////////////////////////////////////////////////////////////
/**
        Get default sub-codec for specified data type
        @param TypeId Type
        @return Pointer to sub-codec, or NULL if none installed
*/
PSubCodec GetDefaultSubCodec ( l_ulong TypeId ) {
        PDataTypeMan o = _GetDataTypeMan(TypeId);
        if ( o )
                return o->DefaultSubcodec;
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
// Save data functions
////////////////////////////////////////////////////////////////////////////////
l_bool SubCodecSaveEmu ( PSubCodec o, l_text File, l_ptr Data, l_ulong *ExtraSave, PErrorHandler h ) {
        PFile f = FileOpen(File,"wb");
        l_bool r = false;
        if ( f ) {
                r = o->Save(f,Data,ExtraSave,NULL,h);
                FileClose(f);
        } else
                CreateError(h,ERROR_CANTOPENTOWRITE,"File not found or read-only file");
        return r;
}
////////////////////////////////////////////////////////////////////////////////
l_bool SaveDataEx ( l_text File, void *Data, l_ulong TypeId, l_ulong CodecId, l_ulong *ExtraSave, PErrorHandler h ) {
        PSubCodec sc = 0;
        if ( CodecId )
                sc = GetSubCodec(CodecId,TypeId);
        else
                sc = GetSubCodecFile(File,TypeId);
        if ( !sc ) return false;
        if ( !ExtraSave ) ExtraSave = sc->DefaultExtraSave;
        if ( sc->SaveToFile )
                return sc->SaveToFile(File,Data,ExtraSave,h);
        else if ( sc->Save )
                return SubCodecSaveEmu(sc,File,Data,ExtraSave,h);
        DebugMessage("DMS 2 : %s : Codec is unable to save %x data type",File,TypeId);
        return false;
}
////////////////////////////////////////////////////////////////////////////////
/** Saves specified data to a file.
        @param File Path to the file
        @param Data Data to write
        @param TypeId Type of data
        @return true on sucess, else returns false (no codec, or codec can't write, or file error)
        @note use SaveDataEx for advanced options
        Similar to SaveDataEx(File,Data,TypeId,0,NULL);
        @see SaveDataEx
        @see LoadData2
        @note This function doesn't support ressources adressing.
*/
l_bool SaveData2 ( l_text File, void*Data, l_ulong TypeId ) {
        return SaveDataEx(File,Data,TypeId,0,0,NULL);
}
////////////////////////////////////////////////////////////////////////////////
// Load data functions
////////////////////////////////////////////////////////////////////////////////
l_bool SubCodecLoadEmu ( PSubCodec o, l_text File, l_ptr *Data, l_ulong *ExtraLoad, PErrorHandler h ) {
        PFile f = FileOpen(File,"rb");
        l_bool r = false;
        if ( f ) {
                r = o->Load(f,Data,ExtraLoad,NULL,h);
                FileClose(f);
        } else
                CreateError(h,ERROR_FILENOTFOUND,"File not found");
        return r;
}
////////////////////////////////////////////////////////////////////////////////
/** Loads data from a file as spefied type usign the specified codec.
        @param File Path to the file
        @param TypeId Type of data to return
        @param CodecId Identifier of codec to use (if 0 autodetect codec)
        @param ExtraLoad Pointer to extra load options of codec (if NULL autofill values)
        @param h Error handler to get information on eventual error
        @return On sucess returns pointer to data, else return NULL if sytem was unable to load file as the specified data type (no codec or data type not avaible for this codec or file error)
        @see LoadData2
        @see SaveData2
        @note This function doesn't support ressources adressing.
*/
void *LoadDataEx ( l_text File, l_ulong TypeId, l_ulong CodecId, l_ulong *ExtraLoad, PErrorHandler h ) {
        PSubCodec sc = 0;
        void *Data = 0;
        if ( CodecId )
                sc = GetSubCodec(CodecId,TypeId);
        else
                sc = GetSubCodecFile(File,TypeId);
        if ( !sc || !File) return NULL;
        if ( !ExtraLoad ) ExtraLoad = sc->DefaultExtraLoad;
        DebugMessage("DMS2 : Load file %s using codec %08x to type %08x",File,sc->CodecId,sc->TypeId);
        if ( sc->Load || sc->LoadFromFile ) {
                if ( sc->LoadFromFile ) {
                        if ( sc->LoadFromFile(File,&Data,ExtraLoad,h) )
       {
        DebugMessage("DMS2 : Load file %s ", File);
       return Data;
       }
                } else {
                        if ( SubCodecLoadEmu(sc,File,&Data,ExtraLoad,h) )
       {
        DebugMessage("DMS2 : Sub codec load emu %s ", File);
       return Data;
       }
                }
                DebugMessage("DMS2 : Codec is unable to load file");
                return NULL;
        }
        DebugMessage("DMS2 : %s Codec is unable to load %x data type",File,TypeId);
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PList   CachedRes = NULL;
l_text  CachedResFile = NULL;
////////////////////////////////////////////////////////////////////////////////
void* LoadDataFromRessource ( l_ulong TypeId, l_text File, l_text Item ) {
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
        if ( a->DataType != TypeId ) return NULL;
        return DuplicateData2(a->DataType,a->Data);
}
////////////////////////////////////////////////////////////////////////////////
/** Loads data from a file as specified type.
        @param File Path to the file to load
        @param TypeId Identifier of data type to return
        @return On sucess returns pointer to data, else return NULL if sytem was unable to load file as the specified data type (no codec or data type not avaible for this codec or file error)
        @note Don't forget to free data : you can use FreeData2(TypeId, Data);
        You can load an item form a ressource susing this syntax : "/path/to/file.res#item"; ex: "/system/xlib/cons.dl#ICON32"
        @see LoadDataEx
        @see SaveData2
*/
void * LoadData2 ( l_text File, l_ulong TypeId ) {
        l_text Item;
        if ( Item = TextRChr(File,'#') ) {
                void *Data;
                l_text ResFile = TextNDup(File,Item-File);
                Item++;
                Data = LoadDataFromRessource(TypeId,ResFile,Item);
                free(ResFile);
                return Data;
        } else

        return LoadDataEx(File,TypeId,0,0,NULL);
}
////////////////////////////////////////////////////////////////////////////////
// Binary data
////////////////////////////////////////////////////////////////////////////////
PBinData NewBinData ( void *Data, l_ulong Size ) {
        PBinData o = malloc(sizeof(TBinData));
        o->Data = Data;
        o->Size = Size;
        return o;
}
////////////////////////////////////////////////////////////////////////////////
void *DuplicateBinData ( void* o ) {
        if ( o ) {
                void    *nd = malloc(BINDATA(o)->Size);
                PBinData n;
                if ( !nd ) return NULL;
                memcpy(nd,BINDATA(o)->Data,BINDATA(o)->Size);
                n = NewBinData(nd,BINDATA(o)->Size);
                if ( !n ) free(nd);
                return n;
        }
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void FreeBinData ( void* o ) {
        if( BINDATA(o)->Data ) free(BINDATA(o)->Data);
        free(o);
}
////////////////////////////////////////////////////////////////////////////////
// Duplicators
////////////////////////////////////////////////////////////////////////////////
void *DuplicateImage (void *Data ) {
        if ( Data ) {
                BITMAP *s =( BITMAP*)Data;
                BITMAP *b = create_bitmap(s->w,s->h);
                if ( b ) blit(s,b,0,0,0,0,s->w,s->h);
                return b;
        }
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void *DuplicateFileRef (void *Data ) {
        PFileRef s = (PFileRef)Data;
        PFileRef d = malloc(sizeof(TFileRef));
        if ( !d ) return NULL;

        d->Drive                = s->Drive;
        d->Size                 = s->Size;
        d->Attributes   = s->Attributes;
        d->LocalName    = TextDup(s->LocalName);
        d->SName                = TextDup(s->SName);

        return d;
}
////////////////////////////////////////////////////////////////////////////////
void *DuplicateFileRefList (void *Data ) {
        PList l = (PList)Data;
        PList n = NewList();
        PListItem a, b;
        b = a = l->Last;
        do {
                ListAdd(n,a->Key,DuplicateFileRef(FILEREF(a->Data)),(void*)&FreeFileRef);
                a = a->Next;
        } while ( a != b);
        return n;
}
////////////////////////////////////////////////////////////////////////////////
// Icons managers
////////////////////////////////////////////////////////////////////////////////
PRleSprite NewRleSpriteStretch (  PImage ico, l_int size ) {
        PRleSprite s;
        PImage buffer = create_bitmap(size,size);
        clear_to_color(buffer, ico->vtable->mask_color);
        stretch_sprite(buffer, ico, 0, 0, size, size);
        s = get_rle_sprite(buffer);
        destroy_bitmap(buffer);
        return s;
}
////////////////////////////////////////////////////////////////////////////////
PIcon NewIcon2 ( p_bitmap i16, p_bitmap i32, p_bitmap i48 ) {
        PIcon i = malloc(sizeof(TIcon));
        if ( !i ) return NULL;
        i->Instances = 1;

        if ( i16 )
                i->Icon16 = get_rle_sprite(i16);
        else if ( i32 )
                i->Icon16 = NewRleSpriteStretch(i32,16);
        else
                i->Icon16 = NewRleSpriteStretch(i48,16);

        if ( i32 )
                i->Icon32 = get_rle_sprite(i32);
        else if ( i48 )
                i->Icon32 = NewRleSpriteStretch(i48,32);
        else
                i->Icon32 = NewRleSpriteStretch(i16,32);

        if ( i48 )
                i->Icon48 = get_rle_sprite(i48);
        else if ( i32 )
                i->Icon48 = NewRleSpriteStretch(i32,48);
        else
                i->Icon48 = NewRleSpriteStretch(i16,48);

        return i;
}
////////////////////////////////////////////////////////////////////////////////
PIcon NewIcon ( p_bitmap i16, p_bitmap i32, p_bitmap i48 ) {
        PIcon i = NewIcon2(i16,i32,i48);
        destroy_bitmap(i16);
        destroy_bitmap(i32);
        destroy_bitmap(i48);
        return i;
}
////////////////////////////////////////////////////////////////////////////////
PIcon DuplicateIcon ( PIcon i ) { // Will help to save memory
        if ( i )
                i->Instances++;
        return i;
}
////////////////////////////////////////////////////////////////////////////////
void FreeIcon ( PIcon i ) {
        i->Instances--;
        if ( !i->Instances ) {
                if ( i->Icon16 ) destroy_rle_sprite(i->Icon16);
                if ( i->Icon32 ) destroy_rle_sprite(i->Icon32);
                if ( i->Icon48 ) destroy_rle_sprite(i->Icon48);
                free(i);
        }
}
////////////////////////////////////////////////////////////////////////////////
// Data operators
////////////////////////////////////////////////////////////////////////////////
/** Frees specified data, characterized by its type
        @param TypeId Data type identifier
        @param Data Pointer to data to free
*/
void FreeData2 ( l_ulong TypeId, void *Data ) {
        PDataTypeMan o = GetDataTypeMan(TypeId);
        if ( o )
                if ( o->Free )
                        o->Free(Data);
}
////////////////////////////////////////////////////////////////////////////////
/** Ducplicates specified data, characterized by its type
        @param TypeId Data type identifier
        @param Data Pointer to data to duplicate
        @return On sucess, pointer to duplicated data, or NULL on error (not enough memory most of times)
*/
void *DuplicateData2 ( l_ulong TypeId, void *Data ) {
        PDataTypeMan o = GetDataTypeMan(TypeId);
        if ( o )
                if ( o->Duplicate )
                        return o->Duplicate(Data);
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void InstallGatewayFromReg ( PRegKey o ) {
        l_ulong Id = strtol(o->Name,NULL,16);
        PRegKey a, b;
        PCodec c;
        DebugMessage("DMS2 : Ignore next warning message");
        c = GetCodec(Id);
        if ( !c ) c = AddCodec(NULL,Id,NULL,NULL,NULL,NULL,NULL,NULL,0,CDCF_FREEICONS);
        if ( o->Type == RKT_TEXT) {
                if ( c->Name ) free(c->Name);
                c->Name = TextDup((l_text)o->Data);
        }

        if ( o->Last ) {
                a = b = o->Last->Next;
                do {
                        if ( !TextCaseCompare(a->Name,"FavoriteType" ) && (a->Type == RKT_LONG) )
                                c->FavoriteType = *((l_ulong*)a->Data);
                        else {
                                if ( a->Type == RKT_LONG ) {
                                        DebugMessage("DMS2 : New gateway from codec %08x -> to data type %08x",c->CodecId,*((l_ulong*)a->Data));
                                        AddNewSubCodec (c,*((l_ulong*)a->Data),0,0,0,0,0,0,0,0,0);
                                } else if ( a->Type == RKT_TEXT ) {
                                        DebugMessage("DMS2 : Sub codec %s (into %08x)",a->Data,c->CodecId);
                                        if ( !TextCaseCompare((l_text)a->Data,"GenericBinary" ) )
                                                AddSubCodec(c,GenericBinary);
                                        else if ( !TextCaseCompare((l_text)a->Data,"GenericText" ) )
                                                AddSubCodec(c,GenericText);
                                        else if ( !TextCaseCompare((l_text)a->Data,"GenericUText" ) )
                                                AddSubCodec(c,GenericUText);
                                }
                        }
                        a = a->Next;
                } while ( a != b );
        }
}
////////////////////////////////////////////////////////////////////////////////
void InstallCodecsIcons ( PRegKey o ) {
        l_ulong Id = strtol(o->Name,NULL,16);
        PRegKey a, b;
        PCodec c;
        PImage Img16 = NULL,Img32 = NULL,Img48 = NULL;
        c = GetCodec(Id);
        if ( !c ) return;

        if ( o->Last ) {
                a = b = o->Last->Next;
                do {
                        if ( !TextCaseCompare(a->Name,"Icon16" ) && ( a->Type == RKT_TEXT ) )
                                Img16 = LoadData2((l_text)a->Data,DTYPE_IMAGE);
                        else if ( !TextCaseCompare(a->Name,"Icon32" ) && ( a->Type == RKT_TEXT ) )
                                Img32 = LoadData2((l_text)a->Data,DTYPE_IMAGE);
                        else if ( !TextCaseCompare(a->Name,"Icon48" ) && ( a->Type == RKT_TEXT ) )
                                Img48 = LoadData2((l_text)a->Data,DTYPE_IMAGE);
                        a = a->Next;
                } while ( a != b );
        }
        if ( Img48 || Img32 || Img16 ) {
                if ( c->Default ) FreeIcon(c->Default);
                c->Default = NewIcon(Img16,Img32,Img48);
        }
}
////////////////////////////////////////////////////////////////////////////////
l_text GetFileExtention ( l_text File )
{
        l_text r = TextRChr(File,'.');
        if ( r ) return r+1;
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PCodec ResCdc = NULL;
////////////////////////////////////////////////////////////////////////////////
l_bool RESLoadFromFile ( l_text file, l_ptr *Data, l_ulong *Extra, PErrorHandler h ) {
  (*Data) = (void*)LoadRessourceFile(file);
  if ( *Data )
                return true;
        else
                return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool RESSaveToFile   ( l_text file, l_ptr  Data, l_ulong *Extra, PErrorHandler h ) {
        SaveRessourceFile(file,(PList)Data);
        return true;
}
////////////////////////////////////////////////////////////////////////////////
void DMS2Init ( void ) {
        PRegKey o;


        FExts    = NewList();
        MimeDats = NewList();
        Codecs   = NewList();
        DatMan   = NewList();

        if ( !FExts || !MimeDats || !Codecs || !DatMan ) {
                DMS2DeInit();
                DebugFatal("DMS2 :: NOT ENOUGH MEMORY");
                return;
        }

        SYSEXPORT(FExts);
        SYSEXPORT(MimeDats);
        SYSEXPORT(Codecs);
        SYSEXPORT(DatMan);
        SYSEXPORT(GetDataTypeMan);
        SYSEXPORT(AddMimeType);
        SYSEXPORT(AddFileExtention);
        SYSEXPORT(RegisterDataType);
        SYSEXPORT(AddCodec);
        SYSEXPORT(GetCodecFileExtention);
        SYSEXPORT(GetCodec);
        SYSEXPORT(AddNewSubCodec);
        SYSEXPORT(NewSubCodec);
        SYSEXPORT(AddSubCodec);
        SYSEXPORT(SetDefaultSubCodec);
        SYSEXPORT(GetSubCodec);
        SYSEXPORT(GetSubCodecFileExtention);
        SYSEXPORT(GetSubCodecMimeType);
        SYSEXPORT(SaveDataEx);
        SYSEXPORT(SaveData2);
        SYSEXPORT(LoadDataEx);
        SYSEXPORT(LoadData2);
        SYSEXPORT(NewBinData);
        SYSEXPORT(FreeData2);
        SYSEXPORT(DuplicateData2);
        SYSEXPORT(GenericText);
        SYSEXPORT(GenericBinary);
        SYSEXPORT(GenericUText);
        SYSEXPORT(GetDefaultSubCodec);
        SYSEXPORT(GetCodecFileInfo);
        SYSEXPORT(LoadRessourceFile);
        SYSEXPORT(SaveRessourceFile);
        SYSEXPORT(DMSWriteRessource);
        SYSEXPORT(DMSLoadRessource);
        SYSEXPORT(DMS2InstallIcons);
        SYSEXPORT(NewIcon);
        SYSEXPORT(NewIcon2);
        SYSEXPORT(FreeIcon);
        SYSEXPORT(DuplicateIcon);
        SYSEXPORT(DMS2InstallIcons);
        SYSEXPORT(GetDynLdIconEx);
        SYSEXPORT(GetDynLdIcon);
        SYSEXPORT(FreeBinData);
        // Load system sub-codecs

        InitGenericSubCodecs();

        // Generic and default handlers

                // NB: If a data type is not registred clipboard can't use it.
        RegisterDataType(DTYPE_BINDATA,&FreeBinData,&DuplicateBinData);
        RegisterDataType(DTYPE_TEXT,&free,(void*)&TextDup);
        RegisterDataType(DTYPE_UTEXT,&free,(void*)&UTextDup);
        RegisterDataType(DTYPE_IMAGE,(void*)&destroy_bitmap,&DuplicateImage);
        RegisterDataType(TYPE_FILEREF,(void*)&FreeFileRef,&DuplicateFileRef);
        RegisterDataType(TYPE_FILEREFLIST,(void*)&FreeList,&DuplicateFileRefList);
        RegisterDataType(DTYPE_ICON,(void*)&FreeIcon,(void*)&DuplicateIcon);

        SetDefaultSubCodec(DTYPE_BINDATA,GenericBinary);
        SetDefaultSubCodec(DTYPE_TEXT,GenericText);
        SetDefaultSubCodec(DTYPE_UTEXT,GenericUText);

        // Registry

        o = ResolveKey("/SYSTEM/DMS/Codecs/Codecs");

        if ( o ) {
                PRegKey a, b;
                if ( o->Last ) {
                        a = b = o->Last->Next;
                        do {
                                InstallGatewayFromReg(a);
                                a = a->Next;
                        } while ( a != b );
                }
        }

        o = ResolveKey("/SYSTEM/DMS/Extentions");

        if ( o ) {
                PRegKey a, b;
                if ( o->Last ) {
                        a = b = o->Last->Next;
                        do {
                                if ( a->Type == RKT_LONG )
                                        AddFileExtention(a->Name,*((l_ulong*)a->Data));
                                a = a->Next;
                        } while ( a != b );
                }
        }
        ResCdc = AddCodec("oZone DMS ressource file",ULONG_ID('D','M','S','R'),0,0,0,NULL,"res","binary/ressource",DTYPE_RESSOURCE,0 );
        AddSubCodec(ResCdc,GenericBinary);
        AddNewSubCodec(ResCdc,DTYPE_RESSOURCE,0,&RESSaveToFile,&RESLoadFromFile,0,0,0,0,0,0 );



}
////////////////////////////////////////////////////////////////////////////////
void DMS2InstallIcons ( void ) {

        PRegKey o = ResolveKey("/SYSTEM/DMS/Codecs/Codecs");

        if ( o ) {
                PRegKey a, b;
                if ( o->Last ) {
                        a = b = o->Last->Next;
                        do {
                                InstallCodecsIcons(a);
                                a = a->Next;
                        } while ( a != b );
                }
        }


}
////////////////////////////////////////////////////////////////////////////////
void DMS2DeInit ( void ) {

        if ( FExts )    FreeList(FExts);
        if ( MimeDats ) FreeList(MimeDats);
        if ( Codecs )   FreeList(Codecs);
        if ( DatMan )   FreeList(DatMan);

}
////////////////////////////////////////////////////////////////////////////////
/**
@}
@}
*/
