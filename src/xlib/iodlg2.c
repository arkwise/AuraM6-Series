////////////////////d////////////////////////////////////////////////////////////
//
//  I/O Dialogs - Core File
//
//  (c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "iodlg.h"
#include "label.h"
#include "window.h"
#include "button.h"
#include "tbox.h"
#include "combobox.h"
#include "fms2.h"
#include "clipbrd.h"
#include "listbox.h"
#include "checkbox.h"
#include "string.h"

////////////////////////////////////////////////////////////////////////////////
l_ulong AppVersion = ULONG_ID(0,0,0,1);
char    AppName[] = "I/O Dialogs";
l_uid   nUID = "iodlg";
l_uid NeededLibs[] = { "widget","listview","fms2","textbox","combobox","button","window","clipbrd","listbox","label","checkbox","" };

PIcon ParentDirIcon = NULL;
l_ulong DirOpenMSG;
l_text GlobalDir = NULL;
PImage IconOpen;
PImage IconSave;
PImage IconAdd;
PImage IconUp;
PImage IconRemove;
PImage IconCancel;

PWidgetClass TreeDirviewClass;
PWidgetClass DirviewClass;


////////////////////////////////////////////////////////////////////////////////
// File types
////////////////////////////////////////////////////////////////////////////////
void FreeFileTypes ( PFileTypes o ) {
        if ( o->Title ) free(o->Title);
        if ( o->Extentions ) FreeList(o->Extentions);
        if ( o->Next ) FreeFileTypes(o->Next);
        free(o);
}
////////////////////////////////////////////////////////////////////////////////
PFileTypes NewFileTypes ( l_text Title, l_text txtexts, PFileTypes Next ) {

        PFileTypes o = malloc(sizeof(TFileTypes));
        if ( !o ) return NULL;
        memset(o,0,sizeof(TFileTypes));

        o->Title = TextDup(Title);

        if ( txtexts ) {
                l_text s,e = txtexts;
                o->Extentions = NewList();
                do {
                        s = TextChr(e,',');
                        if ( s ) {
                                l_text n = TextNDup(e,s-e);
                                ListAdd(o->Extentions,n,NULL,NULL);
                                free(n);
                                e = s+1;
                        } else
                          ListAdd(o->Extentions,e,NULL,NULL);
                } while ( s );
        }
        o->Next = Next;
        return o;
}
////////////////////////////////////////////////////////////////////////////////
PFileTypes FileTypesAt ( PFileTypes o, l_ulong index ) {
        if ( !o ) return NULL;
        if ( !index ) return o;
        return FileTypesAt(o->Next,index-1);
}
////////////////////////////////////////////////////////////////////////////////
// Standart IODlg Actions
////////////////////////////////////////////////////////////////////////////////
l_bool IODlgDropAccept ( PFileInfo DestItemInfo, l_ulong Type, void *Data, l_ulong SupportedMethods, l_ulong *Method ) {
        DebugMessage ("Drag n Drop: IODlgDropAccept: %s", DestItemInfo->Name );

        if ( !(DestItemInfo->Attributes & FA_DIREC) ) return false;

        if ( Type == DTYPE_FILEREFLIST ) {
                l_bool MoveThem = true;
                PListItem a,b;
                b = a = ((PList)Data)->Last;
                if ( a ) do {
                        if ( FILEREF(a->Data)->Drive != DestItemInfo->Drive )
                                MoveThem = false;
                        a = a->Next;
                } while ( (a != b) && MoveThem);
                if ( MoveThem && (SupportedMethods & DMETHOD_MOVE) )
                        *Method = DMETHOD_MOVE;
                else
                        *Method = DMETHOD_COPY;
                return true;
        } else {
                PSubCodec sc = GetDefaultSubCodec(Type);
                if ( sc )
                        if ( sc->Save ||  sc->SaveToFile )
                                if ( SupportedMethods & DMETHOD_COPY ) {
                                        *Method = DMETHOD_COPY;
                                        return true;
                                }
        }
        return false;
}

////////////////////////////////////////////////////////////////////////////////
void IODlgDropDataToFileInfo ( PFileInfo DestItemInfo, void *Data, l_ulong Type, l_ulong Method ) {

        if ( Type == DTYPE_FILEREFLIST ) {
                PListItem a, b;
                PList l = Data;

                if ( Method == DMETHOD_MOVE ) { // Section to improve !
                        PFileInfo dest = DestItemInfo;
                        b = a = l->Last;
                        if ( a ) do {
                                l_text nname = FileNameToPath(dest->LocalName,FILEREF(a->Data)->SName);

                                if ( FILEREF(a->Data)->Attributes & FA_DIREC ) {
                                        if ( dest->Drive->Driver->DirRename )
                                                dest->Drive->Driver->DirRename(dest->Drive,FILEREF(a->Data)->LocalName,nname);
                                } else {
                                        if ( dest->Drive->Driver->FileRename )
                                                dest->Drive->Driver->FileRename(dest->Drive,FILEREF(a->Data)->LocalName,nname);
                                }
                                free(nname);
                                a = a->Next;
                        } while ( a != b );
                } else if ( Method == DMETHOD_COPY )    {
                        PList nl = CollectFilesFromRef(l);
                        if ( nl ) {   // was l....now right? Florian Xaver
                          CopyFilesFromList ( nl, DestItemInfo->VPathName );
                          FreeList(nl);
                        }
                }

        } else {
                PSubCodec sc = GetDefaultSubCodec(Type);
                if ( sc ) {
                        if ( sc->Save ||  sc->SaveToFile ) {
                                PFileInfo f;
                                l_text FileExtention = "dat";
                                l_text FileName;
                                l_text Name;
                                l_ulong nb=0;
                                if ( sc->CodecId ) {
                                        PCodec c = GetCodec(sc->CodecId);
                                        if ( c )
                                                if( c->DefaultExtention )
                                                        FileExtention = c->DefaultExtention;
                                } else { // May be a generic sub codec
                                        PCodec c = GetCodec(Type); // Most of time, generic codecs has as codec id the type id
                                        if ( c )
                                                if ( c->FavoriteType == Type ) // Check if codec is _realy_ type favorite
                                                        if( c->DefaultExtention )
                                                                FileExtention = c->DefaultExtention;
                                }
                                Name = TextArgs("data.%s",FileExtention);
                                FileName = FileNameToPath(DestItemInfo->VPathName,Name);
                                while ( f = FileGetInfo(FileName) ) {
                                        FreeFileInfo(f);
                                        nb++;
                                        free(FileName);
                                        free(Name);
                                        Name = TextArgs("data(%d).%s",nb,FileExtention);
                                        FileName = FileNameToPath(DestItemInfo->VPathName,Name);
                                }
                                SaveData2(FileName,Data,Type);
                                //f = FileGetInfo(FileName);
                                free(FileName);
                                free(Name);
                                //FreeFileInfo(f);
                        }
                }
        }
}
////////////////////////////////////////////////////////////////////////////////
// Dirview
////////////////////////////////////////////////////////////////////////////////
void DirviewItemFree(PDirviewItem o) {
        FreeFileInfo(o->f);
        FreeListviewItem(LISTVIEWITEM(o));

}
////////////////////////////////////////////////////////////////////////////////
PDirviewItem DirviewNewItem ( PDirview o )
{
        PDirviewItem p = malloc(sizeof(TDirviewItem));
        if (!p) return;
        memset(p, 0, sizeof(TDirviewItem));
        ListAdd (LISTVIEW(o)->Items, NULL, p, (void*)&DirviewItemFree);
        return p;
}
////////////////////////////////////////////////////////////////////////////////
l_text format_size (l_ulong size)
{
        if (size < (l_ulong)1024)
                return TextArgs ("%d bytes", (l_ulong)size);
        else if (size < (l_ulong)1024*1024)
                return TextArgs ("%d KB", size / (1024));
        else if (size < (l_ulong)1024*1024*1024)
                return TextArgs ("%d MB", size / (1024*1024));
        else
                return TextArgs ("%d GB", size / (1024*1024*1024));
}

