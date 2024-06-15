/////////////////////////////////////////////////////////////////////////////////
//
//	Graphics Extender
//
//	Aura Milestone 3 
//
//	(c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "allegro/internal/aintern.h"

l_uid	nUID = "grfx";
l_ulong	AppVersion = ULONG_ID(0,5,0,0);
l_char	AppName[] = "Graphics Extender";


l_long TabWidth = 6;


/**
*	NAME: Rect3D
*	DESCRIPTION: Draws a beveled rectangle
*/
void Rect3D ( p_bitmap Bmp, l_int x1, l_int y1, l_int x2, l_int y2, l_color c1, l_color c2 )
{
	vline(Bmp,x1,y1,y2,c1);
	hline(Bmp,x1,y1,x2,c1);
	vline(Bmp,x2,y1,y2,c2);
	hline(Bmp,x1,y2,x2,c2);
}

/**
*	NAME: DrawDotLineVertical
*	DESCRIPTION: Draws a vertical dotted line seperated every pixel
*					on the specified bitmap.
*/
void DrawDotLineVertical ( BITMAP *place, l_int x, l_int y, l_int h, l_color c )
{
	l_int a = MIN(y,h);
	l_int b = MAX(y,h);

	while ( a <= b )
	{
		putpixel(place, x, a, c);
		a+=2;
	}
}

/**
*	NAME: DrawDotLineHorizontal
*	DESCRIPTION: Draws a horizontal dotted line seperated every pixel
*					on the specified bitmap.
*/
void DrawDotLineHorizontal ( BITMAP *place, l_int x, l_int y, l_int w, l_color c )
{
	l_int a = MIN(x,w);
	l_int b = MAX(x,w);

	while ( a <= b )
	{
		putpixel(place, a, y, c);
		a+=2;
	}
}

/**
*	NAME: DrawDotrect
*	DESCRIPTION: Draws a dotted rectangle seperated every pixel
*					on the specified bitmap.
*/
void DrawDotRect ( BITMAP *place, TRect r, l_color c )
{
	DrawDotLineHorizontal ( place, r.a.x, r.a.y, r.b.x, c );
	DrawDotLineVertical	( place, r.a.x, r.a.y, r.b.y, c );
	DrawDotLineHorizontal	( place, r.a.x, r.b.y, r.b.x, c );
	DrawDotLineVertical ( place, r.b.x, r.a.y, r.b.y, c );
}

