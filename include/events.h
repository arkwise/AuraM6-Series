#ifndef _EVENTS_H_INCLUDED_
#define _EVENTS_H_INCLUDED_

/**
*	Mouse State
*/
typedef struct TMouseState
{
	TPoint   p;
	l_int    z;
	l_ulong  b;
} TMouseState, *PMouseState;

/**
*	Keyboard State
*/
typedef struct TKeybState
{
	l_int    Key;
	l_int    Shift;
} TKeybState, *PKeybState;

/**
*	Basic Event
*/
typedef struct TEvent
{

	l_int    Type;
	l_ulong  Message;
	void    *Extra;

	TKeybState  Keyb;
	TMouseState Mouse;
} TEvent, *PEvent;

#define CLEAR_EVENT(e)  CLEAR(e)

typedef struct TEventGenerators *PEventGenerators;
typedef struct TEventGenerators
{
	void (*Generator)(PEvent Event);
	l_bool Enabled;

	PEventGenerators Next;

}  TEventGenerators;

typedef struct TEventHandlers *PEventHandlers;
typedef struct TEventHandlers
{
	void (*Handler)(PEvent Event);

	PEventHandlers Prev;
	PEventHandlers Next;

} TEventHandlers;

/**
*	Standard event type
*/
#define EV_NOTHING      0x00000000
#define EV_MESSAGE      0x00000001
#define EV_KEYBOARD     0x00000002
#define EV_MOUSE        0x00000004


/**
*	Standard messageges
*/
#define MSG_NOTHING     0
#define MSG_QUIT        1
#define MSG_KILLAPP     2

// See <docs/events.txt> for events messages codes attribution

// 0xFFFF50XX Reserved to common operations ( file, clipboard, drag and drop ...)

#define MSG_COPY     	0xFFFF5000
#define MSG_PASTE    	0xFFFF5001
#define MSG_CUT     	0xFFFF5002
#define MSG_DELETE    0xFFFF5003

void InstallEventGenerator(void(*Generator)(PEvent Event));
void InstallEventHandler(void(*Handler)(PEvent Event));
void RemoveEventHandler(void(*Handler)(PEvent Event));
void GenerateEvent(PEvent Event);
void HandleEvent(PEvent Event);
void EventsThread(void *arg);
void PollEvents(void);
void InitializeEventsSystem();
void ShutdownEventsSystem();

void EventAssign ( TEvent *Ev, l_int Type, l_ulong Message, void *Extra, TKeybState  Keyb, TMouseState Mouse );
PEvent NewEvent ( l_int Type, l_ulong Message, void *Extra, TKeybState  Keyb, TMouseState Mouse );

extern PEventHandlers   EventHandlers;
extern PEventGenerators EventGenerators;

#endif