////////////////////////////////////////////////////////////////////////////////
PDirviewItem _DirviewAddItem ( PDirview o, PFileInfo f )
{
l_text l;
        PIcon i;
        struct tm *tme;
        PDirviewItem p = malloc(sizeof(TDirviewItem));
        if ( !p )
                return NULL;
        memset(p, 0, sizeof(TDirviewItem));
        i = FileGetIconsEx(f);
        ListviewInitItem(LISTVIEWITEM(p),f->Name,i);
        if ( i )
                FreeIcon(i);
        p->f = f;


        l = format_size(f->Size);
        ListviewItemPushCol(LISTVIEWITEM(p),l);
        free(l);
        tme = localtime((const time_t *)&f->CTime);
if (f->CTime) l = TextArgs("%02d/%02d/%04d %02d:%02d:%02d",tme->tm_mday,tme->tm_mon+1,tme->tm_year+1900,tme->tm_hour,tme->tm_min,tme->tm_sec);
else l = TextArgs("-");
ListviewItemPushCol(LISTVIEWITEM(p),l);
free(l);
        tme = localtime((const time_t *)&f->MTime);
if (f->MTime) l = TextArgs("%02d/%02d/%04d %02d:%02d:%02d",tme->tm_mday,tme->tm_mon+1,tme->tm_year+1900,tme->tm_hour,tme->tm_min,tme->tm_sec);
else l = TextArgs("-");
ListviewItemPushCol(LISTVIEWITEM(p),l);
free(l);
        tme = localtime((const time_t *)&f->ATime);
if (f->ATime) l = TextArgs("%02d/%02d/%04d %02d:%02d:%02d",tme->tm_mday,tme->tm_mon+1,tme->tm_year+1900,tme->tm_hour,tme->tm_min,tme->tm_sec);
else l = TextArgs("-");
ListviewItemPushCol(LISTVIEWITEM(p),l);
free(l);
/*l = TextArgs("%d", f->Size);
ListviewItemPushCol(LISTVIEWITEM(p),l);  // for attributes, maybe in future
free(l);*/

        ListAdd (LISTVIEW(o)->Items, f->Name, p, (void*)&DirviewItemFree);
// ListviewItemSetColCaption(p,1,l);

        return p;
}
////////////////////////////////////////////////////////////////////////////////
l_int DirviewItemOrderyByName ( PDirviewItem a, PDirviewItem b ) {

// improved by Florian Xaver

        if ( (a->f->Attributes & FA_DIREC) && (b->f->Attributes & FA_DIREC) )
                return TextCaseCompare(a->f->Name,b->f->Name);
        else if ( (!(a->f->Attributes & FA_DIREC) && (b->f->Attributes & FA_DIREC)) )
                return 1;
        else if ( !(a->f->Attributes & FA_DIREC) && !(b->f->Attributes & FA_DIREC) )
                return TextCaseCompare(a->f->Name,b->f->Name);
        else
                return -1;

}
////////////////////////////////////////////////////////////////////////////////
l_int DirviewItemOrderyBySize ( PDirviewItem a, PDirviewItem b ) {

// new by Florian Xaver

        if ( (a->f->Attributes & FA_DIREC) && (b->f->Attributes & FA_DIREC) )
                return TextCaseCompare(a->f->Name,b->f->Name);
        else if ( (!(a->f->Attributes & FA_DIREC) && (b->f->Attributes & FA_DIREC)) )
                return 1;
        else if ( !(a->f->Attributes & FA_DIREC) && !(b->f->Attributes & FA_DIREC) )
                return ((a->f->Size >= b->f->Size) ? 1 : -1);
        else
                return -1;

}

////////////////////////////////////////////////////////////////////////////////
l_int DirviewItemOrderyByCTime ( PDirviewItem a, PDirviewItem b ) {

// new by Florian Xaver

        if ( (a->f->Attributes & FA_DIREC) && (b->f->Attributes & FA_DIREC) )
                return (a->f->CTime >= b->f->CTime) ? 1 : -1;
        else if ( (!(a->f->Attributes & FA_DIREC) && (b->f->Attributes & FA_DIREC)) )
                return 1;
        else if ( !(a->f->Attributes & FA_DIREC) && !(b->f->Attributes & FA_DIREC) )
                return (a->f->CTime >= b->f->CTime) ? 1 : -1;
        else
                return -1;

}

////////////////////////////////////////////////////////////////////////////////
l_int DirviewItemOrderyByATime ( PDirviewItem a, PDirviewItem b ) {

// new by Florian Xaver

        if ( (a->f->Attributes & FA_DIREC) && (b->f->Attributes & FA_DIREC) )
                return (a->f->ATime >= b->f->ATime) ? 1 : -1;
        else if ( (!(a->f->Attributes & FA_DIREC) && (b->f->Attributes & FA_DIREC)) )
                return 1;
        else if ( !(a->f->Attributes & FA_DIREC) && !(b->f->Attributes & FA_DIREC) )
                return (a->f->ATime >= b->f->ATime) ? 1 : -1;
        else
                return -1;

}

////////////////////////////////////////////////////////////////////////////////
l_int DirviewItemOrderyByMTime ( PDirviewItem a, PDirviewItem b ) {

// new by Florian Xaver

        if ( (a->f->Attributes & FA_DIREC) && (b->f->Attributes & FA_DIREC) )
                return (a->f->MTime >= b->f->MTime) ? 1 : -1;
        else if ( (!(a->f->Attributes & FA_DIREC) && (b->f->Attributes & FA_DIREC)) )
                return 1;
        else if ( !(a->f->Attributes & FA_DIREC) && !(b->f->Attributes & FA_DIREC) )
                return (a->f->MTime >= b->f->MTime) ? 1 : -1;
        else
                return -1;

}

