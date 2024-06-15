////////////////////////////////////////////////////////////////////////////////
//
//  Error Management System - Header File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _ERROR_H_INCLUDED_
#define _ERROR_H_INCLUDED_

#define ERROR_NOTENOUGHMEMORY	0xFF0FF001
#define ERROR_ENABLETOSTARTLWP	0xFF0FF002

#define ERROR_FILENOTFOUND  	0xFF0FF003
#define ERROR_UNABLETOREAD  	0xFF0FF004
#define ERROR_UNABLETOWRITE  	0xFF0FF005
#define ERROR_INVALIDFILE  		0xFF0FF006
#define ERROR_CANTOPENTOWRITE  	0xFF0FF007

void ExceptionHandlerInstall( void );
void ExceptionHandlerUninstall( void );

typedef struct TError {
	
	TBListItem l;
	
	l_int ErrorCode;
	l_text Description;
	l_text Log;
	
} TError,*PError;

#define ERROR(o) ((PError)(o))

typedef struct TErrorHandler {
	
	TBList l;
	
} TErrorHandler, *PErrorHandler;


void CreateError ( PErrorHandler h, l_int ErrorCode, l_text Description);
void ErrorLogEx ( PErrorHandler h, l_text txt );
void ErrorLog ( PErrorHandler h, l_text txt, ... );
PErrorHandler CreateErrorHandler ( void );
void FreeErrorHandler(PErrorHandler h);

#define EHandlerIsError(h) (BLIST(h)->Last)


#endif /* _ERROR_H_INCLUDED_ */
