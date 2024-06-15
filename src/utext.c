////////////////////////////////////////////////////////////////////////////////
//
//    Unicode Text Manipulation - Core file
//
//  (c) Copyright 2004 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "stdio.h"
#include "stdarg.h"
#include "types.h"
#include "utext.h"
#include "dynld.h"
#include "internal.h"
#include "debug.h"

/**
*	NAME: CommonBufferUText
*	DESCRIPTION: This is a common buffer text, be careful with it.
*/
l_short CommonBufferUText[COMBUF_TEXT_SIZE];

////////////////////////////////////////////////////////////////////////////////
//	Return length of text t
l_ulong UTextLen( l_utext t )
{
	l_utext l = t;

	if ( !DCkPt("UTextLen.t",t) ) return 0;

	while ( *l ) l++;

	return l-t;
}
////////////////////////////////////////////////////////////////////////////////
//	Return 1st occurence of the char c in text t, if not found return 0
l_utext UTextChr( l_utext t, l_short c )
{
	if ( !DCkPt("UTextChr.t",t) ) return 0;

	while ( *t )
	{
		if ( *t == c ) return t;

		t++;
	}

	if ( c == 0 )
		return t;

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
// Return Last occurence of the char c in text t, if not found return 0
l_utext UTextRChr( l_utext t, l_short c ) {
	l_utext r = 0;
	if ( !DCkPt("UTextRChr.t",t) ) return 0;
	while ( *t ) {
		if ( *t == c ) r = t;
		t++;
  }
  if ( c == 0 ) r = t;
	return r;
}
////////////////////////////////////////////////////////////////////////////////
// Compare 2 texts a and b, return 0 if identical
long UTextCompare ( l_utext a, l_utext b ) {
	if ( !DCkPt("UTextCompare.a",a) || !DCkPt("UTextCompare.b",b) ) return 0;
  while ( *a == *b ) {
	  if (!*a) return 0;
	  a++;
	  b++;
  }
	return *a-*b;
}
////////////////////////////////////////////////////////////////////////////////
// Compare 2 texts a and b on length n, return 0 if identical
long UTextNCompare ( l_utext a, l_utext b, l_ulong n ) {
	if ( !DCkPt("UTextNCompare.a",a) || !DCkPt("UTextNCompare.b",b) ) return 0;
  while ( n ) {
	  if ( *a != *b ) return *a-*b;
	  if ( !*a ) return 0;
	  a++;
	  b++;
	  n--;
  }
  return 0;
}
////////////////////////////////////////////////////////////////////////////////
// Compare 2 texts a and b, return 0 if identical
long UTextCaseCompare ( l_utext a, l_utext b ) {
	if ( !DCkPt("UTextCaseCompare.a",a) || !DCkPt("UTextCaseCompare.b",b) ) return 0;
  while ( UToLower(*a) == UToLower(*b) ) {
	  if (!*a) return 0;
	  a++;
	  b++;
  }
	return UToLower(*a)-UToLower(*b);
}
////////////////////////////////////////////////////////////////////////////////
// Compare 2 texts a and b on length n, return 0 if identical
long UTextNCaseCompare ( l_utext a, l_utext b, l_ulong n ) {
	if ( !DCkPt("UTextNCaseCompare.a",a) || !DCkPt("UTextNCaseCompare.b",b) ) return 0;
  while ( n ) {
	  if ( UToLower(*a) != UToLower(*b) ) return UToLower(*a)-UToLower(*b);
	  if ( !*a ) return 0;
	  a++;
	  b++;
	  n--;
  }
  return 0;
}
////////////////////////////////////////////////////////////////////////////////
// Compare a sequence (sq,n) and a text t, return 0 if identical,
long UTextSqNCaseCompare ( l_utext sq, l_utext t, l_ulong n ) {
	if ( !DCkPt("UTextNCaseCompare.sq",sq) || !DCkPt("UTextNCaseCompare.t",t) ) return 0;
  while ( n ) {
	  if ( tolower(*sq) != tolower(*t) ) return tolower(*sq)-tolower(*t);
	  if ( !*sq ) return 0;
	  sq++;
	  t++;
	  n--;
  }
  return *t;
}
////////////////////////////////////////////////////////////////////////////////
// Copy text s to text d, return d
l_utext UTextCopy ( l_utext d, l_utext s ) {
	l_utext l = d;
	if ( !DCkPt("UTextCopy.d",d) || !DCkPt("UTextCopy.s",s) ) return 0;
  while ( *d = *s ) {
	  s++;
	  d++;
  }
  *d = 0;
	return l;
}
////////////////////////////////////////////////////////////////////////////////
// Copy text s to text d of n chars, add \0 at end ,return d
l_utext UTextNCopy ( l_utext d, l_utext s, l_ulong n  ) {
	l_utext l = d;
	if ( !DCkPt("UTextNCopy.d",d) || !DCkPt("UTextNCopy.s",s) ) return 0;
  while ( ( *d = *s ) && n ) {
	  s++;
	  d++;
	  n--;
  }
  *d = 0;
	return l;
}
////////////////////////////////////////////////////////////////////////////////
// Make a copy of string t in a new memory allocation
l_utext UTextDup ( l_utext t ) {
	l_utext n;
	if ( !DCkPt("UTextDup.t",t) ) return NULL;
	n = (l_utext)malloc((UTextLen(t)*2)+2);
	if ( !n ) return NULL;
	return UTextCopy(n,t);
}
////////////////////////////////////////////////////////////////////////////////
// Make a copy of n chars of text t in a new memory allocation (add \0 at end)
l_utext UTextNDup ( l_utext t, l_ulong n ) {
	l_utext l;
	if ( !DCkPt("UTextNDup.t",t) ) return NULL;
	l = (l_utext)malloc((n*2)+2);
	if ( !l ) return NULL;
	return UTextNCopy(l,t,n);
}
////////////////////////////////////////////////////////////////////////////////
// Add text t at the text d end, text d must have enouf memory allocated
l_utext UTextCat ( l_utext d, l_utext t ) {
	l_utext l = d;
	if ( !DCkPt("UTextCat.d",d) || !DCkPt("UTextCat.t",t) ) return d;
	while ( *d ) d++;
	UTextCopy(d,t);
	return l;
}
////////////////////////////////////////////////////////////////////////////////
// Transform text to upper case
l_utext UTextToUpper( l_utext d ){
	l_utext l = d;
	if ( !DCkPt("UTextToUpper.d",d) ) return 0;
	while ( *d ){ *d = UToUpper(*d); d++; }
	return l;
}
////////////////////////////////////////////////////////////////////////////////
// Transform text to lower case
l_utext UTextToLower( l_utext d ){
	l_utext l = d;
	if ( !DCkPt("UTextToUpper.d",d) ) return 0;
	while ( *d ){ *d = UToLower(*d); d++; }
	return l;
}
////////////////////////////////////////////////////////////////////////////////
l_short UToLower ( l_short c ) {
	if ( c & 0xFF00 )
		return c;
	else
		return tolower(c & 0xFF);
}
////////////////////////////////////////////////////////////////////////////////
l_short UToUpper ( l_short c ) {
	if ( c & 0xFF00 )
		return c;
	else
		return toupper(c & 0xFF);
}
////////////////////////////////////////////////////////////////////////////////
void InitUText ( void ) {
	SYSEXPORT(UTextLen);
	SYSEXPORT(UTextChr);
	SYSEXPORT(UTextCompare);
	SYSEXPORT(UTextNCompare);
	SYSEXPORT(UTextCaseCompare);
	SYSEXPORT(UTextNCaseCompare);
	SYSEXPORT(UTextCopy);
	SYSEXPORT(UTextNCopy);
	SYSEXPORT(UTextDup);
	SYSEXPORT(UTextNDup);
	SYSEXPORT(UTextCat);
	SYSEXPORT(UTextRChr);
	SYSEXPORT(UTextSqNCaseCompare);
	SYSEXPORT(UTextToUpper);
	SYSEXPORT(UTextToLower);
	SYSEXPORT(UToUpper);
	SYSEXPORT(UToLower);
}
////////////////////////////////////////////////////////////////////////////////