////////////////////////////////////////////////////////////////////////////////
PDirviewItem DirviewAddItem ( PDirview o, PFileInfo f )
{
        PDirviewItem p = _DirviewAddItem(o,f);
 if (o->Flags & DVF_ORDERBYNAME)
          ListOrderByData(LISTVIEW(o)->Items,(void*)&DirviewItemOrderyByName);
 else if (o->Flags & DVF_ORDERBYSIZE)
          ListOrderByData(LISTVIEW(o)->Items,(void*)&DirviewItemOrderyBySize);
 else if (o->Flags & DVF_ORDERBYCTIME)
          ListOrderByData(LISTVIEW(o)->Items,(void*)&DirviewItemOrderyByCTime);
 else if (o->Flags & DVF_ORDERBYMTIME)
          ListOrderByData(LISTVIEW(o)->Items,(void*)&DirviewItemOrderyByMTime);
 else if (o->Flags & DVF_ORDERBYATIME)
          ListOrderByData(LISTVIEW(o)->Items,(void*)&DirviewItemOrderyByATime);
 else
          ListOrderByData(LISTVIEW(o)->Items,(void*)&DirviewItemOrderyByName);

        return p;
}
////////////////////////////////////////////////////////////////////////////////
void DirviewGenerateSubs ( PDirview o ) {
  p_bitmap i16;
  p_bitmap i32;
  l_ulong  fi;

        PListItem a, b;
        PList l;

        if ( o->Dir ) FreeFileInfo(o->Dir);
        o->Dir = FileGetInfo(DIRVIEW(o)->Directory);

        ListviewRemoveAllItems(LISTVIEW(o));

        if ( !(o->Flags & DVF_NOPARICON) )
                if ( o->Directory[1] ) {
                        PFileInfo pf = FileGetInfo(GetPathParent(o->Directory));
                        PDirviewItem pi = DirviewNewItem(o);
                        ListviewInitItem(LISTVIEWITEM(pi),"..",ParentDirIcon);
                        pi->f = pf;
                }

        l = ListDir(o->Directory,FA_DIREC|FA_LABEL);
        if ( l ) {

        if ( l->Last )
        {
                a = b = l->Last->Next;

                do
                {
                        if ( FILEINFO(a->Data)->Attributes & FA_DIREC )
                                DirviewAddItem(o,FILEINFO(a->Data));
                        a = a->Next;
                }
                while ( a != b );


                do
                {
                        if ( !(FILEINFO(a->Data)->Attributes & FA_DIREC ))
                        if (    o->Extentions ) {
                                l_text ext = TextRChr(FILEINFO(a->Data)->Name,'.');
                                if ( ext )
                                        if ( ListKeyCaseItem(o->Extentions,ext+1) ) DirviewAddItem(o,FILEINFO(a->Data));
                        } else
                                DirviewAddItem(o,FILEINFO(a->Data));
                        a = a->Next;
                }
                while ( a != b );
        }
        KickList(l);
}

}
////////////////////////////////////////////////////////////////////////////////
void DirviewOpenDirectory ( PDirview o, l_text dir ) {
        if ( o->Directory ) free(o->Directory);
        o->Directory = TextDup(dir);
	
 if (o->Flags & DVF_DESKTOPICON)
	{
	FileRun("applications/nav.app", TextDup(dir));
	}
	else 
	{ 
		DirviewGenerateSubs(o);
		ListviewUpdateAndReDraw(LISTVIEW(o));
	}

}
////////////////////////////////////////////////////////////////////////////////
void DirviewOpenParentDirectory ( PDirview o ) {
        l_text dir = GetPathParent(o->Directory);
        DirviewOpenDirectory(o,dir);
        free(dir);
}
////////////////////////////////////////////////////////////////////////////////
void DirviewRefreshData ( PDirview o ) {
        DirviewGenerateSubs(o);
        ListviewUpdateAndReDraw(LISTVIEW(o));
}
////////////////////////////////////////////////////////////////////////////////
l_bool CollectCallBack ( void* id, l_text curfile );
////////////////////////////////////////////////////////////////////////////////
PList DirviewCollectSelctedFiles ( PDirview o ) {
        PList Lst;
        PProgressPop p;
        PListItem a = LISTVIEW(o)->Items->Last;

        if ( !a ) return NULL;

        Lst = NewList();

        if ( !Lst ) return NULL;

        p = ProgressPopUp(&Me,1,"I/O","Please wait. Collecting files...",0,"");

        do
        {
                if (LISTVIEWITEM(a->Data)->Flags & LVI_SELECTED) {
                        if ( !_GenerateFileRefs( Lst, DIRVIEWITEM(a->Data)->f, NULL, &CollectCallBack, p ) ) {
                                FreeList(Lst);
                                ProgressPopKick(p);
                                return NULL;
                        }
                }
                a = a->Prev;
        }
        while ( a != LISTVIEW(o)->Items->Last );
        ProgressPopKick(p);
        return Lst;
}
////////////////////////////////////////////////////////////////////////////////
PList DirviewGetSelectedFiles ( PDirview o ) {
        PList Lst;
        PListItem a = LISTVIEW(o)->Items->Last;
        if ( !a ) return NULL;
        Lst = NewList();
        if ( !Lst ) return NULL;
        do
        {
                if (LISTVIEWITEM(a->Data)->Flags & LVI_SELECTED)
                        ListAdd(Lst,NULL,NewFileReference(DIRVIEWITEM(a->Data)->f,DIRVIEWITEM(a->Data)->f->Name),(void*)&FreeFileRef);
                a = a->Prev;
        }
        while ( a != LISTVIEW(o)->Items->Last );
        return Lst;
}
////////////////////////////////////////////////////////////////////////////////
void DirviewItemStartDrag ( PWidget o ) {
        DebugMessage ("Drag n Drop: DirviewItemStartDrag");
        PList l = DirviewGetSelectedFiles(DIRVIEW(o));
        if ( l ) {
                DragStart(o,DTYPE_FILEREFLIST,l,DMETHOD_COPY|DMETHOD_MOVE);
                FreeList(l);
        }
}
////////////////////////////////////////////////////////////////////////////////
l_bool DirviewItemDropAccept ( PWidget o, PWidget Caller, PListviewItem Item, l_ulong Type, void *Data, l_ulong SupportedMethods, l_ulong *Method ) {
        DebugMessage ("Drag n Drop: DirviewItemDropAccept");
        PFileInfo DestItemInfo = DIRVIEW(o)->Dir;
        if ( Item ) DestItemInfo = DIRVIEWITEM(Item)->f;

        return IODlgDropAccept(DestItemInfo,Type,Data,SupportedMethods,Method);
}
////////////////////////////////////////////////////////////////////////////////
l_bool DirviewItemDragBeforeDrop    ( PWidget o, PWidget Dest, PListviewItem Item, void *Data, l_ulong Type, l_ulong Method ) {
        DebugMessage ("Drag n Drop: DirviewItemDragBeforeDrop");
        return true;
}
////////////////////////////////////////////////////////////////////////////////
void DirviewItemDragAfterDrop ( PWidget o, PWidget Dest, PListviewItem Item, void*Data, l_ulong Type, l_ulong Method ) {
        DebugMessage ("Drag n Drop: DirviewItemDragAfterDrop");
        DirviewRefreshData(DIRVIEW(o));
}
////////////////////////////////////////////////////////////////////////////////
void DirviewItemDropData ( PWidget o, PWidget Caller, PListviewItem Item, void *Data, l_ulong Type, l_ulong Method ) {
        DebugMessage ("Drag n Drop: DirviewItemDropData");
        PFileInfo DestItemInfo = DIRVIEW(o)->Dir;
        if ( Item ) DestItemInfo = DIRVIEWITEM(Item)->f;

        IODlgDropDataToFileInfo( DestItemInfo, Data, Type, Method );

        DirviewRefreshData(DIRVIEW(o));
        WidgetDraw(o,NULL);
        ScrollBarRedraw(SLLWDG(o));
}
////////////////////////////////////////////////////////////////////////////////
l_bool DirviewEventHandler ( PWidget o, PEvent Ev ) {




        if (Ev->Type == EV_KEYBOARD) {

                if ( SCANCODE(Ev,KEY_DEL) ) {
                        PListviewItem p = GetSelectedItem(LISTVIEW(o));
                        if ( p ) {
                                PList l = DirviewCollectSelctedFiles(DIRVIEW(o));
                                if ( l ) {
                                        DeleteFilesFromList(l);
                                        FreeList(l);
                                        DirviewRefreshData(DIRVIEW(o));
                                }
                        }
                        return true;
                } else if ( Ev->Keyb.Shift & KB_CTRL ) {

                        if ( SCANCODE(Ev,KEY_C) ) {
                                PListviewItem p = GetSelectedItem(LISTVIEW(o));
                                if ( p ) {
                                        //PList l = DirviewCollectSelctedFiles(DIRVIEW(o));
                                        PList l = DirviewGetSelectedFiles(DIRVIEW(o));
                                        if ( l )
                                                ClipboardPut(TYPE_FILEREFLIST,l,false);
                                        else
                                                MessageBox(&Me, "I/O", "Unable to copy files to clipboard.", 0);
                                }

                        } else if ( SCANCODE(Ev,KEY_V) ) {

                                WidgetSendSEvent(o, EV_MESSAGE, MSG_IOMENU_PASTE, Ev->Extra);

                        }
                        return true;
                }       else if ( SCANCODE(Ev,KEY_F5) ) {
                        DirviewRefreshData(DIRVIEW(o));
                        return true;
                }
        }
        if ( Ev->Type == EV_MESSAGE ) {
                if ( Ev->Message == MSG_IODLG_ITEMFILE_VAL ) {
                        if ( DIRVIEWITEM(Ev->Extra)->f->Attributes & FA_DIREC ) {
                                DirviewOpenDirectory(DIRVIEW(o),DIRVIEWITEM(Ev->Extra)->f->VPathName);
                                return true;
                        }
                }
                if ( Ev->Message == MSG_IODLG_OPENPARENT ) {
                        DirviewOpenParentDirectory(DIRVIEW(o));
                        return true;
                }
                if ( Ev->Message == MSG_IODLG_ITEMFILE_MENU ) {
                        if ( ListviewCountSelectedFiles(LISTVIEW(o)) > 1 ) {

                                MessageBox(&Me, "I/O", "Unsupported. Even use CTRL+C to copy selected files.", 0);

                        } else {
                                PMenu m = FileGetMenu2(DIRVIEWITEM(Ev->Extra)->f,0);
                                l_ulong msg = PopUpMenuWait(o->AppOwner,Mouse->State.p,m,MVF_FREEMENU);
                                if ( msg == MSG_IOMENU_COPY ) {
                                        //PList l = GenerateFileRefs(DIRVIEWITEM(Ev->Extra)->f);
                                        PList l = DirviewGetSelectedFiles(DIRVIEW(o));
                                        if ( l )
                                                ClipboardPut(TYPE_FILEREFLIST,l,false);
                                        else // System was unable to collect files, so we put only file reference
                                                MessageBox(&Me, "I/O", "Unable to copy files to clipboard.", 0);

                                } else if ( msg == MSG_IOMENU_DELETE ) {
                                        PList l = GenerateFileRefs(DIRVIEWITEM(Ev->Extra)->f);
                                        if ( l ) { DeleteFilesFromList(l); FreeList(l); DirviewRefreshData(DIRVIEW(o)); }
                                } else if ( msg == MSG_IOMENU_PASTE ) {
                                        PList el = ClipboardGet(TYPE_FILEREFLIST);
                                        if ( el ) {
                                                IODlgDropDataToFileInfo( DIRVIEWITEM(Ev->Extra)->f, el, TYPE_FILEREFLIST, DMETHOD_COPY );
                                        }
                                } else if ( /*msg == MSG_IOMENU_OPEN*/ msg == DirOpenMSG ) {
                                        WidgetSendSEvent(o, EV_MESSAGE, MSG_IODLG_ITEMFILE_VAL, Ev->Extra);
                                } else if ( msg == MSG_IODLG_ITEMFILE_VAL ) {

                                        WidgetSendSEvent(o, EV_MESSAGE, MSG_IODLG_ITEMFILE_VAL, Ev->Extra);

                                } else if ( msg == MSG_IOMENU_RENAME ) {

                                        l_text NewName = ListviewRenameItemBox(LISTVIEW(o),LISTVIEWITEM(Ev->Extra));

                                        if ( NewName ) {
                                                l_text NewVPathName = FileNameToPath(DIRVIEW(o)->Directory,NewName);

                                                l_bool r = false;

                                                if ( DIRVIEWITEM(Ev->Extra)->f->Attributes & FA_DIREC )
                                                        r = DirRename(DIRVIEWITEM(Ev->Extra)->f->VPathName,NewVPathName);
                                                else
                                                        r = FileRename(DIRVIEWITEM(Ev->Extra)->f->VPathName,NewVPathName);

                                                if ( r ) {
                                                        FreeFileInfo(DIRVIEWITEM(Ev->Extra)->f);
                                                        DIRVIEWITEM(Ev->Extra)->f = FileGetInfo(NewVPathName);
                                                        free(LISTVIEWITEM(Ev->Extra)->Caption);
                                                        LISTVIEWITEM(Ev->Extra)->Caption = TextDup(NewName);
                                                        WidgetDraw(o,NULL);
                                                        ScrollBarRedraw(SLLWDG(o));
                                                }
                                                free(NewVPathName);
                                                free(NewName);
                                        }
                                } else if ( msg )
                                        FileHandleMsg(DIRVIEWITEM(Ev->Extra)->f,msg);
                                return true;
                        }
                }


                if ( Ev->Message == MSG_IOMENU_PASTE ) {
                        PList el = ClipboardGet(TYPE_FILEREFLIST);
                        if ( el ) {
                                IODlgDropDataToFileInfo( DIRVIEW(o)->Dir, el, TYPE_FILEREFLIST, DMETHOD_COPY );
                                DirviewRefreshData(DIRVIEW(o));
                        }
                        return true;
                }

                if ( Ev->Message == MSG_IODLG_NEWDIR ) {
                        l_ulong nb = 0;
                        l_text NewName;
                        l_text Out = FileNameToPath(DIRVIEW(o)->Directory,"New directory");
                        PFileInfo of;
                        while ( of = FileGetInfo(Out) ) {
                                free(Out);
                                FreeFileInfo(of);
                                nb++;
                                NewName = TextArgs("New directory (%d)",nb);
                                Out = FileNameToPath(DIRVIEW(o)->Directory,NewName);
                                free(NewName);
                        }
                        MakeDir(Out);
                        of = FileGetInfo(Out);
                        free(Out);
                        if ( of ) {
                                DirviewAddItem(DIRVIEW(o),of);
                                WidgetDraw(o,NULL);
                                ScrollBarRedraw(SLLWDG(o));
                        }
                        return true;
                }

        }

        if ( ListviewEventHandler(o,Ev) ) return true;

        if ( Ev->Message == WEvMouseRUp ) {
                PFileInfo f = FileGetInfo(DIRVIEW(o)->Directory);
                if ( f ) {
                        PMenu m = FileGetMenu2(f,1);
                        l_ulong msg = PopUpMenuWait(o->AppOwner,Mouse->State.p,m,MVF_FREEMENU);

                        if ( msg == MSG_IOMENU_PASTE || msg == MSG_IODLG_NEWDIR ) {
                                WidgetSendSEvent(o, EV_MESSAGE, msg, Ev->Extra);

                        } else
                                FileHandleMsg(f,msg);
                }
                return true;
        }

        return false;
}
////////////////////////////////////////////////////////////////////////////////
void DirviewSetExtentions ( PDirview o, PList Extentions ) {
        o->Extentions = Extentions;
        DirviewGenerateSubs(o);
        ListviewUpdateAndReDraw(LISTVIEW(o));
}
////////////////////////////////////////////////////////////////////////////////
void DirviewFree ( PWidget o )
{
        if ( DIRVIEW(o)->Dir ) FreeFileInfo(DIRVIEW(o)->Dir);
        if ( DIRVIEW(o)->Directory ) free(DIRVIEW(o)->Directory);
        ListviewFree(o);
}
////////////////////////////////////////////////////////////////////////////////
PDirview CreateDirview ( PApplication App, TRect r, l_text dir, PList Extentions, l_ulong Flags )
{
                                //FreeFileInfo(dest);

        PDirview o = malloc(sizeof(TDirview));

        if (!o) return NULL;
        memset(o, 0, sizeof(TDirview));
        InitListview(LISTVIEW(o), DirviewClass, App, r,"File name",LVS_ICONS,LVF_DRAWICONS|LVF_COLUMS|LVF_ALLOWDRAG|LVF_COLRESIZE);
        ListviewAddColum ( LISTVIEW(o), "Size",NULL, 40, 0 );
        ListviewAddColum ( LISTVIEW(o), "Created",NULL, 125, 0 );
        ListviewAddColum ( LISTVIEW(o), "Modified",NULL, 125, 0 );
        ListviewAddColum ( LISTVIEW(o), "Accessed",NULL, 125, 0 );
        //ListviewAddColum ( o, "Attributes",NULL, 75, 0 );


        o->Directory = TextDup(dir);
        o->Extentions = Extentions;

        WIDGET(o)->EventHandler = &DirviewEventHandler;
        WIDGET(o)->FreeEx = &DirviewFree;

        LISTVIEW(o)->OnSelMsg = MSG_IODLG_ITEMFILE_SEL;
        LISTVIEW(o)->OnValMsg = MSG_IODLG_ITEMFILE_VAL;
        LISTVIEW(o)->OnRClkMsg = MSG_IODLG_ITEMFILE_MENU;

        if ( Flags & DVF_MULTISELECT ) LISTVIEW(o)->Flags |= LVF_MULTISELECT;

        o->Flags = Flags;
        LISTVIEW(o)->ItemStartDrag = &DirviewItemStartDrag;
        LISTVIEW(o)->ItemDropAccept             = &DirviewItemDropAccept;
        LISTVIEW(o)->ItemDragBeforeDrop         = &DirviewItemDragBeforeDrop;
        LISTVIEW(o)->ItemDropData               = &DirviewItemDropData;
        LISTVIEW(o)->ItemDragAfterDrop  = &DirviewItemDragAfterDrop;


        DirviewGenerateSubs(o);
        ListviewCalculateMax(WIDGET(o));

        return o;
}
////////////////////////////////////////////////////////////////////////////////
// TreeDirview
////////////////////////////////////////////////////////////////////////////////
PTreeItem TreeDirViewOpenDirEx ( PTreeView t, PTreeItem i, l_text dir ) {

        i->Flags |= TIF_EXPENDED;

        if ( !i->Last )
                if ( i->GenerateSubs )
                        i->GenerateSubs(i);

        if ( i->Last ) {
                l_text S;
                PTreeItem a = i->Last;
                PTreeItem b = a;

                if ( *dir=='\\' || *dir == '/' ) dir++;

                if ( !(*dir) ) return i;
                S = dir;
        while ( *S ) {
                if ( *S=='\\' || *S == '/' ) {
                        do {
                          if ( !TextSqNCaseCompare(dir,a->Caption,S-dir) )
                                return TreeDirViewOpenDirEx( t,a,S+1);
                        a = a->Next;
                } while ( a != b );
                }
                S++;
        }
        do {
                if ( !TextCaseCompare(dir,a->Caption) ) return TreeDirViewOpenDirEx( t,a,"");
                  a = a->Next;
          } while ( a != b );
        }
        return i;
}
////////////////////////////////////////////////////////////////////////////////
void TreeDirViewOpenDir ( PTreeDirview o, l_text dir ) {
        PTreeItem i = TreeDirViewOpenDirEx(TREEVIEW(o),TREEVIEW(o)->Items->Last,dir);
        TreeviewScrollBarRecalculate(TREEVIEW(o));
        if ( i ) TreeViewSelectItem(TREEVIEW(o),i);
}
////////////////////////////////////////////////////////////////////////////////
l_bool DirTreeViewEventHandler ( PWidget o, PEvent Ev ) {

        if ( Ev->Type == EV_MESSAGE ) {
                if ( Ev->Message == MSG_TREEVIEW_ITEM ) {

                        if ( !TREEVIEW(o)->Sel ) return true;

                        if ( TREEDIRVIEW(o)->Linked ) {
                                DirviewOpenDirectory(TREEDIRVIEW(o)->Linked,TREEDIRVIEWITEM(TREEVIEW(o)->Sel)->f->VPathName);
                        }

                        if ( TREEDIRVIEW(o)->Msg ) {
                                if ( TREEDIRVIEW(o)->Msg != MSG_TREEVIEW_ITEM ) // Prevent infinite loops
                                        WidgetSendSEvent(o, EV_MESSAGE, TREEDIRVIEW(o)->Msg, TREEVIEW(o)->Sel);
                        }

                        return true;
                }
                if ( (Ev->Message == MSG_TREEVIEW_ITEM_MENU) && TREEVIEW(o)->Sel ) {
                        PFileInfo f = TREEDIRVIEWITEM(TREEVIEW(o)->Sel)->f;
                        if ( f ) {
                                PMenu m = FileGetMenu2(f,1);
                                if ( m ) {
                                        l_ulong msg;

                                        m->Items = NewMenuItem("Explore",NULL,MSG_TREEVIEW_ITEM,NULL,NULL,
                                                NewMenuItem("Expend",NULL,MSG_TREEVIEW_EXPEND,NULL,NULL,
                                                NewMenuItemSeparator(
                                                m->Items)));

                                        msg = PopUpMenuWait(o->AppOwner,Mouse->State.p,m,MVF_FREEMENU);

                                        if ( msg == MSG_TREEVIEW_ITEM ) {
                                                if ( TREEDIRVIEW(o)->Linked ) {
                                                        DirviewOpenDirectory(TREEDIRVIEW(o)->Linked,TREEDIRVIEWITEM(TREEVIEW(o)->Sel)->f->VPathName);
                                                }

                                                if ( TREEDIRVIEW(o)->Msg ) {
                                                        if ( TREEDIRVIEW(o)->Msg != MSG_TREEVIEW_ITEM ) // Prevent infinite loops
                                                                WidgetSendSEvent(o, EV_MESSAGE, TREEDIRVIEW(o)->Msg, TREEVIEW(o)->Sel);
                                                }

                                        } else if ( msg == MSG_TREEVIEW_EXPEND ) {
                                                TREEVIEW(o)->Sel->Flags |= TIF_EXPENDED;
                                                if ( !TREEVIEW(o)->Sel->Last )
                                                        if ( TREEVIEW(o)->Sel->GenerateSubs )
                                                                TREEVIEW(o)->Sel->GenerateSubs(TREEVIEW(o)->Sel);
                                                TreeviewScrollBarRecalculate(TREEVIEW(o));
                                                WidgetDraw(WIDGET(o),NULL);
                                                ScrollBarRedraw(SLLWDG(o));
                                        } else

                                                FileHandleMsg(f,msg);
                                }
                        }
                        return true;
                }
        }

        return TreeViewEventHandler(o,Ev);
}
////////////////////////////////////////////////////////////////////////////////
void TreeDirviewItemFreeEx ( PTreeviewItem i ) {

        if ( TREEDIRVIEWITEM(i)->f ) FreeFileInfo(TREEDIRVIEWITEM(i)->f);

}

