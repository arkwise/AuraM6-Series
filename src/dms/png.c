
#include"kernel.h"

l_ulong	AppVersion = ULONG_ID(0, 0, 0, 1);
l_char	AppName[] = "libpng + ldpng";
l_uid	nUID = "png";

/*#define PNG_INTERNAL
#define PNG_NO_EXTERN*/
#include"png.h"
/*
#include"png/png.c"
#include"png/pngset.c"
#include"png/pngget.c"
#include"png/pngrutil.c"
#include"png/pngtrans.c"
#include"png/pngwutil.c"
#include"png/pngread.c"
#include"png/pngrio.c"
#include"png/pngwio.c"
#include"png/pngwrite.c"
#include"png/pngrtran.c"
#include"png/pngwtran.c"
#include"png/pngmem.c"
#include"png/pngerror.c"
#include"png/pngpread.c"
*/

#include <allegro.h>
#include <allegro/internal/aintern.h>

double _png_screen_gamma = 2.2;
int _png_compression_level = Z_BEST_COMPRESSION;

static double get_gamma(void)
{
    if (_png_screen_gamma != -1)
	return _png_screen_gamma;
   return 2.2;
}

static void read_data(png_structp png_ptr, png_bytep data, png_uint_32 length)
{
    PFile f = png_get_io_ptr(png_ptr);
    if ((png_uint_32)FileRead(data, 1,length, f) != length)
	png_error(png_ptr, "read error (loadpng calling FileRead)");
}

static void write_data(png_structp png_ptr, png_bytep data, png_uint_32 length)
{
    PFile f = png_get_io_ptr(png_ptr);
    if ((png_uint_32)FileWrite(data,1, length, f) != length)
	png_error(png_ptr, "write error (loadpng calling FileWrite)");
}

static void flush_data(png_structp png_ptr) { (void)png_ptr; }

#define PNG_BYTES_TO_CHECK 4

static int check_if_png(PFile fp)
{
    char buf[PNG_BYTES_TO_CHECK];

    if (FileRead(buf, 1,PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK)
	return 0;

    return (png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK) == 0);
}

static BITMAP *really_load_png(png_structp png_ptr, png_infop info_ptr, RGB *pal)
{
    BITMAP *bmp;
    PALETTE tmppal;
    png_uint_32 width, height, rowbytes;
    int bit_depth, color_type, interlace_type;
    double image_gamma, screen_gamma;
    int intent;
    int bpp, dest_bpp;
    int number_passes, pass;
    png_uint_32 y;

    png_read_info(png_ptr, info_ptr);

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		 &interlace_type, NULL, NULL);

    png_set_packing(png_ptr);

    if ((color_type == PNG_COLOR_TYPE_GRAY) && (bit_depth < 8))
	png_set_expand(png_ptr);

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	png_set_tRNS_to_alpha(png_ptr);

    if (bit_depth == 16)
	png_set_strip_16(png_ptr);

    if ((color_type == PNG_COLOR_TYPE_GRAY) ||
	(color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
	png_set_gray_to_rgb(png_ptr);

    screen_gamma = get_gamma();

    if (png_get_sRGB(png_ptr, info_ptr, &intent)) {
	png_set_sRGB(png_ptr, info_ptr, intent);
    }
    else {
	if (png_get_gAMA(png_ptr, info_ptr, &image_gamma))
	    png_set_gamma(png_ptr, screen_gamma, image_gamma);
	else
	    png_set_gamma(png_ptr, screen_gamma, 0.45455);
    }

    if (!pal)
	pal = tmppal;

    if (color_type & PNG_COLOR_MASK_COLOR) {
	int num_palette, i;
	png_colorp palette;

	if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette)) {
	    for (i = 0; ((i < num_palette) && (i < 256)); i++) {
		pal[i].r = palette[i].red >> 2;		/* 256 -> 64 */
		pal[i].g = palette[i].green >> 2;
		pal[i].b = palette[i].blue >> 2;
	    }

	    for (; i < 256; i++)
		pal[i].r = pal[i].g = pal[i].b = 0;
	}
    }
    else {
	generate_332_palette(pal);
    }

    number_passes = png_set_interlace_handling(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY)
	bpp = 24;
    else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	bpp = 32;
    else
	bpp = rowbytes * 8 / width;

    if (bpp < 8)
	bpp = 8;

    dest_bpp = _color_load_depth(bpp, (bpp == 32));
    bmp = create_bitmap_ex(bpp, width, height);

    for (pass = 0; pass < number_passes; pass++) {
	for (y = 0; y < height; y++)
	    png_read_rows(png_ptr, &bmp->line[y], NULL, 1);
    }

    if (bitmap_color_depth(bmp) == 32) {
	int x, y, c;

	for (y = 0; y < bmp->h; y++) {
	    for (x = 0; x < bmp->w; x++) {
		c = getpixel(bmp, x, y);
		putpixel(bmp, x, y, makeacol32(c & 0xff,
					       (c>>8) & 0xff,
					       (c>>16) & 0xff,
					       (c>>24) & 0xff));
	    }
	}
    }
    else if (bitmap_color_depth(bmp) == 24) {
	int x, y, c;

	for (y = 0; y < bmp->h; y++) {
	    for (x = 0; x < bmp->w; x++) {
		c = getpixel(bmp, x, y);
		putpixel(bmp, x, y, makecol24(c & 0xff,
					      (c>>8) & 0xff,
					      (c>>16) & 0xff));
	    }
	}
	}
	else if ((bitmap_color_depth(bmp) == 8) && (bitmap_color_depth(screen) == 8)) { // System and PNG do not have the same palette :s
		int x, y, c;
		for (y = 0; y < bmp->h; y++) {
		    for (x = 0; x < bmp->w; x++) {
			c = getpixel(bmp, x, y);
			putpixel(bmp, x, y, makecol(pal[c].r<<2,
						      pal[c].g<<2,
						      pal[c].b<<2));
		    }
		}
    }

    if ( (dest_bpp==8) && (dest_bpp != bpp) ) { // fixes colors as _fixup_loaded_bitmap do not works fine with 8 bpp
    	int x, y, c, m = bitmap_color_depth(bmp);
	    BITMAP *old = bmp;
	    bmp = create_bitmap_ex(8, width, height);
		for (y = 0; y < bmp->h; y++) {
		    for (x = 0; x < bmp->w; x++) {
			c = getpixel(old, x, y);
			putpixel(bmp, x, y, makecol_depth(8,getr_depth(m,c),
						      getg_depth(m,c),
						      getb_depth(m,c)));
		    }
		}
    	destroy_bitmap(old);
	} else if (dest_bpp != bpp)
		bmp = _fixup_loaded_bitmap(bmp, pal, dest_bpp);


    png_read_end(png_ptr, info_ptr);

    return bmp;
}

