////////////////////////////////////////////////////////////////////////////////
//
//    BMP loader/saver
//
//    By Seymour Shlien. (from Allegro)
//
//	(c) Copyright 2003 Seymour Shlien. All rights reserved.
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#define DMS2
#include"kernel.h"
#include"stdio.h"
#include"debug.h"
#include"stdlib.h"
#include"string.h"


l_ulong AppVersion = ULONG_ID(0,0,0,1);
char    AppName[] = "BMP loader/saver";
l_uid	nUID = "bmp";



AL_FUNC(BITMAP *, _fixup_loaded_bitmap, (BITMAP *bmp, PALETTE pal, int bpp));

#define BI_RGB          0
#define BI_RLE8         1
#define BI_RLE4         2
#define BI_BITFIELDS    3

#define OS2INFOHEADERSIZE  12
#define WININFOHEADERSIZE  40

typedef unsigned short WORD;
typedef unsigned char  BYTE;
typedef long           LONG;
typedef unsigned long  DWORD;

typedef struct BITMAPFILEHEADER
{
   unsigned short  bfType;
   unsigned long  bfSize;
   unsigned short bfReserved1;
   unsigned short bfReserved2;
   unsigned long  bfOffBits;
} BITMAPFILEHEADER;


/* Used for both OS/2 and Windows BMP.
 * Contains only the parameters needed to load the image
 */
typedef struct BITMAPINFOHEADER
{
   unsigned long  biWidth;
   unsigned long  biHeight;
   unsigned short biBitCount;
   unsigned long  biCompression;
} BITMAPINFOHEADER;


typedef struct WINBMPINFOHEADER  /* size: 40 */
{
   unsigned long  biWidth;
   unsigned long  biHeight;
   unsigned short biPlanes;
   unsigned short biBitCount;
   unsigned long  biCompression;
   unsigned long  biSizeImage;
   unsigned long  biXPelsPerMeter;
   unsigned long  biYPelsPerMeter;
   unsigned long  biClrUsed;
   unsigned long  biClrImportant;
} WINBMPINFOHEADER;


typedef struct OS2BMPINFOHEADER  /* size: 12 */
{
   unsigned short biWidth;
   unsigned short biHeight;
   unsigned short biPlanes;
   unsigned short biBitCount;
} OS2BMPINFOHEADER;
/*
 * Icon directory entry
 */
typedef struct ICONDIRENTRY {
    BYTE        bWidth;
    BYTE        bHeight;
    BYTE        bColorCount;
    BYTE        bReserved;
    WORD        wPlanes;
    WORD        wBitCount;
    DWORD       dwBytesInRes;
    DWORD       dwImageOffset;
} ICONDIRENTRY;
/*
 * Icon directory
 */
typedef struct ICONDIR {
    WORD           idReserved;
    WORD           idType;
    WORD           idCount;
    ICONDIRENTRY   idEntries[1];
} ICONDIR;
/*
 * Cursor directory entry
 */
typedef struct CURSORDIRENTRY {
    BYTE  bWidth;
    BYTE  bHeight;
    BYTE  bColorCount;
    BYTE  bReserved;
    WORD  wXHotspot;
    WORD  wYHotspot;
    DWORD lBytesInRes;
    DWORD dwImageOffset;
} CURSORDIRENTRY;
/*
 * Cursor directory
 */
typedef struct CURSORDIR {
    WORD           cdReserved;
    WORD           cdType;
    WORD           cdCount;
    CURSORDIRENTRY cdEntries[1];
} CURSORDIR;

////////////////////////////////////////////////////////////////////////////////
// BMP Loader
////////////////////////////////////////////////////////////////////////////////

/* read_bmfileheader:
 *  Reads a BMP file header and check that it has the BMP magic number.
 */
static int read_bmfileheader(PFile f, BITMAPFILEHEADER *fileheader)
{
   file_read_short(f,&fileheader->bfType);
   file_read_long(f,&fileheader->bfSize);
   file_read_short(f,&fileheader->bfReserved1);
   file_read_short(f,&fileheader->bfReserved2);
   file_read_long(f,&fileheader->bfOffBits);

   if (fileheader->bfType != 19778)
      return -1;

   return 0;
}



