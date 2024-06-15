/////////////////////////////////////////////////////////////////////////////////
//
//	Timer Core System
//
//	Aura Milestone 3 
//
//	(c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////
#define KERNEL_FILE
#include"kernel.h"
#include <string.h>

PList Timers;
l_int TimerThreadId;
l_ubig ATime = 0;
PListItem LastDeleted = 0;

void AClock ( void )
{
        ATime += 1;
}

void TimersPoll ( void )
{
        PListItem a, n;

        if ( !Timers->Last) return;

        a = Timers->Last;

                do
                {
                        n = a->Next;

                        if ( TIMER(a->Data)->Enable )
                        {
                                if ( ATime-TIMER(a->Data)->LastCall >= TIMER(a->Data)->Period )
                                {
                                        TIMER(a->Data)->LastCall = ATime;
                                        TIMER(a->Data)->Poll(TIMER(a->Data)->Args);
                                }
                        }

                        a = n;
                }
                while ( a != Timers->Last );
}

/*void TimerThread ( void*Args ) {
  while ( 1 ) {
          TimersPoll();
          Yield();
  }
}*/

void ThreadTimerWait(l_ubig Period)
{
        l_ubig LastCall = ATime;
        while ( ATime-LastCall < Period ) { SysPoll(); } //Yield();
}

void TimerSetPeriod ( PTimer o, l_ubig Period ) {
        o->Period = Period;
}

PTimer NewTimer ( PApplication App, l_ubig Period, void (*Poll)(void*), void *Args  )
{
        PTimer o = malloc(sizeof(TTimer));
        if ( !o ) return NULL;
        memset(o,0,sizeof(TTimer));

        o->App = App;
        o->Period = Period;
        o->LastCall= ATime;
        o->Poll = Poll;
        o->Args = Args;
        o->Enable = 1;
        ListAdd(Timers, NULL, o, &free);

        return o;
}

void KillTimer ( PTimer o )
{
        PListItem i = ListFoundItem(Timers,o);
        o->Enable = 0;

        if ( i ) ListRemoveItem(Timers,i);
        LastDeleted = i;
}

void InitTimer ( void )
{
        install_int(&AClock, 1);

        Timers = NewList();

        if ( !Timers ) DebugFatal("ERROR_NOTENOUGHMEMORY");

        //TimerThreadId = NewThread(TimerThread, NULL, 4096, 1);

        SYSEXPORT(NewTimer);
        SYSEXPORT(KillTimer);
        SYSEXPORT(ThreadTimerWait);
        SYSEXPORT(ATime);

}

void ShutDownTimer ( void )
{
        //KillThread(TimerThreadId);
        FreeList(Timers);
}
