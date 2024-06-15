#ifndef _SLIDER_H_INCLUDED_
#define _SLIDER_H_INCLUDED_
#include "widget.h"

#define SLIDER(w)	((PSlider)(w))

/**
*	Messages sent upon move of the slider
*/
#define SL_SCROLLUP		0x00FF1100
#define SL_SCROLLDOWN	0x00FF1200

#define SCTX_START 0x01
#define SCTX_SLIDING 0x02
#define SCTX_SET 0x03


typedef struct TSlider *PSlider;
typedef struct TSlider
{
	struct	TWidget o;

	l_ulong	Value;

	l_ulong	Steps;
	
	
	l_char State;
	l_ulong PointValue;
	
	void (*Notify) ( PSlider o, l_uchar Ctx, l_ulong Value );
	
} TSlider;



PSlider CreateSlider( PApplication App, TRect r, l_ulong Steps );

#endif /* _SLIDER_H_INCLUDED_ */
