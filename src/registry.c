////////////////////////////////////////////////////////////////////////////////
//
//  Registry - Core File
//
//  Copyright (c) 2003 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "registry.h"
#include "internal.h"
#include "debug.h"


/*
updated /xlib path
del all xlib when you rebuild this.

*/
PRegKey MainKey = NULL;

void FreeKey( PRegKey o );

short RDSizes[12] = {
0, // RKT_NODATA
1, // RKT_CHAR
1, // RKT_BOOL
2, // RKT_SHORT
4, // RKT_INT
4, // RKT_LONG
8, // RKT_BIG
4, // RKT_FLOAT
8, // RKT_DOUBLE
16,// RKT_RECT
4, // RKT_POINT
0  // RKT_TEXT
};
////////////////////////////////////////////////////////////////////////////////
// Registry loader
////////////////////////////////////////////////////////////////////////////////

l_bool LoadRegistry ( void ) {
	l_ulong i = 0,Tmp;
	PRegKey Reg = NULL;
	PFile f = FileOpen("system/settings.dat", "rb");
	TRegHead Head;

	if ( !f ) return false;

	if ( MainKey ) FreeKey(MainKey);


	FileRead(&Head,1,sizeof(Head),f);

	Reg = (PRegKey)malloc(Head.Keys*sizeof(TRegKey));

	memset(Reg,0,Head.Keys*sizeof(TRegKey));

	while ( i < Head.Keys ) {

		Reg[i].Name = FileReadBinString(f);
		FileRead(&(Reg[i].Type),1,1,f);

		if ( Reg[i].Type == RKT_TEXT )
		  Reg[i].Data = FileReadBinString(f);
	  else if ( Reg[i].Type == RKT_TEXT ) {
			FileRead(&Tmp,1,4,f);
			if ( Tmp ) Reg[i].Data = &(Reg[Tmp-1]);
		} else if ( Reg[i].Type ) {
			Reg[i].Data = malloc(RDSizes[Reg[i].Type]);
			FileRead(Reg[i].Data,1,RDSizes[Reg[i].Type],f);
		}

		FileRead(&Tmp,1,4,f);
		if ( Tmp ) Reg[i].Parent = &(Reg[Tmp-1]);
		FileRead(&Tmp,1,4,f);
		if ( Tmp ) Reg[i].Next = &(Reg[Tmp-1]);
		FileRead(&Tmp,1,4,f);
		if ( Tmp ) Reg[i].Prev = &(Reg[Tmp-1]);
		FileRead(&Tmp,1,4,f);
		if ( Tmp ) Reg[i].Last = &(Reg[Tmp-1]);
	  i++;
	}

	MainKey = Reg;
	MainKey->__e = 1;
	FileClose(f);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
// Registry saving functions
////////////////////////////////////////////////////////////////////////////////
void GenerateTmpId ( PRegKey o, l_ulong *Id ) {
	o->TmpId = *Id;
	(*Id)++;
	if ( o->Last ) {
	  PRegKey a = o->Last;
	  PRegKey b = a;
	  do {
		  GenerateTmpId(a,Id);
		  a = a->Next;
	  } while ( a != b );
	}
}
////////////////////////////////////////////////////////////////////////////////
l_ulong Nothing = 0;
void WriteKey ( PFile f, PRegKey o ) {
	FileWriteBinString(f,o->Name);
	FileWrite(&(o->Type),1,1,f);
	if ( o->Type == RKT_TEXT )
		FileWriteBinString(f,o->Data);
	else if ( o->Type == RKT_LINK )
		FileWrite(&REGKEY(o->Data)->TmpId,1,4,f);
	else if ( o->Type )
		FileWrite(o->Data,1,RDSizes[o->Type],f);

	FileWrite(o->Parent?&o->Parent->TmpId:&Nothing,1,4,f);
	FileWrite(o->Next?&o->Next->TmpId:&Nothing,1,4,f);
	FileWrite(o->Prev?&o->Prev->TmpId:&Nothing,1,4,f);
	FileWrite(o->Last?&o->Last->TmpId:&Nothing,1,4,f);
	if ( o->Last ) {
	  PRegKey a = o->Last;
	  PRegKey b = a;
	  do {
		  WriteKey(f,a);
		  a = a->Next;
	  } while ( a != b );
	}
}
////////////////////////////////////////////////////////////////////////////////
void SaveRegistry ( void ) {
	TRegHead Head;
	PFile f = FileOpen("system/settings.dat", "wb");
	if ( !f )
   {
   DebugMessage("Cannot open system/settings.dat!");
   return;
   }
	Head.Magic = NREGISTRY_MAGIC;
	Head.Keys = 1;
	GenerateTmpId(MainKey,&Head.Keys);
  Head.Keys--;
	FileWrite(&Head,1,sizeof(TRegHead),f);
	WriteKey(f,MainKey);
	FileClose(f);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
PRegKey ResolveKeyEx ( l_text Name, PRegKey o ) {
	l_text S;
	PRegKey a = o->Last;
	PRegKey b = a;
	if ( o->Type == RKT_LINK ) o = (PRegKey)o->Data;
	if ( *Name=='\\' || *Name == '/' ) Name++;
	if ( !(*Name) ) return o;
	if ( !a ) return NULL;
	S = Name;
  while ( *S ) {
	  if ( *S=='\\' || *S == '/' ) {
		  do {
		    if ( !TextSqNCaseCompare(Name,a->Name,S-Name) ) return ResolveKeyEx( S+1,a);
		    a = a->Next;
	    } while ( a != b );
		//DebugMessage("Errrrr %s",Name);
	    return NULL;
	  }
	  S++;
  }

  do {
		if ( !TextCompare(Name,a->Name) ) return a;
		a = a->Next;
	} while ( a != b );
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PRegKey ResolveKey ( l_text Name) {
	if ( DCkPt("ResolveKey.Name",Name) ) {
  	PRegKey o = ResolveKeyEx(Name,MainKey);
  	if ( !o ) DebugWarning("REGISTRY : Unknow key %s", Name);
  	return o;
 	}
 	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
l_bool KeyExists ( l_text Name ) {
  return 	ResolveKey(Name) ? true : false;
}
////////////////////////////////////////////////////////////////////////////////
void FreeKey( PRegKey o ) {
	if ( o->Data && o->Type != RKT_LINK && o->Type != RKT_NODATA ) free(o->Data);
	if ( o->Name ) free(o->Name);
	if ( o->Last ) {
	  PRegKey a = o->Last;
	  PRegKey b = a, n;
	  do {
		  n = a->Next;
		  FreeKey(a);
		  a = n;
	  } while ( a != b );
	}
	if ( o->__e ) free(o);
}
////////////////////////////////////////////////////////////////////////////////
void DeleteKeyEx( PRegKey o ) {
	PRegKey p;
	if ( !o ) return;
	if ( !o->Parent ) return;
	p = o->Parent;
	if ( o->Next == o ) {
	  p->Last = NULL;
  } else {
	  o->Next->Prev = o->Prev;
	  o->Prev->Next = o->Next;
	  if ( p->Last == o ) p->Last = o->Prev;
  }
	FreeKey(o);
}
////////////////////////////////////////////////////////////////////////////////
void DeleteKey( l_text Name ) {
  DeleteKeyEx(ResolveKey(Name));
}
////////////////////////////////////////////////////////////////////////////////
PRegKey NewKeyEx ( PRegKey p, l_text Name ) {
	PRegKey o ;
	if ( !p ) return 0;
	if ( ResolveKeyEx(Name,p) ) {
		DebugWarning("REGISTRY : Key %s do already exists",Name);
		return 0;
	}
	o = malloc(sizeof(TRegKey));
	if ( !o ) {
		DebugError("REGISTRY : Not enough memory to create key %s",Name);
		return 0;
	}
	memset(o,0,sizeof(TRegKey));
	o->Type = RKT_NODATA;
	o->Name   = TextDup(Name);
	if ( !o->Name ) {
		free(o);
		DebugError("REGISTRY : Not enough memory to create key %s",Name);
		return 0;
	}
	o->Parent = p;
	if ( p->Last ) {
		o->Next = p->Last->Next;
		o->Prev = p->Last;
		p->Last->Next->Prev = o;
		p->Last->Next = o;
		p->Last = o;
  } else {
	  o->Next = o;
	  o->Prev = o;
	  p->Last = o;
  }
  o->__e = 1;

	return o;
}
////////////////////////////////////////////////////////////////////////////////
l_uchar NewKey ( l_text Parent, l_text Name ) {
	PRegKey p = GetOrCreateKey(Parent);
	if ( !p ) return 0;
	if ( NewKeyEx(p,Name) ) return 1;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
PRegKey GetOrCreateKeyEx ( PRegKey o, l_text Name ) {
	l_text S,t;
	PRegKey a = o->Last;
	PRegKey b = a;

	if ( o->Type == RKT_LINK ) o = (PRegKey)o->Data;
	if ( *Name=='\\' || *Name == '/' ) Name++;
	if ( !(*Name) ) return o;

	S = Name;
  while ( *S ) {
	  if ( *S=='\\' || *S == '/' ) {
		 	if ( a ) do {
		    if ( !TextSqNCaseCompare(Name,a->Name,S-Name) ) return GetOrCreateKeyEx(a,S+1);
		    a = a->Next;
	    } while ( a != b );
	    t = TextNDup(Name,S-Name);

	    a = NewKeyEx(o,t);
	    return GetOrCreateKeyEx(a,S+1);
	  }
	  S++;
  }

  if ( a ) do {
		if ( !TextCompare(Name,a->Name) ) return a;
		a = a->Next;
	} while ( a != b );

	return NewKeyEx(o,Name);
}
////////////////////////////////////////////////////////////////////////////////
PRegKey GetOrCreateKey ( l_text key ) {
	return GetOrCreateKeyEx(MainKey,key);
}
////////////////////////////////////////////////////////////////////////////////
void CreateKey ( l_text key ) {
	GetOrCreateKeyEx(MainKey,key);
}
////////////////////////////////////////////////////////////////////////////////
l_uchar RenameKey ( l_text Key, l_text NewName ) {
	PRegKey o = ResolveKey(Key);
	if ( !o ) return 0;
	if ( o->Name )  free(o->Name);
	o->Name   = TextDup(NewName);
	return 1;
}
////////////////////////////////////////////////////////////////////////////////
l_text _GetKeyName ( PRegKey o ) {
	if ( !o->Parent ) {
		return TextDup("/");
	} else {
		l_text KName = TextDup("");
		l_text OName;
		while ( o->Parent ) {
			OName = KName;
			KName = FileNameToPath(o->Name,OName);
			free(OName);
			o = o->Parent;
		}
		return KName;
	}
}
////////////////////////////////////////////////////////////////////////////////
l_text GetParentKeyName ( l_text Key ) {
	l_text n;
	PRegKey o = ResolveKey(Key);
	if ( !o ) return NULL;
	if ( !o->Parent ) return NULL;
	n = _GetKeyName(o->Parent);
	DebugMessage("REGISTRY : %s parent is %s",key,n);
	return n;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
l_bool KeySetData ( l_text Name, l_uchar Type, void *Data ) {
	PRegKey o = GetOrCreateKey(Name);
	if ( !o ) return false;
	if ( o->Data && o->Type != RKT_LINK ) {
		free(o->Data);
	  o->Data = NULL;
	}
	o->Type = Type;
	if ( Type == RKT_LINK ) {
		o->Data = Data;
	} else if ( Type ) {
		if ( Type == RKT_TEXT ) {
			o->Data = TextDup((l_text)Data);
		} else {
			o->Data = malloc(RDSizes[Type]);
			memcpy(o->Data,Data,RDSizes[Type]);
		}
		if ( !o->Data ) {
			DebugError("REGISTRY : Not enough memory to set data into key %s",Name);
			return false;
		}
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void *KeyGetData( l_text Name, l_uchar Type, void *Def ) {
	PRegKey o = ResolveKey(Name);
	if ( !o ) return Def;
	if ( o->Type != Type ) return Def;
	return o->Data;
}
////////////////////////////////////////////////////////////////////////////////
l_bool KeySetText ( l_text Name, l_text Val ) {
	return KeySetData(Name,RKT_TEXT,Val);
}
////////////////////////////////////////////////////////////////////////////////
l_bool KeySetNothing ( l_text Name ) {
	return KeySetData(Name,0,0);
}
////////////////////////////////////////////////////////////////////////////////
l_text KeyGetText ( l_text Name, l_text Def ) {
	return KeyGetData(Name,RKT_TEXT,Def);
}
////////////////////////////////////////////////////////////////////////////////
l_bool KeySetLong ( l_text Name, l_long Val ) {
	return KeySetData(Name,RKT_LONG,&Val);
}
////////////////////////////////////////////////////////////////////////////////
l_long KeyGetLong ( l_text Name, l_long Def ) {
	return *((l_long*)KeyGetData(Name,RKT_LONG,&Def));
}
////////////////////////////////////////////////////////////////////////////////

l_bool KeySetChar ( l_text Name, l_char Val ) {
	return KeySetData(Name,RKT_CHAR,&Val);
}
////////////////////////////////////////////////////////////////////////////////
l_char KeyGetChar ( l_text Name, l_char Def ) {
	return *((l_char*)KeyGetData(Name,RKT_CHAR,&Def));
}
////////////////////////////////////////////////////////////////////////////////
l_bool KeySetBool ( l_text Name, l_bool Val ) {
	return KeySetData(Name,RKT_BOOL,&Val);
}
////////////////////////////////////////////////////////////////////////////////
l_bool KeyGetBool ( l_text Name, l_bool Def ) {
	return *((l_bool*)KeyGetData(Name,RKT_BOOL,&Def));
}
////////////////////////////////////////////////////////////////////////////////
l_bool KeySetShort ( l_text Name, l_short Val ) {
	return KeySetData(Name,RKT_SHORT,&Val);
}
////////////////////////////////////////////////////////////////////////////////
l_short KeyGetShort ( l_text Name, l_short Def ) {
	return *((l_short*)KeyGetData(Name,RKT_SHORT,&Def));
}
////////////////////////////////////////////////////////////////////////////////
l_bool KeySetInt ( l_text Name, l_int Val ) {
	return KeySetData(Name,RKT_INT,&Val);
}
////////////////////////////////////////////////////////////////////////////////
l_int KeyGetInt ( l_text Name, l_int Def ) {
	return *((l_int*)KeyGetData(Name,RKT_INT,&Def));
}
////////////////////////////////////////////////////////////////////////////////
l_bool KeySetBig ( l_text Name, l_ubig Val ) {
	return KeySetData(Name,RKT_BIG,&Val);
}
////////////////////////////////////////////////////////////////////////////////
l_ubig KeyGetBig ( l_text Name, l_ubig Def ) {
	return *((l_ubig*)KeyGetData(Name,RKT_BIG,&Def));
}
////////////////////////////////////////////////////////////////////////////////
l_int GetColorFromHex ( l_text Code ) {
	if ( TextLen(Code) == 6 ) {
		l_int c = strtol(Code, NULL, 16);
		return makecol((c >> 16) & 0xFF,(c >> 8) & 0xFF,c & 0xFF);
	} else
		return 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void RegistryRunEntries ( l_text Name ) {
  PRegKey o =	ResolveKey(Name);
	if ( !o ) return;
  DebugMessage("Run registry entries form %s",Name);
	if ( o->Last ) {
		PRegKey a = o->Last->Next;
	  PRegKey b = a;
	  do {
		  if ( a->Type == RKT_TEXT )
     DebugMessage("->Run %s...", (l_text)a->Data);
		  	DynLdRun((l_text)a->Data,NULL);
		  a = a->Next;
	  } while ( a != b );
  }
}

void RegistryReLoad ( void ) {
	DebugMessage("ReLoading registry...");
	LoadRegistry();
	DebugMessage("Done.");
}

void RegistrySave ( void ) {

	DebugMessage("Saving registry...");
	SaveRegistry();
	DebugMessage("Done.");
}

void NRegistryInit ( void ) {

	l_bool Generate = false;

	DebugMessage("Loading registry...");

	if ( KernelMode != KERNEL_RESETUP ) {
		if ( !LoadRegistry() )
			Generate = true;
	} else
		Generate = true;

	if ( Generate ) {

		DebugMessage("Registry file not found. Set default registry");
		MainKey = malloc(sizeof(TRegKey));
		if ( !MainKey )
			DebugFatal("REGISTRY: Not enough memory");
		memset(MainKey,0,sizeof(TRegKey));
		MainKey->Name = TextDup("");
		if ( !MainKey->Name ) {
			free(MainKey);
			DebugFatal("REGISTRY: Not enough memory");
		}
		KeySetText("/SYSTEM/LIBRARIES/jpg","xlib/jpg.dl");
		KeySetText("/SYSTEM/LIBRARIES/bmp","xlib/bmp.dl");
		KeySetText("/SYSTEM/LIBRARIES/png","xlib/png.dl");
		KeySetText("/SYSTEM/LIBRARIES/grfx","xlib/grfx.dl");
		KeySetText("/SYSTEM/LIBRARIES/clipbrd","xlib/clipbrd.dl");
		KeySetText("/SYSTEM/LIBRARIES/skin","xlib/skin.dl");
		KeySetText("/SYSTEM/LIBRARIES/widget","xlib/widget.dl");
		KeySetText("/SYSTEM/LIBRARIES/button","xlib/button.dl");
		KeySetText("/SYSTEM/LIBRARIES/label","xlib/label.dl");
		KeySetText("/SYSTEM/LIBRARIES/progress","xlib/progress.dl");
		KeySetText("/SYSTEM/LIBRARIES/checkbox","xlib/checkbox.dl");
		KeySetText("/SYSTEM/LIBRARIES/menu","xlib/menu.dl");
		KeySetText("/SYSTEM/LIBRARIES/canvas","xlib/canvas.dl");
		KeySetText("/SYSTEM/LIBRARIES/scrllbar","xlib/scrllbar.dl");
		KeySetText("/SYSTEM/LIBRARIES/textbox","xlib/textbox.dl");
		KeySetText("/SYSTEM/LIBRARIES/window","xlib/window.dl");
		KeySetText("/SYSTEM/LIBRARIES/listbox","xlib/listbox.dl");
		KeySetText("/SYSTEM/LIBRARIES/treeview","xlib/treeview.dl");
		KeySetText("/SYSTEM/LIBRARIES/groupbox","xlib/groupbox.dl");
		KeySetText("/SYSTEM/LIBRARIES/slider","xlib/slider.dl");
		KeySetText("/SYSTEM/LIBRARIES/listview","xlib/listview.dl");
		KeySetText("/SYSTEM/LIBRARIES/combobox","xlib/combobox.dl");
		KeySetText("/SYSTEM/LIBRARIES/toolbar","xlib/toolbar.dl");
		KeySetText("/SYSTEM/LIBRARIES/fms2","xlib/fms2.dl");
		KeySetText("/SYSTEM/LIBRARIES/iodlg","xlib/iodlg.dl");
		KeySetText("/SYSTEM/LIBRARIES/tabbook","xlib/tabbook.dl");
		KeySetText("/SYSTEM/LIBRARIES/cp","xlib/cp.dl");
		KeySetText("/SYSTEM/LIBRARIES/sound","xlib/sound.dl");
		KeySetText("/SYSTEM/LIBRARIES/mp3","xlib/mp3.dl");
		KeySetText("/SYSTEM/LIBRARIES/memfile","xlib/memfile.dl");

		CreateKey("/SYSTEM/BASICDRIVERS/bmp");
		CreateKey("/SYSTEM/BASICDRIVERS/png");
		CreateKey("/SYSTEM/BASICDRIVERS/jpg");

		CreateKey("/SYSTEM/DRIVERS/grfx");
		CreateKey("/SYSTEM/DRIVERS/clipbrd");
		CreateKey("/SYSTEM/DRIVERS/sound");

		CreateKey("/SYSTEM/AUTOLIB/skin");
		CreateKey("/SYSTEM/AUTOLIB/widget");
		CreateKey("/SYSTEM/AUTOLIB/button");
		CreateKey("/SYSTEM/AUTOLIB/label");
		CreateKey("/SYSTEM/AUTOLIB/progress");
		CreateKey("/SYSTEM/AUTOLIB/checkbox");
		CreateKey("/SYSTEM/AUTOLIB/menu");
		CreateKey("/SYSTEM/AUTOLIB/canvas");
		CreateKey("/SYSTEM/AUTOLIB/scrllbar");
		CreateKey("/SYSTEM/AUTOLIB/textbox");
		CreateKey("/SYSTEM/AUTOLIB/window");
		CreateKey("/SYSTEM/AUTOLIB/listbox");
		CreateKey("/SYSTEM/AUTOLIB/treeview");
		CreateKey("/SYSTEM/AUTOLIB/groupbox");
		CreateKey("/SYSTEM/AUTOLIB/slider");
		CreateKey("/SYSTEM/AUTOLIB/listview");
		CreateKey("/SYSTEM/AUTOLIB/combobox");
		CreateKey("/SYSTEM/AUTOLIB/toolbar");
		CreateKey("/SYSTEM/AUTOLIB/fms2");
		CreateKey("/SYSTEM/AUTOLIB/iodlg");
		CreateKey("/SYSTEM/AUTOLIB/tabbook");
		CreateKey("/SYSTEM/AUTOLIB/cp");


		KeySetText("/SYSTEM/STARTUP/desktop","xapps/desktop.app");


		KeySetInt("/SYSTEM/MOUSE/speed", 0);
		KeySetInt("/SYSTEM/MOUSE/dblclk", 4);
		KeySetText("/SYSTEM/MOUSE/CURSORS/arrow","SYSTEM/CURSORS/ARROW.BMP");
		KeySetText("/SYSTEM/MOUSE/CURSORS/resize","SYSTEM/CURSORS/RESIZE.BMP");

		KeySetInt("/SYSTEM/SCREEN/WIDTH",1024);
		KeySetInt("/SYSTEM/SCREEN/HEIGHT",768);
		KeySetInt("/SYSTEM/SCREEN/DEPTH",16);
		KeySetInt("/SYSTEM/SCREEN/REFRESH",60);

		KeySetText("/SYSTEM/KEYBOARD/LAYOUTS/France","SYSTEM/KEYBOARD/fr.okl");

		KeySetText("/SYSTEM/FONTS","SYSTEM/FONTS/HELV13.FNT");

		CreateKey("/USER/STARTUP");

		KeySetText("/USER/DESKTOP/color", "3D61AD");
		KeySetInt ("/USER/DESKTOP/alignment", 0);
		KeySetText("/USER/DESKTOP/wallpaper", "DESKTOP/start.bmp");
		KeySetText("/USER/DESKTOP/theme", "DESKTOP/plex.ini");

		CreateKey("/USER/GUI");
		KeySetText("/USER/GUI/SKIN", "(none)");
		KeySetText("/USER/GUI/SKINS/CLASSIX", "classix.ini");
		KeySetText("/USER/GUI/SKINS/NONE", "");
		KeySetText("/USER/GUI/SKINPATH", "SYSTEM/SKINS/");

		CreateKey("/USER/DESKTOP/THEME");
    KeySetText("/USER/DESKTOP/THEME", "desktop/aura.ini");


		KeySetText("/USER/COLOR/OSD", "888888");

		CreateKey("/SYSTEM/FMS");
		CreateKey("/SYSTEM/DMS");

		KeySetInt("/SYSTEM/DMS/Extentions/ln", 0x00000007);
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/00000007","Links");
		KeySetInt("/SYSTEM/DMS/Codecs/Codecs/00000007/FavoriteType",0x00000007);
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/00000007/text","GenericText");
		KeySetInt("/SYSTEM/DMS/Codecs/Codecs/00000007/link",0x00000007);


		KeySetInt("/SYSTEM/DMS/Extentions/dl", 0x44796e4c);
		KeySetInt("/SYSTEM/DMS/Extentions/app", 0x44796e4c);

		KeySetText("/SYSTEM/DMS/Codecs/Codecs/44796e4c","DynLd executales");
		KeySetInt("/SYSTEM/DMS/Codecs/Codecs/44796e4c/virtual",0x44796e4c);
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/44796e4c/binary","GenericBinary");
		KeySetInt("/SYSTEM/DMS/Codecs/Codecs/44796e4c/FavoriteType",0x44796e4c);

		KeySetInt("/SYSTEM/DMS/Extentions/exe", 0x44457865);
		KeySetInt("/SYSTEM/DMS/Extentions/com", 0x44457865);
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/44457865","Dos executable");
		KeySetInt("/SYSTEM/DMS/Codecs/Codecs/44457865/virtual",0x44457865);
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/44457865/binary","GenericBinary");
		KeySetInt("/SYSTEM/DMS/Codecs/Codecs/44457865/FavoriteType",0x44457865);

		KeySetText("/SYSTEM/FMS/44457865", "Dos execultable");
		KeySetText("/SYSTEM/FMS/44457865/Icon16", "/system/SYSTEM/ICONS/app16.bmp");
		KeySetText("/SYSTEM/FMS/44457865/Icon32", "/system/SYSTEM/ICONS/app32.bmp");

		CreateKey ("/SYSTEM/FMS/44457865/Actions");
		KeySetText("/SYSTEM/FMS/44457865/Actions/Execute", "/xapps/rundos.app");
		KeySetInt ("/SYSTEM/FMS/44457865/Actions/Execute/FavoriteWeight", 0xFFFFFFF);

		KeySetInt("/SYSTEM/DMS/Extentions/&folder", 0x44697220);
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/44697220","Directory (VFile)");
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/44697220/Icon16", "/system/SYSTEM/ICONS/folder16.bmp");
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/44697220/Icon32", "/system/SYSTEM/ICONS/folder32.bmp");
		KeySetInt("/SYSTEM/DMS/Codecs/Codecs/44697220/virtual",0x44697220);
		KeySetInt("/SYSTEM/DMS/Codecs/Codecs/44697220/FavoriteType",0x44697220);


		KeySetInt("/SYSTEM/DMS/Extentions/&hdd", 0x48444420);
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/48444420","Hard Drive (VFile)");
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/48444420/Icon16", "/system/SYSTEM/ICONS/drive16.bmp");
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/48444420/Icon32", "/system/SYSTEM/ICONS/drive32.bmp");
		KeySetInt("/SYSTEM/DMS/Codecs/Codecs/48444420/virtual",0x44697220);
		KeySetInt("/SYSTEM/DMS/Codecs/Codecs/48444420/FavoriteType",0x44697220);

		KeySetText("/SYSTEM/FMS/44697220", "Directory");
		KeySetText("/SYSTEM/FMS/44697220/Icon16", "/system/SYSTEM/ICONS/folder16.bmp");
		KeySetText("/SYSTEM/FMS/44697220/Icon32", "/system/SYSTEM/ICONS/folder32.bmp");

		CreateKey ("/SYSTEM/FMS/44697220/Actions");
		KeySetText("/SYSTEM/FMS/44697220/Actions/Open", "/xapps/nav.app");
		KeySetInt ("/SYSTEM/FMS/44697220/Actions/Open/FavoriteWeight", 0xFFFFFFF);

		KeySetText("/SYSTEM/FMS/00000003", "Image");
		KeySetText("/SYSTEM/FMS/00000003/Icon16", "/system/SYSTEM/ICONS/img16.bmp");
		KeySetText("/SYSTEM/FMS/00000003/Icon32", "/system/SYSTEM/ICONS/img32.bmp");

		CreateKey ("/SYSTEM/FMS/00000003/Actions");
		KeySetText("/SYSTEM/FMS/00000003/Actions/Open", "/xapps/immagine.app");
		KeySetInt ("/SYSTEM/FMS/00000003/Actions/Open/FavoriteWeight", 0xFFFFFFF);

		KeySetInt("/SYSTEM/DMS/Extentions/txt", 0x00000001);
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/00000001","Text file");
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/00000001/text","GenericText");
		KeySetText("/SYSTEM/DMS/Codecs/Codecs/00000001/binary","GenericBinary");
		KeySetInt("/SYSTEM/DMS/Codecs/Codecs/00000001/FavoriteType",0x00000001);

		KeySetText("/SYSTEM/FMS/00000001", "Text");
		KeySetText("/SYSTEM/FMS/00000001/Icon16", "/system/SYSTEM/ICONS/txt16.bmp");
		KeySetText("/SYSTEM/FMS/00000001/Icon32", "/system/SYSTEM/ICONS/txt32.bmp");

		CreateKey ("/SYSTEM/FMS/00000001/Actions");
		KeySetText("/SYSTEM/FMS/00000001/Actions/Open", "xapps/xtext.app");
		KeySetInt ("/SYSTEM/FMS/00000001/Actions/Open/FavoriteWeight", 0xFFFFFFF);

		KeySetText("/SYSTEM/FMS/00000006", "Ressource");
		KeySetText("/SYSTEM/FMS/00000006/Icon16", "/system/SYSTEM/ICONS/res16.bmp");
		KeySetText("/SYSTEM/FMS/00000006/Icon32", "/system/SYSTEM/ICONS/res32.bmp");
		CreateKey ("/SYSTEM/FMS/00000006/Actions");
		KeySetText("/SYSTEM/FMS/00000006/Actions/Open", "/xapps/resedit.app");
		KeySetInt ("/SYSTEM/FMS/00000006/Actions/Open/FavoriteWeight", 0xFFFF);

	}

	SYSEXPORT(RegistryReLoad);
	SYSEXPORT(RegistrySave);
	SYSEXPORT(ResolveKey);
	SYSEXPORT(KeyExists);
	SYSEXPORT(NewKey);
	SYSEXPORT(DeleteKey);
	SYSEXPORT(DeleteKeyEx);
	SYSEXPORT(KeySetText);
	SYSEXPORT(KeyGetText);
	SYSEXPORT(KeySetLong);
	SYSEXPORT(KeyGetLong);
	SYSEXPORT(KeySetChar);
	SYSEXPORT(KeyGetChar);
	SYSEXPORT(KeySetBool);
	SYSEXPORT(KeyGetBool);
	SYSEXPORT(KeySetShort);
	SYSEXPORT(KeyGetShort);
	SYSEXPORT(KeySetInt);
	SYSEXPORT(KeyGetInt);
	SYSEXPORT(KeySetBig);
	SYSEXPORT(KeyGetBig);
	SYSEXPORT(KeySetNothing);
	SYSEXPORT(GetParentKeyName);
	SYSEXPORT(RenameKey);
	SYSEXPORT(CreateKey);
	SYSEXPORT(GetOrCreateKey);
	SYSEXPORT(GetColorFromHex);

	DebugMessage("Registry loaded");
}

void NRegistryUnInit ( void ) {
	DebugMessage("Saving registry...");
	SaveRegistry();
	DebugMessage("Registry saved");
	FreeKey(MainKey);
	DebugMessage("Registry freed");
}


