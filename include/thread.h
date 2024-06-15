
#ifndef _thread_h_included_
#define _thread_h_included_

#define Yield()               
#define NewThread(p,a,s,r)    
#define KillThread(i)         
#define ThreadId()            
#define PauseMultitasking()   
#define UnPauseMultitasking() 

/*

#if (defined KERNEL_FILE) && (defined LWP_THREAD)

#include "lwp.h"

#define Yield()               lwp_yield()
#define NewThread(p,a,s,r)    lwp_spawn(p,a,s,r)
#define KillThread(i)         lwp_kill(i)
#define ThreadId()            lwp_getpid()
#define PauseMultitasking()   lwp_thread_disable()
#define UnPauseMultitasking() lwp_thread_enable()

#else 

void Yield (void);
int  NewThread ( void (*proc)(void*),void *arg,int stack_length,unsigned priority );
int  KillThread ( int Id );
int  ThreadId ( void );
void PauseMultitasking ( void );
void UnPauseMultitasking ( void );

#endif

l_bool ThreadInit ( void );
void   ThreadUnInit ( void );

*/

#endif /* _thread_h_included_ */
