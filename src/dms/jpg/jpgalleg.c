/*
 *         __   _____    ______   ______   ___    ___
 *        /\ \ /\  _ `\ /\  ___\ /\  _  \ /\_ \  /\_ \
 *        \ \ \\ \ \L\ \\ \ \__/ \ \ \L\ \\//\ \ \//\ \      __     __
 *      __ \ \ \\ \  __| \ \ \  __\ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\
 *     /\ \_\/ / \ \ \/   \ \ \L\ \\ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \
 *     \ \____//  \ \_\    \ \____/ \ \_\ \_\/\____\/\____\ \____\ \____ \
 *      \/____/    \/_/     \/___/   \/_/\/_/\/____/\/____/\/____/\/___L\ \
 *                                                                  /\____/
 *                                                                  \_/__/
 *
 *      Version 2.2, by Angelo Mottola, 2000-2004.
 *
 *		Adapted to ozonz GUI by Julien Etelain
 *
 *      Public library functions.
 *
 *      See the readme.txt file for instructions on using this package in your
 *      own programs.
 *
 */

#include"kernel.h"
#include <jpg/internal.h>


l_ulong AppVersion = ULONG_ID(2,2,0,1);
char    AppName[] = "JPG (Alleg) loader/saver";
l_uid	nUID = "jpg";


IO_VTABLE _jpeg_io;

const unsigned char _jpeg_zigzag_scan[64] = {
	 0, 1, 5, 6,14,15,27,28,
	 2, 4, 7,13,16,26,29,42,
	 3, 8,12,17,25,30,41,43,
	 9,11,18,24,31,40,44,53,
	10,19,23,32,39,45,52,54,
	20,22,33,38,46,51,55,60,
	21,34,37,47,50,56,59,61,
	35,36,48,49,57,58,62,63
};

const char *_jpeg_component_name[] = { "Y", "Cb", "Cr" };

int jpgalleg_error = JPG_ERROR_NONE;


/* _jpeg_trace:
 *  Internal debugging routine: prints error to stderr if in debug mode.
 */
void
_jpeg_trace(const char *msg, ...)
{
#ifdef DEBUG
	va_list ap;
	
	va_start(ap, msg);
	vfprintf(stderr, msg, ap);
	va_end(ap);
	fprintf(stderr, "\n");
#else
	(void)msg;
#endif
}


/* load_jpg:
 *  Loads a JPG image from a file into a BITMAP.
 */
BITMAP *
load_jpg( char *filename, RGB *palette)
#undef JPGAL_FUNC
#define JPGAL_FUNC "load_jpg"
{
	PFile f;
	BITMAP *bmp;
	PALETTE pal;
	
	if (!palette)
		palette = pal;
	
	f = FileOpen(filename, "rb");
	if (!f) {
		TRACE("Cannot open %s for reading", filename);
		return NULL;
	}
	_jpeg_io = _jpeg_file_io;
	_jpeg_io.file = f;
	
	bmp = _jpeg_decode(palette);
	
	FileClose(f);
	return bmp;
}




/* save_jpg_ex:
 *  Saves a BITMAP into a JPG file using given quality and subsampling mode.
*/
int
save_jpg_ex( char *filename, BITMAP *bmp, AL_CONST RGB *palette, int quality, int flags)
#undef JPGAL_FUNC
#define JPGAL_FUNC "save_jpg_ex"
{
	PFile f;
	PALETTE pal;
	int result;
	
	if (!palette)
		palette = pal;
	
	f = FileOpen(filename, "rb");
	if (!f) {
		TRACE("Cannot open %s for writing", filename);
		return -1;
	}
	_jpeg_io = _jpeg_file_io;
	_jpeg_io.file = f;
	
	result = _jpeg_encode(bmp, palette, quality, flags);
	
	FileClose(f);
	return result;
}


////////////////////////////////////////////////////////////////////////////////
l_bool JPGSaveToFile   ( l_text file, l_ptr  Data, l_ulong *Extra, PErrorHandler h ) {
	save_jpg_ex(file, (BITMAP*)Data, NULL, DEFAULT_QUALITY, DEFAULT_FLAGS);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool JPGLoadFromFile ( l_text file, l_ptr *Data, l_ulong *Extra, PErrorHandler h ) {
  (*Data) = (void*)load_jpg(file,NULL);
  if ( *Data )
		return true;
	else
		return false;
}
////////////////////////////////////////////////////////////////////////////////
PCodec Jpg;
l_bool LibMain ( int argc, l_text *argv )
{
	
	Jpg = AddCodec("JPEG",ULONG_ID('J','P','G',' '),0,0,0,NULL,"jpg","image/jpeg",DTYPE_IMAGE,0 );
	AddSubCodec(Jpg,GenericBinary);
 	AddNewSubCodec(Jpg,DTYPE_IMAGE,0,&JPGSaveToFile,&JPGLoadFromFile,NULL,NULL,0,0,0,0 );
	

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close ( void ) {
	
	
}
