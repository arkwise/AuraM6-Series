////////////////////////////////////////////////////////////////////////////////
//
//	File Management System v2 - Header file
//
//	See also types.h for definitions
//
//	(c) Copyright 2004 Point Mad. All rights reserved.
//
//	Documentation: docs/dms-fms-v2-draft.txt
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _FMS2_H_INCLUDED_
#define _FMS2_H_INCLUDED_

typedef struct TDataTypeEx {

	l_ulong  TypeId;

	l_text   TypeName;
	
	PIcon Default;
	PIcon (*GetIcons) ( PFileInfo i );

	
	PList Actions;

	l_ulong Flags;
	
} TDataTypeEx, *PDataTypeEx;
	
#define DTEF_FREEICONS 0x01
#define DTEF_STRICTINTERNAL 0x02

#define DATATYPEEX(o) ((PDataTypeEx)(o))

typedef struct TDTAction  {


	l_ulong ActionMsg;
	l_text  Name;
	l_text  Runner;
	l_bool  (*Run) ( PFileInfo f, int argc, l_text *argv, PApplication Host, PErrorHandler h );

	l_ulong FavoriteWeight;
	
} TDTAction, *PDTAction;

#define DTACTION(o) ((PDTAction)(o))

extern PList DTEs;


l_bool FileHandleMsg ( PFileInfo f, l_ulong Msg  );
PMenu FileGetMenu2 ( PFileInfo f, l_ulong Flags );
p_bitmap FileGetIcon2 ( PFileInfo f, l_ulong Size, l_ulong *ToFree );
void FileGetIcons2 ( PFileInfo f, p_bitmap *i16,p_bitmap *i32,p_bitmap *i48,l_ulong *fi );

l_bool RunFile2 ( PFileInfo f, int argc, l_text *argv );
PDTAction AddDTAction ( PDataTypeEx dte, l_text Name,l_text Runner, l_bool(*Run)(PFileInfo,int argc, l_text *argv, PApplication Host, PErrorHandler),l_ulong FavoriteWeight );
PDataTypeEx AddDataTypeEx( l_ulong TypeId,l_text TypeName,p_bitmap Icon16,p_bitmap Icon32,p_bitmap Icon48,PIcon (*GetIcons) ( PFileInfo ),l_ulong Flags );
PDataTypeEx _GetDataTypeEx( l_ulong TypeId );
l_ulong GetTypeDefaultMsg ( l_ulong TypeId ) ;
l_bool ExecutePDAction ( PDTAction a, PFileInfo f, int argc, l_text *argv );
PIcon FileGetIconsEx ( PFileInfo f );
l_bool FileRun ( l_text file, l_text Args );

// Advanced run-times
// - Custom error handler, do not display error messages
// - Allow host application (console based apps)
l_bool ExecutePDAction2( PDTAction a, PFileInfo f, int argc, l_text *argv, PApplication Host, PErrorHandler h );
l_bool RunFile3 ( PFileInfo f, int argc, l_text *argv, PApplication Host, PErrorHandler h );
l_bool FileHandleMsg2 ( PFileInfo f, l_ulong Msg, PErrorHandler h  );

#endif