////////////////////////////////////////////////////////////////////////////////
void DirTreeViewGenerateSubs ( PTreeItem o  );
////////////////////////////////////////////////////////////////////////////////
PTreeDirviewItem TreeDirviewAddItem ( PTreeItem Parent, PFileInfo f ) {
        PIcon Icon;
        l_text Name = f->Name;
        PTreeDirviewItem i = malloc(sizeof(TTreeDirviewItem));
        if ( !i ) return;
        memset(i,0,sizeof(TTreeDirviewItem));
        Icon = FileGetIconsEx(f);
        if ( !TextCompare(Name,"")) Name = "Computer";
        TreeviewInitItem(TREEVIEWITEM(i),Name,NULL,Icon,&DirTreeViewGenerateSubs,&TreeDirviewItemFreeEx);
        FreeIcon(Icon);

        i->f = f;

        TreeviewAddItem(Parent,TREEVIEWITEM(i));


        return ;
}

////////////////////////////////////////////////////////////////////////////////
l_int TreeDirviewItemOrderyByName ( PFileInfo a, PFileInfo b ) {

// improved by Florian Xaver

        if ( (a->Attributes & FA_DIREC) && (b->Attributes & FA_DIREC) )
                return TextCaseCompare(a->Name,b->Name);
        else
                return -1;

}




