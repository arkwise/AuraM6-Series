////////////////////////////////////////////////////////////////////////////////
//
//    Text Manipulation - Core file
//
//  (c) Copyright 2003 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "text.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "dynld.h"
#include "internal.h"
#include "debug.h"

/** @defgroup kerneltext Text manipulation (kernel)
        A set of functions for string manipulation
        @{
*/
/**     @file
        ASCII Text Manipulation.
*/

/**
*       This is a common buffer text, be careful with it.
*/
char CommonBufferText[COMBUF_TEXT_SIZE];

/**
*       Return length of text t
*/
l_ulong TextLen(l_text t)
{
        l_text l = t;

        if (!DCkPt("TextLen.t", t))
                return 0;

        while (*l)
                l++;

        return l - t;
}

/**
*       Return first occurence of char c in text t, if occurence is not found return 0
*/
l_text TextChr(l_text t, l_char c)
{
        if (!DCkPt("TextChr.t", t))
                return 0;

        while (*t)
        {
                if (*t == c)
                        return t;

                t++;
        }

        if (c == 0)
                return t;

        return 0;
}

/**
*       Return last occurence of char c in text t, if occurence not found return 0
*/
l_text TextRChr(l_text t, l_char c)
{
        l_text r = 0;
        if (!DCkPt("TextRChr.t", t))
                return 0;
        while (*t)
        {
                if (*t == c)
                        r = t;
                t++;
        }
        if (c == 0)
                r = t;
        return r;
}

/**
*       Compare strings a and b, return 0 if identical
*/
long TextCompare(l_text a, l_text b)
{
        if (!DCkPt("TextCompare.a", a) || !DCkPt("TextCompare.b", b))
                return 0;
        while (*a == *b)
        {
                if (!*a)
                        return 0;
                a++;
                b++;
        }
        return *a - *b;
}

/**
*       Compare strings a and b on length n, return 0 if identical
*/
long TextNCompare(l_text a, l_text b, l_ulong n)
{
        if (!DCkPt("TextNCompare.a", a) || !DCkPt("TextNCompare.b", b))
                return 0;
        while (n)
        {
                if (*a != *b)
                        return *a - *b;
                if (!*a)
                        return 0;
                a++;
                b++;
                n--;
        }
        return 0;
}

/**
*       Compare strings a and b and dont mind the case, return 0 if identical
*/
long TextCaseCompare(l_text a, l_text b)
{
        if (!DCkPt("TextCaseCompare.a", a) || !DCkPt("TextCaseCompare.b", b))
                return 0;
        while (tolower(*a) == tolower(*b))
        {
                if (!*a)
                        return 0;
                a++;
                b++;
        }
        return tolower(*a) - tolower(*b);
}

/**
*        Compare strings a and b on length n and dont mind the case, return 0 if identical
*/
long TextNCaseCompare(l_text a, l_text b, l_ulong n)
{
        if (!DCkPt("TextNCaseCompare.a", a) || !DCkPt("TextNCaseCompare.b", b))
                return 0;
        while (n)
        {
                if (tolower(*a) != tolower(*b))
                        return tolower(*a) - tolower(*b);
                if (!*a)
                        return 0;
                a++;
                b++;
                n--;
        }
        return 0;
}

/**
*       Compare a sequence (sq, n) and a text t, return 0 if identical,
*/
long TextSqNCaseCompare(l_text sq, l_text t, l_ulong n)
{
        if (!DCkPt("TextNCaseCompare.sq", sq) || !DCkPt("TextNCaseCompare.t", t))
                return 0;
        while (n)
        {
                if (tolower(*sq) != tolower(*t))
                        return tolower(*sq) - tolower(*t);
                if (!*sq)
                        return 0;
                sq++;
                t++;
                n--;
        }
        return *t;
}