/* read_win_bminfoheader:
 *  Reads information from a BMP file header.
 */
static int read_win_bminfoheader(PFile f, BITMAPINFOHEADER *infoheader)
{
   WINBMPINFOHEADER win_infoheader;

   file_read_long(f,&win_infoheader.biWidth);
   file_read_long(f,&win_infoheader.biHeight);
   file_read_short(f,&win_infoheader.biPlanes);
   file_read_short(f,&win_infoheader.biBitCount);
   file_read_long(f,&win_infoheader.biCompression);
   file_read_long(f,&win_infoheader.biSizeImage);
   file_read_long(f,&win_infoheader.biXPelsPerMeter);
   file_read_long(f,&win_infoheader.biYPelsPerMeter);
   file_read_long(f,&win_infoheader.biClrUsed);
   file_read_long(f,&win_infoheader.biClrImportant);

   infoheader->biWidth = win_infoheader.biWidth;
   infoheader->biHeight = win_infoheader.biHeight;
   infoheader->biBitCount = win_infoheader.biBitCount;
   infoheader->biCompression = win_infoheader.biCompression;

   return 0;
}


/* read_os2_bminfoheader:
 *  Reads information from an OS/2 format BMP file header.
 */
static int read_os2_bminfoheader(PFile f, BITMAPINFOHEADER *infoheader)
{
   OS2BMPINFOHEADER os2_infoheader;

   file_read_short(f,&os2_infoheader.biWidth);
   file_read_short(f,&os2_infoheader.biHeight);
   file_read_short(f,&os2_infoheader.biPlanes);
   file_read_short(f,&os2_infoheader.biBitCount);

   infoheader->biWidth = os2_infoheader.biWidth;
   infoheader->biHeight = os2_infoheader.biHeight;
   infoheader->biBitCount = os2_infoheader.biBitCount;
   infoheader->biCompression = 0;

   return 0;
}



/* read_bmicolors:
 *  Loads the color palette for 1,4,8 bit formats.
 */
static void read_bmicolors(int ncols, RGB *pal, PFile f,int win_flag)
{
   int i;

   for (i=0; i<ncols; i++) {
      pal[i].b = file_get_char(f) / 4;
      pal[i].g = file_get_char(f) / 4;
      pal[i].r = file_get_char(f) / 4;
      if (win_flag)
	 file_get_char(f);
   }
}



/* read_1bit_line:
 *  Support function for reading the 1 bit bitmap file format.
 */
static void read_1bit_line(int length, PFile f, BITMAP *bmp, int line)
{
   unsigned char b[32];
   unsigned long n;
   int i, j, k;
   int pix;

   for (i=0; i<length; i++) {
      j = i % 32;
      if (j == 0) {
	 n = file_get_mlong(f);
	 for (k=0; k<32; k++) {
	    b[31-k] = (char)(n & 1);
	    n = n >> 1;
	 }
      }
      pix = b[j];
      bmp->line[line][i] = pix;
   }
}



/* read_4bit_line:
 *  Support function for reading the 4 bit bitmap file format.
 */
static void read_4bit_line(int length, PFile f, BITMAP *bmp, int line)
{
   unsigned char b[8];
   unsigned long n;
   int i, j, k;
   int temp;
   int pix;

   for (i=0; i<length; i++) {
      j = i % 8;
      if (j == 0) {
	 file_read_long(f,&n);
	 for (k=0; k<4; k++) {
	    temp = n & 255;
	    b[k*2+1] = temp & 15;
	    temp = temp >> 4;
	    b[k*2] = temp & 15;
	    n = n >> 8;
	 }
      }
      pix = b[j];
      bmp->line[line][i] = pix;
   }
}



/* read_8bit_line:
 *  Support function for reading the 8 bit bitmap file format.
 */
