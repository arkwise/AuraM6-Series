#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "label.h"
#include "button.h"
#include "sound.h"
#include "slider.h"
#include "menu.h"

l_ulong AppVersion	= ULONG_ID(0,0,0,1);
char    AppName[]	= "Volume";
l_uid	nUID		= "ap:vol";
l_uid NeededLibs[]	= { "widget", "mp3","sound","" };

PWindow w;

////////////////////////////////////////////////////////////////////////////////
void SlideDigiNotify ( PSlider o, l_uchar Ctx, l_ulong Value ) {
	
	if ( Ctx == SCTX_SET ) {
		
		SetVolumeDigi(Value);
		
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void SlideMidiNotify ( PSlider o, l_uchar Ctx, l_ulong Value ) {
	
	if ( Ctx == SCTX_SET ) {
		
		SetVolumeMidi(Value);
		
	}
	
}
////////////////////////////////////////////////////////////////////////////////

l_bool AppEventHandler ( PWidget o, PEvent Ev )
{
	if ( Ev->Type == EV_MESSAGE )
	{
		if ( Ev->Message == WM_CLOSE )
		{
			CloseApp(&Me);

			return true;
		}
		
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_int Main ( int argc, l_text *argv )
{
	TRect r;
	PButton b = NULL;
	PSlider sl;
	PLabel l;

	RectAssign(&r,0, 0, 255, 90);
	w = CreateWindow( &Me, r, "Volume", WF_FRAME|WF_CAPTION|WF_MINIMIZE|WF_CENTERED );
	InsertWidget(DeskTop, WIDGET(w));
	WIDGET(w)->AppEvHdl = &AppEventHandler;

	RectAssign(&r,5, 0, 250, 15);
	l = CreateLabel(&Me, r, "Digital volume");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));
	
	RectAssign(&r,5, 20, 250, 40);
	sl = CreateSlider(&Me,r,255);
	sl->Value = VolumeDigi;
	InsertWidget(WIDGET(w), WIDGET(sl));
	sl->Notify = &SlideDigiNotify;
	
	RectAssign(&r,5, 45, 250, 60);
	l = CreateLabel(&Me, r, "MIDI volume");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(w), WIDGET(l));
	
	RectAssign(&r,5, 65, 250, 85);
	sl = CreateSlider(&Me,r,255);
	sl->Value = VolumeMidi;
	InsertWidget(WIDGET(w), WIDGET(sl));
	sl->Notify = &SlideMidiNotify;
	
	WidgetDrawAll(WIDGET(w));
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////
void Close (void)
{
	WidgetDispose(WIDGET(w));
}
////////////////////////////////////////////////////////////////////////////////
