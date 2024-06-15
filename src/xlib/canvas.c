#include "kernel.h"
#include "widget.h"
#include "canvas.h"

l_ulong	AppVersion = ULONG_ID(0,8,0,0);
l_char	AppName[] = "Canvas widget";
l_uid	nUID = "canvas";
l_uid NeededLibs[] = { "widget", "" };


/**
*	NAME: CDraw
*	DESCRIPTION: The standard drawing function
*/
_PUBLIC void  CDraw(PWidget o, p_bitmap buffer, PRect w)
{
	rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, o->BackgroundColor);
}

/**
*	NAME: CEventHandler
*	DESCRIPTION: The standard event handler
*/
_PUBLIC l_bool  CEventHandler(PWidget o, PEvent Ev )
{
	return false;
}

/**
*	NAME: CreateCanvas
*	DESCRIPTION: Creates a new canvas widget
*/
_PUBLIC PCanvas  CreateCanvas(PApplication App, TRect r)
{
	PCanvas c = malloc(sizeof(TCanvas));

	if (!c) return NULL;

	memset(c, 0, sizeof(TCanvas));

	IntialiseWidget(App, StdWidgetClass, WIDGET(c), r );

	WIDGET(c)->Draw = &CDraw;
	WIDGET(c)->EventHandler = &CEventHandler;

	return c;
}

l_bool LibMain ( int argc, l_text *argv )
{
	APPEXPORT(CreateCanvas);

	return true;
}

void Close (void)
{

}