static void read_8bit_line(int length, PFile f, BITMAP *bmp, int line)
{
   unsigned char b[4];
   unsigned long n;
   int i, j, k;
   int pix;

   for (i=0; i<length; i++) {
      j = i % 4;
      if (j == 0) {
	 file_read_long(f,&n);
	 for (k=0; k<4; k++) {
	    b[k] = (char)(n & 255);
	    n = n >> 8;
	 }
      }
      pix = b[j];
      bmp->line[line][i] = pix;
   }
}



/* read_24bit_line:
 *  Support function for reading the 24 bit bitmap file format, doing
 *  our best to convert it down to a 256 color palette.
 */
static void read_24bit_line(int length, PFile f, BITMAP *bmp, int line)
{
   int i, nbytes;
   RGB c;

   nbytes=0;

   for (i=0; i<length; i++) {
      c.b = file_get_char(f);
      c.g = file_get_char(f);
      c.r = file_get_char(f);
      bmp->line[line][i*3+_rgb_r_shift_24/8] = c.r;
      bmp->line[line][i*3+_rgb_g_shift_24/8] = c.g;
      bmp->line[line][i*3+_rgb_b_shift_24/8] = c.b;
      nbytes += 3;
   }

   nbytes = nbytes % 4;
   if (nbytes != 0)
      for (i=nbytes; i<4; i++)
	 file_get_char(f);
}



/* read_image:
 *  For reading the noncompressed BMP image format.
 */
static void read_image(PFile f, BITMAP *bmp, AL_CONST BITMAPINFOHEADER *infoheader)
{
   int i, line;

   for (i=0; i<(int)infoheader->biHeight; i++) {
      line = i;

      switch (infoheader->biBitCount) {

	 case 1:
	    read_1bit_line(infoheader->biWidth, f, bmp, infoheader->biHeight-i-1);
	    break;

	 case 4:
	    read_4bit_line(infoheader->biWidth, f, bmp, infoheader->biHeight-i-1);
	    break;

	 case 8:
	    read_8bit_line(infoheader->biWidth, f, bmp, infoheader->biHeight-i-1);
	    break;

	 case 24:
	    read_24bit_line(infoheader->biWidth, f, bmp, infoheader->biHeight-i-1);
	    break;
      }
   }
}



/* read_RLE8_compressed_image:
 *  For reading the 8 bit RLE compressed BMP image format.
 */
static void read_RLE8_compressed_image(PFile f, BITMAP *bmp, AL_CONST BITMAPINFOHEADER *infoheader)
{
   unsigned char count, val, val0;
   int j, pos, line;
   int eolflag, eopicflag;

   eopicflag = 0;
   line = infoheader->biHeight - 1;

   while (eopicflag == 0) {
      pos = 0;                               /* x position in bitmap */
      eolflag = 0;                           /* end of line flag */

      while ((eolflag == 0) && (eopicflag == 0)) {
	 count = file_get_char(f);
	 val = file_get_char(f);

	 if (count > 0) {                    /* repeat pixel count times */
	    for (j=0;j<count;j++) {
	       bmp->line[line][pos] = val;
	       pos++;
	    }
	 }
	 else {
	    switch (val) {

	       case 0:                       /* end of line flag */
		  eolflag=1;
		  break;

	       case 1:                       /* end of picture flag */
		  eopicflag=1;
		  break;

	       case 2:                       /* displace picture */
		  count = file_get_char(f);
		  val = file_get_char(f);
		  pos += count;
		  line -= val;
		  break;

	       default:                      /* read in absolute mode */
		  for (j=0; j<val; j++) {
		     val0 = file_get_char(f);
		     bmp->line[line][pos] = val0;
		     pos++;
		  }

		  if (j%2 == 1)
		     val0 = file_get_char(f);    /* align on word boundary */
		  break;

	    }
	 }

	 if (pos-1 > (int)infoheader->biWidth)
	    eolflag=1;
      }

      line--;
      if (line < 0)
	 eopicflag = 1;
   }
}



/* read_RLE4_compressed_image:
 *  For reading the 4 bit RLE compressed BMP image format.
 */
