////////////////////////////////////////////////////////////////////////////////
//
//  File Input/Output - Header File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _vfile_included_
#define _vfile_included_

typedef struct TFileInfo *PFileInfo;
typedef struct TFile *PFile;
typedef struct TDrive *PDrive;

l_bool file_read_char   ( PFile f, l_char *o );
l_bool file_read_short  ( PFile f, l_short *o );
l_bool file_read_long   ( PFile f, l_long *o );
l_bool file_write_char  ( PFile f, l_char o );
l_bool file_write_short ( PFile f, l_short o );
l_bool file_write_long  ( PFile f, l_long o );
l_int  file_get_char    ( PFile f );
l_long file_get_mlong   ( PFile f );

l_bool WriteString ( FILE *f, l_text s );
l_text ReadString ( FILE *f );

l_text GSVFSResolvePath( l_text szPath );
FILE* GSFileOpen ( l_text szPath, l_text szMode );
void GSFileClose ( FILE* fFile );
int GSSystemVFSInstall ( void );


typedef struct TFileDriver {

	l_text DriverName;

	PFile   (*FileOpen)( PDrive d, l_text File, l_text Mode  );
	void    (*FileClose)( PFile f );

	l_int   (*FileRead)( PFile f, void *d, l_ulong s );
	l_int   (*FileWrite)( PFile f, void *d, l_ulong s );
	l_int	(*FileGetPos)( PFile f, l_ulong *Pos );
	l_int	(*FileSeek)( PFile f, l_int Type, l_long Pos );

	l_int	(*FileEOF)( PFile f );
	
	
	l_bool  (*FileDelete)( PDrive d, l_text File );
	l_bool  (*FileRename)( PDrive d, l_text File, l_text NewName );
	PFileInfo  (*FileGetInfo)( PDrive d, l_text VFile, l_text File );

	l_int	(*FileFlush)( PFile f );

	l_bool  (*DirMake)( PDrive d, l_text Dir );
	l_bool  (*DirDelete)( PDrive d, l_text Dir );
	l_bool  (*DirRename)( PDrive d, l_text Dir, l_text NewName );

	PList   (*List)( PDrive d, l_text VDir, l_text Dir, l_uchar Attrib );

	void	(*RefreshDriveInfo)(PDrive);

	l_bool	(*FileSetAttributes)( PDrive d, l_text File, l_uchar Attrib );


} TFileDriver, *PFileDriver;

#define VDRIVE(o) ((PDrive)(o))
typedef struct TDrive {

	l_text Name;

	l_ulong TotalSpace;
	l_ulong UsedSpace;
	l_uchar Type;

	void *Ex1;
	void *Ex2;

	PFileDriver Driver;

} TDrive;

#define FILEINFO(o) ((PFileInfo)(o))
typedef struct TFileInfo
{
	l_text  Name;
	l_text  VPathName;
	l_text  LocalName;
	l_uchar Attributes;
	l_uchar Type;
	l_ulong MTime;
	l_ulong ATime;
	l_ulong CTime;

	l_ulong Size;

	PDrive Drive;

	void *FMS; // If set, point to manager needed by this file.
	void *Ex;
} TFileInfo;

typedef struct TFileRef {
	
	l_text 		SName; 
	l_text  	LocalName;
	l_uchar 	Attributes;
	l_ulong 	Size;
	PDrive 		Drive;
	
} TFileRef, *PFileRef;

#define FILEREF(o) ((PFileRef)(o))

#ifndef FA_RDONLY
	#define FA_RDONLY       1  // Read Only
	#define FA_HIDDEN       2  // Hidden
	#define FA_SYSTEM       4  // System
	#define FA_LABEL        8  // Label : Drive i suppose
	#define FA_DIREC        16 // Directory
	#define FA_ARCH         32 // Archive
#endif /* FA_RDONLY */


#define FT_FILE    0
#define FT_FOLDER  1
#define FT_LFLOPPY 2
#define FT_FLOPPY  3
#define FT_TAPE    4
#define FT_HDD     5
#define FT_CD      6
#define FT_DVD     7
#define FT_RAM     8
#define FT_CARD    9
#define FT_NETWK   10
#define FT_ZIP     11
#define FT_VFILE   128

typedef struct TFile {



	PDrive Drive;

	void *Ex1;
	void *Ex2;

	l_ulong  FileSize;
	l_ulong  BufferSize;
	l_ulong  BufferPos;
	l_ulong  Pos;
	l_uchar *Buffer;

	l_ulong Flags;

} TFile;


l_text FileNameToPath ( l_text Dir, l_text File );

void FreeDrive ( PDrive o );

void FreeFileInfo ( PFileInfo o );

#define FSF_READ 0x01
#define FSF_WRITE 0x02

PList ListDir ( l_text Dir, l_uchar Attrib );
PFile FileOpen ( l_text File, l_text Mode );
PFile FileOpen2 ( PFileInfo fi, l_text Mode );
void FileClose ( PFile f );
l_int FileRead( void *o, l_ulong n, l_ulong s, PFile f );
l_int FileWrite( void *o, l_ulong n, l_ulong s, PFile f );
l_int FileSetPos( PFile f, l_ulong *Pos );
l_int FileSeek( PFile f, l_long Pos, l_int Type );
l_int FileGetPos( PFile f, l_ulong *Pos );
l_ulong FileLength ( PFile f );
l_bool FileWriteBinString ( PFile f, l_text Txt );
l_text FileReadBinString ( PFile f );
l_bool FileWriteTextString ( PFile f, l_text Txt );
l_bool FilePrintf ( PFile f, l_text Format, ... );

l_text FileNameToPath ( l_text Dir, l_text File );
l_text GetPathParent ( l_text dir );
PFileInfo FileGetInfo ( l_text File );

void FreeFileInfo ( PFileInfo o );
l_bool FileSetAttributes ( l_text File, l_uchar Attrib );


PFileInfo AddFileInfoToList ( PList l, l_text  Name, l_text  VDir, l_uchar Attributes,
													l_uchar Type,l_ulong MTime,l_ulong ATime,l_ulong CTime,
													l_ulong Size,PDrive Drive, void *FMS, void *Ex );

PDrive AddDrive ( l_text Name,l_uchar Type, void *Ex1, void *Ex2,PFileDriver Driver );

void FreeFileRef ( PFileRef o );
PFileRef NewFileReference ( PFileInfo f, l_text SName );
l_bool _GenerateFileRefs ( PList Lst, PFileInfo f, l_text SDir, l_bool (*CallBack) ( void*,l_text ), void* id );
PList ListDir2 ( PFileInfo fi, l_uchar Attrib );
	l_bool _GenerateFileRefs2 ( PList Lst, PFileRef f, l_text SDir, l_bool (*CallBack) ( void*,l_text ), void* id );
l_bool MakeDir ( l_text Path );
l_int FileEOF ( PFile f );
l_text FileVPathName ( PDrive D, l_text LocalName );
l_bool FileRename ( l_text File, l_text NewName );
l_bool DirRename ( l_text File, l_text NewName );

l_text FileGets ( l_text s, l_int n, PFile f );
l_text ResolveFileName ( l_text File );
l_text RelativeName ( l_text Dir, l_text AbsFile );

void RemoveDrive ( PDrive o );
void GSSystemVFSUnInstall ( void );

#endif /* _vfile_included_ */
