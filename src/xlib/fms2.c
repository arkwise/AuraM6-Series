////////////////////////////////////////////////////////////////////////////////
//
//	File Management System v 2 - Core file
//
//	(c) Copyright 2004 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "iodlg.h"
#include "fms2.h"
#include "window.h"

l_ulong AppVersion = ULONG_ID(0,0,0,1);
char    AppName[] = "File Management System v2";
l_uid	nUID = "fms2";
l_uid NeededLibs[] = { "menu","window", "" };

/** @defgroup fms File Management System (FMS)
	Manages file actions and informations. Provides generic icons for data.
	@ingroup dmsfms
	@{
*/
/**	@file
	FMS core file (uid:'fms2')
*/

////////////////////////////////////////////////////////////////////////////////
PList DTEs = NULL, GAction = NULL;
l_ulong NextActionMsg = 0xFFFE0000;
PDataTypeEx BinaryExtender;
////////////////////////////////////////////////////////////////////////////////
PFileInfo LinkGetLinkedFile ( PFileInfo f ) {
	l_text filename = LoadData2(f->VPathName,TYPE_TEXT);
	PFileInfo o = NULL;
	if ( filename ) {
		o = FileGetInfo(filename);
		free(filename);
	}
	return o;
}
////////////////////////////////////////////////////////////////////////////////
PCodec __GetCodecFileInfo ( PFileInfo f, PFileInfo *lnk ) {
	PCodec c = GetCodecFileInfo(f);
	if ( lnk ) *lnk = 0;
	if ( !c ) return NULL;
	if ( c->FavoriteType == DTYPE_FILEREF ) {
		PFileInfo lf = LinkGetLinkedFile(f);
		if ( lf ) {
			*lnk = lf;
			return GetCodecFileInfo(lf);
		} else
			DebugWarning("FMS2 : Invalid link");
	}
	return c;
}
////////////////////////////////////////////////////////////////////////////////
void FreeDataTypeEx ( PDataTypeEx o ) {
	if ( o->TypeName ) free(o->TypeName);
	if ( o->Actions ) FreeList(o->Actions);
	if ( o->Default ) FreeIcon(o->Default);

	free(o);
}
////////////////////////////////////////////////////////////////////////////////
void FreeDTAction ( PDTAction o ) {
	if ( o->Name ) free(o->Name);
	if ( o->Runner ) free(o->Runner);
	free(o);
}
////////////////////////////////////////////////////////////////////////////////
PDataTypeEx _GetDataTypeEx( l_ulong TypeId ) {
	if ( DTEs->Last ) {
		PListItem a = DTEs->Last->Next, b = a;
		do {
			if ( DATATYPEEX(a->Data)->TypeId == TypeId ) return DATATYPEEX(a->Data);
			a = a->Next;
		} while ( a != b );
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PDataTypeEx AddDataTypeEx( l_ulong TypeId,l_text TypeName,p_bitmap Icon16,p_bitmap Icon32,p_bitmap Icon48,PIcon (*GetIcons) ( PFileInfo ), l_ulong Flags ) {
	PDataTypeEx o;
	
	DebugMessage("FMS2 : DTE %08x '%s'",TypeId,TypeName);

	if ( o = _GetDataTypeEx(TypeId) ) {
		if ( o->TypeName ) free(o->TypeName);
		if ( o->Default ) FreeIcon(o->Default);
		if ( Icon16 || Icon32 || Icon48 ) {
			if ( Flags & DTEF_FREEICONS ) 
				o->Default = NewIcon(Icon16,Icon32,Icon48);
			else
				o->Default = NewIcon2(Icon16,Icon32,Icon48);
		} else
			o->Default = NULL;
		o->TypeName = TextDup(TypeName);
		DebugWarning("FMS2 : Data Type Extender %08x yet declared : Overwrite icons and name",TypeId);
		return o;
	}
	o = malloc(sizeof(TDataTypeEx));
	if ( !o ) return NULL;
	memset(o,0,sizeof(TDataTypeEx));
	o->TypeId = TypeId;
	o->TypeName = TextDup(TypeName);
	
	if ( Icon16 || Icon32 || Icon48 ) {
		if ( Flags & DTEF_FREEICONS ) 
			o->Default = NewIcon(Icon16,Icon32,Icon48);
		else
			o->Default = NewIcon2(Icon16,Icon32,Icon48);
	}
	o->GetIcons = GetIcons;
	o->Flags = Flags;
	o->Actions = NewList();
	ListAdd(DTEs,NULL,o,(void*)&FreeDataTypeEx);
	return o;	
}
////////////////////////////////////////////////////////////////////////////////
PDTAction AddDTAction ( PDataTypeEx dte, l_text Name,l_text Runner, l_bool(*Run)(PFileInfo,int,l_text*,PApplication,PErrorHandler),l_ulong FavoriteWeight ) {
	PDTAction o = malloc(sizeof(TDTAction));
	if ( !o ) return NULL;
	memset(o,0,sizeof(TDTAction));
	
	if ( Run )
		DebugMessage("FMS2 :     %08x : New action '%s' (%d) -> Internal %08x",dte->TypeId,Name,FavoriteWeight,Run);
	else
		DebugMessage("FMS2 :     %08x : New action '%s' (%d) -> '%s'",dte->TypeId,Name,FavoriteWeight,Runner);
	
	o->Name = TextDup(Name);
	o->Runner = TextDup(Runner);
	o->Run = Run;
	o->FavoriteWeight = FavoriteWeight;
	o->ActionMsg = NextActionMsg++;
	
	ListAdd(dte->Actions,NULL,o,(void*)&FreeDTAction);
	ListAdd(GAction,NULL,o,NULL);
	return o;
}

////////////////////////////////////////////////////////////////////////////////
void _CollectTypeActions ( l_ulong TypeId, PList Lst ) {
	PDataTypeEx dte = _GetDataTypeEx(TypeId);
	if ( !dte && !Lst->Last ) dte = BinaryExtender;
	if ( dte )
	if ( dte->Actions->Last ) {
		PListItem a = dte->Actions->Last->Next, b = a;
		do {
			ListAdd(Lst,NULL,a->Data,NULL);
			a = a->Next;
		} while ( a != b ); 
	}
}
////////////////////////////////////////////////////////////////////////////////
l_long DTActionFavComparator ( void *a, void *b ) {
	return DTACTION(b)->FavoriteWeight-DTACTION(a)->FavoriteWeight;
}
////////////////////////////////////////////////////////////////////////////////
PList CollectFileActions ( PFileInfo f ) {
	PFileInfo lnk = NULL;
	PCodec c = __GetCodecFileInfo(f,&lnk);
	PList Lst = NewList();
	if ( lnk ) FreeFileInfo(lnk);
	if ( !Lst ) return NULL;
	if ( c ) {
		if ( c->SubCodecs->Last ) {
			PListItem a = c->SubCodecs->Last->Next, b = a;
			do {
				_CollectTypeActions(SUBCODEC(a->Data)->TypeId,Lst);
				a = a->Next;
			} while ( a != b );
		} else
			_CollectTypeActions(BinaryExtender->TypeId,Lst);
	} else
		_CollectTypeActions(BinaryExtender->TypeId,Lst);
	ListOrderByData(Lst,&DTActionFavComparator);
	return Lst;
}
////////////////////////////////////////////////////////////////////////////////
PList CollectTypeActions ( l_ulong TypeId ) {
	PList Lst = NewList();
	if ( !Lst ) return NULL; 
	_CollectTypeActions(TypeId,Lst);
	ListOrderByData(Lst,&DTActionFavComparator);
	return Lst;
}
////////////////////////////////////////////////////////////////////////////////
PDTAction GetActionByMsg ( l_ulong Msg ) {
	if ( GAction->Last ) {
		PListItem a = GAction->Last->Next, b = a;
		do {
			if ( DTACTION(a->Data)->ActionMsg == Msg ) return DTACTION(a->Data);
			a = a->Next;
		} while ( a != b );
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
l_bool ExecutePDAction2 ( PDTAction a, PFileInfo f, int argc, l_text *argv, PApplication Host, PErrorHandler h ) {
	PFileInfo lnk;
	l_bool r;
	__GetCodecFileInfo(f,&lnk);
	if ( lnk ) f = lnk;
	if ( a->Run )
		r = a->Run(f,argc,argv,Host,h);
	else {
		int nargc = argc+2, i = 0;
		l_text *nargv = malloc((nargc+1)*sizeof(l_text));
		nargv[0] = TextDup(a->Runner);
		nargv[1] = TextDup(f->VPathName);
		while ( i < argc ) {
			nargv[i+2] = TextDup(argv[i]);	
			i++;
		}
		nargv[nargc] = 0;
		r = DynLdRunEx(a->Runner,nargc,nargv,Host,h)?true:false;
		FreeArgs(nargc,nargv);
	}
	if ( lnk ) FreeFileInfo(lnk);		
	return r;
}
////////////////////////////////////////////////////////////////////////////////
l_bool ExecutePDAction ( PDTAction a, PFileInfo f, int argc, l_text *argv ) {
	l_bool result;
	PErrorHandler h = CreateErrorHandler();
	result = ExecutePDAction2(a,f,argc,argv,NULL,h);
	DebugMessage("Could i report ?");
	if ( EHandlerIsError(h) ) WindowNotifyError(&Me,h);
	FreeErrorHandler(h);
	return result;
}
////////////////////////////////////////////////////////////////////////////////
PDTAction GetFileDefaultAction ( PFileInfo f ) {
	PList Lst = CollectFileActions(f);
	if ( Lst )
		if ( Lst->Last ) return	DTACTION(Lst->Last->Next->Data);
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
l_ulong GetTypeDefaultMsg ( l_ulong TypeId ) {
	PList Lst = CollectTypeActions(TypeId);
	if ( Lst )
		if ( Lst->Last ) return	DTACTION(Lst->Last->Next->Data)->ActionMsg;
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
l_bool RunFile3 ( PFileInfo f, int argc, l_text *argv, PApplication Host, PErrorHandler h ) {
	PDTAction a = GetFileDefaultAction(f);
	if ( a ) return ExecutePDAction2(a,f,argc,argv,Host,h);
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool RunFile2 ( PFileInfo f, int argc, l_text *argv ) {
	PDTAction a = GetFileDefaultAction(f);
	if ( a ) return ExecutePDAction(a,f,argc,argv);
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool FileRun ( l_text file,l_text Args ) {

	PFileInfo f = FileGetInfo(file);
	if ( f ) {
		l_text argv[2] = { 0, 0 };
		l_bool r;
		if ( Args ) argv[0] = TextDup(Args);
		r = RunFile2(f,Args?1:0,argv);
		if ( Args ) free(argv[0]);
		FreeFileInfo(f);
		return r;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool RunMessage (  l_ulong Msg, PFileInfo f, int argc, l_text *argv ) {
	PDTAction a = GetActionByMsg(Msg);
	if ( a ) return ExecutePDAction(a,f,argc,argv);
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool ExecutableRun ( PFileInfo f, int argc, l_text *argv, PApplication Host, PErrorHandler h ) {
	l_bool r;
	int nargc = argc+1;
	l_text *nargv = malloc((nargc+1)*sizeof(l_text));
	nargv[0] = TextDup(f->VPathName);
	while ( argc ) {
		nargv[argc] = TextDup(argv[argc-1]);	
		argc--;
	}
	nargv[nargc] = 0;
	r = DynLdRunEx(f->VPathName,nargc,nargv,Host,h) ? true : false;
	FreeArgs(nargc,nargv);
	return r;
}
////////////////////////////////////////////////////////////////////////////////
PIcon FileGetIconsEx ( PFileInfo f ) {
	PFileInfo lnk = NULL;
	PIcon i = NULL;
	PCodec c = __GetCodecFileInfo(f,&lnk);
	PDataTypeEx dte = NULL;
	if ( lnk ) f = lnk;

	if ( c ) {
		if ( c->GetIcons )
			i = c->GetIcons(f);

		if ( !i ) i = DuplicateIcon(c->Default);
	
		dte = _GetDataTypeEx(c->FavoriteType);
	}
	if ( !dte ) dte = BinaryExtender;
	
	if ( !i )
		if ( dte->GetIcons ) 
			i = dte->GetIcons(f);
		
	if ( !i ) i = DuplicateIcon(dte->Default);
	
	if ( !i ) i = DuplicateIcon(BinaryExtender->Default);
	
	if ( lnk ) FreeFileInfo(lnk);
	return i;
}
////////////////////////////////////////////////////////////////////////////////
PImage RleSpriteToImage (  PRleSprite ico ) {
	PImage img = create_bitmap(ico->w,ico->h);
	clear_to_color(img, img->vtable->mask_color);
	draw_rle_sprite(img,ico,0,0);
	return img;
}
////////////////////////////////////////////////////////////////////////////////
void FileGetIcons2 ( PFileInfo f, p_bitmap *i16,p_bitmap *i32,p_bitmap *i48,l_ulong *fi ) {
	PIcon i = FileGetIconsEx(f);
	if ( !i ) return;
	if ( !i->Instances ) {
		DebugError("FMS2 : MAY CRASH : FREEED ICON RETURNED !");
	} else {
		if ( fi ) *fi = 1;
		if ( i16 ) *i16 = RleSpriteToImage(i->Icon16);
		if ( i32 ) *i32 = RleSpriteToImage(i->Icon32);
		if ( i48 ) *i48 = RleSpriteToImage(i->Icon48);
	}
	FreeIcon(i);
}
////////////////////////////////////////////////////////////////////////////////
p_bitmap FileGetIcon2 ( PFileInfo f, l_ulong Size, l_ulong *ToFree ) {
	PIcon i = FileGetIconsEx(f);
	
	if ( !i ) return NULL;
	if ( !i->Instances ) {
		DebugError("FMS2 : MAY CRASH : FREEED ICON RETURNED !");
	} else {
		p_bitmap bmp = NULL;
		if ( ToFree ) *ToFree = 1;
		if ( Size == 16 ) bmp = RleSpriteToImage(i->Icon16);
		if ( Size == 32 ) bmp = RleSpriteToImage(i->Icon32);
		if ( Size == 48 ) bmp = RleSpriteToImage(i->Icon48);
		return bmp;
	}
	FreeIcon(i);
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PMenu FileGetMenu2 ( PFileInfo f, l_ulong Flags ) {
	PMenu m = NewMenu(NULL);
	PList Lst = CollectFileActions(f);
	PListItem a, b;
	if ( Lst ) {
		if ( Lst->Last ) {
			a = b = Lst->Last->Next;
			do {
				AddMenuItem (m,DTACTION(a->Data)->Name,0,DTACTION(a->Data)->ActionMsg,0,0);
				a = a->Next;
			} while ( a != b);
		}
		FreeList(Lst);
	}
	AddMenuItemSeparator(m);
	if ( (f->Attributes & FA_DIREC) && Flags ) 
		AddMenuItem(m,"New directory",NULL,MSG_IODLG_NEWDIR,0,NULL);
	if ( !Flags ) 
		AddMenuItem(m,"Rename",0,MSG_IOMENU_RENAME,0,0);
	AddMenuItem (m,"Delete",0,MSG_IOMENU_DELETE,0,0);
	AddMenuItemSeparator(m);
	
	AddMenuItem (m,"Copy",0,MSG_IOMENU_COPY,0,0);
	AddMenuItem (m,"Cut",0,MSG_IOMENU_CUT,0,0);
	if ( f->Attributes & FA_DIREC ) 
		AddMenuItem (m,"Paste",0,MSG_IOMENU_PASTE,0,0);
	AddMenuItemSeparator(m);
	AddMenuItem (m,"Propreties",0,MSG_IOMENU_PROPERTIES,0,0);	
	return m;
}
////////////////////////////////////////////////////////////////////////////////
l_bool FileHandleMsg2 ( PFileInfo f, l_ulong Msg, PErrorHandler h ) {
	PDTAction a;
	if ( Msg == MSG_IOMENU_PROPERTIES ) return DynLdRun("xlib/fprop.dl",f->VPathName)?true:false;
	a = GetActionByMsg(Msg);
	if ( !a ) return false;
	return ExecutePDAction2(a,f,0,NULL,NULL,h);
}
////////////////////////////////////////////////////////////////////////////////
l_bool FileHandleMsg ( PFileInfo f, l_ulong Msg ) {
	
	PDTAction a;
	if ( Msg == MSG_IOMENU_PROPERTIES ) return DynLdRun("xlib/fprop.dl",f->VPathName)?true:false;
	a = GetActionByMsg(Msg);
	if ( !a ) return false;
	return ExecutePDAction(a,f,0,NULL);
	
}
////////////////////////////////////////////////////////////////////////////////
// Registry setup
////////////////////////////////////////////////////////////////////////////////
void InstallActionFromReg( PDataTypeEx dte, PRegKey o ) {
	PDTAction act = AddDTAction(dte,o->Name,(l_text)o->Data,NULL,NULL);
	if ( o->Last ) {
		PRegKey a = o->Last->Next, b = a;
		do {
			if ( !TextCaseCompare(a->Name,"FavoriteWeight") && (a->Type == RKT_LONG) )
				act->FavoriteWeight = *((l_ulong*)a->Data);
			a = a->Next;
		} while ( a != b );
	}	
}
////////////////////////////////////////////////////////////////////////////////
void InstallActionsFromReg( PDataTypeEx dte, PRegKey o ) {
	
	if ( o->Last ) {
		PRegKey a = o->Last->Next, b = a;
		do {
			if ( a->Type == RKT_TEXT ) {
				InstallActionFromReg(dte,a);
			}
			a = a->Next;
		} while ( a != b );
	}	
}
////////////////////////////////////////////////////////////////////////////////
void InstallDTEFromReg ( PRegKey o ) {
	
	if ( o->Last && ( o->Type == RKT_TEXT ) ) {
		l_ulong TypeId = strtol(o->Name,NULL,16);
		PDataTypeEx dte;
		PImage Img16 = NULL,Img32 = NULL,Img48 = NULL;
		PRegKey a = o->Last->Next, b = a;
		
		if ( !(dte = _GetDataTypeEx(TypeId)) )
			dte = AddDataTypeEx(TypeId,(l_text)o->Data,NULL,NULL,NULL,NULL,DTEF_FREEICONS);
	
		do {
			if ( !TextCaseCompare(a->Name,"Icon16" ) && ( a->Type == RKT_TEXT ) )
				Img16 = LoadData2((l_text)a->Data,DTYPE_IMAGE);
			else if ( !TextCaseCompare(a->Name,"Icon32" ) && ( a->Type == RKT_TEXT ) )
				Img32 = LoadData2((l_text)a->Data,DTYPE_IMAGE);
			else if ( !TextCaseCompare(a->Name,"Icon48" ) && ( a->Type == RKT_TEXT ) )
				Img48 = LoadData2((l_text)a->Data,DTYPE_IMAGE);
			else if ( !TextCaseCompare(a->Name,"Actions" ) )
				InstallActionsFromReg(dte,a);
			a = a->Next;
		} while ( a != b );
		
		if ( Img48 || Img32 || Img16 ) {
			if ( dte->Default ) FreeIcon(dte->Default);
			dte->Default = NewIcon(Img16,Img32,Img48);
		}
	}
} 
////////////////////////////////////////////////////////////////////////////////
l_bool LibMain ( int argc, l_text *argv )
{
	PDataTypeEx t;
	PRegKey o;
	
	DTEs = NewList();
	
	if ( !DTEs ) {
		DebugError("FMS2 : Unable to initialise");
		return false;
	}

	GAction = NewList();
	
	if ( !GAction ) {
		FreeList(DTEs);
		DebugError("FMS2 : Unable to initialise");
		return false;
	}

	DMS2InstallIcons();
	SYSEXPORT(FileHandleMsg2);
	SYSEXPORT(FileHandleMsg);
	SYSEXPORT(FileGetMenu2);
	SYSEXPORT(FileGetIcon2);
	SYSEXPORT(FileGetIcons2);
	SYSEXPORT(RunFile2);
	SYSEXPORT(RunFile3);

	SYSEXPORT(AddDTAction);
	SYSEXPORT(AddDataTypeEx);
	SYSEXPORT(_GetDataTypeEx);
	SYSEXPORT(GetTypeDefaultMsg);
	SYSEXPORT(DTEs);
	SYSEXPORT(ExecutePDAction);
	SYSEXPORT(ExecutePDAction2);
	SYSEXPORT(FileGetIconsEx);
	SYSEXPORT(FileRun);

	BinaryExtender = AddDataTypeEx(DTYPE_BINDATA,"Unknown binary",ListKey(Me.Ressource,"FILE16"),ListKey(Me.Ressource,"FILE32"),ListKey(Me.Ressource,"FILE48"),NULL,0);
	AddDTAction(BinaryExtender,"Open with...","Applications/openwth.app",NULL,0);
	
	
	t = AddDataTypeEx(DTYPE_DYNLD,"DynLd executable",ListKey(Me.Ressource,"APP16"),ListKey(Me.Ressource,"APP32"),ListKey(Me.Ressource,"APP48"),&GetDynLdIcon,0);
	AddDTAction(t,"Execute",NULL,&ExecutableRun,100);
	o = ResolveKey("/SYSTEM/FMS");
	
	
	if ( o ) {
		PRegKey a, b;
		if ( o->Last ) {
			a = b = o->Last->Next;
			do {
				if ( a->Type == RKT_TEXT ) InstallDTEFromReg(a);
				a = a->Next;
			} while ( a != b );
		}
	}
	
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{
	if ( DTEs ) FreeList(DTEs);
	if ( GAction ) FreeList(GAction);
}
/**@}*/
