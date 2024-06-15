#include "widget.h"

#ifndef _CANVAS_H_INCLUDED_
#define _CANVAS_H_INCLUDED_

typedef struct TCanvas *PCanvas;
typedef struct TCanvas
{
	struct		TWidget o;
} TCanvas;

#define CANVAS(c)	((PCanvas)(c))

PCanvas CreateCanvas(PApplication App, TRect r);

#endif /* _CANVAS_H_INCLUDED_ */
