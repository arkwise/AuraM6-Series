/**
*       Copyright (c) 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*       FILE:                   vfile.c
*
*       PROJECT:                Phoenix engine - Core
*
*       DESCRIPTION:    This file contains virtual filesystem functions and its
*                                               implementation.
*
*       CONTRIBUTORS:
*                                       Lukas Lipka
*                                       Etelain Julien
*
*       TODO:                   @ Add support for Linux filesystem (EXT2)
*                                       @ Add drive recognition code (for eg. hdd, fdd, cdrom)
*
*       BUG:                    @
*
*       MISTAKE:                @ Drive B:\ is recognized during loading even if it doesnt exist,
*                                               so it has to be skipped.
*
*       IDEA:                   @
*/

/** @defgroup vfile Virtual File System.
        This module do an abstraction of local systelm filesystem to ensure platforms compatibilty.
        Most standart functions are provided but under an unuasual name. (fread becomes FileRead)
        @{
*/
/**     @file
        VFile core file
*/

#include "kernel.h"
#include <stdio.h>
#include <stdarg.h>
#include "internal.h"
#include "string.h"
#include "types.h"
#include "debug.h"


#define TYPE_HDD                        0xF0000001
#define TYPE_FDD                        0xF0000002
#define TYPE_CDROM                      0xF0000004

PList LocalDrives = NULL;


#ifdef _OZONE__LINUX_
extern PFileDriver LinuxFileDriver;
#define SysFileDriver LinuxFileDriver
#else
extern PFileDriver MsDosFileDriver;
#define SysFileDriver MsDosFileDriver
#endif

extern PFileDriver RootFileDriver;

PDrive DefDrive = NULL;
PDrive RootDrive = NULL;

#ifdef _OZONE__LINUX_
#include <sys/stat.h>
l_long filelength( l_text name )
{
    struct stat status;
    if (stat(name, &status)==-1) return 0;
    return status.st_size;
}
#endif

void FreeFileDriver ( PFileDriver o ) {
        if ( o->DriverName ) free(o->DriverName);
        free(o);
}


l_bool file_read_char ( PFile f, l_char *o )
{
        if (FileRead(o, 1, 1, f) != 1) return false;
        return true;
}

l_bool file_read_short ( PFile f, l_short *o )
{
        if (FileRead(o,2,1,f) != 1) return false;
        return true;
}

l_bool file_read_long ( PFile f, l_long *o )
{
        if (FileRead(o,4,1,f) != 1) return false;
        return true;
}

l_bool file_write_char ( PFile f, l_char o )
{
        if (FileWrite(&o,1,1,f) != 1) return false;
        return true;
}

l_bool file_write_short ( PFile f, l_short o )
{
        if (FileWrite(&o,2,1,f) != 1) return false;
        return true;
}

l_bool file_write_long ( PFile f, l_long o )
{
        if (FileWrite(&o,4,1,f) != 1) return false;
        return true;
}

l_int file_get_char ( PFile f )
{
        l_uchar o;

        if ( FileRead(&o, 1, 1, f) != 1 ) return EOF;

        return o;
}

l_text ReadString ( FILE *f )
{
        l_ushort length = 0;
        l_text  s       = 0;

        fread(&length, 1, 2, f);
        s = (l_text)malloc(length+1);
        if ( !s ) return NULL;
        fread(s, 1, length, f);
        s[length] = 0;

        return s;
}

l_bool WriteString ( FILE *f, l_text s )
{
        l_ushort length = TextLen(s);

        fwrite(&length, 1, 2, f);
        fwrite(s, 1, length, f);
        return 1;
}

/**
        Reads a long in motorola format from a file stream
        @param f File stream
        @return the value or EOF if end of file is reached
*/
l_long file_get_mlong ( PFile f )
{
        l_int b1, b2, b3, b4;

        if ((b1 = file_get_char(f)) != EOF)
                if ((b2 = file_get_char(f)) != EOF)
                        if ((b3 = file_get_char(f)) != EOF)
                                if ((b4 = file_get_char(f)) != EOF)
                                        return (((long)b1 << 24) | ((long)b2 << 16) | ((long)b3 << 8) | (long)b4);

        return EOF;
}