////////////////////////////////////////////////////////////////////////////////
l_ulong FontCharLength ( FONT *f, int ch ) {
	if ( ch != '\r' )
		return f->vtable->char_length(f,ch);
	else
		return 0;
}
////////////////////////////////////////////////////////////////////////////////
l_int TextGetTabWidth ( FONT *f, l_long sx, l_long x ) {
	l_long w = FontCharLength(f,' ')*TabWidth;
	return sx+((((x-sx)/w)+1)*w)-x;
}
////////////////////////////////////////////////////////////////////////////////
l_ulong FontTextLengthEx ( FONT *f, l_text Txt, l_text End, l_bool p  ) {
	l_ulong l = 0;
	while ( *Txt && Txt != End ) {
		
		if ( *Txt == '\t' )
		 	l += TextGetTabWidth(f,0,l);
		else if ( p )
			l += f->vtable->char_length(f,'*');
		else if ( *Txt != '\r' )
			l += f->vtable->char_length(f,(l_int)*Txt);
			
	  Txt++;
	}
	return l;
}
////////////////////////////////////////////////////////////////////////////////
void DrawTextEx ( p_bitmap bmp, FONT *f, l_long x, l_long y, l_text Txt, l_text End, l_int color, l_bool p ) {
	l_long sx = x;
	while ( *Txt && Txt != End ) {
		if ( *Txt == '\t' )
		 	x += TextGetTabWidth(f,sx,x);
		else if ( *Txt != '\r' ) {
			if ( p )
				x += f->vtable->render_char( f, '*', color, -1,bmp, x, y );
			else
				x += f->vtable->render_char( f, *Txt, color, -1,bmp, x, y );
		}
	  Txt++;
  }
}
////////////////////////////////////////////////////////////////////////////////
l_text WhereInText ( FONT *f, l_long x, l_text Txt, l_text End ) {
	if ( x <= 0 ) return Txt;
	while ( *Txt && Txt != End ) {
		if ( *Txt != '\r' )
			x -= f->vtable->char_length(f,(l_int)*Txt);
		if ( x <= 0 ) return Txt;
	  Txt++;
  }
  return Txt;
}
////////////////////////////////////////////////////////////////////////////////
l_text WhereInTextMultiLines ( FONT *f, l_long x, l_long y, l_text Txt, l_text End, l_bool p ) {
	l_int w = 0,sx = x;
	y -= text_height(f)+1;
	if ( x <= 0 && y <= 0 ) return Txt;
	while ( *Txt && Txt != End ) {
		
		if ( *Txt == '\n' ) {
			if ( y <= 0 ) return Txt;
			y -= text_height(f)+1;
		}
		
		if ( y <= 0 ) {
			if ( *Txt != '\r' )
			if ( *Txt != '\n' ) {
				if ( *Txt == '\t' )
					w = TextGetTabWidth(f,sx,x);
				else if ( p )
					w = f->vtable->char_length(f,'*');
				else
					w = f->vtable->char_length(f,(l_int)*Txt);
				if ( x <= w/2 ) return Txt;
				if ( *Txt != '\n' ) x -= w;
			} else if ( x <= 0 ) 
				return Txt;
  	}
  	Txt++;
  }
  return Txt;
}
////////////////////////////////////////////////////////////////////////////////
void TextSize ( FONT *f, l_text Txt, l_ulong *w, l_ulong *h ) {
	l_int x = 0, y = 0;
	(*h) = text_height(f);
	(*w) = 0;
	while ( *Txt ) {
		if ( *Txt == '\n' ) {
			(*h) += text_height(f)+1;
			x = 0;	
		} else if ( *Txt != '\r' ) {
			x += f->vtable->char_length(f,(l_int)*Txt);
		}
		(*w) = max(*w,x);
  		Txt++;
  	}
}
////////////////////////////////////////////////////////////////////////////////
void LocatePointText ( FONT *f, l_text Txt, l_text p, l_long *dy, l_long *dx, l_bool pw ) {
	l_int x = 0, y = 0;
	while ( *Txt ) {
		if ( Txt == p ) {
			if ( dy ) *dy = y;
			if ( dx ) *dx = x;
			return;
		}
		if ( *Txt != '\r' )
		if ( *Txt == '\n' ) {
			y += text_height(f)+1;
			x = 0;
		} else
			if ( *Txt == '\t' )
				x += TextGetTabWidth(f,0,x);
			else if ( pw )
				x += f->vtable->char_length(f,'*');
			else
				x += f->vtable->char_length(f,(l_int)*Txt);
			
  	Txt++;
  }
	if ( Txt == p ) {
		if ( dy ) *dy = y;
		if ( dx ) *dx = x;
		return;
	}
}
////////////////////////////////////////////////////////////////////////////////
void DrawNiceTextCenter ( p_bitmap bmp, FONT *f, l_long x, l_long y, l_ulong w, l_text Txt, l_int color ) {
	l_ulong l = text_length(f,Txt);
	if ( l > w ) {
		x -= (w/2);
		w  = w+x-text_length(f,"...");
		while ( *Txt && x+f->vtable->char_length(f,(l_int)*Txt) < w ) {
			x += f->vtable->render_char( f, *Txt, color, -1, bmp, x, y );
	  	Txt++;
  	}
		textout_ex(bmp, f, "...", x, y, color, -1);
	} else
	   textout_ex(bmp, f, Txt, x - l/2, y, color, -1);
}
////////////////////////////////////////////////////////////////////////////////
void DrawNiceText ( p_bitmap bmp, FONT *f, l_long x, l_long y, l_ulong w, l_text Txt, l_int color ) {
	l_ulong l = text_length(f,Txt);
	if ( w <= text_length(f,"...") ) return;
	if ( l > w ) {
		w  = w+x-text_length(f,"...");
		while ( *Txt && x+f->vtable->char_length(f,(l_int)*Txt) < w ) {
			x += f->vtable->render_char( f, *Txt, color, -1, bmp, x, y );
	  	Txt++;
  	}
  	textout_ex(bmp, f, "...", x, y, color, -1);
	} else
	   textout_ex(bmp, f, Txt, x , y, color, -1);
}
////////////////////////////////////////////////////////////////////////////////
void GetClip ( p_bitmap bmp, PRect r ) {
	if ( bmp->clip ) {
		r->a.x = bmp->cl;
		r->a.y = bmp->ct;
		r->b.x = bmp->cr;
		r->b.y = bmp->cb;
	} else {
		r->a.x = 0;
		r->a.y = 0;
		r->b.x = 0;
		r->b.y = 0;
	}
}
////////////////////////////////////////////////////////////////////////////////
void ImageGreyScale ( p_bitmap b ) {
	l_ulong x=0,y=0; 
	l_int i=0,c=0;
	while ( y < b->h ) {
		x = 0;
		while ( x < b->w ) {
			c = getpixel(b,x,y);
			i = (getr(c)+getg(c)+getb(c))/3;
			putpixel(b,x,y,makecol(i,i,i));
			x++;
		}	
		y++;
	}
}