////////////////////////////////////////////////////////////////////////////////
void DirTreeViewGenerateSubs ( PTreeItem o  )
{
        PListItem a, b;
        PList l = ListDir(TREEDIRVIEWITEM(o)->f->VPathName,FA_DIREC|FA_LABEL);
        EmptySubTreeItems(o);
        if ( !l ) return;

        if ( l->Last ) {
           ListOrderByData(l,(void*)&TreeDirviewItemOrderyByName);
           a = b = l->Last->Next;
           do {
                if ( FILEINFO(a->Data)->Attributes & FA_DIREC ) {
                        TreeDirviewAddItem(o,FILEINFO(a->Data));

                }
                a = a->Next;
           } while ( a != b );
        }
        KickList(l);
}
////////////////////////////////////////////////////////////////////////////////
void TreeDirviewItemStartDrag ( PWidget o ) {
        DebugMessage ("Drag n Drop: TreeDirviewItemStartDrag");
/*        PList l = DirviewGetSelectedFiles(DIRVIEW(o));
        if ( l ) {
                DragStart(o,DTYPE_FILEREFLIST,l,DMETHOD_COPY|DMETHOD_MOVE);
                FreeList(l);
        }*/
}
////////////////////////////////////////////////////////////////////////////////
l_bool TreeDirviewItemDropAccept ( PWidget o, PWidget Caller, PTreeviewItem Item, l_ulong Type, void *Data, l_ulong SupportedMethods, l_ulong *Method ) {
        DebugMessage ("Drag n Drop: TreeDirviewItemDropAccept");
        if ( Item )
                return IODlgDropAccept(TREEDIRVIEWITEM(Item)->f,Type,Data,SupportedMethods,Method);
        return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool TreeDirviewItemDragBeforeDrop    ( PWidget o, PWidget Dest, PTreeviewItem Item, void *Data, l_ulong Type, l_ulong Method ) {
        DebugMessage ("Drag n Drop: TreeDirviewItemDragBeforeDrop");
        return true;
}
////////////////////////////////////////////////////////////////////////////////
void TreeDirviewItemDragAfterDrop ( PWidget o, PWidget Dest, PTreeviewItem Item, void*Data, l_ulong Type, l_ulong Method ) {
        DebugMessage ("Drag n Drop: TreeDirviewItemDragAfterDrop");
        if ( Method == DMETHOD_MOVE )
                TreeViewDeleteItem(TREEVIEW(o),Item);
        WidgetDraw(o,NULL);
        ScrollBarRedraw(SLLWDG(o));
}
////////////////////////////////////////////////////////////////////////////////
void TreeDirviewItemDropData ( PWidget o, PWidget Caller, PTreeviewItem Item, void *Data, l_ulong Type, l_ulong Method ) {
        PFileInfo DestItemInfo = TREEDIRVIEWITEM(Item)->f;
        DebugMessage ("Drag n Drop: TreeDirviewItemDropData: Destination: %s", DestItemInfo->Name);

        IODlgDropDataToFileInfo( DestItemInfo, Data, Type, Method );


        TreeviewUnExpendItem(TREEVIEW(o),Item);
        WidgetDraw(o,NULL);
        ScrollBarRedraw(SLLWDG(o));
}
////////////////////////////////////////////////////////////////////////////////
PTreeDirview CreateTreeDirview ( PApplication App, TRect r ) {
        PFileInfo fi; PIcon Icon;
        PTreeDirview o = malloc(sizeof(TTreeDirview));

        if ( !o ) return NULL;
        memset(o,0,sizeof(TTreeDirview));

        InitTreeView(TREEVIEW(o),TreeDirviewClass,App,r,MSG_TREEVIEW_ITEM);

        TREEVIEW(o)->Flags |= TVF_ALLOWDRAG;

        TREEVIEW(o)->ItemStartDrag              = &TreeDirviewItemStartDrag;
        TREEVIEW(o)->ItemDropAccept     = &TreeDirviewItemDropAccept;
        TREEVIEW(o)->ItemDragBeforeDrop = &TreeDirviewItemDragBeforeDrop;
        TREEVIEW(o)->ItemDropData               = &TreeDirviewItemDropData;
        TREEVIEW(o)->ItemDragAfterDrop  = &TreeDirviewItemDragAfterDrop;

        TREEVIEW(o)->OnMenuMsg = MSG_TREEVIEW_ITEM_MENU;

        WIDGET(o)->EventHandler = &DirTreeViewEventHandler;

        TreeDirviewAddItem(TREEVIEW(o)->Items,FileGetInfo("/"));

        return o;
}
////////////////////////////////////////////////////////////////////////////////
// IO Box
////////////////////////////////////////////////////////////////////////////////

#define CB_FILTER       0xFF00FFB0
#define TB_ADD          0xFF00FFB1
#define TB_REMOVE       0xFF00FFB2
#define TB_GOUP         0xFF00FFB4
#define LB_CHANGE       0xFF00FFB8

l_text IOBox ( l_text Title, l_ulong Flags, l_text *Dir, PFileTypes Types, l_bool FExists )
{
        TRect r;
        PList DefExt = NULL;

        PWindow w;
        PButton b;
        PButton BAdd;
        PButton BRemove;
        PButton BUp;
        PListbox l;
        PDirview dir;
        PCheckbox c;
        PLabel t;

        PTextbox  FileName;
        PCombobox tsel;
        l_ulong    msg = 0;
        l_text    OpenFile = NULL;
        p_bitmap Icon = LoadImage("./SYSTEM/ICONS/folder16.bmp");

        if ( !Dir ) Dir = &GlobalDir;

        if ( Types ) DefExt = Types->Extentions; // DO NOT MOVE THIS !

        RectAssign(&r, 0, 0, 600, 440);
        w = CreateWindow(&Me, r, Title, WF_CAPTION|WF_FRAME|WF_CENTERED|WF_MINIMIZE);
        InsertWidget(DeskTop, WIDGET(w));

        RectAssign(&r, 10, 10, 70, 40);
        BAdd = CreateButton(&Me,r,"Add",TB_ADD);
        BAdd->Icon = IconAdd;
        InsertWidget(WIDGET(w), WIDGET(BAdd));

        RectAssign(&r, 75, 10, 160, 40);
        BRemove = CreateButton(&Me,r,"Remove",TB_REMOVE);
        BRemove->Icon = IconRemove;
        BRemove->Flags |= BF_DISABLED;
        InsertWidget(WIDGET(w), WIDGET(BRemove));

        RectAssign(&r, 165, 10, 215, 40);
        BUp = CreateButton(&Me,r,"Up",TB_GOUP);
        BUp->Icon = IconUp;
        InsertWidget(WIDGET(w), WIDGET(BUp));

        RectAssign(&r, 10, 50, 215, 295);
        l = CreateListbox(&Me, r);
        l->AddItem(l, "Home", "/", Icon);
        l->AddItem(l, "Desktop", "/system/DESKTOP/", Icon);
        l->AddItem(l, "Floppy", "/a/", Icon);
        l->AddItem(l, "CD-ROM", "/d/", Icon);
        l->AddSeperator(l);
        l->AddItem(l, "Documents", NULL, Icon);
        l->OnSelect = LB_CHANGE;
        InsertWidget(WIDGET(w), WIDGET(l));

        RectAssign(&r, 225, 50, 590, 295);
        dir = CreateDirview(&Me, r, *Dir?*Dir:"", DefExt, DVF_NOPARICON|DVF_ORDERBYNAME);
        LISTVIEW(dir)->Style = LVS_LIST;
        InsertWidget(WIDGET(w), WIDGET(dir));

        RectAssign(&r, 10, 315, 80, 340);
        t = CreateLabel(&Me,r,"Filename:");
        WIDGET(t)->BackgroundColor = COL_3DFACE;
        InsertWidget(WIDGET(w), WIDGET(t));

        RectAssign(&r, 90, 315, 430, 340);
        FileName = CreateTextbox(&Me,r,TBF_EDITABLE);
        InsertWidget(WIDGET(w), WIDGET(FileName));

        RectAssign(&r, 440, 315, 590, 340);
        tsel = NewComboBox(&Me, r, CB_FILTER);
        InsertWidget(WIDGET(w), WIDGET(tsel));

        if ( Types )
        {
                PFileTypes ft = Types;

                while ( ft )
                {
                        ComboboxAddItem(tsel,ft->Title,NULL);
                        ft = ft->Next;
                }

        }
        else
        {
                ComboboxAddItem(tsel, "All files (*.*)", NULL);
        }

        ComboboxSelectIndex(tsel, 1);


        RectAssign(&r, 405, 400, 490, 430);
        b = CreateButton(&Me,r,"Cancel",WM_CLOSE);
        b->Icon = IconCancel;
        InsertWidget(WIDGET(w), WIDGET(b));

        RectAssign(&r ,505, 400, 590, 430);
        if ( Flags & IOBOX_SAVE )
        {
                b = CreateButton(&Me, r, "Save", MSG_IODLG_BOX_OK);
                b->Icon = IconSave;
        }
        else
        {
                b = CreateButton(&Me, r, "Open", MSG_IODLG_BOX_OK);
                b->Icon = IconOpen;
        }

        InsertWidget(WIDGET(w), WIDGET(b));
        WidgetDrawAll(WIDGET(w));

        while ( (msg = WidgetExecute(WIDGET(w))) != WM_CLOSE )
        {
                switch ( msg )
                {
                        case WM_CLOSE:
                                break;
                        break;

                        case TB_GOUP:
                                DirviewOpenParentDirectory(dir);
                                TextBoxSetText(FileName, "");
                        break;

                        case LB_CHANGE:
                                if ( ((PListboxItem)(l->SelectedRow)) )
                                {
                                        if ( BRemove->Flags & BF_DISABLED )
                                        {
                                                BRemove->Flags &= ~BF_DISABLED;
                                                WidgetDraw(WIDGET(BRemove), NULL);
                                        }

                                        if ( l->SelectedRow->Data )
                                                DirviewOpenDirectory(dir, l->SelectedRow->Data);

                                }
                                else
                                {
                                        BRemove->Flags |= BF_DISABLED;
                                        WidgetDraw(WIDGET(BRemove), NULL);
                                }
                        break;

                        case MSG_IODLG_BOX_OK:
                        {
                                if ( TextCompare(FileName->Text, "") )
                                        OpenFile = FileNameToPath(dir->Directory,FileName->Text);
                        }
                        break;

                        case CB_FILTER:
                                if ( Types && tsel->Selected )
                                {
                                        PFileTypes ft = FileTypesAt(Types,ComboboxItemIndex(tsel,tsel->Selected)-1);
                                        if ( ft ) DirviewSetExtentions(dir,ft->Extentions);
                                }
                        break;

                        case MSG_IODLG_ITEMFILE_SEL:
                        {
                                PListviewItem a = GetSelectedItem(LISTVIEW(dir));
                                if ( a )
                                {
                                        if ( DIRVIEWITEM(a)->f->Attributes & FA_DIREC )
                                                TextBoxSetTextEx(FileName,"");
                                        else
                                                TextBoxSetTextEx(FileName,a->Caption);
                                }
                        }
                        break;

                        case MSG_IODLG_ITEMFILE_VAL:
                        {
                                PListviewItem a = GetSelectedItem(LISTVIEW(dir));
                                if ( a )
                                {
                                        if ( !(DIRVIEWITEM(a)->f->Attributes & FA_DIREC) )
                                        {
                                                OpenFile = FileNameToPath(dir->Directory,a->Caption);
                                        }
                                }
                        }
                        break;
                }

                /**
                 * TODO: Clean this crap
                 */
                if ( OpenFile )
                {
                        PFileInfo fi = FileGetInfo(OpenFile);

                        if ( fi )
                        {
                                if ( fi->Attributes & FA_DIREC )
                                {
                                        DirviewOpenDirectory(dir,OpenFile);
                                        TextBoxSetText(FileName,"");
                                        free(OpenFile);
                                        OpenFile = NULL;
                                        FreeFileInfo(fi);
                                }
                                else if ( !FExists )
                                {
                                        l_ulong msg = MessageBox(&Me,"Warning", "File already exists. Do you want to replace it?", MBB_YESNOCANCEL|MBI_WARNING);
                                        FreeFileInfo(fi);
                                        if ( msg == MSG_YES ) break;
                                        free(OpenFile);
                                        OpenFile = NULL;
                                        if ( msg == MSG_CANCEL ) break;
                                }
                                else
                                {
                                        FreeFileInfo(fi);
                                        break;
                                }
                        }
                        else
                        {
                                if ( FExists )
                                {
                                        free(OpenFile);
                                        OpenFile = NULL;
                                }
                                else break;
                        }
                }
        }

        if ( *Dir ) free(*Dir);
        *Dir = TextDup(dir->Directory);

        WidgetDispose(WIDGET(w));
        DestroyImage(Icon);



        return OpenFile;
}
////////////////////////////////////////////////////////////////////////////////
// Files manipulation GUI progress info
////////////////////////////////////////////////////////////////////////////////
l_bool CollectCallBack ( void* id, l_text curfile ) {

        SysPoll();

        if ( WIDGET(PROGRESSPOP(id)->W)->MessageBuffer == WM_CLOSE ) {
                ProgressPopSetInfo((PProgressPop)id,"Stopping...");
                return false;
        } else
        {
                ProgressPopSetInfo((PProgressPop)id,"Adding %s",curfile);
                DebugMessage("Adding %s...", curfile);
        }
        return true;
}
////////////////////////////////////////////////////////////////////////////////
PList GenerateFileRefs ( PFileInfo f ) {
        PList Lst = NewList();
        PProgressPop p;
        p = ProgressPopUp(&Me,1,"I/O","Please wait. Collecting files...",0,"");
        if ( !_GenerateFileRefs( Lst, f, NULL, &CollectCallBack, p ) ) {
                FreeList(Lst);
                ProgressPopKick(p);
                return NULL;
        }
        ProgressPopKick(p);
        return Lst;
}
////////////////////////////////////////////////////////////////////////////////
PList CollectFilesFromRef ( PList l ) {
        PList Lst;
        PProgressPop p;
        PListItem a = l->Last;
        if ( !a ) return NULL;
        Lst = NewList();
        if ( !Lst ) return NULL;
        p = ProgressPopUp(&Me,1,"I/O","Please wait. Collecting files...",0,"");
        do
        {       // below generates a null-pointer-warning in kernel.txt: filetopath.dir
                if ( !_GenerateFileRefs2( Lst, (PFileRef)a->Data, NULL, &CollectCallBack, p ) ) {
                        FreeList(Lst);
                        ProgressPopKick(p);
                        return NULL;
                }
                a = a->Prev;
        }
        while ( a != l->Last );
        ProgressPopKick(p);
        return Lst;
}
////////////////////////////////////////////////////////////////////////////////
l_bool FileCopier ( PProgressPop p, PFile s, PFile d ) {
        l_char Buffer[4096];
        l_int r = 0;
        while ( !FileEOF(s) ) {
                r = FileRead(&Buffer,1,4096,s);
                SysPoll();
                FileWrite(&Buffer,1,r,d);
                ProgressPopStep(p,r/1024);
                SysPoll();
                if ( WIDGET(p->W)->MessageBuffer == WM_CLOSE ) ProgressPopSetInfo(p,"Stopping...");
        }
        if ( WIDGET(p->W)->MessageBuffer == WM_CLOSE ) return false;
        return true;
}
////////////////////////////////////////////////////////////////////////////////
void CopyFilesFromList ( PList l, l_text Dest ) {
// here is a bug! crashs pc if drag n drop of directories... Florian Xaver

if ( l->Last ) {
        l_ulong GlobalSize = 0;
        PProgressPop p = 0;
        PListItem a = l->Last->Next, b = a;
        l_bool Ignore = false, ReplaceAll = false;
        l_ulong SamePlace = 0;

        do {
                PFileRef r = FILEREF(a->Data);

                if ( r->Attributes & FA_DIREC )
                        GlobalSize += 1;
                else
                        GlobalSize += r->Size/1024;

                a = a->Next;
        } while ( a != b );

        p = ProgressPopUp(&Me,7,"I/O","Please wait. Copying files...",GlobalSize,"...");
        DebugMessage("Copying files...");

        do {
                PFileRef r = FILEREF(a->Data);
                l_text Out = SamePlace ? NULL : FileNameToPath(Dest,r->SName);
                PFileInfo of = ( ReplaceAll || SamePlace ) ? NULL : FileGetInfo(Out);
                ProgressPopSetInfo(p,"%s",r->SName);
                SysPoll();

                Ignore = false;

                if ( of ) {

                        if ( of->Drive == r->Drive && !TextCompare(of->LocalName,r->LocalName) ) {
                                l_text NewSName = TextArgs("Copy of %s",r->SName);
                                free(Out);
                                FreeFileInfo(of);
                                SamePlace = 1;
                                Out = FileNameToPath(Dest,NewSName);
                                free(NewSName);
                                while ( of = FileGetInfo(Out) ) {
                                        free(Out);
                                        FreeFileInfo(of);
                                        NewSName = TextArgs("Copy(%d) of %s",SamePlace-1,r->SName);
                                        Out = FileNameToPath(Dest,NewSName);
                                        free(NewSName);
                                        SamePlace++;
                                }
                        } else {

                                l_text text = TextArgs("File %s (%d bytes) yet exists.\nDo you want to replace it with %s (%d bytes) ?",Out,of->Size,r->SName,r->Size);
                                l_ulong msg = MessageBox(&Me,"I/O",text,MBB_YESYESALLNOCANCEL);

                                if ( msg == MSG_CANCEL ) break;
                                if ( msg == MSG_NO ) Ignore = true;
                                if ( msg == MSG_YESALL ) ReplaceAll = true;

                                free(text);
                                FreeFileInfo(of); of = NULL;

                        }
                }

                if ( !Ignore ) {

                if ( SamePlace ) {
                        l_text NewSName;
                        if ( SamePlace > 1 )
                                NewSName = TextArgs("Copy(%d) of %s",SamePlace-1,r->SName);
                        else
                                NewSName = TextArgs("Copy of %s",r->SName);
                        Out = FileNameToPath(Dest,NewSName);
                }


                if ( r->Attributes & FA_DIREC ) {
                        MakeDir(Out);
                        SysPoll();
                        ProgressPopStep(p,1);
                } else if ( r->Drive->Driver->FileOpen ) {
                        PFile s = 0;
                        PFile d = 0;
                        s = r->Drive->Driver->FileOpen(r->Drive,r->LocalName,"rb");
                        d = FileOpen(Out,"wb");
                        if ( d && s ) {
                                FileCopier(p,s,d);
                        }       else
                        {
                                DebugMessage ("Couldn't copy file - reason unknown! :-(");
                                ProgressPopStep(p,r->Size/1024);
                        }
                        if ( s ) FileClose(s);
                        if ( d ) FileClose(d);
                        DebugMessage ("File is copied!");
//                                MessageBox(&Me,"I/O","ok",MBB_OK);


                }

                }
                if (Out)  {free(Out); Out = 0;}
                if ( WIDGET(p->W)->MessageBuffer == WM_CLOSE ) break;
                a = a->Next;
        } while ( a != b );

        ProgressPopKick(p);
        }
}
////////////////////////////////////////////////////////////////////////////////
void DeleteFilesFromList ( PList l ) {

        l_ulong Nb = 0;
        PProgressPop p;
        PListItem a = l->Last, b = a;

        do {
                Nb++;
                a = a->Prev;
        } while ( a != b );

        p = ProgressPopUp(&Me,7,"I/O","Please wait. Removing files...",Nb,"...");

        do {
                PFileRef r = FILEREF(a->Data);

                ProgressPopSetInfo(p,"%s",r->SName);

                SysPoll();

                if ( r->Attributes & FA_DIREC ) {
                        if ( r->Drive->Driver->DirDelete )
                                r->Drive->Driver->DirDelete(r->Drive,r->LocalName);
                } else {
                        if ( r->Drive->Driver->FileDelete )
                                r->Drive->Driver->FileDelete(r->Drive,r->LocalName);
                }

                if ( WIDGET(p->W)->MessageBuffer == WM_CLOSE ) break;


                ProgressPopStep(p,1);
                a = a->Prev;
        } while ( a != b );

        ProgressPopKick(p);

}
////////////////////////////////////////////////////////////////////////////////
PWidget  StdCreateDirview ( PApplication App, TRect r )
{
        return WIDGET(CreateDirview (  App,  r, "/", NULL, NULL ));
}
////////////////////////////////////////////////////////////////////////////////
PWidget  StdCreateTreeDirview ( PApplication App, TRect r )
{
        return WIDGET(CreateTreeDirview( App, r ));
}
////////////////////////////////////////////////////////////////////////////////
l_bool LibMain ( int argc, l_text *argv )
{
        DirviewClass = CreateWidgetClass( ULONG_ID('D','r','V','w'), ListviewClass, 0 );
        DirviewClass->StdCreate = &StdCreateDirview;

        TreeDirviewClass = CreateWidgetClass( ULONG_ID('D','T','r','V'), TreeviewClass, 0 );
        TreeDirviewClass->StdCreate = &StdCreateTreeDirview;


        GlobalDir = TextDup("/");

        ParentDirIcon = NewIcon(LoadImage("SYSTEM/ICONS/folpar16.bmp"),LoadImage("SYSTEM/ICONS/folpar32.bmp"),NULL);

        APPEXPORT(IOBox);
        APPEXPORT(CreateDirview);
        APPEXPORT(DirviewOpenDirectory);
        APPEXPORT(TreeDirViewOpenDir);
        APPEXPORT(FreeFileTypes);
        APPEXPORT(NewFileTypes);
        APPEXPORT(DirviewOpenParentDirectory);//
        APPEXPORT(CreateTreeDirview);
        APPEXPORT(CollectFilesFromRef);
        APPEXPORT(DirviewRefreshData);
        /*MouseShow();
        IOBox ( "Select a file", "Open", "/", NULL, true );*/

        DirOpenMSG = GetTypeDefaultMsg(DTYPE_DIRECTORY);

        IconOpen = LoadImage("./SYSTEM/ICONS/open.bmp");
        IconSave = LoadImage("./SYSTEM/ICONS/save.bmp");
        IconAdd = LoadImage("SYSTEM/ICONS/add.bmp");
        IconUp = LoadImage("SYSTEM/ICONS/up.bmp");
        IconRemove = LoadImage("SYSTEM/ICONS/remove.bmp");
        IconCancel = LoadImage("./SYSTEM/ICONS/cancel.bmp");



        return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{
        if ( GlobalDir ) free(GlobalDir);
        if ( ParentDirIcon ) FreeIcon(ParentDirIcon);

        if ( IconOpen ) destroy_bitmap(IconOpen);
        if ( IconSave ) destroy_bitmap(IconSave);
        if ( IconAdd ) destroy_bitmap(IconAdd);
        if ( IconUp ) destroy_bitmap(IconUp);
        if ( IconRemove ) destroy_bitmap(IconRemove);
        if ( IconCancel ) destroy_bitmap(IconCancel);

}
////////////////////////////////////////////////////////////////////////////////
