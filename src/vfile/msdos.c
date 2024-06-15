////////////////////////////////////////////////////////////////////////////////
//
//  Codename Phoenix I/O to MS I/O layer driver
//
//      (c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////


#include "kernel.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include "string.h"

#ifdef _OZONE__WIN32_
#include<io.h>
#endif

PFileDriver MsDosFileDriver;
////////////////////////////////////////////////////////////////////////////////
#ifdef _OZONE__DJGPP_
////////////////////////////////////////////////////////////////////////////////
l_ulong MsDosTimeConvert( unsigned short date, unsigned short time ) {
        struct tm t;

        memset(&t,0,sizeof(t));

        t.tm_hour = (time >> 11) & 0x1f;
  t.tm_min  = (time >>  5) & 0x3f;
  t.tm_sec  = (time & 0x1f) * 2;

  t.tm_mon  = (date >>  5) & 0x0f;
  t.tm_mday = (date & 0x1f);
  t.tm_year = ((date >> 9) & 0x7f) + 80;

        return mktime(&t);
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
PFile MsDosFileOpen ( PDrive d, l_text File, l_text Mode ) {
        PFile o = malloc(sizeof(TFile));
        l_text RF = NULL;
        if ( !o ) return NULL;
        memset(o,0,sizeof(TFile));
        RF = FileNameToPath(d->Ex1,File);
        DebugMessage("Try to open %s (file: %s, Mode: %s)...",RF, File, Mode);
        o->Ex1 = fopen(RF,Mode);
        if ( !o->Ex1 ) { if (o) free(o); if (RF) free(RF); return NULL; }
        o->FileSize =  filelength(fileno((FILE*)o->Ex1));
        o->Drive = d;
        DebugMessage("Opened (Size: %d, adress of TFILE: %d)!",o->FileSize, &o);
//if (o->FileSize==0) return NULL;  // I HAVE TO REMOVE IT!!!
        return o;
}
////////////////////////////////////////////////////////////////////////////////

void MsDosFileClose  ( PFile f ) {
        if (f->Ex1) fclose(f->Ex1);
        if (f) free(f);
}
////////////////////////////////////////////////////////////////////////////////
l_int MsDosFileRead   ( PFile f, void *d, l_ulong s ) {
  return fread(d,1,s,(FILE*)f->Ex1);
}
////////////////////////////////////////////////////////////////////////////////
l_int MsDosFileWrite  ( PFile f, void *d, l_ulong s ){
  return fwrite(d,1,s,(FILE*)f->Ex1);
}
////////////////////////////////////////////////////////////////////////////////
l_int MsDosFileEOF  ( PFile f ){
  return feof((FILE*)f->Ex1);
}
////////////////////////////////////////////////////////////////////////////////
l_int MsDosFileGetPos ( PFile f, l_ulong *Pos ) {
#ifdef _OZONE__WIN32_
        __int64 p;
        int r;
        r = fgetpos((FILE*)f->Ex1,&p);
        *Pos = p;
        return r;
#endif
#ifdef _OZONE__DJGPP_
        return fgetpos((FILE*)f->Ex1,Pos);
#endif
}
////////////////////////////////////////////////////////////////////////////////
l_int  MsDosFileSeek ( PFile f, l_int Type, l_long Pos ) {
        return fseek((FILE*)f->Ex1,Pos,Type);
}
////////////////////////////////////////////////////////////////////////////////
l_bool  MsDosFileDelete ( PDrive d, l_text File ) {
        l_text RF = FileNameToPath(d->Ex1,File);
        l_bool r = false;
        if ( RF ) {
                if ( remove(RF) )
                r = true;
                free(RF);
        }
        return r;
}
////////////////////////////////////////////////////////////////////////////////

l_bool  MsDosFileRename ( PDrive d, l_text File, l_text NewName ) {
        l_text RF1 = FileNameToPath(d->Ex1,File);
        l_text RF2 = FileNameToPath(d->Ex1,NewName);
        l_bool r = false;
        if ( !RF1 || !RF2 ) return false;
        if ( !rename(RF1,RF2) ) r = true;

        free(RF1); free(RF2);
        return r;
}

////////////////////////////////////////////////////////////////////////////////
PFileInfo  MsDosFileGetInfo ( PDrive d, l_text VFile, l_text File ) {

        if ( !*File ) {
                PFileInfo i = malloc(sizeof(TFileInfo));
                memset(i,0,sizeof(TFileInfo));
                i->Name = TextDup(d->Name);
                i->VPathName = TextDup(VFile);
                i->LocalName = TextDup(File);
                i->Attributes = FA_DIREC|FA_LABEL|FA_RDONLY;
                i->Type = d->Type;
                i->Size = d->TotalSpace;
                i->Drive = d;
                return i;
        } else {

                l_text RF = FileNameToPath(d->Ex1,File);

#ifdef _OZONE__DJGPP_

                struct ffblk  ff;


                if ( !findfirst(RF, &ff, FA_RDONLY|FA_HIDDEN|FA_SYSTEM|FA_LABEL|FA_DIREC|FA_ARCH ) ) {

                        PFileInfo i = malloc(sizeof(TFileInfo));


                        memset(i,0,sizeof(TFileInfo));

                        i->Name = TextDup(ff.ff_name);
                        i->VPathName = TextDup(VFile);
                        i->LocalName = TextDup(File);
                        i->Attributes = ff.ff_attrib;
                        if ( i->Attributes & FA_DIREC )
                                i->Type = FT_FOLDER;
                        else
                                i->Type = FT_FILE;

                        i->MTime = MsDosTimeConvert(ff.ff_fdate,ff.ff_ftime);
                        i->CTime = MsDosTimeConvert(ff.lfn_cdate,ff.lfn_ctime);
                        i->ATime = MsDosTimeConvert(ff.lfn_adate,ff.lfn_atime);

                        i->Size = ff.ff_fsize;
                        i->Drive = d;

                        return i;

                }
                return NULL;
#endif
#ifdef _OZONE__WIN32_

                struct _finddata_t  ff;
                l_int hdl;
                hdl = _findfirst(RF, &ff);
                if ( hdl != -1 ) {

                        PFileInfo i = malloc(sizeof(TFileInfo));


                        memset(i,0,sizeof(TFileInfo));

                        i->Name = TextDup(ff.name);
                        i->VPathName = TextDup(VFile);
                        i->LocalName = TextDup(File);
                        i->Attributes = ff.attrib;
                        if ( i->Attributes & FA_DIREC )
                                i->Type = FT_FOLDER;
                        else
                                i->Type = FT_FILE;

                        i->CTime = ff.time_create;
                        i->ATime = ff.time_access;
                        i->MTime = ff.time_write;

                        i->Size = ff.size;
                        i->Drive = d;

                        return i;

                }
                return NULL;


#endif

  }
        return NULL;

}
////////////////////////////////////////////////////////////////////////////////
l_int  MsDosFileLength ( PFile f ) {
        f->FileSize =  filelength(fileno((FILE*)f->Ex1));
        return f->FileSize;
}
////////////////////////////////////////////////////////////////////////////////
l_int  MsDosFileFlush ( PFile f ) {
        return fflush((FILE*)f->Ex1);
}
////////////////////////////////////////////////////////////////////////////////
l_bool  MsDosDirMake    ( PDrive d, l_text Dir ) {
        l_text RF = FileNameToPath(d->Ex1,Dir);
        l_bool r = false;
        if ( RF ) {
#ifdef _OZONE__DJGPP_
                if ( mkdir(RF,S_IWUSR) )
#endif
#ifdef _OZONE__WIN32_
                if ( _mkdir(RF) )
#endif
                r = true;
                free(RF);
        }
        return r;
}
////////////////////////////////////////////////////////////////////////////////
l_bool  MsDosDirDelete   ( PDrive d, l_text Dir ) {
        l_text RF = FileNameToPath(d->Ex1,Dir);
        l_bool r = false;
        if ( RF ) {
                if ( rmdir(RF) )
                r = true;
                free(RF);
        }
        return r;
}
////////////////////////////////////////////////////////////////////////////////
PList MsDosList ( PDrive d, l_text VDir, l_text Dir, l_uchar Attrib ) {

        PList l = NewList();
        PFileInfo i;
        l_text RF0,RF;

#ifdef _OZONE__DJGPP_




        struct ffblk  ff;
        l_int         r, s = TextLen(Dir);

        RF0 = FileNameToPath(d->Ex1,Dir);
        RF = FileNameToPath(RF0,"*.*");

        free(RF0);


        r = findfirst(RF, &ff, Attrib);

        while (!r)
        {

                i = malloc(sizeof(TFileInfo));
                memset(i,0,sizeof(TFileInfo));

                if ( TextCompare(ff.ff_name,".") && TextCompare(ff.ff_name,"..") ) {

                i->Name = TextDup(ff.ff_name);
                i->VPathName = FileNameToPath(VDir,i->Name);
                i->LocalName = *Dir ? FileNameToPath(Dir,i->Name) : TextDup(i->Name);
                i->Attributes = ff.ff_attrib;
                if ( i->Attributes & FA_DIREC )
                        i->Type = FT_FOLDER;
                else
                        i->Type = FT_FILE;

                i->MTime = MsDosTimeConvert(ff.ff_fdate,ff.ff_ftime);
                i->CTime = MsDosTimeConvert(ff.lfn_cdate,ff.lfn_ctime);
                i->ATime = MsDosTimeConvert(ff.lfn_adate,ff.lfn_atime);

                i->Size = ff.ff_fsize;
                i->Drive = d;

                ListAdd(l,ff.ff_name,i,(void*)&FreeFileInfo);

                }

                r = findnext(&ff);
        }

        free(RF);

#endif
#ifdef _OZONE__WIN32_


        struct _finddata_t  ff;
        l_int         hdl,r = 0, s = TextLen(Dir);

        RF0 = FileNameToPath(d->Ex1,Dir);
        RF = FileNameToPath(RF0,"*.*");

        free(RF0);


        hdl = _findfirst(RF, &ff);


        Attrib |= FA_ARCH|FA_RDONLY;

    if ( hdl != - 1 )

        while (!r)
        {

                i = malloc(sizeof(TFileInfo));
                memset(i,0,sizeof(TFileInfo));

                if ( !( ff.attrib & ~Attrib ) && TextCompare(ff.name,".") && TextCompare(ff.name,"..") ) {

                i->Name = TextDup(ff.name);
                i->VPathName = FileNameToPath(VDir,i->Name);
                i->LocalName = *Dir ? FileNameToPath(Dir,i->Name) : TextDup(i->Name);
                i->Attributes = ff.attrib;
                if ( i->Attributes & FA_DIREC )
                        i->Type = FT_FOLDER;
                else
                        i->Type = FT_FILE;

                i->CTime = ff.time_create;
                i->ATime = ff.time_access;
                i->MTime = ff.time_write;
                i->Size = ff.size;
                i->Drive = d;

                ListAdd(l,ff.name,i,(void*)&FreeFileInfo);

                }

                r = _findnext(hdl,&ff);
        }

        _findclose(hdl);

        free(RF);

#endif
        return l;
}

#ifdef _OZONE__WIN32_

int _w32_setfileattributes ( char *file, unsigned short attribs );

#endif

////////////////////////////////////////////////////////////////////////////////
l_bool MsDosFileSetAttributes ( PDrive d, l_text File, l_uchar Attrib ) {
#ifdef _OZONE__DJGPP_
        l_text RF = FileNameToPath(d->Ex1,File);
        l_bool r = false;
        if ( RF ) {
                if ( _chmod(RF,1,Attrib) )
                r = true;
                free(RF);
        }
        return r;
#endif
#ifdef _OZONE__WIN32_
        l_text RF = FileNameToPath(d->Ex1,File);
        l_bool r = false;
        if ( RF ) {
                if ( _w32_setfileattributes(RF,Attrib) )
                r = true;
                free(RF);
        }
        return r;
#endif

}

////////////////////////////////////////////////////////////////////////////////
void InitMsDosFileDriver ( void ) {

        MsDosFileDriver = malloc(sizeof(TFileDriver));
        MsDosFileDriver->DriverName = TextDup("Codename Phoenix I/O to MS I/O layer driver");
        MsDosFileDriver->FileOpen = &MsDosFileOpen;
        MsDosFileDriver->FileClose = &MsDosFileClose;
        MsDosFileDriver->FileRead = &MsDosFileRead;
        MsDosFileDriver->FileWrite = &MsDosFileWrite;
        MsDosFileDriver->FileGetPos = &MsDosFileGetPos;
        MsDosFileDriver->FileSeek = &MsDosFileSeek;
        MsDosFileDriver->FileDelete = &MsDosFileDelete;
        MsDosFileDriver->FileRename = &MsDosFileRename;
        MsDosFileDriver->FileGetInfo = &MsDosFileGetInfo;

  MsDosFileDriver->FileFlush = &MsDosFileFlush;

        MsDosFileDriver->DirMake = &MsDosDirMake;
        MsDosFileDriver->DirDelete = &MsDosDirDelete;
        MsDosFileDriver->DirRename = &MsDosFileRename;
        MsDosFileDriver->List = &MsDosList;
        MsDosFileDriver->FileSetAttributes = &MsDosFileSetAttributes;
        MsDosFileDriver->FileEOF = &MsDosFileEOF;

        MsDosFileDriver->RefreshDriveInfo = NULL;
        DebugMessage("MsDosFileDriver=%x",MsDosFileDriver);
}
