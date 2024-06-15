/**
*       (c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*       FILE:                   error.c
*
*       PROJECT:                Phoenix engine - Core
*
*       DESCRIPTION:    Error and exception handling functions
*
*       CONTRIBUTORS:
*                                       Lukas Lipka
*
*       TODO:                   @ Add ASM interupt reprogramming code.
*
*       BUG:                    @
*
*       MISTAKE:                @
*
*       IDEA:                   @
*/

#include "kernel.h"
#include "internal.h"
#include <string.h>

/**
*       NAME: ERROR_SystemHalt
*       DESCRIPTION: Halts the whole system, cannot be undone
*/
_PRIVATE void  ERROR_SystemHalt ( void )
{
        printf("[EXCEPTION] => SYSTEM HALTED!\n");
        DebugFatal("SYSTEM HALTED!");
}

/**
*       NAME: ERROR_SystemPause
*       DESCRIPTION: Pauses the whole system, while we handle the exception
*/
_PRIVATE void  ERROR_SystemPause ( void )
{

}

/**
*       NAME: ERROR_SystemResume
*       DESCRIPTION: Resumes the whole system from a previous pause
*/
_PRIVATE void  ERROR_SystemResume ( void )
{

}

/**
 *      Exception functions
*/
_PUBLIC void  EXCEPTION_isr0 ( void )
{
        DebugError("0 Division by zero");
}

_PUBLIC void  EXCEPTION_isr1 ( void )
{
        DebugError("1 Debug exception");
}

_PUBLIC void  EXCEPTION_isr2 ( void )
{
        DebugError("2 Intel reserved");
}

_PUBLIC void  EXCEPTION_isr3 ( void )
{
        DebugError("3 Breakpoint");
}

_PUBLIC void  EXCEPTION_isr4 ( void )
{
        DebugError("4 Overflow");
}

_PUBLIC void  EXCEPTION_isr5 ( void )
{
        DebugError("5 Bounds check");
}

_PUBLIC void  EXCEPTION_isr6 ( void )
{
        DebugError("6 Invalid OPCODE");
}

_PUBLIC void  EXCEPTION_isr7 ( void )
{
        DebugError("7 Coprocessor Not Available");
}

_PUBLIC void  EXCEPTION_isr8 ( void )
{
        DebugError("8 Double Fault");
}

_PUBLIC void  EXCEPTION_isr9 ( void )
{
        DebugError("9 Coprocessor Segment Overrun");
}

_PUBLIC void  EXCEPTION_isr10 ( void )
{
        DebugError("10 Invalid TSS");
}

_PUBLIC void  EXCEPTION_isr11 ( void )
{
        DebugError("11 Segment Not Present");
}

_PUBLIC void  EXCEPTION_isr12 ( void )
{
        DebugError("12 Stack Exception");
}

_PUBLIC void  EXCEPTION_isr13 ( void )
{
        DebugError("13 General Protection Exception (Triple Fault)");
}

_PUBLIC void  EXCEPTION_isr14 ( void )
{
        DebugError("14 Page Fault");
}

_PUBLIC void  EXCEPTION_isr15 ( void )
{
        DebugError("15 Intel reserved");
}

_PUBLIC void  EXCEPTION_isr16 ( void )
{
        DebugError("16 Coprocessor Error");
}

void CreateError ( PErrorHandler h, l_int ErrorCode, l_text Description) {
        if ( h ) {
                PError e;
                if ( BLIST(h)->Last ) {
                        e = ERROR(BLIST(h)->Last);
                        if ( e->ErrorCode == ErrorCode ) return;
                }
                e = malloc(sizeof(TError));
                if ( !e ) return;
                memset(e,0,sizeof(TError));
                e->ErrorCode = ErrorCode;
                e->Description = TextDup(Description);
                BListAddItem(BLIST(h),BLISTITEM(e));
        }
}

void ErrorLogEx ( PErrorHandler h, l_text txt ) {
        if ( h )
                if ( BLIST(h)->Last ) {
                        PError e = ERROR(BLIST(h)->Last);
                        if ( e->Log ) {
                                l_int l = TextLen(e->Log);
                                l_text Ex = realloc(e->Log,l+TextLen(txt)+1);
                                TextCopy(Ex+l,txt);
                                e->Log = Ex;
                        } else
                                e->Log = TextDup(txt);
                }
}

void ErrorLog ( PErrorHandler h, l_text txt, ... ){
        if ( h )
                if ( BLIST(h)->Last ) {
                        va_list argptr;

                        va_start(argptr, txt);
                        vsprintf(CommonBufferText, txt, argptr);
                        va_end(argptr);

                        ErrorLogEx(h,CommonBufferText);
                }
}

void FreeError ( PError o ) {

        if ( o->Description ) free(o->Description);
        if ( o->Log ) free(o->Log);
        free(o);
}

PErrorHandler CreateErrorHandler ( void ) {
        PErrorHandler o = malloc(sizeof(TErrorHandler));
        if ( !o ) return NULL;
        memset(o,0,sizeof(TErrorHandler));
        return o;
}

void FreeErrorHandler(PErrorHandler h) {
        if ( BLIST(h)->Last ) {
                PBListItem a,b,n;
                a = b = h->l.Last;
                do {
                        n = a->Prev;
                        free(a);
                        a = n;
                } while ( a != b );
        }
        free(h);
}




_PUBLIC void  ExceptionHandlerInstall ( void )
{
        DebugMessage("Starting exception handler for exceptions (0-16)");

        /**
        *       Reprogram int 8, for ISR 0-16
        */
        DebugMessage("Reprogramming interrupt 8 => 0-16 ISR");

        DebugMessage("Reprogramming interrupt successful");
}

_PUBLIC void  ExceptionHandlerUninstall ( void )
{
        DebugMessage("Reprogramming interrupt 8 => NORMAL");

        DebugMessage("Reprogramming interrupt successful");
}