static void read_RLE4_compressed_image(PFile f, BITMAP *bmp, AL_CONST BITMAPINFOHEADER *infoheader)
{
   unsigned char b[8];
   unsigned char count;
   unsigned short val0, val;
   int j, k, pos, line;
   int eolflag, eopicflag;

   eopicflag = 0;                            /* end of picture flag */
   line = infoheader->biHeight - 1;

   while (eopicflag == 0) {
      pos = 0;
      eolflag = 0;                           /* end of line flag */

      while ((eolflag == 0) && (eopicflag == 0)) {
	 count = file_get_char(f);
	 val = file_get_char(f);

	 if (count > 0) {                    /* repeat pixels count times */
	    b[1] = val & 15;
	    b[0] = (val >> 4) & 15;
	    for (j=0; j<count; j++) {
	       bmp->line[line][pos] = b[j%2];
	       pos++;
	    }
	 }
	 else {
	    switch (val) {

	       case 0:                       /* end of line */
		  eolflag=1;
		  break;

	       case 1:                       /* end of picture */
		  eopicflag=1;
		  break;

	       case 2:                       /* displace image */
		  count = file_get_char(f);
		  val = file_get_char(f);
		  pos += count;
		  line -= val;
		  break;

	       default:                      /* read in absolute mode */
		  for (j=0; j<val; j++) {
		     if ((j%4) == 0) {
			file_read_short(f,&val0);
			for (k=0; k<2; k++) {
			   b[2*k+1] = val0 & 15;
			   val0 = val0 >> 4;
			   b[2*k] = val0 & 15;
			   val0 = val0 >> 4;
			}
		     }
		     bmp->line[line][pos] = b[j%4];
		     pos++;
		  }
		  break;
	    }
	 }

	 if (pos-1 > (int)infoheader->biWidth)
	    eolflag=1;
      }

      line--;
      if (line < 0)
	 eopicflag = 1;
   }
}
////////////////////////////////////////////////////////////////////////////////
BITMAP *LoadImageBmpSubEx ( PFile f, unsigned long biSize, l_bool Mask ) {
	 RGB    *pal = NULL;
   BITMAPINFOHEADER infoheader;
   BITMAP *bmp;
   PALETTE tmppal;
   int want_palette = TRUE;
   int ncol;
   int bpp, dest_depth;

   /* we really need a palette */
   if (!pal) {
      want_palette = FALSE;
      pal = tmppal;
   }

   if (biSize == WININFOHEADERSIZE) {
      if (read_win_bminfoheader(f, &infoheader) != 0) return NULL;
   } else if (biSize == OS2INFOHEADERSIZE) {
      if (read_os2_bminfoheader(f, &infoheader) != 0) return NULL;
   } else
      return NULL;

   if ( infoheader.biBitCount != 24 ) {
      ncol = 1<<infoheader.biBitCount;
      read_bmicolors(ncol, pal, f, 1);
   };

   if (infoheader.biBitCount == 24) {
      bpp = 24;
      generate_332_palette(pal);
   }
   else
      bpp = 8;


   dest_depth = _color_load_depth(bpp, FALSE);

   bmp = create_bitmap_ex(bpp, infoheader.biWidth, infoheader.biHeight);
   if (!bmp) {
      FileClose(f);
      return NULL;
   }

   clear_bitmap(bmp);

   switch (infoheader.biCompression) {

      case BI_RGB:
	 read_image(f, bmp, &infoheader);
	 break;

      case BI_RLE8:
	 read_RLE8_compressed_image(f, bmp, &infoheader);
	 break;

      case BI_RLE4:
	 read_RLE4_compressed_image(f, bmp, &infoheader);
	 break;

      default:
	 destroy_bitmap(bmp);
	 bmp = NULL;
   }

   if (dest_depth != bpp) {
      /* restore original palette except if it comes from the bitmap */
      if ((bpp != 8) && (!want_palette))
	 pal = NULL;

      bmp = _fixup_loaded_bitmap(bmp, pal, dest_depth);
   }

   /* construct a fake palette if 8-bit mode is not involved */
   if ((bpp != 8) && (dest_depth != 8) && want_palette)
      generate_332_palette(pal);

   return bmp;
}
////////////////////////////////////////////////////////////////////////////////
BITMAP *LoadImageBmpEx(char *filename) {
	 BITMAP *bmp;
   BITMAPFILEHEADER fileheader;
   PFile f;
   unsigned long biSize;
   f = FileOpen(filename, "rb");
   if (!f)
      return NULL;
   if (read_bmfileheader(f, &fileheader) != 0) {
      FileClose(f);
      return NULL;
   }
   file_read_long(f,&biSize);
   bmp = LoadImageBmpSubEx(f,biSize,false);
   FileClose(f);
   return bmp;
}
////////////////////////////////////////////////////////////////////////////////
int SaveImageBmpSubEx (PFile f, BITMAP *bmp/*, AL_CONST RGB *pal*/) {
	 RGB    *pal = NULL;
   PALETTE tmppal;
   int bfSize;
   int depth = bitmap_color_depth(bmp);
   int bpp = (depth == 8) ? 8 : 24;
   int filler = 3 - ((bmp->w*(bpp/8)-1) & 3);
   int c, i, j;

   if (!pal) {
      get_palette(tmppal);
      pal = tmppal;
   }

   /* info_header */
   bfSize = bmp->w * bmp->h * bpp/8;

   file_write_long(f,bmp->w);              /* biWidth */
   file_write_long(f,bmp->h);              /* biHeight */
   file_write_short(f,1);                   /* biPlanes */
   file_write_short(f,bpp);                 /* biBitCount */
   file_write_long(f,0);                   /* biCompression */
   file_write_long(f,bfSize);              /* biSizeImage */
   file_write_long(f,0);                   /* biXPelsPerMeter */
   file_write_long(f,0);                   /* biYPelsPerMeter */

   if (bpp == 8) {
      file_write_long(f,256);              /* biClrUsed */
      file_write_long(f,256);              /* biClrImportant */

      /* palette */
      for (i=0; i<256; i++) {
	 file_write_char(f,_rgb_scale_6[pal[i].b]);
	 file_write_char(f,_rgb_scale_6[pal[i].g]);
	 file_write_char(f,_rgb_scale_6[pal[i].r]);
	 file_write_char(f,0);
      }
   }
   else {
      file_write_long(f,0);                /* biClrUsed */
      file_write_long(f,0);                /* biClrImportant */
   }

   /* image data */
   for (i=bmp->h-1; i>=0; i--) {
      for (j=0; j<bmp->w; j++) {
	 if (bpp == 8) {
	    file_write_char(f,getpixel(bmp, j, i));
	 }
	 else {
	    c = getpixel(bmp, j, i);
	    file_write_char(f,getb_depth(depth, c));
	    file_write_char(f,getg_depth(depth, c));
	    file_write_char(f,getr_depth(depth, c));
	 }
      }

      for (j=0; j<filler; j++)
	 file_write_char(f,0);
   }

   return *allegro_errno;
}
////////////////////////////////////////////////////////////////////////////////

