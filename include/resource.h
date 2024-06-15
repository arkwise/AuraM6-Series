#ifndef _RES_H_INCLUDED_
#define _RES_H_INCLUDED_

#include "types.h"

/**
*	Version info
*/
#define RESLNKVERSION ULONG_ID(0,0,0,1)


typedef struct TResEntry
{
	l_char Name[32];
	l_ulong DataType;
	l_ulong DataEncode;
	l_ulong DataSize;
} TResEntry;

typedef struct TResHead
{
	l_ulong Magic;
	l_ulong FormatVersion;
	l_ulong Entries;
} TResHead;


PList	LoadRessourceFile	( l_text File );
l_bool	SaveRessourceFile	( l_text File, PList Data  );
void	DMSWriteRessource	( PFile f, PList Lst, l_ulong *Entries );
PList	DMSLoadRessource	( PFile f, l_ulong Entries );

PIcon GetDynLdIcon ( PFileInfo f );
PIcon GetDynLdIconEx ( l_text File );

#endif /* _RES_H_INCLUDED_ */