/**
*       Copy text s to text d, return d
*/
l_text TextCopy(l_text d, l_text s)
{
        l_text l = d;
        if (!DCkPt("TextCopy.d", d) || !DCkPt("TextCopy.s", s))
                return 0;
        while (*d = *s)
        {
                s++;
                d++;
        }
        *d = 0;
        return l;
}
/**
*       Copy text s to text d of n chars, add '\\0' at end ,return d
*/
l_text TextNCopy(l_text d, l_text s, l_ulong n)
{
        l_text l = d;
        if (!DCkPt("TextNCopy.d", d) || !DCkPt("TextNCopy.s", s))
                return 0;
        while ((*d = *s) && n)
        {
                s++;
                d++;
                n--;
        }
        *d = 0;
        return l;
}
/**
*       Make a copy of string t in a new memory allocation
*/
l_text TextDup(l_text t)
{
        l_text n;
        if (!DCkPt("TextDup.t", t))
                return NULL;
        n = (l_text)malloc(TextLen(t) + 1);
        if (!n)
                return NULL;
        return TextCopy(n, t);
}
/**
*       Make a copy of n chars of text t in a new memory allocation (add '\\0' at end)
*/
l_text TextNDup(l_text t, l_ulong n)
{
        l_text l;
        if (!DCkPt("TextNDup.t", t))
                return NULL;
        l = (l_text)malloc(n + 1);
        if (!l)
                return NULL;
        return TextNCopy(l, t, n);
}
/**
*       Add text t at the text d end, text d must have enouf memory allocated
*/
l_text TextCat(l_text d, l_text t)
{
        l_text l = d;
        if (!DCkPt("TextCat.d", d) || !DCkPt("TextCat.t", t))
                return d;
        while (*d)
                d++;
        TextCopy(d, t);
        return l;
}
/**
*       Create a string with args
*/
l_text TextArgs(l_text szFormat, ...)
{
        va_list argptr;

        if (!DCkPt("TextArgs.szFormat", szFormat))
                return NULL;

        va_start(argptr, szFormat);
        vsprintf(CommonBufferText, szFormat, argptr);
        va_end(argptr);

        return (l_text)TextDup(CommonBufferText);
}
/**
*       Transform text to upper case
*/
l_text TextToUpper(l_text d)
{
        l_text l = d;
        if (!DCkPt("TextToUpper.d", d))
                return 0;
        while (*d)
        {
                *d = toupper(*d);
                d++;
        }
        return l;
}
/**
*       Transform text to lower case
*/
l_text TextToLower(l_text d)
{
        l_text l = d;
        if (!DCkPt("TextToUpper.d", d))
                return 0;
        while (*d)
        {
                *d = tolower(*d);
                d++;
        }
        return l;
}
////////////////////////////////////////////////////////////////////////////////
l_char ToLower(l_char c)
{
        return tolower(c);
}
////////////////////////////////////////////////////////////////////////////////
// Hash a string
l_ulong TextHash(l_text t)
{
        l_ulong i = 0, k = 0;
        l_uint len = strlen(t);

        if (len <= 0)
                return 0;

        while (k < len)
                i = ((i << 6) + (t[k++] & 0x7f));

        return i;
}
/**
@}
*/
////////////////////////////////////////////////////////////////////////////////
void InitText(void)
{
        SYSEXPORT(TextLen);
        SYSEXPORT(TextChr);
        SYSEXPORT(TextCompare);
        SYSEXPORT(TextNCompare);
        SYSEXPORT(TextCaseCompare);
        SYSEXPORT(TextNCaseCompare);
        SYSEXPORT(TextCopy);
        SYSEXPORT(TextNCopy);
        SYSEXPORT(TextDup);
        SYSEXPORT(TextNDup);
        SYSEXPORT(TextCat);
        SYSEXPORT(TextRChr);
        SYSEXPORT(TextArgs);
        SYSEXPORT(TextSqNCaseCompare);
        SYSEXPORT(TextToUpper);
        SYSEXPORT(TextToLower);
        SYSEXPORT(TextHash);
        SYSEXPORT(ToLower);
}
////////////////////////////////////////////////////////////////////////////////
