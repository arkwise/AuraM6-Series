#ifndef _OZDESK_H_INCLUDED_
#define _OZDESK_H_INCLUDED_
#include "menu.h"
/**
*
*/
typedef struct TWMENTRY *PWMENTRY;
typedef struct TWMENTRY
{
	TListItem Itm;
	TRect		BufAbs;
} TWMENTRY;
#define WMENTRY(o) ((PWMENTRY)(o))

/**
*	Traybar icon
*/
typedef struct TTRAYBARICON *PTRAYBARICON;
typedef struct TTRAYBARICON
{
	p_bitmap	Icon;

	l_text		Tooltip;
	PMenu		Menu;

	TRect		BufAbs;

	PWidget		MsgDest;
} TTRAYBARICON;


/**
*	Theme item
*/
typedef struct TTHEMEITEM *PTHEMEITEM;
typedef struct TTHEMEITEM
{
	p_bitmap	Skin;

	l_int		Left;
	l_int		Right;
	l_int		Top;
	l_int		Bottom;
} TTHEMEITEM;

/**
*	Global widgets
*/
extern PWidget GSPanel;
extern PWidget GSSidebar;
extern PWidget GSTraybar;

/**
*	Global sizes
*/
extern l_int GSPanelPosition;
extern l_int GSPanelHeight;

/**
*	Global bitmaps
*/
extern p_bitmap GSWallpaper;

extern PTHEMEITEM THMAlertFace;
extern PTHEMEITEM THMPanelFace;
extern PTHEMEITEM THMSidebarFace;
extern PTHEMEITEM THMSidebarClock;
extern PTHEMEITEM THMTraybarFace;

/**
*	Global colors
*/
extern l_color PanelColor;

/**
*	Sizes defines
*/
#define STARTBTN_WIDTH	55
#define CPBTN_WIDTH	55

/**
*	Button messages
*/
#define BTN_CP		0xFFFF0002
#define BTN_APP		0xFFFF0003
#define BTN_USR		0xFFFF0004
#define BTN_navbtn		0xFFFF0005
#define BTN_Ok 0xFFFF0006
#define BTN_Ok2 0xFFFF0007
#define BTN_Exit 0xFFFF0010

#define IDM_QUIT		0xF00F0001
#define IDM_PROGRAMS	0xF00F0002
#define IDM_CP			0xF00F0003
#define IDM_ABOUT		0xF00F0004
#define DM_PROPERTIES	0xF00F0008
#define IDM_NAV		0xF00F0007
#define IDM_Exit 0xF00F0010
#define IDM_VOL_ADJ	0xF00F4420
#define IDM_MUTE	0xF00F4410
#define IDM_GONNAMIN 0xF00F0444

/**
*	Wallpaper draw styles
*/
#define WALLPAPER_BLIT		0
#define WALLPAPER_TILE		1
#define WALLPAPER_CENTER	2

#define DESKTOP_REGISTRY	"SYSTEM/REGS/xdesktop.reg"

void DrawLikeSkin(p_bitmap out, p_bitmap Skin, l_int x1, l_int y1, l_int x2, l_int y2, l_int Left, l_int Right, l_int Top, l_int Bottom);

#endif /* _OZDESK_H_INCLUDED_ */
