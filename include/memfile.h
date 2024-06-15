////////////////////////////////////////////////////////////////////////////////
//
//  Memory files - Header File
//
//	(c) Copyright 2004 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MEMFILE_H_INCLUDED_
#define _MEMFILE_H_INCLUDED_

PFile MemFileNewEmpty ( void );

PBinData FileExtractToBinData ( PFile f ); // Works for all PFile,

PFile   MemFileFromBinData ( PBinData b );

PFile   MemFileFromFreeBinData ( PBinData b ); // Use less memory

PBinData MemFileCloseToBinData ( PFile f ); // Works only for memfiles, use less memory

#endif
