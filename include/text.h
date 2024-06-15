////////////////////////////////////////////////////////////////////////////////
//
//    Text Manipulation - Header file
//
//  (c) Copyright 2003 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _text_h_included_
#define _text_h_included_

#include "types.h"

l_ulong TextLen			( l_text t );
l_text TextChr			( l_text t, l_char c );
l_text TextRChr			( l_text t, l_char c );
long TextCompare		( l_text a, l_text b );
long TextNCompare		( l_text a, l_text b, l_ulong n );
long TextCaseCompare    ( l_text a, l_text b );
long TextNCaseCompare   ( l_text a, l_text b, l_ulong n );
long TextSqNCaseCompare ( l_text sq, l_text t, l_ulong n );

l_text TextCopy			( l_text d, l_text s );
l_text TextNCopy		( l_text d, l_text s, l_ulong n  );
l_text TextDup			( l_text t );
l_text TextNDup			( l_text t, l_ulong n );
l_text TextCat			( l_text d, l_text t );
l_text TextArgs			(l_text szFormat, ...);


void InitText ( void );

l_char ToLower ( l_char c );

#endif /* _text_h_included_ */
