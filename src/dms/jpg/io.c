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
 *      Input/output helper routines module.
 *
 *      See the readme.txt file for instructions on using this package in your
 *      own programs.
 */


#include "jpg/internal.h"


static int file_io_getc(void);
static int file_io_putc(int);
static int memory_io_getc(void);
static int memory_io_putc(int);
static int dummy_io_getc(void);
static int dummy_io_putc(int);

const IO_VTABLE _jpeg_file_io = { NULL, NULL, 0, 0, 0, file_io_getc, file_io_putc };
const IO_VTABLE _jpeg_memory_io = { NULL, NULL, 0, 0, 0, memory_io_getc, memory_io_putc };
const IO_VTABLE _jpeg_dummy_io = { NULL, NULL, 0, 0, 0, dummy_io_getc, dummy_io_putc };

static int current_byte = 0;
static int bytes_read = 0;
static int chunk_len = 0;
static void *chunk = NULL;

// Linux defines those macro, but we get some conflicts....
#ifdef getc
#undef getc
#endif

#ifdef putc
#undef putc
#endif

/* file_io_getc:
 *  Byte input routine for file access mode.
 */
static int
file_io_getc(void)
#undef JPGAL_FUNC
#define JPGAL_FUNC "file_io_getc"
{
	int result = file_get_char(_jpeg_io.file);
	
	if (result < 0) {
		TRACE("Error reading from file");
		jpgalleg_error = JPG_ERROR_READING_FILE;
	}
	return result;
}


/* file_io_putc:
 *  Byte output routine for file access mode.
 */
static int
file_io_putc(int c)
#undef JPGAL_FUNC
#define JPGAL_FUNC "file_io_putc"
{
	if (file_write_char(_jpeg_io.file,c) < 0) {
		TRACE("Error writing to file");
		jpgalleg_error = JPG_ERROR_WRITING_FILE;
		return -1;
	}
	return 0;
}


/* memory_io_getc:
 *  Byte input routine for memory access mode.
 */
static int
memory_io_getc(void)
#undef JPGAL_FUNC
#define JPGAL_FUNC "memory_io_getc"
{
	int result;
	
	if (_jpeg_io.buffer_pos >= _jpeg_io.buffer_size) {
		TRACE("Tried to read memory past buffer size");
		jpgalleg_error = JPG_ERROR_INPUT_BUFFER_TOO_SMALL;
		return -1;
	}
	result = _jpeg_io.buffer[_jpeg_io.buffer_pos];
	_jpeg_io.buffer_pos++;
	return result;
}


/* memory_io_putc:
 *  Byte output routine for memory access mode.
 */
static int
memory_io_putc(int c)
#undef JPGAL_FUNC
#define JPGAL_FUNC "memory_io_putc"
{
	if (_jpeg_io.buffer_pos >= _jpeg_io.buffer_size) {
		TRACE("Tried to write memory past buffer size");
		jpgalleg_error = JPG_ERROR_OUTPUT_BUFFER_TOO_SMALL;
		return -1;
	}
	_jpeg_io.buffer[_jpeg_io.buffer_pos] = c;
	_jpeg_io.buffer_pos++;
	return 0;
}


/* dummy_io_getc:
 *  Dummy byte input routine for first pass of optimized encoding mode.
 */
static int
dummy_io_getc()
{
	return 0;
}


/* dummy_io_putc:
 *  Dummy byte output routine for first pass of optimized encoding mode.
 */
static int
dummy_io_putc(int c)
{
	(void)c;
	
	return 0;
}


/* _jpeg_getc:
 *  Reads a byte from the input stream.
 */
INLINE int
_jpeg_getc(void)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_getc"
{
	bytes_read++;
	_jpeg_io.current_bit = 0;
	return _jpeg_io.getc();
}


/* _jpeg_putc:
 *  Writes a byte to the output stream.
 */
INLINE int
_jpeg_putc(int c)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_putc"
{
	return _jpeg_io.putc(c);
}


/* _jpeg_getw:
 *  Reads a word from the input stream.
 */
INLINE int
_jpeg_getw(void)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_getw"
{
	int result;
	
	result = _jpeg_getc() << 8;
	result |= _jpeg_getc();
	return result;
}


