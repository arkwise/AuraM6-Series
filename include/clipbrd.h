////////////////////////////////////////////////////////////////////////////////
//
//  Clipboard - Header File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _CLIPBOARD_H_INCLUDED_
#define _CLIPBOARD_H_INCLUDED_

typedef struct TClipboardItem {
	
	l_ulong 	Type;
	void 		 *Data;
	
	void 	*(*GetData) ( void *Arg, l_ulong Mode );
	
} TClipboardItem, *PClipboardItem;

void ClipboardFree ( void );
void *ClipboardGet ( l_ulong Type );
void 	ClipboardPut ( l_ulong Type, void *Data, l_bool Duplicate );


#endif /* _CLIPBOARD_H_INCLUDED_ */
