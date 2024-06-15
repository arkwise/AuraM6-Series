#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "label.h"
#include "button.h"
#include "canvas.h"
#include <ft2build.h>  
#include FT_FREETYPE_H
////////////////////////////////////////////////////////////////////////////////
l_ulong AppVersion = ULONG_ID(0,1,0,0);
char    AppName[] = "Sample Application";
l_uid	nUID = "app:sample";
l_uid NeededLibs[] = { "widget", "button","label","window","" };
////////////////////////////////////////////////////////////////////////////////
#define FWIDTH  320
#define FHEIGHT 220
#define MSG_STD		0x20101
#define MSG_FT		0x20102

////////////////////////////////////////////////////////////////////////////////
PCanvas	c 	= 0;
PLabel l = 0;
PTimer t = 0;
l_ulong nb = 0;
l_ubig ltime = 0;
PWindow w = 0;
l_int size = 10;
////////////////////////////////////////////////////////////////////////////////
FT_Library library;
FT_Face face; 
////////////////////////////////////////////////////////////////////////////////
void TimerPoll( void *A ) {
	
		/*size ++;
		if ( size > 80 ) size = 10;
	*/
	nb++;
	if ( ATime-ltime >= 1000 ) {
		ltime = ATime;
		LabelSetText(l,"%d FPS",nb);
		nb = 0;
		
		
	}
	WidgetDraw(WIDGET(c), NULL);
}
////////////////////////////////////////////////////////////////////////////////
void my_draw_bitmap( FT_Bitmap *FTBmp, PImage Out, int dx, int dy ) {
	unsigned int x = 0, y = 0, col = makecol(0,0,0);
	unsigned char *scanline = FTBmp->buffer, max;
	
	for(y=0;y<FTBmp->rows;y++)
	{
		for(x=0;x<FTBmp->width;x++) {
			col = makeacol(255-(*scanline),255-(*scanline),255-(*scanline),255);
			_putpixel32(Out,dx+x,dy+y,col);
			scanline++;
		}
	}
	
} // 06 87 43 64 21
////////////////////////////////////////////////////////////////////////////////
void STDDraw (PWidget o, p_bitmap buffer, PRect w) {
	
 	l_text Text = "oZone is great !";
 	
   	rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,COL_3DFACE); 	

   	textout(buffer, default_font, Text, o->Absolute.a.x, o->Absolute.a.y, makecol(0,0,0));
   	
}
////////////////////////////////////////////////////////////////////////////////
void FTDraw (PWidget o, p_bitmap buffer, PRect w) {
	
 	FT_GlyphSlot slot = face->glyph;
 	FT_UInt glyph_index; 
 	int pen_x = o->Absolute.a.x, pen_y = o->Absolute.a.y;
 	l_text BText = "oZone is great !", Text;
   	rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,COL_3DFACE); 	

   	/*for ( size=10; size<50; size += 5 ) {*/
	   	Text = BText;
   		pen_x = o->Absolute.a.x;
	 	FT_Set_Pixel_Sizes( face,  0,  size ); 
	 	pen_y += size;
	 	while ( *Text ) {
		 	 if ( FT_Load_Char( face, *Text, FT_LOAD_RENDER ) ) continue;  
		 	 
		 	 my_draw_bitmap( &slot->bitmap, buffer, pen_x + slot->bitmap_left, pen_y - slot->bitmap_top );
		 	 pen_x += slot->advance.x >> 6; 
		 	 Text++;
		}
		/*pen_y += 2;
	}*/
}
////////////////////////////////////////////////////////////////////////////////
l_bool AppEventHandler ( PWidget o, PEvent Ev ) {
	if ( Ev->Type == EV_MESSAGE ) {
		if ( Ev->Message == WM_CLOSE ) {
			CloseApp(&Me);
			return true;
		}
		
		if ( Ev->Message == MSG_STD ) {
			WIDGET(c)->Draw = &STDDraw;
			return true;
		}
		if ( Ev->Message == MSG_FT ) {
			
			WIDGET(c)->Draw = &FTDraw;
			return true;
		}
		
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_int Main( int argc, l_text *argv ) {
	TRect r;
	PButton b = NULL;
	l_int Error;
	
	
	Error = FT_Init_FreeType( &library ); 
	
	if ( Error ) { 
		MessageBox ( &Me, "Error", "Unable to initialise FreeTypeLibary", MBI_ERROR|MBB_OK );
		return false;
	}
	
 	Error = FT_New_Face( library, "SYSTEM/fonts/tahoma.TTF", 0, &face ); 
 	if ( Error == FT_Err_Unknown_File_Format ) { 
	 	MessageBox ( &Me, "Error", "Font format is unsupported", MBI_ERROR|MBB_OK ); 
	 	return false;
	} else if ( Error ) { 
		MessageBox ( &Me, "Error", "Font file could not be opened or read, or simply that it is broken", MBI_ERROR|MBB_OK );
		return false;
	}
	
	Error = FT_Set_Pixel_Sizes( face,  0,  18 ); 
	 
	 
	RectAssign(&r,0, 0, FWIDTH+110, FHEIGHT);
	w = CreateWindow( &Me, r, "oZone Free Type", WF_FRAME|WF_CAPTION|WF_MINIMIZE|WF_CENTERED );
	InsertWidget(DeskTop, WIDGET(w));
	WIDGET(w)->AppEvHdl = &AppEventHandler;

	WidgetSize(&r,FWIDTH+5, 0, 100, 15);
	l = CreateLabel(&Me, r,"Statistics");
	InsertWidget(WIDGET(w), WIDGET(l));

	WidgetSize(&r,FWIDTH+15, r.b.y+2, 90, 15);
	l = CreateLabel(&Me, r,"");
	InsertWidget(WIDGET(w), WIDGET(l));
	
	WidgetSize(&r,FWIDTH+15, r.b.y+2, 90, 20);
	b = CreateButton(&Me, r,"Classic",MSG_STD);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,FWIDTH+15, r.b.y+2, 90, 20);
	b = CreateButton(&Me, r,"Free Type",MSG_FT);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,2, 1, FWIDTH-3, FHEIGHT-4);
	c = CreateCanvas(&Me, r);
	WIDGET(c)->Draw = &FTDraw;
	WIDGET(c)->Flags |= WFForceBuffer;
	InsertWidget(WIDGET(w), WIDGET(c));
	
	WidgetDrawAll(WIDGET(w));

	t = NewTimer(&Me,0, &TimerPoll,NULL);
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close( void ) {
	WidgetDispose(WIDGET(w));
	KillTimer(t);
}
////////////////////////////////////////////////////////////////////////////////