/* _jpeg_putw:
 *  Writes a word to the output stream.
 */
INLINE int
_jpeg_putw(int w)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_putw"
{
	int result;
	
	result = _jpeg_putc((w >> 8) & 0xff);
	result |= _jpeg_putc(w & 0xff);
	return result;
}


/* _jpeg_get_bit:
 *  Reads a bit from the input stream.
 */
INLINE int
_jpeg_get_bit(void)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_get_bit"
{
	_jpeg_io.current_bit--;
	if (_jpeg_io.current_bit < 0) {
		current_byte = _jpeg_io.getc();
		_jpeg_io.current_bit = 7;
		if (current_byte == 0xff)
			/* Special encoding for 0xff, which in JPGs is encoded like 2 bytes:
			 * 0xff00. Here we skip the next byte (0x00)
			 */
			_jpeg_io.getc();
		else if (current_byte == -1)
			return -1;
	}
	return (current_byte >> _jpeg_io.current_bit) & 0x1;
}


/* _jpeg_put_bit:
 *  Writes a bit to the output stream.
 */
INLINE int
_jpeg_put_bit(int bit)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_put_bit"
{
	current_byte |= (bit << _jpeg_io.current_bit);
	_jpeg_io.current_bit--;
	if (_jpeg_io.current_bit < 0) {
		if (_jpeg_io.putc(current_byte))
			return -1;
		if (current_byte == 0xff)
			_jpeg_io.putc(0);
		_jpeg_io.current_bit = 7;
		current_byte = 0;
	}
	return 0;
}


/* _jpeg_flush_bits:
 *  Flushes the current byte by filling unset bits with 1.
 */
void
_jpeg_flush_bits(void)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_flush_bits"
{
	while (_jpeg_io.current_bit < 7)
		_jpeg_put_bit(1);
}


/* _jpeg_open_chunk:
 *  Opens a chunk for reading.
 */
void
_jpeg_open_chunk(void)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_open_chunk"
{
	bytes_read = 0;
	chunk_len = _jpeg_getw();
	_jpeg_io.current_bit = 0;
}


/* _jpeg_close_chunk:
 *  Closes the chunk being read, eventually skipping unused bytes.
 */
void
_jpeg_close_chunk(void)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_close_chunk"
{
	while (bytes_read < chunk_len)
		_jpeg_getc();
	_jpeg_io.current_bit = 0;
}


/* _jpeg_eoc:
 *  Returns true if the end of chunk being read is reached, otherwise false.
 */
int
_jpeg_eoc(void)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_eoc"
{
	return (bytes_read < chunk_len) ? FALSE : TRUE;
}


/* _jpeg_new_chunk:
 *  Creates a new chunk for writing.
 */
void
_jpeg_new_chunk(int type)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_new_chunk"
{
	char *c = (char *)malloc(65536);
	
	c[0] = 0xff;
	c[1] = type;
	chunk_len = 2;
	chunk = c;
}


/* _jpeg_write_chunk:
 *  Writes the current chunk to the output stream.
 */
void
_jpeg_write_chunk(void)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_write_chunk"
{
	char *c;
	
	if (!chunk)
		return;
	c = (char *)chunk;
	c[2] = (chunk_len >> 8) & 0xff;
	c[3] = chunk_len & 0xff;
	chunk_len += 2;
	
	while (chunk_len > 0) {
		_jpeg_putc(*c);
		chunk_len--;
		c++;
	}
	free(chunk);
	chunk = NULL;
	_jpeg_io.current_bit = 7;
	current_byte = 0;
}


/* _jpeg_chunk_putc:
 *  Writes a byte to the current chunk.
 */
void
_jpeg_chunk_putc(int c)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_chunk_putc"
{
	char *p = (char *)chunk + chunk_len + 2;
	
	*p = c;
	chunk_len++;
}


/* _jpeg_chunk_putw:
 *  Writes a word to the current chunk.
 */
void
_jpeg_chunk_putw(int w)
#undef JPGAL_FUNC
#define JPGAL_FUNC "_jpeg_chunk_putw"
{
	_jpeg_chunk_putc((w >> 8) & 0xff);
	_jpeg_chunk_putc(w & 0xff);
}
