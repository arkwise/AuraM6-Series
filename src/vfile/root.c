////////////////////////////////////////////////////////////////////////////////
//
//  Root Drive Driver
//
//      (c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////


#include "kernel.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <string.h>

PFileDriver RootFileDriver;

extern PList LocalDrives;

#ifdef _OZONE__LINUX_
extern PFileDriver LinuxFileDriver;
#define SysFileDriver LinuxFileDriver
#else
extern PFileDriver MsDosFileDriver;
#define SysFileDriver MsDosFileDriver
#endif
////////////////////////////////////////////////////////////////////////////////
PFileInfo  RootFileGetInfo ( PDrive d, l_text VFile, l_text File ) {
        PDrive Drv = ListKey(LocalDrives,File);

        if ( Drv ) {
                PFileInfo i = malloc(sizeof(TFileInfo));
                memset(i,0,sizeof(TFileInfo));
                i->Name = TextDup(Drv->Name);
                i->VPathName = TextDup(VFile);
                i->LocalName = TextDup(File);
                i->Attributes = FA_DIREC|FA_LABEL|FA_RDONLY;
                i->Type = Drv->Type;
                i->Size = Drv->TotalSpace;
                i->Drive = Drv;
                return i;

        } else
          return SysFileDriver->FileGetInfo(d,VFile,File);

}

////////////////////////////////////////////////////////////////////////////////
l_int FileInfoOrder ( PFileInfo a, PFileInfo b ) {

                return TextCaseCompare(a->Name,b->Name);

}
///////////////////////////////////////////////////////////////////////////////
PList RootList ( PDrive d, l_text VDir, l_text Dir, l_uchar Attrib ) {

        if ( !*Dir ) {
                PListItem a, b;
                PFileInfo i;
                PList l = SysFileDriver->List(d,VDir,Dir,Attrib);
                if ( !l ) l = NewList();

                Attrib |= FA_ARCH|FA_RDONLY;

        if ( !LocalDrives->Last || ( (FA_DIREC|FA_LABEL) & ~Attrib ) ) return l;

                a = b = LocalDrives->Last->Next;

        if ( a ) do {
                        i = malloc(sizeof(TFileInfo));
                        memset(i,0,sizeof(TFileInfo));
                        i->Name = TextDup(VDRIVE(a->Data)->Name);
                        i->VPathName = FileNameToPath(VDir,i->Name);
                        i->LocalName = FileNameToPath(Dir,i->Name);
                        i->Attributes = FA_DIREC|FA_LABEL|FA_RDONLY;
                        i->Type = VDRIVE(a->Data)->Type;
                        i->Size =VDRIVE(a->Data)->TotalSpace;
                        i->Drive = VDRIVE(a->Data);
                        ListAdd(l,VDRIVE(a->Data)->Name,i,(void*)&FreeFileInfo);
                a = a->Next;
        } while ( a != b );

        ListOrderByData(l,(void*)&FileInfoOrder);

                return l;

        } else
          return SysFileDriver->List(d,VDir,Dir,Attrib);

}
////////////////////////////////////////////////////////////////////////////////
void InitRootFileDriver ( void ) {

        RootFileDriver = malloc(sizeof(TFileDriver));
        RootFileDriver->DriverName = TextDup("Root drive driver");

        RootFileDriver->FileOpen   = SysFileDriver->FileOpen;
        RootFileDriver->FileClose  = SysFileDriver->FileClose;
        RootFileDriver->FileRead   = SysFileDriver->FileRead;
        RootFileDriver->FileWrite  = SysFileDriver->FileWrite;
        RootFileDriver->FileGetPos = SysFileDriver->FileGetPos;
        RootFileDriver->FileSeek = SysFileDriver->FileSeek;

        RootFileDriver->FileDelete = SysFileDriver->FileDelete;
        RootFileDriver->FileRename = SysFileDriver->FileRename;

        RootFileDriver->FileFlush = SysFileDriver->FileFlush;

        RootFileDriver->FileGetInfo = &RootFileGetInfo;
        RootFileDriver->DirMake = SysFileDriver->DirMake;
        RootFileDriver->DirDelete = SysFileDriver->DirDelete;
        RootFileDriver->DirRename = SysFileDriver->FileRename;
        RootFileDriver->List = &RootList;
        RootFileDriver->FileSetAttributes = SysFileDriver->FileSetAttributes;
        RootFileDriver->FileEOF = SysFileDriver->FileEOF;

        RootFileDriver->RefreshDriveInfo = NULL;
}
