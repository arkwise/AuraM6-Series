#ifndef _OZDESK_H_INCLUDED_
#define _OZDESK_H_INCLUDED_

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

extern PTHEMEITEM THMPanelFace;
extern PTHEMEITEM THMPanelStart;
extern PTHEMEITEM THMPanelStartOver;
extern PTHEMEITEM THMPanelStartDown;
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

/**
*	Button messages
*/
#define BTN_START		0xFFFF0001

#define IDM_QUIT		0xF00F0001
#define IDM_PROGRAMS	0xF00F0002
#define IDM_CP			0xF00F0003
#define IDM_ABOUT		0xF00F0004
#define DM_PROPERTIES	0xF00F0008


/**
*	Wallpaper draw styles
*/
#define WALLPAPER_BLIT		0
#define WALLPAPER_TILE		1
#define WALLPAPER_CENTER	2

#define DESKTOP_REGISTRY	"SYSTEM/REGS/xdesktop.reg"

void DrawLikeSkin(p_bitmap out, p_bitmap Skin, l_int x1, l_int y1, l_int x2, l_int y2, l_int Left, l_int Right, l_int Top, l_int Bottom);

#endif /* _OZDESK_H_INCLUDED_ */
