/************************************************************************
// File lwp.h - Headers for lwp.c
// Copyright (C) 1997 Paolo De Marino
//
// Original Source Code by Sengan Short (sengan.short@durham.ac.uk)
// and Josh Turpen (snarfy@goodnet.com).
//
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version,
//  with the only exception that all the people in the THANKS file
//  must receive credit.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this library; see the file COPYING.LIB.
//  If not, write to the Free Software Foundation, Inc., 675 Mass Ave,
//  Cambridge, MA 02139, USA.
//
//  For contacting the author send electronic mail to
//     paolodemarino@usa.net
//
//  Or paper mail to
//
//     Paolo De Marino
//     Via Donizetti 1/E
//     80127 Naples
//     Italy
//
// History: see history.txt
************************************************************************/

#ifndef _LWP_H
#define _LWP_H

#include <dpmi.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define LWP_MAIN -1  /* Id of the MAIN thread. Cannot be killed! */
#define LWP_TCP  -2  /* Id of the TCP thread. Cannot be killed too! */

/* for programming IRQ8 */
#define RTC2            0xF
#define RTC4            0xE
#define RTC8            0xD
#define RTC16           0xC
#define RTC32           0xB
#define RTC64           0xA
#define RTC128          0x9
#define RTC256          0x8
#define RTC512          0x7
#define RTC1024         0x6
#define RTC2048         0x5
#define RTC4096         0x4
#define RTC8192         0x3

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
        int owned;     // Is it owned or not?
        int owner_id;  // If yes, by whom?
} lwp_semaphore;

typedef struct
{
   time_t   secs;          // Seconds from 0:0:0 1/1/70 GMT
   short    msecs;         // As from uclock()
} lwp_time;

/* Don't change this struct.  The scheduler needs to know their positions. */
/* If you have to add anything, add it AFTER stack */

typedef void (*lwp_atkill_fct)(void);

#define MAX_ATKILL 32

typedef struct lwp_struct {
	int lwpid;
	struct lwp_struct *next;
	int stklen;
   unsigned int *stack,
/* --------------From now on, can be freely modified--------------*/
				    *stackTop;

	unsigned priority;        /* Number of time slices for this thread */

	enum {
		LWP_RUNNING,
		LWP_SLEEPING,
		LWP_WAIT_SEMAPHORE,
		LWP_WAIT_TRUE,
	   LWP_WAIT_FALSE,
		LWP_DEAD
	} status;

	union {
		lwp_semaphore *what_sema;  /* If status==WAIT_SEMAPHORE			*/
      volatile int  *what_int;   /* If status==WAIT_TRUE|WAIT_FALSE	*/
      lwp_time      wakeup_time; /* If status==SLEEPING					*/
   } waiting;

	void *userptr;                 /* User per-thread pointer			*/

	lwp_atkill_fct atkill[MAX_ATKILL];
} lwp;

/* User functions */
int lwp_init(int irq, int speed);
int lwp_spawn(void (*proc)(void*),void *arg,int stack_length,unsigned priority);
int lwp_atkill( void (*proc)(void));
int lwp_kill(int lwpid);
int lwp_getpid(void);
void lwp_setuserptr(void *usrdata);
void *lwp_getuserptr(void);
void lwp_yield(void);
int lwp_getactive(void);

void     lwp_set_priority(unsigned priority);
unsigned lwp_get_priority(void);

void lwp_sleep(unsigned int secs,unsigned short msecs);
void lwp_wait_true(volatile int *what);   // Wait until what != 0
void lwp_wait_false(volatile int *what);  // Wait until what == 0
void lwp_pulse_true(volatile int *what);  // Free all the tasks waiting for
void lwp_pulse_false(volatile int *what); // what to become true/false
void lwp_wait_semaphore(lwp_semaphore *sema);
int lwp_release_semaphore(lwp_semaphore *sema);
void lwp_init_semaphore(lwp_semaphore *sema);

extern void _lwp_pm_irq8_timer_hook(void);
extern void _lwp_pm_irq0_timer_hook(void);

/* for locking critical code */
extern long  _lwpasm_start;
extern long  _lwpasm_end;
int _lwp_lock_data( void *lockaddr, unsigned long locksize );
int _lwp_unlock_data( void *lockaddr, unsigned long locksize );
int _lwp_lock_code( void *lockaddr, unsigned long locksize );
int _lwp_unlock_code( void *lockaddr, unsigned long locksize );

extern lwp *_lwp_cur;
extern int _lwp_heavy_dbg;/* NOTE - TO BE REMOVED IN FINAL RELEASE! */

/* for blocking a task switch */
volatile void lwp_thread_enable( void );
volatile void lwp_thread_disable( void );

#ifdef __cplusplus
}
#endif

#if 0
#include "lwpconio.h"
#include "lwpstdio.h"
#include "lwppc.h"
#include "lwpstdlib.h"
#endif
#endif