BITMAP *load_png_ex ( PFile fp, RGB *pal)
{
    BITMAP *bmp;
    png_structp png_ptr;
    png_infop info_ptr;

    if (!check_if_png(fp)) {
	FileClose(fp);
	return NULL;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
				     (void *)NULL, NULL, NULL);
    if (!png_ptr) {
	FileClose(fp);
	return NULL;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
	png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
	FileClose(fp);
	return NULL;
    }

    if (setjmp(png_ptr->jmpbuf)) {
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	FileClose(fp);
	return NULL;
    }

    png_set_read_fn(png_ptr, fp, (png_rw_ptr)read_data);

    png_set_sig_bytes(png_ptr, PNG_BYTES_TO_CHECK);

    bmp = really_load_png(png_ptr, info_ptr, pal);

    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);


    return bmp;
}

BITMAP *load_png( l_text filename, RGB *pal)
{
	PFile fp;
	BITMAP *bmp;
	DebugMessage("Try to load %s...",filename);
	fp = FileOpen(filename, "rb");
	if ( !fp ) return NULL;
	bmp = load_png_ex(fp,pal);
	FileClose(fp);
	return bmp;
}

static int save_hicolour(png_structp png_ptr, BITMAP *bmp, int depth)
{
    unsigned char *row, *p;
    int i, j, c;

    row = malloc(bmp->w * 3);
    if (!row)
	return 0;

    for (i=0; i<bmp->h; i++) {
	p = row;
        for (j = 0; j < bmp->w; j++) {
            c = getpixel(bmp, j, i);
            if (depth == 15) {
		*p++ = getr15(c);
		*p++ = getg15(c);
		*p++ = getb15(c);
            }
	    else {
		*p++ = getr16(c);
		*p++ = getg16(c);
		*p++ = getb16(c);
            }
        }

        png_write_row(png_ptr, row);
    }

    free(row);

    return 1;
}

static int save_truecolour(png_structp png_ptr, BITMAP *bmp)
{
    unsigned char *row, *p;
    int i, j, c;

    row = malloc(bmp->w * 4);
    if (!row)
	return 0;

    for (i=0; i<bmp->h; i++) {
	p = row;
        for (j = 0; j < bmp->w; j++) {
            c = getpixel(bmp, j, i);

	    *p++ = getr32(c);
	    *p++ = getg32(c);
	    *p++ = getb32(c);
	    if ( (getr32(c) == 255) && (getg32(c) == 0) && (getb32(c) == 255) )
	    *p++ = 0;
	    else
	    *p++ = 255-geta32(c);

        }

        png_write_row(png_ptr, row);
    }

    free(row);

    return 1;
}