////////////////////////////////////////////////////////////////////////////////
p_bitmap ImageNewGreyScaled ( p_bitmap s ) {
	p_bitmap b = create_bitmap(s->w,s->h);

	l_ulong x=0,y=0; 
	l_int i=0,c=0;
	
	while ( y < b->h ) {
		x = 0;
		while ( x < b->w ) {
			c = getpixel(s,x,y);
			i = (getr(c)+getg(c)+getb(c))/3;
			putpixel(b,x,y,makecol(i,i,i));
			x++;
		}	
		y++;
	}
	
	return b;
}

p_bitmap ImageNewGreyScaleWithMask ( p_bitmap s )
{
	p_bitmap b = create_bitmap (s->w, s->h);

	l_ulong x = 0, y = 0; 
	l_int i = 0, c = 0, mask = makecol (255, 0, 255);
	
	while (y < b->h)
	{
		x = 0;
		
		while (x < b->w)
		{
			c = getpixel(s,x,y);

			if (c != mask)
			{
				i = (getr(c)+getg(c)+getb(c))/3;
				i = makecol (i,i,i);
			}
			else i = mask;

			putpixel(b,x,y,i);
			x++;
		}

		y++;
	}
	
	return b;
}

////////////////////////////////////////////////////////////////////////////////
l_bool LibMain ( int argc, l_text *argv )
{

	APPEXPORT(Rect3D);
	APPEXPORT(DrawDotLineHorizontal);
	APPEXPORT(DrawDotLineVertical);
	APPEXPORT(DrawDotRect);

	APPEXPORT(FontCharLength);
	APPEXPORT(FontTextLengthEx);
	APPEXPORT(DrawTextEx);
	APPEXPORT(WhereInText);
	APPEXPORT(WhereInTextMultiLines);
	APPEXPORT(LocatePointText);
	APPEXPORT(DrawNiceTextCenter);
	APPEXPORT(DrawNiceText);
	APPEXPORT(GetClip);
	APPEXPORT(TextSize);
	APPEXPORT(ImageGreyScale);
	APPEXPORT(ImageNewGreyScaled);
	APPEXPORT(ImageNewGreyScaleWithMask);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{

}
