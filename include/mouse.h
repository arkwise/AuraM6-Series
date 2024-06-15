
/**	
	@addtogroup mouse
	@{
*/
/**
	@file
	Mouse header file.
*/

#ifndef _MOUSE_H_INCLUDED_
#define _MOUSE_H_INCLUDED_

typedef struct TCursor *PCursor;
typedef struct TCursor
{
	p_bitmap	Image;
	l_ulong		SpotX;
	l_ulong		SpotY;
} TCursor;

typedef struct TMouse *PMouse;
typedef struct TMouse
{
	
	TMouseState State;
	
	//TPoint		Where;
	//l_int			Wheel;
	//l_int			Button;


	l_bool		Moved;

	l_bool		Visible;

	TRect		CursorPosition;

	l_int		ButtonChanges;



	PCursor   Cur;

	
	
	
	
	
	
	// Private Data
	p_bitmap PrevScr; // Private Data
	l_int Speed;		// Change theses thru API functions,
	l_int DblClkSpeed;	// otherwise it wont have any efect.
} TMouse;


#define MSG_MOUSE_SCLLUP 	0xFFFF1901
#define MSG_MOUSE_SCLLDWN 0xFFFF1902

/*
*	Mouse buttons
*/
#define BUTTON_LEFT      0x00000001
#define BUTTON_RIGHT     0x00000002
#define BUTTON_MIDDLE    0x00000004

/*
*	Default mouse cursors (defined below)
*/
#define CUR_POINTER     cursor_pointer
#define CUR_RESIZE      cursor_resize
#define CUR_CLOCK       cursor_clock
#define CUR_PENCIL      cursor_pencil

extern PCursor cursor_pointer;
extern PCursor cursor_resize;
extern PCursor cursor_clock;
extern PCursor cursor_pencil;


#define CURSOR_OVER_RECT(r)   _PointInRect(Mouse->State.p, (r))
#define CURSOR_IN_RECT(r)   _PointInRect(Mouse->State.p, (r))

_PUBLIC void  GSSystemMouseInstall();
_PUBLIC void  GSSystemMouseUninstall();

void MouseShow();
void MouseHide();

void MouseSetCursor ( PCursor Cur );
void MouseSetCursorNamed ( l_text Name );

void MouseSetCursorSpeed ( l_int Speed );
void MouseSetDblClkSpeed ( l_int Speed );


/**
	System cursors collection.
	Found cursors using the item key.
	@see ListKey
*/
extern PList Cursors;

/*
*	Why hide/show the mouse each time you draw somewhere under the mouse
*	when you can recap the mouse? Recap will redraw the mouse on rect 'r'
*	on 'from' and before update it PrevScr buffer. <= WTF?
*/
void _MouseRecap ( p_bitmap from, TRect r );

#define MouseRecap(from,r) if (_RectOverlay(Mouse->CursorPosition,r )) _MouseRecap(from,r)

extern PMouse Mouse;

l_bool  __MouseStop ( void );
l_bool  __MouseRestart ( void );

PCursor NewCursor ( p_bitmap Img, l_int SpotX, l_int SpotY );

#endif

/**
@}
*/
