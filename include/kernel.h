/**
*	Platform information
*/
#include "platform.h"

#include "allegro.h"

#include "gs.h"
#include "types.h"
#include "list.h"
#include "error.h"
#include "dynld.h"
#include "debug.h"
#include "metrics.h"
#include "vfile.h"
#include "image.h"
#include "events.h"
#include "keyboard.h"
#include "mouse.h"
#include "tasks.h"
#include "registry.h"
#include "ini.h"
#include "grfx.h"
#include "screen.h"
#include "skin.h"
#include "text.h"
#include "utext.h"
#include "dms2.h"
#include "thread.h"
#include "timer.h"
#include "gfx.h"
#include "font.h"

/**
*	Kernel information
*/
extern char* KernelName;
extern char* KernelCopyright;
extern int   KernelVersion;
extern char* KernelTextVersion;
extern TApplication SystemApp;

#define SysPoll() PollEvents(); TimersPoll(); PollTasks()

/******************networking
void (*socket_send_byte)(int id,char c);
void (*socket_send)(int id,char *buff,int numbytes);
int (*socket_recieve)(int id,char *buff,int num);
void (*socket_close)(int id);
int (*socket_connect)(int id,char *ipaddress,int port);
int (*socket_bind)(int id,char *ipaddress,int port);
int (*socket_listen)(int id);
int (*socket_accept)(int id);
int (*socket_select)(int id,int usecdelay);
int (*socket_bind)(int id,char *ipaddr,int port);
int (*socket_open)(void);
**/
