/**
*	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*	FILE:			debug.c
*
*	PROJECT:		Phoenix engine - Core
*
*	DESCRIPTION:	This is the debugging system that is used for printing
*						error and information messages into kernel.txt
*
*	CONTRIBUTORS:
*					Lukas Lipka
*					Julien Etelain
*
*	TODO:			@
*
*	BUG:			@
*
*	MISTAKE:		@
*
*	IDEA:			@
*/

#include "kernel.h"
#include "internal.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/** @defgroup debug Debug and logging system.
	If your software or the system crashes during development use the set of debug functions to find the error.
	@{
*/
/**	@file
	Debug core file
*/

FILE* DebugFile;

/**
	Writes a message into the debug file using the label "Message:"
*/
void DebugMessage ( char* szFormat, ... )
{
	va_list argptr;

	va_start(argptr, szFormat);

	if (DebugFile)
	{
		fprintf(DebugFile, "Message: ");
		vfprintf(DebugFile, szFormat, argptr);
		fprintf(DebugFile,"\n");
		fflush(DebugFile);
	}
	
	va_end(argptr);
}

/**
	Writes a message into the debug file using the label "Warning:"
*/
void DebugWarning ( char* szFormat, ... )
{
	va_list argptr;

	va_start(argptr, szFormat);

	if (DebugFile)
	{
		fprintf(DebugFile, "Warning: ");
		vfprintf(DebugFile, szFormat, argptr);
		fprintf(DebugFile,"\n");
		fflush(DebugFile);
	}

	va_end(argptr);
}

/**
	Writes a message into the debug file using the label "Error:"
*/
void DebugError ( char* szFormat, ... )
{
	va_list argptr;

	va_start(argptr, szFormat);

	if (DebugFile)
	{
		fprintf(DebugFile, "ERROR: ");
		vfprintf(DebugFile, szFormat, argptr);
		fprintf(DebugFile,"\n");
		fflush(DebugFile);
	}


	va_end(argptr);
}

/**
	Writes a message into the debug file using the label "Fatal:" and halts the kernel.
*/
void DebugFatal ( char* szFormat, ... )
{
	va_list argptr;

	va_start(argptr, szFormat);

	if (DebugFile)
	{
		fprintf(DebugFile, "FATAL ERROR: ");
		vfprintf(DebugFile, szFormat, argptr);
		fprintf(DebugFile,"\n");
		fprintf(DebugFile,"\n\nA fatal error has occurred. Aborting program execution.\n");
		fflush(DebugFile);
	}


	va_end(argptr);

	abort();
}

/*
	Initializes the debug system
*/
void GSSystemDebugInstall ( l_text szFilename )
{
	time_t t = time(NULL);
	DebugFile = NULL;

	if (!szFilename)
		DebugFile = NULL;
	else
	{
		DebugFile = fopen(szFilename, "wt");

		if ( !DebugFile )
			printf("DEBUG::DEBUG - Can't open file '%s' for writing\n", szFilename);
	}

	/*
	*	Print kernel information
	*/
	#ifdef _OZONE__WIN32_
		DebugMessage("Starting log to %s on %s\t%s\n\t\tKERNEL: %s  VERSION: %d.%d.%d.%d\n\t\t\tWIN32 Release\n",
				szFilename, ctime(&t), KernelCopyright, KernelName,
				ULID_A(Me.Version),ULID_B(Me.Version),ULID_C(Me.Version),ULID_D(Me.Version) );
	#endif
	#ifdef _OZONE__DJGPP_
		DebugMessage("Starting log to %s on %s\t%s\n\t\tKERNEL: %s  VERSION: %d.%d.%d.%d\n\t\t\tDJGPP Release\n",
				szFilename, ctime(&t), KernelCopyright, KernelName,
				ULID_A(Me.Version),ULID_B(Me.Version),ULID_C(Me.Version),ULID_D(Me.Version) );
	#endif
	#ifdef _OZONE__LINUX_
		DebugMessage("Starting log to %s on %s\t%s\n\t\tKERNEL: %s  VERSION: %d.%d.%d.%d\n\t\t\tLINUX Release\n",
				szFilename, ctime(&t), KernelCopyright, KernelName,
				ULID_A(Me.Version),ULID_B(Me.Version),ULID_C(Me.Version),ULID_D(Me.Version));
	#endif

}


l_bool DebugCheckPtr ( l_text inf, void *ptr ) {
	if ( !ptr )
	{
		DebugWarning("NULL pointer at %s", inf);

		return false;
	}

	return true;
}

/*
	Uninstalls the debug system
*/
void GSSystemDebugUninstall ( void )
{
	DebugMessage("Logging stopped, closing log file");

	if (DebugFile)
	{
		fclose(DebugFile);
		DebugFile = NULL;
	}
}


