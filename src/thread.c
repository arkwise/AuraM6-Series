
#include"kernel.h"
#include <setjmp.h>
/*
#if !(defined LWP_THREAD) && (defined __DJGPP__)
////////////////////////////////////////////////////////////////////////////////
// Simple Thread System
//
// :( Not working :(
////////////////////////////////////////////////////////////////////////////////

typedef struct TThread *PThread;
typedef struct TThread {
	void (*Proc)(void*);
	void *Arg;
	PThread Next;
	l_uint Id;
	jmp_buf ProcState;
} TThread;

PThread Threads = NULL;
PThread LastThread = NULL;
PThread CurThread = NULL;
PThread OldThread = NULL;

PThread MainThread = NULL;

l_uint Ids = 0;

////////////////////////////////////////////////////////////////////////////////
void Yield (void) {
	if ( Threads ) {
		DebugMessage("Prev : %x",CurThread->Id);
		OldThread = CurThread;
		if ( !CurThread )
			CurThread = Threads;
		else
			CurThread = CurThread->Next?CurThread->Next:Threads;
		DebugMessage("Old : %x, New : %x, Switch to %x",OldThread,CurThread,CurThread->Id);
		if ( OldThread ) {
			DebugMessage("Save : %x",OldThread->Id);
			if(setjmp(OldThread->ProcState) != 0) return;
	  }
		DebugMessage("	Go!");

		longjmp(CurThread->ProcState, 1);
	}
}
////////////////////////////////////////////////////////////////////////////////
static void Go ( void ) {
		DebugMessage("Let's go!");
	//if ( CurThread ) if ( CurThread->Proc ) CurThread->Proc(CurThread->Arg);
	return;
}
////////////////////////////////////////////////////////////////////////////////
PThread AllocThread ( void (*proc)(void*),void *arg,int stack_length ) {
	PThread o = malloc(sizeof(TThread));
	void *Stack ;
	if ( !o ) return NULL;
	memset(o,0,sizeof(TThread));
	Stack = malloc(stack_length);
	if ( !Stack ) { free(o); return NULL; }
	memset(Stack,0,stack_length);

	o->Proc=proc;
	o->Arg=arg;

	setjmp(o->ProcState);

	o->ProcState[0].__esp = (unsigned)Stack;
  o->ProcState[0].__eip = (unsigned)proc;

  if ( !Threads ) {
    Threads    = o;
    LastThread = o;
  } else {
	  LastThread->Next = o;
  }
  o->Id = Ids++;
  return o;
}
////////////////////////////////////////////////////////////////////////////////
int  NewThread ( void (*proc)(void*),void *arg,int stack_length,unsigned priority ){
  PThread o = AllocThread(proc,arg,stack_length);
	if ( o ) {
		DebugMessage("NewThread %x %d",o,o->Id);

		return (int)o;
	} else
		return 0;
}
////////////////////////////////////////////////////////////////////////////////
int  KillThread ( int Id ) {
	PThread o = Threads;
	PThread p = NULL;
	while ( o ) {
		if ( Id == (int)o ) {
			if ( p == NULL ) {
				Threads = o->Next;
				if ( o == LastThread ) LastThread = NULL;
				if ( o == CurThread )  CurThread = NULL;
			} else {
				p->Next = o->Next;
				if ( o == LastThread ) LastThread = p;
				if ( o == CurThread )  CurThread = NULL;
			}
			free((void*)o->ProcState[0].__esp);
			free(o);
			return 1;
		}
		p = o;
		o = o->Next;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
int  ThreadId ( void ) {
	return (int)CurThread;
}
////////////////////////////////////////////////////////////////////////////////
void PauseMultitasking ( void ) {

}
////////////////////////////////////////////////////////////////////////////////
void UnPauseMultitasking ( void ) {

}
////////////////////////////////////////////////////////////////////////////////
l_bool ThreadInit ( void ) {
  MainThread = AllocThread(NULL,NULL,4096);
	CurThread = MainThread;
}
////////////////////////////////////////////////////////////////////////////////
void ThreadUnInit ( void ) {


}
////////////////////////////////////////////////////////////////////////////////
#elif (defined LWP_THREAD) && (defined __DJGPP__)
////////////////////////////////////////////////////////////////////////////////
// LWP Thread Support
////////////////////////////////////////////////////////////////////////////////
#include"lwp.h"

l_bool ThreadInit ( void ) {

	DebugMessage ("Intialise pre-emptive multitasking kernel");
	if (!lwp_init(8, RTC8192))
	{
	//	ExError(NULL,0,ERROR_ENABLETOSTARTLWP,"Kernel","LWP not started");
		return false;
	}
	DebugMessage("Initialised at 8192 Mhz on IRQ 8");

}
////////////////////////////////////////////////////////////////////////////////
void ThreadUnInit ( void ) {



}
////////////////////////////////////////////////////////////////////////////////

#endif*/
