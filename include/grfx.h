////////////////////////////////////////////////////////////////////////////////
//
//  Graphics Extender - Header File
//
//  (c) Copyright 2003 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _grfx_h_included_
#define _grfx_h_included_

void DrawDotLineVertical ( BITMAP *place, l_int x, l_int y, l_int h, l_color c );
void DrawDotLineHorizontal ( BITMAP *place, l_int x, l_int y, l_int w, l_color c );
void DrawDotRect ( BITMAP *place, TRect r, l_color c );

void Rect3D ( p_bitmap Bmp, l_int x1, l_int y1, l_int x2, l_int y2, l_int c1, l_int c2 );

l_ulong FontCharLength ( FONT *f, int ch );
l_ulong FontTextLengthEx ( FONT *f, l_text Txt, l_text End, l_bool p  );
void DrawTextEx ( p_bitmap bmp, FONT *f, l_long x, l_long y, l_text Txt, l_text End, l_int color, l_bool p );

l_text WhereInText ( FONT *f, l_long x, l_text Txt, l_text End );
l_text WhereInTextMultiLines ( FONT *f, l_long x, l_long y, l_text Txt, l_text End, l_bool p );
void LocatePointText ( FONT *f, l_text Txt, l_text p, l_long *dy, l_long *dx, l_bool pw );

void DrawNiceTextCenter ( p_bitmap bmp, FONT *f, l_long x, l_long y, l_ulong w, l_text Txt, l_int color );
void DrawNiceText ( p_bitmap bmp, FONT *f, l_long x, l_long y, l_ulong w, l_text Txt, l_int color );

void TextSize ( FONT *f, l_text Txt, l_ulong *w, l_ulong *h );

void ImageGreyScale ( p_bitmap s );
p_bitmap ImageNewGreyScaled ( p_bitmap s );
p_bitmap ImageNewGreyScaleWithMask ( p_bitmap s );



void GetClip ( p_bitmap bmp, PRect r );


void draw_sprite_greyscale ( p_bitmap dest, p_bitmap src, l_int d_x, l_int d_y );
void draw_sprite_tile  ( p_bitmap dest, p_bitmap src, l_int x, l_int y, l_int w, l_int h );

#define SetClip(bmp,r) set_clip_rect(bmp,(r).a.x,(r).a.y,(r).b.x,(r).b.y)



#define DrawIcon16(bmp,icon,x,y) \
if ( icon ) { \
	if ( icon->Icon16 ) \
		draw_rle_sprite(bmp,icon->Icon16,x,y); \
}

#define DrawIcon32(bmp,icon,x,y) \
if ( icon ) { \
	if ( icon->Icon32 ) \
		draw_rle_sprite(bmp,icon->Icon32,x,y); \
}

#define DrawIcon48(bmp,icon,x,y) \
if ( icon ) { \
	if ( icon->Icon48 ) \
		draw_rle_sprite(bmp,icon->Icon48,x,y); \
}
#endif /* _grfx_h_included_ */
