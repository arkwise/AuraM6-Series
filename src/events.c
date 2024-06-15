/**
*	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*	FILE:			events.c
*
*	PROJECT:		Phoenix engine - Core
*
*	DESCRIPTION:	Main event handler system for handling events
*
*	CONTRIBUTORS:
*					Lukas Lipka
*
*	TODO:			@
*
*	BUG:			@
*
*	MISTAKE:		@
*
*	IDEA:			@
*/
#define KERNEL_FILE
#include "kernel.h"

PEventGenerators EventGenerators	= NULL;
PEventGenerators LastEventGenerator	= NULL;
PEventHandlers EventHandlers		= NULL;
PEventHandlers LastEventHandler		= NULL;
PEvent Event						= NULL;
l_bool GSExit						= false;

void InstallEventGenerator(void(*Generator)(PEvent Event))
{
	PEventGenerators      p;

	if (!Generator) return;

	if (!EventGenerators)
	{
		EventGenerators = NEW(TEventGenerators);
		EventGenerators->Generator = Generator;
		EventGenerators->Enabled = TRUE;
		EventGenerators->Next = NULL;
		LastEventGenerator = EventGenerators;

		return;
	}

	p = NEW(TEventGenerators);
	p->Generator = Generator;
	p->Enabled = TRUE;
	p->Next = NULL;
	LastEventGenerator->Next = p;
	LastEventGenerator = p;
}

void InstallEventHandler(void(*Handler)(PEvent Event))
{
	PEventHandlers        p;
	if (!Handler) return;

	if (!EventHandlers)
	{
		EventHandlers = NEW(TEventHandlers);
		EventHandlers->Handler = Handler;
		EventHandlers->Prev = LastEventHandler;
		EventHandlers->Next = NULL;
		LastEventHandler = EventHandlers;

		return;
	}

	p = NEW(TEventHandlers);
	p->Handler = Handler;
	p->Prev = LastEventHandler;
	p->Next = NULL;
	LastEventHandler->Next = p;
	LastEventHandler = p;
}

void RemoveEventHandler(void(*Handler)(PEvent Event))
{
	PEventHandlers p;
	p = EventHandlers;

	if (!p) return;

	do
	{
		if (p->Handler == Handler)
		{
			if (p->Next) p->Next->Prev = p->Prev; else LastEventHandler = p->Prev;
			if (p->Prev) p->Prev->Next = p->Next; else EventHandlers = p->Next;

			free(p);

			return;
		}

		p = p->Next;
	}
	while (p);
}

void EventAssign ( TEvent *Ev, l_int Type, l_ulong Message, void *Extra, TKeybState  Keyb, TMouseState Mouse )
{
	Ev->Type    = Type;
	Ev->Message = Message;
	Ev->Extra   = Extra;
	Ev->Keyb    = Keyb;
	Ev->Mouse   = Mouse;
}

PEvent NewEvent ( l_int Type, l_ulong Message, void *Extra, TKeybState  Keyb, TMouseState Mouse )
{
	PEvent Ev = malloc(sizeof(TEvent));
	Ev->Type    = Type;
	Ev->Message = Message;
	Ev->Extra   = Extra;
	Ev->Keyb    = Keyb;
	Ev->Mouse   = Mouse;
	return Ev;
}

void GenerateEvent(PEvent Event)
{
	PEventGenerators p = EventGenerators;
	TEvent ev;

	Event->Type = EV_NOTHING;
	Event->Extra = NULL;
	Event->Message = MSG_NOTHING;

	if (!EventGenerators) return;

	do
	{
		ev.Type = EV_NOTHING;
		ev.Extra = NULL;

		if (p->Enabled)
	    {
			p->Generator(&ev);
			SET(Event->Type, ev.Type);

			Event->Message = ev.Message;

			if (ev.Type & EV_KEYBOARD)
			{
				Event->Keyb = ev.Keyb;

			}

			if (ev.Extra)
				Event->Extra = ev.Extra;
		}

		p = p->Next;
	}
	while (p);

//DebugMessage("+++ GenerateEvent...");

}

void HandleEvent(PEvent Event)
{
	PEventHandlers p = EventHandlers;

	if (!EventHandlers || Event->Type == EV_NOTHING)
 {
// DebugMessage("+++ HandleEvent: EV_NOTHING 1");
		return;
 }

	do
	{
//		PEventHandlers  Next = p->Next; // commented code has been improved
		if (Event && p) p->Handler(Event);

		if (Event->Type == EV_NOTHING)
 {
// DebugMessage("+++ HandleEvent: EV_NOTHING 2");
 return;
 }

//		p = Next;
		p = p->Next;
	} while (p);

	if (Event->Type == EV_MESSAGE && Event->Message == MSG_QUIT)
	{
		GSExit = TRUE;
		DebugMessage("+++ SYSTEM GOING DOWN FOR HALT +++\n");
	}

//DebugMessage("+++ HandleEvent END");

}

void PollEvents(void)
{
	if ( Event )
	{
		GenerateEvent(Event);
		HandleEvent(Event);
	}

}
/*
void EventsThread(void *arg)
{
	while ( 1 )
	{
		PollEvents();
		Yield();
	}
}
*/
void InitializeEventsSystem()
{
	Event = NEW(TEvent);
}

void ShutdownEventsSystem()
{
	if (Event)
		free(Event);
}
