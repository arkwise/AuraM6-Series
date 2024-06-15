////////////////////////////////////////////////////////////////////////////////
//
//	Graphics Extender - Platform relative
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////


#include "kernel.h"
#include "grfx.h"
#include "allegro/internal/aintern.h"

////////////////////////////////////////////////////////////////////////////////
#define GREYSCALE_BLIT(sbits, ssize, dbits, dsize) \
{                                                                            \
   for (y=0; y<h; y++) {                                                	 \
      s = bmp_read_line(src, y+s_y) + s_x*dsize;                             \
      d = bmp_write_line(dest, d_y+y) + d_x*dsize;                           \
     for (x=0; x<w; x++) {                                         	 		 \
        bmp_select(src);                                                	 \
        c = bmp_read##sbits(s);                                         	 \
                                                                        	 \
        r = getr##sbits(c);                                             	 \
        g = getg##sbits(c);                                              	 \
        b = getb##sbits(c);                                             	 \
        i = (r+g+b)/3;														 \
        if ( c != bitmap_mask_color(src) ) {								 \
        bmp_select(dest);                                               	 \
        bmp_write##dbits(d, makecol##dbits(i, i, i));                   	 \
    	}                                                               	 \
        s += ssize;                                                     	 \
        d += dsize;                                                     	 \
     }                                                                  	 \
   }                                                                         \
                                                                             \
   bmp_unwrite_line(src);                                                    \
   bmp_unwrite_line(dest);                                                   \
}
////////////////////////////////////////////////////////////////////////////////
#define GREYSCALE_BLIT_EX(bpp,size) \
{\
	\
	switch (bitmap_color_depth(dest)) {\
      case 8:\
         GREYSCALE_BLIT(bpp, size, 8, 1)\
         break;\
      case 15:\
         GREYSCALE_BLIT(bpp, size, 15, sizeof(short))\
         break;\
      case 16:\
         GREYSCALE_BLIT(bpp, size, 16, sizeof(short))\
         break;\
      case 24:\
         GREYSCALE_BLIT(bpp, size, 24, 3)\
         break;\
      case 32:\
         GREYSCALE_BLIT(bpp, size, 32, sizeof(long))\
         break;\
   } \
}
////////////////////////////////////////////////////////////////////////////////
static int get_replacement_mask_color(BITMAP *bmp)
{
   int depth, c, g = 0;

   depth = bitmap_color_depth(bmp);

   if (depth == 8) {
      if (rgb_map)
         return rgb_map->data[31][1][31];
      else
         return bestfit_color(_current_palette, 63, 1, 63);
   }
   else {
      do
         c = makecol_depth(depth, 255, ++g, 255);
      while (c == bitmap_mask_color(bmp));

      return c;
   }
}
////////////////////////////////////////////////////////////////////////////////

static int makecol_trans(BITMAP *src, BITMAP *dest, int c, int r, int g, int b)
{
   static int rc;
   static BITMAP *rc_bmp = NULL;

   if (c == bitmap_mask_color(src)) {
      c = bitmap_mask_color(dest);
      return c;
   }

   c = makecol_depth(bitmap_color_depth(dest), r, g, b);

   if (c == bitmap_mask_color(dest)) {
      if (dest != rc_bmp) {
         rc = get_replacement_mask_color(dest);
         rc_bmp = dest;
      }
      c = rc;
   }

   return c;
}
////////////////////////////////////////////////////////////////////////////////
void draw_sprite_greyscale ( p_bitmap dest, p_bitmap src, l_int d_x, l_int d_y ) {

   int x, y, c, r, g, b,i,w=src->w,h=src->h,s_x=0,s_y=0;
   unsigned long s, d;

   if (d_x < dest->cl) {
      d_x -= dest->cl;
      w += d_x;
      s_x -= d_x;
      d_x = dest->cl;
   }
   if (d_y < dest->ct) {
      d_y -= dest->ct;
      h += d_y;
      s_y -= d_y;
      d_y = dest->ct;
   }

   if (d_x+w > dest->cr)
      w = dest->cr - d_x;

   if (d_y+h > dest->cb)
      h = dest->cb - d_y;

   if ((w <= 0) || (h <= 0))
      return;

	switch (bitmap_color_depth(src)) {
      case 8:
         GREYSCALE_BLIT_EX( 8, 1)
         break;
      case 15:
         GREYSCALE_BLIT_EX( 15, sizeof(short))
         break;
      case 16:
         GREYSCALE_BLIT_EX( 16, sizeof(short))
         break;
      case 24:
         GREYSCALE_BLIT_EX( 24, 3)
         break;
      case 32:
         GREYSCALE_BLIT_EX( 32, sizeof(long))
         break;
   }

}
////////////////////////////////////////////////////////////////////////////////
void draw_sprite_tile (p_bitmap dest, p_bitmap src, l_int x, l_int y, l_int w, l_int h)
{
	l_int xx = x;
	l_int yy = y;

	for ( xx = 0; xx < w; xx += w)
		for ( yy = 0; yy < h; yy += h)
    		blit(src, dest, 0, 0, x, y, src->w, src->h);
}
