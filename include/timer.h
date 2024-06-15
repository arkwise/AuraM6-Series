////////////////////////////////////////////////////////////////////////////////
//
//  Timer Header File
//
//  (c) Copyright 2003 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _timer_h_included_
#define _timer_h_included_

#define TIMER(w)	((PTimer)(w))

typedef struct TTimer {
	PApplication App;
	l_ubig LastCall;
	l_ubig Period;
	//l_ulong Msg;
	l_bool  Enable;
	void *Args;
	void (*Poll)(void*);
} TTimer, *PTimer;


// Those timers (PTimer) works in an App thread, if there are too long they
//   slow down all other app events, in this case use threading timers.
PTimer NewTimer ( PApplication App, l_ubig Period, void (*Poll)(void*), void *Args  );
void KillTimer ( PTimer o );

/* How make a Threading Timer
  ----------------------------

void MyThreadTimer(void*Args){
  while ( 1 ) {
		ThreadTimerWait([Period]);
		// do stuff....
  }
}

// Initialise timer

l_int TimerId;
TimerId = NewThread(&MyThreadTimer,NULL,4096,1);

// Kill timer

KillThread(TimerId);

*/

void TimerSetPeriod ( PTimer o, l_ubig Period );

void ThreadTimerWait(l_ubig Period);

void TimersPoll ( void );

void InitTimer ( void );
void ShutDownTimer ( void );

extern l_ubig ATime;

#endif /* _timer_h_included_ */