int SaveImageBmpEx ( char *filename, BITMAP *bmp/*, AL_CONST RGB *pal*/) {
   PFile f;
   int bfSize;
   int depth = bitmap_color_depth(bmp);
   int bpp = (depth == 8) ? 8 : 24;

   if (bpp == 8) {
      bfSize = 54                      /* header */
	       +256*4                  /* palette */
	       +bmp->w*bmp->h;         /* image data */
   }
   else {
      bfSize = 54                      /* header */
	       +bmp->w*bmp->h*3;       /* image data */
   }

   f = FileOpen(filename, "wb");
   if (!f)
      return *allegro_errno;

   /* file_header */
   file_write_short(f,0x4D42);              /* bfType ("BM") */
   file_write_long(f,bfSize);              /* bfSize */
   file_write_short(f,0);                   /* bfReserved1 */
   file_write_short(f,0);                   /* bfReserved2 */

   if (bpp == 8)                       /* bfOffBits */
      file_write_long(f,54+256*4);
   else
      file_write_long(f,54);

   /* info_header */
   bfSize = bmp->w * bmp->h * bpp/8;

   file_write_long(f,40);                  /* biSize */

   SaveImageBmpSubEx(f,bmp);

   FileClose(f);
   return *allegro_errno;
}
////////////////////////////////////////////////////////////////////////////////
// Cursor
////////////////////////////////////////////////////////////////////////////////
// TO BE TESTED...
PCursor LoadCur ( l_text file ) {
  PCursor Cur;
  PFile f = FileOpen(file,"rb");
  CURSORDIR *PCursorDir;
  if ( !f ) return NULL;
  PCursorDir = malloc(sizeof(CURSORDIR));
  if ( !PCursorDir ) return NULL;
  file_read_short( f, &(PCursorDir->cdReserved) );
  file_read_short( f, &(PCursorDir->cdType) );
  file_read_short( f, &(PCursorDir->cdCount) );
  PCursorDir = realloc( PCursorDir, ( sizeof( WORD ) * 3 ) +
                                    ( sizeof( CURSORDIRENTRY ) * PCursorDir->cdCount ) );
  if ( !PCursorDir ) return NULL;
  FileRead( &(PCursorDir->cdEntries), sizeof(CURSORDIRENTRY), PCursorDir->cdCount, f );
  Cur = malloc(sizeof(TCursor));
  if ( !Cur ) {  free( PCursorDir); return NULL; };
  memset(Cur,0,sizeof(TCursor));
  FileSetPos(f,&PCursorDir->cdEntries[0].dwImageOffset);
  Cur->Image = LoadImageBmpSubEx(f,WININFOHEADERSIZE,true);
  Cur->SpotX = PCursorDir->cdEntries[0].wXHotspot;
  Cur->SpotY = PCursorDir->cdEntries[0].wYHotspot;
  free( PCursorDir );
  return Cur;
}
////////////////////////////////////////////////////////////////////////////////
// DMS interface
////////////////////////////////////////////////////////////////////////////////
l_bool BMPSave ( PFile file, l_ptr  Data, l_ulong *EndOffset, l_ulong *Extra, PErrorHandler h ) {
	SaveImageBmpSubEx(file,(BITMAP*)Data);
  if ( EndOffset ) FileGetPos(file,EndOffset);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool BMPLoad ( PFile file, l_ptr *Data, l_ulong *EndOffset, l_ulong *Extra, PErrorHandler h ) {
  (*Data) = (void*)LoadImageBmpSubEx(file,WININFOHEADERSIZE,false);
  if ( EndOffset ) FileGetPos(file,EndOffset);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool BMPSaveToFile   ( l_text file, l_ptr  Data, l_ulong *Extra, PErrorHandler h ) {
  SaveImageBmpEx(file,(BITMAP*)Data);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool BMPLoadFromFile ( l_text file, l_ptr *Data, l_ulong *Extra, PErrorHandler h ) {
  (*Data) = (void*)LoadImageBmpEx(file);
  if ( *Data )
		return true;
	else
		return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool CURLoadFromFile ( l_text file, l_ptr *Data, l_ulong *Extra, PErrorHandler h ) {
  (*Data) = (void*)LoadCur(file);
  if ( *Data )
		return true;
	else
		return false;
}
////////////////////////////////////////////////////////////////////////////////
PCodec Bmp;
PCodec Cur;

l_bool LibMain ( int argc, l_text *argv )
{
 DebugMessage("Loading LibMain of BMP.dl");
	Bmp = AddCodec("Windows BMP (load OS/2)",ULONG_ID('B','M','P',' '),0,0,0,NULL,"bmp","image/bmp",DTYPE_IMAGE,0 );
	AddSubCodec(Bmp,GenericBinary);
 	AddNewSubCodec(Bmp,DTYPE_IMAGE,0,&BMPSaveToFile,&BMPLoadFromFile,&BMPSave,&BMPLoad,0,0,0,0 );

	Cur = AddCodec("Windows Cursors (untested)",ULONG_ID('C','U','R',' '),0,0,NULL,NULL,"cur","image/cur",DTYPE_CURSOR,0 );
	AddSubCodec(Cur,GenericBinary);
	AddNewSubCodec(Cur,DTYPE_CURSOR,0,0,&CURLoadFromFile,0,0,0,0,0,0 );

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close ( void ) {


}
