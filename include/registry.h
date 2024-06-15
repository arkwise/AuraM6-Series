////////////////////////////////////////////////////////////////////////////////
//
//  Registry - Header File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _NREG_H_INCLUDED_
#define _NREG_H_INCLUDED_

#define NREGISTRY_MAGIC LONG_ID('P','h','R','g')



typedef struct TRegKey *PRegKey;
typedef struct TRegKey {

	l_text   Name;
	l_uchar  Type;
	void    *Data;

	PRegKey Parent;
	PRegKey Last;
	PRegKey Next;
	PRegKey Prev;

	l_ulong TmpId;

	l_uchar __e; // Do not use, reserved.

} TRegKey;

#define REGKEY(o) ((PRegKey)(o))


#define RKT_NODATA   0x00
#define RKT_CHAR     0x01
#define RKT_BOOL     0x02
#define RKT_SHORT    0x03
#define RKT_INT      0x04
#define RKT_LONG     0x04
#define RKT_BIG      0x05
#define RKT_FLOAT    0x06
#define RKT_DOUBLE   0x07
#define RKT_RECT     0x08
#define RKT_POINT    0x09
#define RKT_TEXT     0x0A
#define RKT_LINK     0x0B

typedef struct TRegHead {
	l_ulong Magic;
	l_ulong Keys;
} TRegHead;

void NRegistryInit ( void );
void NRegistryUnInit ( void );

void RegistryReLoad ( void ); // Do not save changes...
void RegistrySave ( void );


PRegKey ResolveKey ( l_text Name );
l_bool  KeyExists ( l_text Name );
l_uchar NewKey ( l_text Parent, l_text Name );

void DeleteKey( l_text Name );
void DeleteKeyEx( PRegKey o );

l_bool KeySetText ( l_text Name, l_text Val );
l_text KeyGetText ( l_text Name, l_text Def );
l_bool KeySetLong ( l_text Name, l_long Val );
l_long KeyGetLong ( l_text Name, l_long Def );

l_bool KeySetChar ( l_text Name, l_char Val );
l_char KeyGetChar ( l_text Name, l_char Def );
l_bool KeySetBool ( l_text Name, l_bool Val );
l_bool KeyGetBool ( l_text Name, l_bool Def );
l_bool KeySetShort ( l_text Name, l_short Val );
l_short KeyGetShort ( l_text Name, l_short Def );
l_bool KeySetInt ( l_text Name, l_int Val );
l_int KeyGetInt ( l_text Name, l_int Def );
l_bool KeySetBig ( l_text Name, l_ubig Val );
l_ubig KeyGetBig ( l_text Name, l_ubig Def );

l_bool KeySetNothing ( l_text Name );

l_text GetParentKeyName ( l_text Key );
l_uchar RenameKey ( l_text Key, l_text NewName );

void RegistryRunEntries ( l_text Name );

void CreateKey ( l_text key );
PRegKey GetOrCreateKey ( l_text key );

l_int GetColorFromHex ( l_text Code );

#endif
