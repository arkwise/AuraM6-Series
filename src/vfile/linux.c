////////////////////////////////////////////////////////////////////////////////
//
//  Codename Phoenix I/O to Linux I/O layer driver
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <dirent.h>


PFileDriver LinuxFileDriver;

////////////////////////////////////////////////////////////////////////////////
l_ulong LinuxAttribesToMs ( struct stat *s ) {
	
   l_ulong attrib = 0;
   uid_t euid = geteuid();

   if (euid != 0) {
      if (s->st_uid == euid) {
	 if ((s->st_mode & S_IWUSR) == 0)
	    attrib |= FA_RDONLY;
      }
      else if (s->st_gid == getegid()) {
	 if ((s->st_mode & S_IWGRP) == 0)
	    attrib |= FA_RDONLY;
      }
      else if ((s->st_mode & S_IWOTH) == 0) {
	 attrib |= FA_RDONLY;
      }
   }

   if (S_ISDIR(s->st_mode))
      attrib |= FA_DIREC;

   return attrib;

}
////////////////////////////////////////////////////////////////////////////////
PFile LinuxFileOpen ( PDrive d, l_text File, l_text Mode ) {
	struct stat s;
	PFile o = malloc(sizeof(TFile));
	l_text RF = NULL;
	if ( !o ) return NULL;
	memset(o,0,sizeof(TFile));
	RF = FileNameToPath(d->Ex1,File);
	DebugMessage("Try to open %s...",RF);
	o->Ex1 = fopen(RF,Mode);
	free(RF);
	if ( !o->Ex1 ) { free(o); return NULL; }
	fstat(fileno((FILE*)o->Ex1),&s);
	if (S_ISDIR(s.st_mode)) { free(o); fclose((FILE*)o->Ex1); return NULL;  }
	o->FileSize =  s.st_size;
	o->Drive = d;
	return o;
}
////////////////////////////////////////////////////////////////////////////////
void LinuxFileClose  ( PFile f ) {
	fclose((FILE*)f->Ex1);
	free(f);
}
////////////////////////////////////////////////////////////////////////////////
l_int LinuxFileRead   ( PFile f, void *d, l_ulong s ) {
  return fread(d,1,s,(FILE*)f->Ex1);	
}
////////////////////////////////////////////////////////////////////////////////
l_int LinuxFileWrite  ( PFile f, void *d, l_ulong s ){
  return fwrite(d,1,s,(FILE*)f->Ex1);
}
////////////////////////////////////////////////////////////////////////////////
l_int LinuxFileEOF  ( PFile f ){
  return feof((FILE*)f->Ex1);
}
////////////////////////////////////////////////////////////////////////////////
l_int LinuxFileGetPos ( PFile f, l_ulong *Pos ) {
	l_int r = 0;
	*Pos = ftell((FILE*)f->Ex1);
	return r;
}
////////////////////////////////////////////////////////////////////////////////
l_int  LinuxFileSeek ( PFile f, l_int Type, l_long Pos ) {
	return fseek((FILE*)f->Ex1,Pos,Type);
}
////////////////////////////////////////////////////////////////////////////////
l_bool  LinuxFileDelete ( PDrive d, l_text File ) {
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
l_bool  LinuxFileRename ( PDrive d, l_text File, l_text NewName ) {
	l_text RF1 = FileNameToPath(d->Ex1,File); 
	l_text RF2 = FileNameToPath(d->Ex1,NewName); 
	l_bool r = false;
	if ( !RF1 || !RF2 ) return false;
	if ( !rename(RF1,RF2) ) r = true;
	
	free(RF1); free(RF2);
	return r;
}
////////////////////////////////////////////////////////////////////////////////
PFileInfo  LinuxFileGetInfo ( PDrive d, l_text VFile, l_text File ) {
	
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
		PFileInfo i = NULL;
		l_text RF = FileNameToPath(d->Ex1,File); 
		
		struct stat s;

		if ( !RF ) return NULL;

		if ( !stat(RF, &s) ) {

			i = malloc(sizeof(TFileInfo));
			memset(i,0,sizeof(TFileInfo));

			i->Name = TextDup(get_filename(File));
			i->VPathName = TextDup(VFile);
			i->LocalName = TextDup(File);
			i->Attributes = LinuxAttribesToMs(&s);

			if ( i->Attributes & FA_DIREC )
				i->Type = FT_FOLDER;
			else
				i->Type = FT_FILE;

			i->CTime = s.st_ctime;
			i->ATime = s.st_atime;
			i->MTime = s.st_mtime;

			i->Size = s.st_size;
			i->Drive = d;
		}
		return i;
  }
	return NULL;
	
}
////////////////////////////////////////////////////////////////////////////////
l_int  LinuxFileLength ( PFile f ) {
	f->FileSize =  filelength(fileno((FILE*)f->Ex1));
	return f->FileSize;
}
////////////////////////////////////////////////////////////////////////////////
l_int  LinuxFileFlush ( PFile f ) {
	return fflush((FILE*)f->Ex1);
}
////////////////////////////////////////////////////////////////////////////////
l_bool  LinuxDirMake (PDrive d, l_text Dir) {
	l_text RF = FileNameToPath(d->Ex1, Dir); 	
	l_bool r = false;

	if ( RF )
	{
		if ( mkdir(RF,S_IRWXU|S_IRWXG|S_IRWXO|S_IWUSR |S_IXUSR) )
			r = true;
	
		free(RF);
	}

	return r;	
}
////////////////////////////////////////////////////////////////////////////////
l_bool  LinuxDirDelete   ( PDrive d, l_text Dir ) {
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
PList LinuxList ( PDrive d, l_text VDir, l_text Dir, l_uchar Attrib ) {
	struct stat s;
	DIR *dir;
	struct dirent *de;
	
	PList l = NewList();
	PFileInfo i;
	l_text RF,rF;

	RF = FileNameToPath(d->Ex1,Dir);
	
	Attrib |= FA_ARCH|FA_RDONLY;
	
	if ( RF ) {
		dir = opendir(RF);
		if ( d ) {
			while ( de = readdir(dir) ) {
				if ( TextCompare(de->d_name,".") && TextCompare(de->d_name,"..") ) {

				i = malloc(sizeof(TFileInfo));
				memset(i,0,sizeof(TFileInfo));


				i->Name = TextDup(de->d_name);
				i->VPathName = FileNameToPath(VDir,i->Name);
				i->LocalName = *Dir ? FileNameToPath(Dir,i->Name) : TextDup(i->Name);
				
				rF = FileNameToPath(RF,i->Name);
				stat(rF,&s);
				free(rF);
				
				i->Attributes = LinuxAttribesToMs(&s);

				if ( !( i->Attributes & ~Attrib ) ) {
				
					if ( i->Attributes & FA_DIREC )
						i->Type = FT_FOLDER;
					else
						i->Type = FT_FILE;
	
					i->CTime = s.st_ctime;
					i->ATime = s.st_atime;
					i->MTime = s.st_mtime;
	
					i->Size = s.st_size;
					i->Drive = d;
					ListAdd(l,i->Name,i,(void*)&FreeFileInfo);
				
				} else {
					FreeFileInfo(i);
					
				}
				}
			}
			closedir(dir);
		}
		free(RF);
	}
	return l;
}



////////////////////////////////////////////////////////////////////////////////
l_bool LinuxFileSetAttributes ( PDrive d, l_text File, l_uchar Attrib ) {
	/*l_text RF = FileNameToPath(d->Ex1,Dir); 	
	l_bool r = false;
	if ( RF ) {
	  r = true;
		free(RF);
	}
	return r;*/
	return false;
}
////////////////////////////////////////////////////////////////////////////////
void InitLinuxFileDriver ( void ) {
	
	LinuxFileDriver = malloc(sizeof(TFileDriver));
	LinuxFileDriver->DriverName = TextDup("Codename Phoenix I/O to Linux I/O layer driver");
	LinuxFileDriver->FileOpen = &LinuxFileOpen;
	LinuxFileDriver->FileClose = &LinuxFileClose;
	LinuxFileDriver->FileRead = &LinuxFileRead;
	LinuxFileDriver->FileWrite = &LinuxFileWrite;
	LinuxFileDriver->FileGetPos = &LinuxFileGetPos;
	LinuxFileDriver->FileSeek = &LinuxFileSeek;
	LinuxFileDriver->FileDelete = &LinuxFileDelete;
	LinuxFileDriver->FileRename = &LinuxFileRename;
	LinuxFileDriver->FileGetInfo = &LinuxFileGetInfo;
	LinuxFileDriver->FileFlush = &LinuxFileFlush;
	LinuxFileDriver->DirMake = &LinuxDirMake;
	LinuxFileDriver->DirDelete = &LinuxDirDelete;
	LinuxFileDriver->DirRename = &LinuxFileRename;
	LinuxFileDriver->List = &LinuxList;
	LinuxFileDriver->FileSetAttributes = &LinuxFileSetAttributes;
	LinuxFileDriver->FileEOF = &LinuxFileEOF;
	LinuxFileDriver->RefreshDriveInfo = NULL;
}