int save_png_ex ( PFile fp, BITMAP *bmp, AL_CONST RGB *pal)
{
    png_structp png_ptr;
    png_infop info_ptr;
    int depth, colour_type;

    depth = bitmap_color_depth(bmp);
    if ((!bmp) || ((depth == 8) && (!pal)))
	return -1;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
				      (void *)NULL, NULL, NULL);
    if (!png_ptr) {
	FileClose(fp);
	return -1;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
	FileClose(fp);
	png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	return -1;
    }

    if (setjmp(png_ptr->jmpbuf)) {
	FileClose(fp);
	png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	return -1;
    }

    png_set_write_fn(png_ptr, fp, (png_rw_ptr)write_data, flush_data);

    if (depth == 8)
	colour_type = PNG_COLOR_TYPE_PALETTE;
    else if (depth == 32)
	colour_type = PNG_COLOR_TYPE_RGB_ALPHA;
    else
	colour_type = PNG_COLOR_TYPE_RGB;

    png_set_IHDR(png_ptr, info_ptr, bmp->w, bmp->h, 8, colour_type,
		 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
		 PNG_FILTER_TYPE_BASE);

    if (colour_type == PNG_COLOR_TYPE_PALETTE) {
	png_color palette[256];
	int i;

	for (i = 0; i < 256; i++) {
	    palette[i].red = _rgb_scale_6[pal[i].r];   /* 64 -> 256 */
	    palette[i].green = _rgb_scale_6[pal[i].g];
	    palette[i].blue = _rgb_scale_6[pal[i].b];
	}

	png_set_PLTE(png_ptr, info_ptr, palette, 256);
    }

    png_write_info(png_ptr, info_ptr);


    png_set_compression_level(png_ptr, _png_compression_level);

    if ((depth == 15) || (depth == 16)) {
	if (!save_hicolour(png_ptr, bmp, depth)) {
	    FileClose(fp);
	    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	    return -1;
	}
    }
    else if (depth == 32) {
	if (!save_truecolour(png_ptr, bmp)) {
	    FileClose(fp);
	    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	    return -1;
	}
    }
    else
	png_write_image(png_ptr, bmp->line);

    png_write_end(png_ptr, info_ptr);

    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

    return 0;
}

int save_png( l_text filename, BITMAP *bmp, AL_CONST RGB *pal)
{
    PFile fp;
    fp = FileOpen(filename, "wb");
    if ( !fp ) return -1;
	save_png_ex(fp,bmp,pal);
	FileClose(fp);
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
l_bool PNGSave ( PFile file, l_ptr  Data, l_ulong *EndOffset, l_ulong *Extra, PErrorHandler h ) {
	save_png_ex(file,(BITMAP*)Data,NULL);
	if ( EndOffset ) FileGetPos(file,EndOffset);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool PNGLoad ( PFile file, l_ptr *Data, l_ulong *EndOffset, l_ulong *Extra, PErrorHandler h ) {
	(*Data) = (void*)load_png_ex(file,NULL);
	if ( EndOffset ) FileGetPos(file,EndOffset);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool PNGSaveToFile   ( l_text file, l_ptr  Data, l_ulong  *Extra, PErrorHandler h ) {

  	save_png(file,(BITMAP*)Data,NULL);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool PNGLoadFromFile ( l_text file, l_ptr *Data, l_ulong *Extra, PErrorHandler h ) {
	(*Data) = (void*)load_png(file,NULL);
	if ( *Data )
		return true;
	else
		return false;
}
////////////////////////////////////////////////////////////////////////////////
PCodec Png;

l_int LibMain( int argc, l_text *argv )
{
PSubCodec o;
	Png = AddCodec("Portable Network Graphic (.png)",ULONG_ID('P','N','G',' '),0,0,0,NULL,"png","image/png",DTYPE_IMAGE,0 );
	AddSubCodec(Png,GenericBinary);
 	o = AddNewSubCodec(Png,DTYPE_IMAGE,0,&PNGSaveToFile,&PNGLoadFromFile,&PNGSave,&PNGLoad,0,0,0,0 );

	SetDefaultSubCodec(DTYPE_IMAGE,o);
	return true;
}

void Close(void)
{


}