//#ifdef _OZONE__DJGPP_

l_text FileNameToPath ( l_text Dir, l_text File ) {
        if ( !DCkPt("FileNameToPath.Dir",Dir) ) return TextDup(File);
        {
        l_int s = TextLen(Dir);
        if ( Dir[s-1] == '/' ||  Dir[s-1] == '\\' )
                return TextArgs("%s%s",Dir,File);
        else
                return TextArgs("%s/%s",Dir,File);
        }
}

void RemoveDrive ( PDrive o ) {
        PListItem i = ListFoundItem(LocalDrives,o);

        if ( i )
                ListRemoveItem(LocalDrives,i);
}


PDrive AddDrive ( l_text Name,l_uchar Type, void *Ex1, void *Ex2, PFileDriver Driver ) {
        PDrive o = malloc(sizeof(TDrive));
        if (!o) return NULL;
        memset(o,0,sizeof(TDrive));


        o->Name = TextDup(Name);
        o->Type = Type;
        o->Ex1 = Ex1;
        o->Ex2 = Ex2;
        o->Driver = Driver;

        if (o->Driver->RefreshDriveInfo ) o->Driver->RefreshDriveInfo(o);

        ListAdd(LocalDrives,Name,o,(void*)&FreeDrive);

        return o;
}
/**
        Free a File Information.
        @param o FileInfo to free
        @see FileGetInfo
*/
void FreeFileInfo ( PFileInfo o ) {
        if ( o->Name ) free(o->Name);
        if ( o->VPathName ) free(o->VPathName);
        if ( o->LocalName ) free(o->LocalName);
        free(o);
}

void FreeDrive ( PDrive o ) {
        if ( o->Name ) free(o->Name);
        free(o);
}

PDrive FoundDrive (l_text File, l_text *Out ) {
        l_text O = File;
        if ( !TextNCaseCompare(File,"local://",8 ) ) File += 7;

        if ( *File == '/' || *File == '\\' ) {

                l_text F,d;
                PDrive Drv;

                File++;
                F = File;
                while ( *File ) {
                        if ( *File == '/' || *File == '\\' ) break;
                  File++;
                }

                if ( File == F ) {
                        (*Out) = TextDup(O+1);
                  return RootDrive;
                }


                d = TextNDup(F,File-F);
                if ( *File == '/' || *File == '\\' )File++;
                //DebugMessage(" %s => '%s' '%s' ",O,d,File);

                Drv = ListKey(LocalDrives,d);

                if ( Drv ) {
                        (*Out) = TextDup(File);
                        //DebugMessage("    => Use drive '%s' with '%s' driver ",Drv->Name,Drv->Driver->DriverName);

                        return Drv;

                } else {

                        (*Out) = TextDup(O+1);
                  return RootDrive;
                }


        }

        *Out = TextDup(File);
        return DefDrive;
}

/**
        List a directory.
        @param Dir Name of directory to list
        @param Attrib Filter by attributes
        @return Return listing as a PList of PFileInfo
*/
PList ListDir ( l_text Dir, l_uchar Attrib ) {
        l_text LFile = NULL;
        PList  l = NULL;
        PDrive Drv = FoundDrive(Dir,&LFile);
        if ( Drv->Driver->List ) l = Drv->Driver->List(Drv,Dir,LFile,Attrib);
        free(LFile);
        return l;
}

/**
        List a directory.
        @param fi PFileInfo of the directory
        @param Attrib Filter by attributes
        @return Return listing as a PList of PFileInfo
*/
PList ListDir2 ( PFileInfo fi, l_uchar Attrib ) {
        if ( fi->Drive->Driver->List )
                return fi->Drive->Driver->List(fi->Drive,fi->VPathName,fi->LocalName,Attrib);
        return NULL;
}

