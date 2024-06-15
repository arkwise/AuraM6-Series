#ifndef _GROUPBOX_H_INCLUDED_
#define _GROUPBOX_H_INCLUDED_

#include "widget.h"

typedef struct TGroupbox *PGroupbox;
typedef struct TGroupbox
{
	struct	TWidget o;

	l_text Caption;

} TGroupbox;

#define GROUPBOX(w)	((PGroupbox)(w))

PGroupbox CreateGroupbox( PApplication App, TRect r, l_text Caption );

#endif /* _SCROLLBAR_H_INCLUDED_ */
