////////////////////////////////////////////////////////////////////////////////
//
//  I/O Dialogs - Header File
//
//  (c) Copyright 2003 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _IODLG_H_INCLUDED_
#define _IODLG_H_INCLUDED_

#include "listview.h"
#include "treeview.h"

#define DIRVIEW(o) ((PDirview)(o))
typedef struct TTreeDirview 	*PTreeDirview;
typedef struct TDirview 			*PDirview;
typedef struct TFileTypes 		*PFileTypes;


typedef struct TFileTypes {

	l_text Title;

	PList Extentions;

	PFileTypes Next;

} TFileTypes;




typedef struct TDirview
{
	struct	TListview o;

	l_text  Directory;

	l_ulong Flags;

	PList Extentions;

	PFileInfo Dir;

} TDirview;


typedef struct TDirviewItem *PDirviewItem;
typedef struct TDirviewItem
{
	struct	TListviewItem o;

	PFileInfo f;

} TDirviewItem;

#define DIRVIEWITEM(o) ((PDirviewItem)(o))


// Flag - disables the parent dir icon when set
#define DVF_NOPARICON				0x00000001
#define DVF_MULTISELECT             0x00000002
#define DVF_PREDRAG                 0x00000004

#define DVF_ORDERBYNAME             0x00000008
#define DVF_ORDERBYSIZE             0x00000010
#define DVF_ORDERBYMTIME            0x00000020
#define DVF_ORDERBYCTIME            0x00000040
#define DVF_ORDERBYATIME            0x00000080

// 0xFFFF51XX Reserved to I/O Dialogs
#define MSG_IODLG_ITEMFILE_VAL		0xFFFF5100
#define MSG_IODLG_ITEMFILE_SEL		0xFFFF5102
#define MSG_IODLG_OPENPARENT		0xFFFF5104

#define MSG_IODLG_BOX_OK    		0xFFFF5105
#define MSG_IODLG_ITEMFILE_MENU		0xFFFF5106

#define MSG_IOMENU_OPEN				0xFFFF5108
#define MSG_IOMENU_PROPERTIES		0xFFFF5109
#define MSG_IOMENU_DELETE			0xFFFF510A

#define MSG_IOMENU_COPY				0xFFFF510B
#define MSG_IOMENU_CUT				0xFFFF510C
#define MSG_IOMENU_PASTE			0xFFFF510D

#define MSG_IODLG_NEWDIR			0xFFFF510E
#define MSG_IOMENU_RENAME			0xFFFF510F

#define MSG_TREEVIEW_ITEM			0xFFFF5110
#define MSG_TREEVIEW_ITEM_MENU		0xFFFF5111
#define MSG_TREEVIEW_EXPEND			0xFFFF5112

/**
 * Flags for IOBox()
 */
#define IOBOX_SAVE					0x00000001
#define IOBOX_OPEN					0x00000002

l_text IOBox ( l_text Title, l_ulong Flags, l_text *Dir, PFileTypes Types, l_bool FExists );
PDirview CreateDirview ( PApplication App, TRect r, l_text dir, PList Extentions, l_ulong Flags );
void DirviewOpenDirectory ( PDirview o, l_text dir );
void TreeDirViewOpenDir ( PTreeDirview o, l_text dir );
PList GenerateFileRefs ( PFileInfo f );
PList CollectFilesFromRef ( PList l );
void CopyFilesFromList ( PList l, l_text Dest );
void DeleteFilesFromList ( PList l );

void FreeFileTypes ( PFileTypes o );
PFileTypes NewFileTypes ( l_text Title, l_text txtexts, PFileTypes Next );

typedef struct TTreeDirview
{
	struct	TTreeView o;

	l_ulong Msg;

	PDirview Linked;

} TTreeDirview;

typedef struct TTreeDirviewItem *PTreeDirviewItem;
typedef struct TTreeDirviewItem
{
	struct TTreeItem o;

	PFileInfo f;

} TTreeDirviewItem;


#define TREEDIRVIEW(o) ((PTreeDirview)(o))
#define TREEDIRVIEWITEM(o) ((PTreeDirviewItem)(o))

PTreeDirview CreateTreeDirview ( PApplication App, TRect r );

#endif /* _IODLG_H_INCLUDED_ */
