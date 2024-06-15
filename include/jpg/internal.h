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
 *      Internal library definitions.
 *
 *      See the readme.txt file for instructions on using this package in your
 *      own programs.
 */


#ifndef _JPGALLEG_INTERNAL_H_
#define _JPGALLEG_INTERNAL_H_

#include <allegro.h>
#include <allegro/internal/aintern.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "jpgalleg.h"
#include "mmx.h"

#ifdef TRACE
	#undef TRACE
#endif
#ifdef DEBUG
	#define TRACE fprintf(stderr, "[JPGalleg: " JPGAL_FUNC "] "); _jpeg_trace
#else
	#define TRACE 1 ? (void)0 : _jpeg_trace
#endif


#define FIX_0_298631336		2446
#define FIX_0_390180644		3196
#define FIX_0_541196100		4433
#define FIX_0_765366865		6270
#define FIX_0_899976223		7373
#define FIX_1_175875602		9633
#define FIX_1_501321110		12299
#define FIX_1_847759065		15137
#define FIX_1_961570560		16069
#define FIX_2_053119869		16819
#define FIX_2_562915447		20995
#define FIX_3_072711026		25172


#define CHUNK_SOI		0xffd8
#define CHUNK_EOI		0xffd9

#define CHUNK_JPG0		0xf0
#define CHUNK_JPG13		0xfd
#define CHUNK_DHP		0xde
#define CHUNK_EXP		0xdf
#define CHUNK_DNL		0xdc
#define CHUNK_DAC		0xcc
#define CHUNK_SOF3		0xc3
#define CHUNK_SOF5		0xc5
#define CHUNK_SOF6		0xc6
#define CHUNK_SOF7		0xc7
#define CHUNK_SOF9		0xc9
#define CHUNK_SOF10		0xca
#define CHUNK_SOF11		0xcb
#define CHUNK_SOF13		0xcd
#define CHUNK_SOF14		0xce
#define CHUNK_SOF15		0xcf
#define CHUNK_APP0		0xe0
#define CHUNK_APP1		0xe1
#define CHUNK_APP2		0xe2
#define CHUNK_APP3		0xe3
#define CHUNK_APP4		0xe4
#define CHUNK_APP5		0xe5
#define CHUNK_APP6		0xe6
#define CHUNK_APP7		0xe7
#define CHUNK_APP8		0xe8
#define CHUNK_APP9		0xe9
#define CHUNK_APP10		0xea
#define CHUNK_APP11		0xeb
#define CHUNK_APP12		0xec
#define CHUNK_APP13		0xed
#define CHUNK_APP14		0xee
#define CHUNK_APP15		0xef

#define CHUNK_TEM		0x01
#define CHUNK_RST0		0xffd0
#define CHUNK_RST1		0xffd1
#define CHUNK_RST2		0xffd2
#define CHUNK_RST3		0xffd3
#define CHUNK_RST4		0xffd4
#define CHUNK_RST5		0xffd5
#define CHUNK_RST6		0xffd6
#define CHUNK_RST7		0xffd7

#define CHUNK_JPG		0xc8

#define CHUNK_SOF0		0xc0
#define CHUNK_SOF1		0xc1
#define CHUNK_SOF2		0xc2
#define CHUNK_DHT		0xc4
#define CHUNK_SOS		0xda
#define CHUNK_DQT		0xdb
#define CHUNK_DRI		0xdd
#define CHUNK_COM		0xfe

#define SOF0_DEFINED		0x01
#define DHT_DEFINED		0x02
#define SOS_DEFINED		0x04
#define DQT_DEFINED		0x08
#define APP0_DEFINED		0x10
#define APP1_DEFINED		0x20
#define DRI_DEFINED		0x40
#define IS_PROGRESSIVE		0x80

#define JFIF_OK			0x1F
#define EXIF_OK			0x2F

#define LUMINANCE		0
#define CHROMINANCE		1

#define PASS_WRITE		0
#define PASS_COMPUTE_HUFFMAN    1


/* This expression is made to return:
 *   Quality = 100 -> Factor = 55
 *   Quality = 50  -> Factor = 10
 *   Quality = 1   -> Factor = 0.05
 * Other values follow a logarithmic curve
 */
#define QUALITY_FACTOR(q)       ((q) < 50 ? (pow(10.0, ((q) - 28.705189) / 21.294811)) : (pow(10.0, ((q) +17.534468) / 67.534468)))


/* Default quality/flags values for save_jpg() and save_memory_jpg() */
#define DEFAULT_QUALITY		75
#define DEFAULT_FLAGS		JPG_SAMPLING_444



typedef struct HUFFMAN_ENTRY
{
	int encoded_value;
	int value;
	int bits_length;
	int frequency;
} HUFFMAN_ENTRY;


typedef struct HUFFMAN_TABLE
{
	HUFFMAN_ENTRY entry[257];
	HUFFMAN_ENTRY *entry_of_length[16];
	HUFFMAN_ENTRY *code[256];
} HUFFMAN_TABLE;


typedef struct HUFFMAN_NODE
{
	HUFFMAN_ENTRY *entry;
	int frequency;
	struct HUFFMAN_NODE *parent;
	struct HUFFMAN_NODE *left, *right;
	struct HUFFMAN_NODE *prev, *next;
} HUFFMAN_NODE;


typedef struct HUFFMAN_TREE
{
	HUFFMAN_NODE *node;
	HUFFMAN_NODE *head, *tail;
	int depth;
} HUFFMAN_TREE;


typedef struct DATA_BUFFER
{
	short data[64];
} DATA_BUFFER;


typedef struct IO_VTABLE
{
	void *file;
	unsigned char *buffer;
	int buffer_pos;
	int buffer_size;
	int current_bit;
	int (*getc)(void);
	int (*putc)(int);
} IO_VTABLE;


extern INLINE int _jpeg_getc(void);
extern INLINE int _jpeg_putc(int);
extern INLINE int _jpeg_putw(int);
extern INLINE int _jpeg_getw(void);
extern INLINE int _jpeg_get_bit(void);
extern INLINE int _jpeg_put_bit(int);
extern void _jpeg_flush_bits(void);
extern void _jpeg_open_chunk(void);
extern void _jpeg_close_chunk(void);
extern int _jpeg_eoc(void);
extern void _jpeg_new_chunk(int);
extern void _jpeg_write_chunk(void);
extern void _jpeg_chunk_putc(int);
extern void _jpeg_chunk_putw(int);

extern BITMAP *_jpeg_decode(RGB *);
extern void _jpeg_mmx_idct(int *, int *, int *, int *);
extern void _jpeg_mmx_ycbcr2rgb(int, int, int, int, int, int, int);
extern void _jpeg_mmx_ycbcr2bgr(int, int, int, int, int, int, int);

extern int _jpeg_encode(BITMAP *, AL_CONST RGB *, int, int);
extern void _jpeg_mmx_rgb2ycbcr(int, short *, short *, short *, short *, short *, short *);
extern void _jpeg_mmx_bgr2ycbcr(int, short *, short *, short *, short *, short *, short *);

extern void _jpeg_trace(const char *, ...);

extern const IO_VTABLE _jpeg_file_io, _jpeg_memory_io, _jpeg_dummy_io;
extern IO_VTABLE _jpeg_io;
extern const unsigned char _jpeg_zigzag_scan[];
extern const char *_jpeg_component_name[];


#endif
