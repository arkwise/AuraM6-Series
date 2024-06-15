#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "canvas.h"

l_ulong UID			= ULONG_ID('I','M','M','G');
l_ulong AppVersion	= ULONG_ID(0,1,0,0);
char AppName[]		= "Immagine";
l_uid	nUID		= "app:immagine";
l_uid NeededLibs[]	= { "canvas","window","" };

PWindow w = 0;
PCanvas c = 0;
p_bitmap BMP = 0;

l_bool AppEventHandler ( PWidget o, PEvent Event )
{
	if ( Event->Type == EV_MESSAGE )
	{
		switch ( Event->Message )
		{
			case WM_CLOSE:
			{
				WidgetDispose(WIDGET(o));
				CloseApp(&Me);
				return true;
			}
			break;
		}
	}

	return false;
}

void Draw ( PWidget o, p_bitmap buffer, PRect w ){
	rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, makecol(0,0,0));

	if ( BMP ){
		draw_sprite( buffer, BMP, o->Absolute.a.x, o->Absolute.a.y);
	}
}

l_int Main ( int argc, l_text *argv )
{
	TRect r;
	TRect rr;

	if ( argc > 0 ){
		BMP = LoadData2(argv[1],TYPE_IMAGE);
		if ( !BMP ) return false;
	} else
		return false;

	RectAssign(&r, 0, 0, BMP->w, BMP->h);
	w = CreateWindow(&Me, r, "Immagine v0.2", WF_NORMAL|WF_CENTERED);
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	InsertWidget(WIDGET(DeskTop), WIDGET(w));

	RectAssign(&r,0,0, BMP->w, BMP->h);
	c = CreateCanvas(&Me, r);
	WIDGET(c)->Draw = &Draw;
	InsertWidget(WIDGET(w), WIDGET(c));

	WidgetDrawAll(WIDGET(w));

	WindowSetCaption(w,"Immagine v0.2 - %s",argv[1]);

	return true;
}

void Close (void)
{

}