#ifdef _OZONE__LINUX_
void InitLinuxFileDriver ( void );
#define InitSysFileDriver InitLinuxFileDriver

#else
void InitMsDosFileDriver ( void );
#define InitSysFileDriver InitMsDosFileDriver
#endif


void InitRootFileDriver ( void );

void DebugListDir ( l_text Dir ) {
        PListItem a, b;
        PList l = ListDir(Dir,FA_DIREC);

        if ( !l ) return;
        if ( !l->Last ) return;

         DebugMessage("-> %s",Dir);

                a = b = l->Last->Next;

        if ( a ) do {

                DebugMessage("%s %s",FILEINFO(a->Data)->Attributes & FA_DIREC ? "[Dir]":"     ",FILEINFO(a->Data)->Name);

                a = a->Next;
        } while ( a != b );

        FreeList(l);
}
////////////////////////////////////////////////////////////////////////////////
l_bool FileRename ( l_text File, l_text NewName ) {
        l_text LFile1 = NULL;
        l_text LFile2 = NULL;
        l_bool  r = false;
        PDrive Drv1 = FoundDrive(File,&LFile1);
        PDrive Drv2 = FoundDrive(NewName,&LFile2);
        if ( Drv1 != Drv2 ) { DebugError("Different drivers %s %s",File,NewName); return false; }
        if ( Drv1->Driver->FileRename ) r = Drv1->Driver->FileRename(Drv1,LFile1,LFile2);
        free(LFile1);
        free(LFile2);
        return r;
}
////////////////////////////////////////////////////////////////////////////////
l_bool DirRename ( l_text File, l_text NewName ) {
        l_text LFile1 = NULL;
        l_text LFile2 = NULL;
        l_bool  r = false;
        PDrive Drv1 = FoundDrive(File,&LFile1);
        PDrive Drv2 = FoundDrive(NewName,&LFile2);
        if ( Drv1 != Drv2 ) { DebugError("Different drivers %s %s",File,NewName); return false; }
        if ( Drv1->Driver->DirRename ) r = Drv1->Driver->DirRename(Drv1,LFile1,LFile2);
        free(LFile1);
        free(LFile2);
        return r;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Open a file stream
        @param File Name of the file to open
        @param Mode Filestream mode
        @return NULL if file not found (read), or enable to create file (write) or the filestream
*/
PFile FileOpen ( l_text File, l_text Mode ) {
        l_text LFile = NULL;
        PFile  f = NULL;
        PDrive Drv = FoundDrive(File,&LFile);
        if ( Drv->Driver->FileOpen ) f = Drv->Driver->FileOpen(Drv,LFile,Mode);
        free(LFile);
        return f;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Open a file stream from of FileInfo reference
        @param fi FileInfo reference
        @param Mode Filestream mode
        @return NULL if file not found (read), or enable to create file (write) or the filestream
*/
PFile FileOpen2 ( PFileInfo fi, l_text Mode ) {
        if ( fi->Drive->Driver->FileOpen )
                return fi->Drive->Driver->FileOpen(fi->Drive,fi->LocalName,Mode);
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Close a file stream
        @param f File stream
*/
void FileClose ( PFile f ) {
        f->Drive->Driver->FileClose(f);
}
////////////////////////////////////////////////////////////////////////////////
/**
        Determines if pointer is at the end of the file
        @param f File stream
        @return true if pointer is at the end of the file, else false
*/
l_int FileEOF ( PFile f ) {
        return f->Drive->Driver->FileEOF(f);
}
////////////////////////////////////////////////////////////////////////////////
/**
        Read data from a file stream and store it to memory. Reads n*s bytes.
        @param o Pointer to the memory to store data
        @param s Size of a block
        @param n Number of blocks to read
        @param f File stream
        @return Number of bytes read, of EOF if end of file is reached
*/
l_int FileRead( void *o, l_ulong n, l_ulong s, PFile f ) {
        return f->Drive->Driver->FileRead(f,o,n*s);
}
////////////////////////////////////////////////////////////////////////////////
/**
        Write data to a file stream from memory. Write n*s bytes.
        @param o Pointer to the data to write
        @param s Size of a block
        @param n Number of blocks to write
        @param f File stream
        @return Number of bytes written
*/
l_int FileWrite( void *o, l_ulong n, l_ulong s, PFile f ) {
        return f->Drive->Driver->FileWrite(f,o,n*s);
}
////////////////////////////////////////////////////////////////////////////////
l_int FileSetPos( PFile f, l_ulong *Pos ) {
        return f->Drive->Driver->FileSeek(f,SEEK_SET,*Pos);
}
////////////////////////////////////////////////////////////////////////////////
l_int FileSeek( PFile f, l_long Pos, l_int Type ) {
        return f->Drive->Driver->FileSeek(f,Type,Pos);
}
////////////////////////////////////////////////////////////////////////////////
l_int FileGetPos( PFile f, l_ulong *Pos ) {
        return f->Drive->Driver->FileGetPos(f,Pos);
}
////////////////////////////////////////////////////////////////////////////////
l_ulong FileLength ( PFile f ) {
        return f->FileSize;
}
////////////////////////////////////////////////////////////////////////////////
// Ported form djgpp's fgets, Copyright (C) 1994 DJ Delorie
/**
        Read a line form a file buffer.
        Stops if buffer is full or when a end of line or end of file is reached.
        @param s Pointer to the place to store data (buffer)
        @param n Size of the reserved place (size of buffer)
        @param f File stream
        @return s when a line is read, return NULL when end of file is reached and that no bytes have been stored.
*/
l_text FileGets ( l_text s, l_int n, PFile f ) {
        l_int    c = 0;
        l_char *cs = s;
        while ( --n > 0 && (c = file_get_char(f)) != EOF ) {
                *cs++ = c;
                if ( c == '\n' || c == '\r' ) break;
        }
        if ( c == EOF && cs == s ) return NULL;
        if ( c == '\n' || c == '\r' ) cs--;
        *cs = '\0';
        return s;
}
////////////////////////////////////////////////////////////////////////////////
// Text length must be smaller than 64k
/**
        Write a string to a file stream in binary mode. The string can be read by FileReadBinString.
        @param f File stream
        @param Txt Pointer to the text to write
        @return true on sucess, or false on error
        @see FileReadBinString
*/
l_bool FileWriteBinString ( PFile f, l_text Txt ) {
  l_ushort s = TextLen(Txt);
  FileWrite(&s,1,2,f);
  FileWrite(Txt,1,s,f);
        return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Read a string from a file stream in binary mode written by FileWriteBinString.
        @param f File stream
        @return A pointer to the text (new allocation), or NULL on error
        @note Don't forget to free the returned data
        @see FileWriteBinString
*/
l_text FileReadBinString ( PFile f ) {
        l_ushort length = 0;
        l_text  s       = 0;

        FileRead(&length, 1, 2, f);
        s = (l_text)malloc(length+1);
        if ( !s ) return NULL;
        FileRead(s, 1, length, f);
        s[length] = 0;
        return s;
}
////////////////////////////////////////////////////////////////////////////////
l_bool FileWriteTextString ( PFile f, l_text Txt ) {
  FileWrite(Txt,1,TextLen(Txt),f);
        return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool FilePrintf ( PFile f, l_text Format, ... ) {
        l_char Buffer[10240];
        va_list argptr;
        va_start(argptr, Format);
        vsprintf(Buffer, Format, argptr);
        va_end(argptr);
        FileWriteTextString(f,Buffer);
        return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Get a File Information from a file.
        @param File Name of the file
        @return NULL if file is not found, else return the File Info
*/
PFileInfo FileGetInfo ( l_text File ) {
        l_text LFile = NULL;
        PFileInfo  i = NULL;
        PDrive Drv = FoundDrive(File,&LFile);
        if ( Drv->Driver->FileGetInfo ) i = Drv->Driver->FileGetInfo(Drv,File,LFile);
        free(LFile);
        return i;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Change a file attributes
        @param File Name of the file
        @param Attrib Attributes to set to the file
        @return true on success, else false
*/
l_bool FileSetAttributes ( l_text File, l_uchar Attrib ) {
        l_text LFile = NULL;
        PDrive Drv = FoundDrive(File,&LFile);
        l_bool i;
        if ( Drv->Driver->FileSetAttributes ) i = Drv->Driver->FileSetAttributes(Drv,LFile,Attrib);
        free(LFile);
        return i;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Determines the parent of a path.
        Path should be an absolute one (exemple : "/c/my files/stuff")
        @param dir Path
        @return the path to the parent (new allocation), or NULL on error
        @note Don't forget to free the returned text.
*/
l_text GetPathParent ( l_text dir ) {
        l_text a = dir;
        l_text e = 0, oe = 0;
        while ( *a ) {
                if ( *a == '/' ) { oe = e; e = a; }
                a++;
        }
        if ( a-1 == e  )
                e = oe;

        if ( e && e != dir )
                return TextNDup(dir,e-dir);
        else
                return TextDup("/");
}
////////////////////////////////////////////////////////////////////////////////
PFileInfo AddFileInfoToList ( PList l, l_text  Name, l_text  VDir, l_uchar Attributes,
                                                                                                        l_uchar Type,l_ulong MTime,l_ulong ATime,l_ulong CTime,
                                                                                                        l_ulong Size,PDrive Drive, void *FMS, void *Ex ) {
        PFileInfo o = malloc(sizeof(TFileInfo));
        if ( !o ) return NULL;
        memset(o,0,sizeof(TFileInfo));

        o->Name = TextDup(Name);
        o->VPathName = FileNameToPath(VDir,Name);;
        o->Attributes = Attributes;
        o->Type = Type;
        o->MTime = MTime;
        o->ATime = ATime;
        o->CTime = CTime;
        o->Size = Size;
        o->Drive = Drive;
        o->FMS = FMS;
        o->Ex = Ex;
        ListAdd(l,Name,o,(void*)&FreeFileInfo);
        return o;
}
////////////////////////////////////////////////////////////////////////////////
PFileRef NewFileReference ( PFileInfo f, l_text SName ) {
        PFileRef o = malloc(sizeof(TFileRef));
        if ( !o ) return NULL;
        memset(o,0,sizeof(TFileRef));

        o->Drive                        = f->Drive;
        o->Size                         = f->Size;
        o->Attributes   = f->Attributes;
        o->LocalName    = TextDup(f->LocalName);
        o->SName                        = TextDup(SName);

        return o;
}
////////////////////////////////////////////////////////////////////////////////
void FreeFileRef ( PFileRef o ) {

        if ( o->LocalName )     free(o->LocalName);
        if ( o->SName )                         free(o->SName);
        free(o);
}
////////////////////////////////////////////////////////////////////////////////
l_bool _GenerateFileRefs ( PList Lst, PFileInfo f, l_text SDir, l_bool (*CallBack) ( void*,l_text ), void* id ) {
        l_text F = FileNameToPath(SDir,f->Name);
        PFileRef o;
        if ( CallBack ) if ( !CallBack(id,F) ) return false;

        o = NewFileReference(f,F);

        if ( !o ) return false;

        ListAdd(Lst,NULL,o,(void*)&FreeFileRef);
        if ( f->Attributes & FA_DIREC ) {
                PList dir = ListDir(f->VPathName,FA_RDONLY|FA_HIDDEN|FA_SYSTEM|FA_LABEL|FA_DIREC|FA_ARCH);
                if ( dir ) {
                        if ( dir->Last ) {
                                PListItem a = dir->Last->Next;
                                PListItem b = a;
                                do {
                                        if ( !_GenerateFileRefs(Lst,a->Data,F,CallBack,id) ) return false;
                                        a = a->Next;
                                } while ( a != b);
                        }
                        FreeList(dir);
                }
        }
        free(F);

        return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool _GenerateFileRefs2 ( PList Lst, PFileRef f, l_text SDir, l_bool (*CallBack) ( void*,l_text ), void* id ) {
        l_text F = FileNameToPath(SDir,f->SName);

//DebugMessage("Start CallBack");
        if ( CallBack ) if ( !CallBack(id,F) ) return false;
//DebugMessage("CallBack returned true");

        ListAdd(Lst,NULL,DuplicateFileRef(f),(void*)&FreeFileRef);
        if ( f->Attributes & FA_DIREC ) {
                PList dir = NULL;
                if ( f->Drive->Driver->List ) dir = f->Drive->Driver->List(f->Drive,f->LocalName,f->LocalName,FA_RDONLY|FA_HIDDEN|FA_SYSTEM|FA_LABEL|FA_DIREC|FA_ARCH);
                if ( dir ) {
                        if ( dir->Last ) {
                                PListItem a = dir->Last->Next;
                                PListItem b = a;
                                do {
                                        if ( !_GenerateFileRefs(Lst,a->Data,F,CallBack,id) ) return false;
                                        a = a->Next;
                                } while ( a != b);
                        }
                        FreeList(dir);
                }
        }
        free(F);

        return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
        Creates a directory
        @param Path Path of the directory to create
        @return true on success or false on error
*/
l_bool MakeDir ( l_text Path ) {
        l_text LFile = NULL;
        l_bool  i = 0;
        PDrive Drv = FoundDrive(Path,&LFile);
        if ( Drv->Driver->DirMake ) i = Drv->Driver->DirMake(Drv,LFile);
        free(LFile);
        return i;
}
////////////////////////////////////////////////////////////////////////////////
l_text FileVPathName ( PDrive D, l_text LocalName ) {

        if ( D == DefDrive ) return TextArgs("/system/%s",LocalName);
        if ( D == RootDrive ) return TextArgs("/%s",LocalName);
        return TextArgs("/%s/%s",D->Name,LocalName);
}
////////////////////////////////////////////////////////////////////////////////
l_text _ResolveFileName ( l_text File ) {

        l_text  t = File, // Current char
                        l = NULL, // Last token
                        c = File; // Current token

        do {
                if ( (*t == '/') || !*t ) {
                        /*if ( !(t-c) && l ) {
                                l_text n = TextDup(t);
                                free(File);
                                return _ResolveFileName(n);
                        } else */if ( !TextSqNCaseCompare(c,"..",t-c) ) {
                                if ( l ) {
                                        l_text n = TextDup(File);
                                        TextCopy(n+(l-File),n+(t+(*t?1:0)-File));
                                        free(File);
                                        return _ResolveFileName(n);
                                } else {
                                        l_text n = TextDup(File);
                                        TextCopy(n+(c-File),n+(t+(*t?1:0)-File));
                                        free(File);
                                        return _ResolveFileName(n);
                                }
                        } else if ( !TextSqNCaseCompare(c,".",t-c) ) {
                                l_text n = TextDup(File);
                                TextCopy(n+(c-File),n+(t+(*t?1:0)-File));
                                free(File);
                                return _ResolveFileName(n);
                        }

                        if ( t != File ) l = c;
                        c = t+1;
                }

                //t++;
        } while ( *(t++) );
        return File;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Clean a path. Removes and solve the "/./", "/../", "//"
        @param File Path to clean
        @return the cleaned path (new allocation)
        @note don't forget to free the returned text
*/
l_text ResolveFileName ( l_text File ) {

        return _ResolveFileName(TextDup(File));

}
////////////////////////////////////////////////////////////////////////////////
l_text RelativeName ( l_text Dir, l_text AbsFile ) {
        l_text dt = Dir, ldt = Dir, ft = AbsFile, lft = AbsFile, a;
        l_text Rel = NULL ;

        do {
                dt = TextChr(dt+1,'/');
                ft = TextChr(ft+1,'/');
                if ( !dt || !ft ) {
                        dt = ldt;
                        ft = ldt;
                        break;
                }
                if ( TextNCaseCompare(Dir,AbsFile,dt-Dir) )
                        break;
                lft = ft;
                ldt = dt;
        } while ( 1 );


        if ( dt ) {

                ldt = dt;
                while ( dt = TextChr(dt+1,'/') ) {
                        if ( Rel ) {
                                a = Rel;
                                Rel = TextArgs("%s/..",Rel);
                                free(a);
                        } else
                                Rel = TextDup("..");
                        ldt = dt;
                }
                if ( TextLen(ldt) > 1 ) {
                        if ( Rel ) {
                                a = Rel;
                                Rel = TextArgs("%s/..",Rel);
                                free(a);
                        } else
                                Rel = TextDup("..");
                }
        }

        if ( Rel ) {
                a = Rel;
                Rel = TextArgs("%s/%s",Rel,ft+1);
                free(a);
        } else
                Rel = TextDup(ft+1);
        return Rel;
}

////////////////////////////////////////////////////////////////////////////////

/**
*       NAME: GSSystemVFSInstall
*       DESCRIPTION: Installs the VFS, is called during kernel setup.
*/
int GSSystemVFSInstall ( void )
{
        LocalDrives = NewList();
        InitSysFileDriver();
        InitRootFileDriver();
#ifdef _OZONE__DJGPP_
        {
                l_int d = 0;

                while ( d < 27 )
                {
                        if ( _is_remote_drive(d) != -1 )
                        {
                                if ( d != 1 )
                                {
                                        DebugMessage("Drive %c:\\ mapped as /%c/", 'a'+d,'a'+d);

                                        AddDrive(TextArgs("%c",'a'+d),FT_HDD,TextArgs("%c:/",'a'+d),NULL,MsDosFileDriver);
                                }
                        }
                        d++;
                }

                if ( !d )
                        DebugError("MS-DOS VFile :: No drives found!");
        }
#endif

#ifdef _OZONE__WIN32_
        {
                l_int d = 0;
                l_ulong uDriveMask = _getdrives();

                if (uDriveMask == 0)
                        DebugError("WIN32 VFile :: No drives found!");
                else {
                        while (uDriveMask) {
                                if (uDriveMask & 1) {
                                        DebugMessage("Drive %c:\\ mapped as /%c/", 'a'+d,'a'+d);
                                        AddDrive(TextArgs("%c",'a'+d),FT_HDD,TextArgs("%c:/",'a'+d),NULL,MsDosFileDriver);
                                }

                                d++;
                                uDriveMask >>= 1;
                        }
                }
        }
#endif

        DefDrive = malloc(sizeof(TDrive));
        memset(DefDrive,0,sizeof(TDrive));

        DefDrive->Name = TextDup("&__basic");
        DefDrive->Type = FT_FOLDER;
        DefDrive->Ex1 = TextDup("./");
        DefDrive->Ex2 = NULL;
        DefDrive->Driver = SysFileDriver;

        RootDrive = malloc(sizeof(TDrive));
        memset(RootDrive,0,sizeof(TDrive));

        RootDrive->Name = TextDup("");
        RootDrive->Type = FT_FOLDER;
        RootDrive->Ex1 = TextDup("./SYSTEM/ROOT/");
        RootDrive->Ex2 = NULL;
        RootDrive->Driver = RootFileDriver;

        AddDrive("system",FT_FOLDER,".",NULL,SysFileDriver);
        AddDrive("xapps",FT_FOLDER,"./xapps/",NULL,SysFileDriver);
#ifdef _OZONE__LINUX_

        AddDrive("local",FT_HDD,TextDup(""),NULL,LinuxFileDriver);
#endif

        return 0;
}


void GSSystemVFSUnInstall ( void ) {

        FreeList(LocalDrives);
        FreeFileDriver(SysFileDriver);
        FreeFileDriver(RootFileDriver);

}
