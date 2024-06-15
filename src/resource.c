////////////////////////////////////////////////////////////////////////////////
//
//	Resource files - DMS V2 - Core file
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#define DMS2

#include "kernel.h"
////////////////////////////////////////////////////////////////////////////////
PList DMSLoadRessource ( PFile f, l_ulong Entrys ) {
  PList Lst = NewList();
  TResEntry E;
  //PCodec  M;
  PSubCodec M;
  l_ulong   O;
  if ( !Lst ) return NULL;
  #ifdef TEST
    DebugMessage ("LoadRessource :: Starting loading 0x%08x from file 0x%08x",Lst,f);
  #endif
  while ( Entrys ) {
    FileRead(&E,sizeof(TResEntry),1,f);
    //M = FoundCodecByEncoder(E.DataType,E.DataEncode);
    M = GetSubCodec(E.DataEncode,E.DataType);
	  if ( M ) {
		  void *Data;
		  	FileGetPos(f,&O);
			O += E.DataSize;
		  if ( M->Load(f,&Data/*,&(E.DataSize)*/,&O,M->DefaultExtraLoad,NULL) ) {
			  PListItem i;
			  PDataTypeMan m = GetDataTypeMan(E.DataType);
			  void *fr = NULL;
			  if ( m ) fr = m->Free;
		    i =  NewListItemEx(E.Name,Data,fr,E.DataType,0/*E.DataSize*/);
		    ListAddItem(Lst,i);
		    #ifdef TEST
		      DebugMessage ("LoadRessource :: 0x%08x :: %d '%s' loaded, %s",Lst,Entrys,E.Name,Data);
		    #endif
	    } else
	      DebugError ("LoadRessource :: 0x%08x :: DataType/DataEncode do not support loading from ressource file ! : 0x%08x/0x%08x",Lst,E.DataType,E.DataEncode);
	  } else
	    DebugError ("LoadRessource :: 0x%08x :: Unknow DataType/DataEncode : 0x%08x/0x%08x",Lst,E.DataType,E.DataEncode);
	  Entrys--;
  }
  return Lst;
}
////////////////////////////////////////////////////////////////////////////////
void *DMSLoadSpecificRessource ( PFile f, l_ulong Entrys, l_text Name, l_ulong Type ) {
	TResEntry E;
	PSubCodec M;
	l_ulong   O;
	while ( Entrys ) {
		FileRead(&E,sizeof(TResEntry),1,f);
		
		if ( TextCaseCompare(E.Name,Name) ) {
			FileGetPos(f,&O);
			O += E.DataSize;
			FileSetPos(f,&O);		
		} else {
			if ( E.DataType != Type ) return NULL;
			
			M = GetSubCodec(E.DataEncode,E.DataType);
			if ( M ) {
				void *Data;
				FileGetPos(f,&O);
				O += E.DataSize;
				if ( M->Load(f,&Data,&O,M->DefaultExtraLoad,NULL) ) {
					return Data;
				}
			}
		} 
		Entrys--;
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void DMSWriteRessource ( PFile f, PList Lst, l_ulong *Entrys ) {
	PListItem a,b;

	TResEntry E;
  //PCodec  M;
  PSubCodec M;

  l_ulong   O = 0, St, En;
  #ifdef TEST
	  DebugMessage("WriteRessource :: 0x%08x :: START",Lst);
	#endif
	(*Entrys) = 0;
	if ( !Lst->Last ) return;

	a = b = Lst->Last->Next;

	do {
		//M = DefaultRessourceCodec(a->DataType);
		M = GetDefaultSubCodec(a->DataType);
		
		if ( M ) {
			if ( M->Save ) {
				memset(&E,0,sizeof(TResEntry));
			  /*E.DataType = M->DataTypeManaged;
			  E.DataEncode = M->DataEncodeManaged;*/
			  E.DataType = M->TypeId;
			  E.DataEncode = M->CodecId;
			  memcpy(&E.Name,a->Key,min(31,TextLen(a->Key)));

			  FileGetPos(f,&En);
			  FileWrite(&E,sizeof(TResEntry),1,f);
			  FileGetPos(f,&St);
				if ( M->Save(f,a->Data/*,a->DataSize*/,&O,M->DefaultExtraSave,NULL) ) {
				  E.DataSize = O-St;
				  #ifdef TEST
				    DebugMessage("WriteRessource :: Wrote... %d %d 0x%08x 0x%08x '%s', %s",E.DataSize,a->DataSize,E.DataType,E.DataEncode,E.Name, a->Data);
			    #endif
				  //fsetpos(f,&En);
			    FileSeek(f, En, SEEK_SET);
			    FileWrite(&E,sizeof(TResEntry),1,f);
			    //fsetpos(f,&O);
			    FileSeek(f, O, SEEK_SET);
		    } else
		      DebugError ("WriteRessource :: 0x%08x :: ERROR WRITTING TO FILE !!",Lst);
		  } else
        DebugError ("WriteRessource :: 0x%08x :: Default DataType dot not support write : 0x%08x",Lst,a->DataType);
    } else
      DebugError ("WriteRessource :: 0x%08x :: DataType not supported : 0x%08x",Lst,a->DataType);


		(*Entrys)++;
		a = a->Next;
	} while ( a != b );
	#ifdef TEST
	  DebugMessage("WriteRessource :: 0x%08x :: END",Lst);
  #endif
}
////////////////////////////////////////////////////////////////////////////////
l_bool SaveRessourceFile ( l_text File, PList Data  )
{
	PFile f;
	l_text a;
	TResHead Head;
	
	if ( a = TextRChr(File,'.') ) {
		if ( !TextCaseCompare(a,".app") || !TextCaseCompare(a,".dl") ) {
			TDynLdHeader DynHead;
			f = FileOpen(File,"rb+");
			if ( !f ) return false;
			FileRead(&DynHead, 1, sizeof(TDynLdHeader), f);
			if ( DynHead.Magic != ULONG_ID('D','n','L','d') ) {
				FileClose(f);
				DebugError("DYNLD/RESSOURCE :: Application is not a valid DynLD.");
				return false;
			}
			if ( DynHead.FileFormatVersion > DYNLDVERSION ) {
				FileClose(f);
				DebugError("DYNLD/RESSOURCE :: Application has invalid version number of DynLD linker.");
				return false;
			}
			FileSetPos(f,&(DynHead.RessourceOffset));
			DMSWriteRessource(f,Data,&(DynHead.RessourceEntries));
			FileSeek(f, 0, SEEK_SET);
			FileWrite(&DynHead,sizeof(TDynLdHeader),1,f);
			FileClose(f);
			return true;
		}
	}
	
	f = FileOpen(File,"wb");
	if ( !f ) return false;
	
	Head.Magic = ULONG_ID('D','M','S','R');
	Head.FormatVersion = ULONG_ID(0,0,0,1);
	FileWrite(&Head,sizeof(TResHead),1,f);

	DMSWriteRessource(f,Data,&Head.Entries);

	FileSeek(f, 0, SEEK_SET);
	FileWrite(&Head,sizeof(TResHead),1,f);
	FileClose(f);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
PList LoadRessourceFile ( l_text File )
{
	l_text a;
	PList Data = NULL;
	TResHead Head;
	PFile f = FileOpen(File, "rb");
	
	if ( !f ) return NULL;
	
	if ( a = TextRChr(File,'.') ) {
		if ( !TextCaseCompare(a,".app") || !TextCaseCompare(a,".dl") ) {
			TDynLdHeader DynHead;
			FileRead(&DynHead, 1, sizeof(TDynLdHeader), f);
			if ( DynHead.Magic != ULONG_ID('D','n','L','d') ) {
				FileClose(f);
				DebugError("DYNLD/RESSOURCE :: Application is not a valid DynLD.");
				return NULL;
			}
			if ( DynHead.FileFormatVersion > DYNLDVERSION ) {
				FileClose(f);
				DebugError("DYNLD/RESSOURCE :: Application has invalid version number of DynLD linker.");
				return NULL;
			}
			FileSetPos(f,&(DynHead.RessourceOffset));
			Data = DMSLoadRessource(f,DynHead.RessourceEntries);
			FileClose(f);
			return Data;
		}
	}

	FileRead(&Head,sizeof(TResHead),1,f);
	if ( Head.Magic == ULONG_ID('D','M','S','R') )
	{
		if ( Head.FormatVersion == ULONG_ID(0,0,0,1) )
		{
			Data = DMSLoadRessource(f,Head.Entries);
		}
	}

	FileClose(f);

	return Data;
}
////////////////////////////////////////////////////////////////////////////////
PIcon GetDynLdIconEx ( l_text File ) {
	PList Data = NULL;
	PFile f = FileOpen(File, "rb");
	p_bitmap i16,i32,i48;
	TDynLdHeader DynHead;

	if ( !f ) return NULL;

	FileRead(&DynHead, 1, sizeof(TDynLdHeader), f);
	if ( DynHead.Magic != ULONG_ID('D','n','L','d') ) {
		FileClose(f);
		DebugError("DYNLD/RESSOURCE :: Application is not a valid DynLD.");
		return NULL;
	}
	if ( DynHead.FileFormatVersion != DYNLDVERSION ) {
		FileClose(f);
		DebugError("DYNLD/RESSOURCE :: Application has invalid version number of DynLD linker.");
		return NULL;
	}
	if ( !DynHead.RessourceEntries ) {
		FileClose(f);
		return NULL;
	}
	
	FileSetPos(f,&(DynHead.RessourceOffset));
	i16 = DMSLoadSpecificRessource(f,DynHead.RessourceEntries,"ICON16",DTYPE_IMAGE);
	FileSetPos(f,&(DynHead.RessourceOffset));
	i32 = DMSLoadSpecificRessource(f,DynHead.RessourceEntries,"ICON32",DTYPE_IMAGE);
	FileSetPos(f,&(DynHead.RessourceOffset));
	i48 = DMSLoadSpecificRessource(f,DynHead.RessourceEntries,"ICON48",DTYPE_IMAGE);
	FileClose(f);
	if ( i16 || i32 || i48 )
		return NewIcon(i16,i32,i48);
	
	return NULL;
	
}
////////////////////////////////////////////////////////////////////////////////
PIcon GetDynLdIcon ( PFileInfo f ) {
	return GetDynLdIconEx(f->VPathName);
}
////////////////////////////////////////////////////////////////////////////////
