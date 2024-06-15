////////////////////////////////////////////////////////////////////////////////
//
//    UText Manipulation - Header file
//
//  (c) Copyright 2003 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _utext_h_included_
#define _utext_h_included_

l_ulong UTextLen			( l_utext t );
l_utext UTextChr			( l_utext t, l_short c );
l_utext UTextRChr			( l_utext t, l_short c );
long UTextCompare		( l_utext a, l_utext b );
long UTextNCompare		( l_utext a, l_utext b, l_ulong n );
long UTextCaseCompare    ( l_utext a, l_utext b );
long UTextNCaseCompare   ( l_utext a, l_utext b, l_ulong n );
long UTextSqNCaseCompare ( l_utext sq, l_utext t, l_ulong n );

l_utext UTextCopy			( l_utext d, l_utext s );
l_utext UTextNCopy		( l_utext d, l_utext s, l_ulong n  );
l_utext UTextDup			( l_utext t );
l_utext UTextNDup			( l_utext t, l_ulong n );
l_utext UTextCat			( l_utext d, l_utext t );


void InitUText ( void );

l_short UToLower ( l_short c );
l_short UToUpper ( l_short c );

#endif /* _text_h_included_ */
