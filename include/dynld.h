////////////////////////////////////////////////////////////////////////////////
//
//  Dynamic Loading System - Header File
//
//  (c) Copyright 2003,2004 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include <setjmp.h>


#include "types.h"
#include "list.h"
#include "stdio.h"
#include "vfile.h"
#include "error.h"

#ifndef _DYNLD_H_INCLUDED_
#define _DYNLD_H_INCLUDED_

/**
*	DynLD version
*/
#define DYNLDVERSION  ULONG_ID(0,6,3,0)

/**
*	Application states
*/
/*
#define DynLdStateStarting 0x00
#define DynLdStateRunning  0x01
#define DynLdStateShutDown 0x02
#define DynLdStateDied     0x03*/

#define DynLdStateToFree   0xFF

#define DYNLD_STRATING   0x01
#define DYNLD_RUNNING    0x02

/**
*	Output types
*/
#define DYNLD_TYPELIB 0x00
#define DYNLD_TYPEAPP 0x01

typedef struct TConsole *PConsole;

typedef struct TApplication *PApplication;
////////////////////////////////////////////////////////////////////////////////
/*typedef struct TDynLdEvent *PDynLdEvent;
typedef struct TDynLdEvent {

	struct TEvent Ev;

  void     *Dest;
  void    (*Redirect) ( void *, PDynLdEvent );
} TDynLdEvent;
#define DYNLDEVENT(o) ((PDynLdEvent)(o))*/

////////////////////////////////////////////////////////////////////////////////
typedef struct TApplication {
	l_text Name;
	l_text FileName;
  	l_ulong Version;
	void *Data;

	l_int (*Main)( int argc, l_text *argv );
	void (*Close)(void);

	PList	Ressource;

	l_uchar	State;          // What i am doing ?
	l_char	Type;           // App or Lib ?
	l_int	ThreadId;       // If App, Thread Id
	
	/* ** Due to unknown threading bugs, this section is unactivated **
  PList	EventsWaitList; // If App, Events waiting for app widegts...
  */

  	l_uid 	 UID;
  	l_uid 	*Libs;
  	l_ulong  Users;

	PConsole 	Cons;
	PFile 		StdIn;
	PFile 		StdOut;
	
#ifdef ALLEGRO_H
  	PIcon		Icon;
#else
	void 		*Icon;  
#endif

	int 	argc;
	l_text 	*argv;

	PApplication Host;
	
	
} TApplication;

#define APPLICATION(o) ((PApplication)(o))


typedef struct TDynLdHeader
{
	l_ulong	Magic;
	l_ulong	FileFormatVersion;
	l_ulong	SupportedApiVersion;
	l_ulong	Time;

	l_ulong Relocations;
	l_ulong	Importations;
	l_ulong	Size;

	//l_ulong	UID;
	l_uid   UID;
	l_ulong	FileVersion;
	l_ulong	Flags;

	l_uchar	Type;
	l_ulong	MainOffset;
	l_ulong	CloseOffset;
	l_ulong LibsOffset;

	l_uchar	Compression;
	l_ulong	OriginalSize;
	
	l_ulong RessourceOffset;
	l_ulong	RessourceEntries;
	
} TDynLdHeader, *PDynLdHeader;

typedef struct TDynLdReloc
{
	l_ulong	Type;
	l_ulong	Address;
	l_ulong	Symbol;
} TDynLdReloc, *PDynLdReloc;

typedef struct TDynLdExt
{
	l_text	Name;
	l_ulong	Symbol;
} TDynLdExt, *PDynLdExt;

typedef struct TSymbol
{
	struct TListItem	Itm;
	PApplication		App;
} TSymbol, *PSymbol;

#define SYMBOL(o) ((PSymbol)(o))


#define NoneOffset 0xFFFFFFFF


#define REL32_ABSOLUTE 0x01
#define REL32_RELATIVE 0x02


/**
*	DynLD system
*/
void InitDynLd ( void );
void ShutDownDynLd ( void );

/**
*	Event managment
*/

/* ** Due to unknown threading bugs, this section is unactivated **

void AppSendEvent ( PApplication App, l_ulong Type, l_ulong Msg, void *Extra, void *Dest, void (*Redirect)(void *, PDynLdEvent) );
l_bool AppGetNextEvent ( PApplication App, PDynLdEvent Ev );
void AppRedirectEvent ( PApplication App, PDynLdEvent Ev );
void AppSendEventEv ( PApplication App, TEvent Ev, void *Dest, void (*Redirect) ( void *, PDynLdEvent ) );
/*


/**
*	System functions
*/
PApplication DynLdRun ( l_text Filename, l_text Args );
PApplication DynLdRun2 ( l_text Filename, l_text Args );
PApplication DynLdRunEx ( l_text Filename, l_int argc, l_text* argv, PApplication Host, PErrorHandler h );
void CloseApp ( PApplication App );
PSymbol AddSymbol ( l_text Name, void *Value, PApplication App );
l_long ResolveSymbol ( PApplication App, l_text Name );

/**
*	Export macros
*/
#define APPEXPORT(Name)			AddSymbol(#Name, &Name, &Me)
#define APPEXPORTAS(Func, Name)	AddSymbol(Name, &Func, &Me)
#define SYSEXPORT(Name)			AddSymbol(#Name, (void*)&Name, NULL)
#define SYSEXPORTAS(Func, Name)	AddSymbol(Name, (void*)&Func, NULL)

/**
*	Application handle
*/
extern TApplication Me;

/**
*	Export system
*/
void RegisterExports();

l_bool DynLdInstallLibrary ( l_text file );

void _ParseArgs ( l_text Args, l_text Initial, int *argc, l_text **argv );
l_text *DuplicateArgs (  int argc, l_text *argv );
void FreeArgs (  int argc, l_text *argv );

void BreakApp ( PApplication App );
PApplication DynLdCommand ( l_text Command );

void RegistryLoadLibraries ( l_text Name );


typedef struct TAppExtender {
	
	void (*Init) (PApplication App);
	
	void (*DeInit) (PApplication App);
	
} TAppExtender, *PAppExtender;

PAppExtender InstallAppExtender ( void (*Init)(PApplication), void (*DeInit)(PApplication) );
void RemoveAppExtender ( PAppExtender o);

#endif /* _DYNLD_H_INCLUDED_ */
