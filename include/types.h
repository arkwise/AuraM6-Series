#ifndef _TYPES_H_INCLUDED_
#define _TYPES_H_INCLUDED_

#include "platform.h"

/**
*	Variable types
*/
typedef	long				l_long;
typedef	int					l_int;
#ifdef _OZONE__WIN32_
typedef	unsigned _int64		l_ubig;
#else
typedef	unsigned long long	l_ubig;
#endif
typedef	int					l_bool;
typedef	char				l_char;
typedef short				l_short;
typedef unsigned short		l_ushort;
typedef	unsigned char		l_byte,  l_uchar;
typedef	unsigned int		l_word,  l_uint;
typedef	unsigned long		l_dword, l_ulong;
typedef	char				l_tag[32];
typedef	long				l_rect;
typedef	int					l_color;
typedef	char*				l_text;
typedef	short*			l_utext;// Unicode text
typedef double			l_double;
typedef	void*				l_ptr;

typedef char        l_uid[13]; // Use only 12 first bytes. (13th byte must be '\0') (UTF-8)
  
#ifdef ALLEGRO_H
typedef	BITMAP*			p_bitmap;
typedef	BITMAP*			PImage;
typedef RLE_SPRITE*		PRleSprite;

#define IMAGE(o) ((PImage)(o))
#define RLESPRITE(o) ((PRleSprite)(o))


#endif

typedef	l_int				t_message;
typedef	void*				t_pointer;
typedef	t_pointer			t_data;

/**
*	DMS specific (see dms.h)
*/
typedef  unsigned long		l_datatype;
typedef  void*				l_data;
typedef  unsigned long		l_dataencode;

#define TYPE_UNKNOW_BINARY 0
#define TYPE_TEXT          1
#define TYPE_UTEXT         2
#define TYPE_IMAGE         3
#define TYPE_ICON          4
#define TYPE_CURSOR        5
#define TYPE_LIST          6
#define TYPE_FILEREF       7
#define TYPE_FILEREFLIST   8

// Gateways types (FMS stuff)
#define DTYPE_DYNLD 		ULONG_ID('D','y','n','L')
#define DTYPE_DOSEXE 		ULONG_ID('D','E','x','e')
#define DTYPE_DIRECTORY 	ULONG_ID('D','i','r',' ')

// Standart data types
#define DTYPE_TEXT          0x00000001
#define DTYPE_UTEXT         0x00000002
#define DTYPE_IMAGE         0x00000003
#define DTYPE_ICON          0x00000004
#define DTYPE_CURSOR        0x00000005
#define DTYPE_LIST          0x00000006
#define DTYPE_RESSOURCE 	DTYPE_LIST
#define DTYPE_FILEREF       0x00000007
#define DTYPE_FILEREFLIST   0x00000008
#define DTYPE_SOUNDSTREAM   0x00000009
#define DTYPE_IWIDGET   	0x00000010
#define DTYPE_APPINFO   	0x00000011 // Type not yet defined
#define DTYPE_BINDATA 		0xFFFFFFFF

typedef struct TBinData {
	
	void *Data;	
	l_ulong Size;
	
} TBinData, *PBinData;

#define BINDATA(o) ((PBinData)(o))

#ifdef SIMPLECOLOR
	typedef unsigned long TSColor, *PSColor;
#else
	typedef struct TSColor {
		l_uchar idx;
		l_uchar r, g, b;
	} TSColor, *PSColor;
#endif

#ifndef LOWORD
#define LOWORD(w)	(((w)<<16)>>16)
#endif

#ifndef HIWORD
#define HIWORD(w)	((w)>>16)
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif

#define ULONG_ID(a, b, c, d) ((a << 24) | (b << 16) | (c << 8) | d)

#define LONG_ID(d, c, b, a) ((a << 24) | (b << 16) | (c << 8) | d)


#define ULID_A(v) ((v & 0xFF000000) >> 24)
#define ULID_B(v) ((v & 0x00FF0000) >> 16)
#define ULID_C(v) ((v & 0x0000FF00) >> 8)
#define ULID_D(v) (v & 0x000000FF)

/**
*	Usefull MACRO tools
*/
#define CLEAR(o)        if (o) memset((o), 0, sizeof(*(o)))
#define NEW(o)          (o *) malloc(sizeof(o))
#define RGB(r,g,b)      makecol(r, g, b)
#define SET(s,o)        s |= (o)
#define UNSET(s,o)      s &=~(o)

// See DMS2.C and DMS2.H for informations on PIcon funtions
#ifdef ALLEGRO_H

typedef struct TIcon {
	
	l_ulong Instances;
	
	PRleSprite Icon16;
	PRleSprite Icon32;
	PRleSprite Icon48;
	
} TIcon, *PIcon;

#endif

#endif /* _TYPES_H_INCLUDED_ */
