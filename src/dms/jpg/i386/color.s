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
 *      MMX optimized YCbCr <--> RGB color conversion routines.
 *
 *      See the readme.txt file for instructions on using this package in your
 *      own programs.
 */


#include <jpg/mmx.h>

#ifdef JPGALLEG_MMX


#ifdef DJGPP
	#define FUNC(name)            .globl _##name ; .balign 8, 0x90 ; _##name:
	#define GLOBL(name)           _##name
#else
	#define FUNC(name)            .globl name ; .balign 8, 0x90 ; ##name:
	#define GLOBL(name)           name
#endif


#define ARG1       8(%ebp)
#define ARG2      12(%ebp)
#define ARG3      16(%ebp)
#define ARG4      20(%ebp)
#define ARG5      24(%ebp)
#define ARG6      28(%ebp)
#define ARG7      32(%ebp)


.data
.balign 32, 0x90

/* 10.6 factors */
i_cb_factor_rgb:	.short -22, 113, 0, 0
i_cr_factor_rgb:	.short 90, -46, 0, 0
i_cb_factor_bgr:	.short 113, -22, 0, 0
i_cr_factor_bgr:	.short -46, 90, 0, 0
i_level_shift:		.short 128, 128, 0, 0

f_y_factor:		.short 7, 38, 19, 0
f_cb_factor_rgb:	.short -11, -21, 32, 0
f_cr_factor_rgb:	.short 32, -27, -5, 0
f_cb_factor_bgr:	.short 32, -21, -11, 0
f_cr_factor_bgr:	.short -5, -27, 32, 0


.text



/*
 *	void
 *	_jpeg_mmx_ycbcr2rgb(int addr, int y1, int cb1, int cr1, int y2, int cb2, int cr2)
 */
FUNC(_jpeg_mmx_ycbcr2rgb)
	pushl %ebp;
	movl %esp, %ebp;
	pushl %edi;
	
	movl ARG1, %edi;
	movd ARG2, %mm0;
	movd ARG5, %mm1;
	movq %mm0, %mm6;
	movq %mm1, %mm7;
	punpcklwd %mm0, %mm0;
	punpcklwd %mm1, %mm1;
	punpckldq %mm6, %mm0;
	punpckldq %mm7, %mm1;
	psllw $6, %mm0;			/* mm0 = |     |  y1 |  y1 |  y1 |  in 10.6 fixed format */
	psllw $6, %mm1;			/* mm1 = |     |  y2 |  y2 |  y2 |  in 10.6 fixed format */
	
	movd ARG3, %mm2;
	movd ARG6, %mm3;
	punpcklwd %mm2, %mm2;
	punpcklwd %mm3, %mm3;
	psubw (i_level_shift), %mm2;
	psubw (i_level_shift), %mm3;
	pmullw (i_cb_factor_rgb), %mm2;
	pmullw (i_cb_factor_rgb), %mm3;
	psllq $16, %mm2;		/* mm2 = |     | 113 * (cb1 - 128) | -22 * (cb1 - 128) |     | */
	psllq $16, %mm3;		/* mm3 = |     | 113 * (cb2 - 128) | -22 * (cb2 - 128) |     | */
	
	movd ARG4, %mm4;
	movd ARG7, %mm5;
	punpcklwd %mm4, %mm4;
	punpcklwd %mm5, %mm5;
	psubw (i_level_shift), %mm4;
	psubw (i_level_shift), %mm5;
	pmullw (i_cr_factor_rgb), %mm4;	/* mm4 = |     |     | -46 * (cr1 - 128) |  90 * (cr1 - 128) | */
	pmullw (i_cr_factor_rgb), %mm5;	/* mm5 = |     |     | -46 * (cr2 - 128) |  90 * (cr2 - 128) | */
	
	paddw %mm2, %mm0;
	paddw %mm3, %mm1;
	paddw %mm4, %mm0;
	paddw %mm5, %mm1;
	
	psraw $6, %mm0;
	psraw $6, %mm1;

	packuswb %mm1, %mm0;		/* mm0 = |    B1 | G1 R1 |    B2 | G2 R2 | */

	movq %mm0, (%edi);
	
	emms;
	
	popl %edi;
	popl %ebp;
	ret;



/*
 *	void
 *	_jpeg_mmx_ycbcr2bgr(int addr, int y1, int cb1, int cr1, int y2, int cb2, int cr2)
 */
FUNC(_jpeg_mmx_ycbcr2bgr)
	pushl %ebp;
	movl %esp, %ebp;
	pushl %edi;
	
	movl ARG1, %edi;
	
	movd ARG2, %mm0;
	movd ARG5, %mm1;
	movq %mm0, %mm6;
	movq %mm1, %mm7;
	punpcklwd %mm0, %mm0;
	punpcklwd %mm1, %mm1;
	punpckldq %mm6, %mm0;
	punpckldq %mm7, %mm1;
	psllw $6, %mm0;			/* mm0 = |     |  y1 |  y1 |  y1 |  in 10.6 fixed format */
	psllw $6, %mm1;			/* mm1 = |     |  y2 |  y2 |  y2 |  in 10.6 fixed format */
	
	movd ARG3, %mm2;
	movd ARG6, %mm3;
	punpcklwd %mm2, %mm2;
	punpcklwd %mm3, %mm3;
	psubw (i_level_shift), %mm2;
	psubw (i_level_shift), %mm3;
	pmullw (i_cb_factor_bgr), %mm2;	/* mm2 = |     |     | -22 * (cb1 - 128) | 113 * (cb1 - 128) | */
	pmullw (i_cb_factor_bgr), %mm3;	/* mm3 = |     |     | -22 * (cb2 - 128) | 113 * (cb2 - 128) | */
	
	movd ARG4, %mm4;
	movd ARG7, %mm5;
	punpcklwd %mm4, %mm4;
	punpcklwd %mm5, %mm5;
	psubw (i_level_shift), %mm4;
	psubw (i_level_shift), %mm5;
	pmullw (i_cr_factor_bgr), %mm4;
	pmullw (i_cr_factor_bgr), %mm5;
	psllq $16, %mm4;		/* mm4 = |     |  90 * (cr1 - 128) | -46 * (cr1 - 128) |     | */
	psllq $16, %mm5;		/* mm5 = |     |  90 * (cr2 - 128) | -46 * (cr2 - 128) |     | */
	
	paddw %mm2, %mm0;
	paddw %mm3, %mm1;
	paddw %mm4, %mm0;
	paddw %mm5, %mm1;
	
	psraw $6, %mm0;
	psraw $6, %mm1;

	packuswb %mm1, %mm0;		/* mm0 = |    R1 | G1 B1 |    R2 | G2 B2 | */

	movq %mm0, (%edi);
	
	emms;
	
	popl %edi;
	popl %ebp;
	ret;



/*
 *	void
 *	_jpeg_mmx_rgb2ycbcr(int addr, short *y1, short *cb1, short *cr1, short *y2, short *cb2, short *cr2)
 */
FUNC(_jpeg_mmx_rgb2ycbcr)
	pushl %ebp;
	movl %esp, %ebp;
	pushl %edi;
	pushl %eax;
	pushl %ebx;
	
	movl ARG1, %edi;
	
	pxor %mm7, %mm7;
	movq (%edi), %mm0;
	movq %mm0, %mm3;
	punpcklbw %mm7, %mm0;		/* mm0 = |     |  B1 |  G1 |  R1 | */
	punpckhbw %mm7, %mm3;		/* mm3 = |     |  B2 |  G2 |  R2 | */
	movq %mm0, %mm1;
	movq %mm3, %mm4;
	movq %mm0, %mm2;
	movq %mm3, %mm5;
	
	pmullw (f_y_factor), %mm0;
	pmullw (f_y_factor), %mm3;
	pmullw (f_cb_factor_rgb), %mm1;
	pmullw (f_cb_factor_rgb), %mm4;
	pmullw (f_cr_factor_rgb), %mm2;
	pmullw (f_cr_factor_rgb), %mm5;
	
	movd %mm0, %eax;
	psrlq $16, %mm0;
	movd %mm0, %ebx;
	addw %bx, %ax;
	psrlq $16, %mm0;
	movd %mm0, %ebx;
	addw %bx, %ax;
	sarw $6, %ax;
	subl $128, %eax;
	movl ARG2, %ebx;
	movw %ax, (%ebx);

	movd %mm1, %eax;
	psrlq $16, %mm1;
	movd %mm1, %ebx;
	addw %bx, %ax;
	psrlq $16, %mm1;
	movd %mm1, %ebx;
	addw %bx, %ax;
	sarw $6, %ax;
	movl ARG3, %ebx;
	movw %ax, (%ebx);
	
	movd %mm2, %eax;
	psrlq $16, %mm2;
	movd %mm2, %ebx;
	addw %bx, %ax;
	psrlq $16, %mm2;
	movd %mm2, %ebx;
	addw %bx, %ax;
	sarw $6, %ax;
	movl ARG4, %ebx;
	movw %ax, (%ebx);
	
	movd %mm3, %eax;
	psrlq $16, %mm3;
	movd %mm3, %ebx;
	addw %bx, %ax;
	psrlq $16, %mm3;
	movd %mm3, %ebx;
	addw %bx, %ax;
	sarw $6, %ax;
	subl $128, %eax;
	movl ARG5, %ebx;
	movw %ax, (%ebx);

	movd %mm4, %eax;
	psrlq $16, %mm4;
	movd %mm4, %ebx;
	addw %bx, %ax;
	psrlq $16, %mm4;
	movd %mm4, %ebx;
	addw %bx, %ax;
	sarw $6, %ax;
	movl ARG6, %ebx;
	movw %ax, (%ebx);

	movd %mm5, %eax;
	psrlq $16, %mm5;
	movd %mm5, %ebx;
	addw %bx, %ax;
	psrlq $16, %mm5;
	movd %mm5, %ebx;
	addw %bx, %ax;
	sarw $6, %ax;
	movl ARG7, %ebx;
	movw %ax, (%ebx);
	
	emms;
	
	popl %ebx;
	popl %eax;
	popl %edi;
	popl %ebp;
	ret;



/*
 *	void
 *	_jpeg_mmx_bgr2ycbcr(int addr, short *y1, short *cb1, short *cr1, short *y2, short *cb2, short *cr2)
 */
FUNC(_jpeg_mmx_bgr2ycbcr)
	pushl %ebp;
	movl %esp, %ebp;
	pushl %edi;
	pushl %eax;
	pushl %ebx;
	
	movl ARG1, %edi;
	
	pxor %mm7, %mm7;
	movq (%edi), %mm0;
	movq %mm0, %mm3;
	punpcklbw %mm7, %mm0;		/* mm0 = |     |  R1 |  G1 |  B1 | */
	punpckhbw %mm7, %mm3;		/* mm3 = |     |  R2 |  G2 |  B2 | */
	movq %mm0, %mm1;
	movq %mm3, %mm4;
	movq %mm0, %mm2;
	movq %mm3, %mm5;
	
	pmullw (f_y_factor), %mm0;
	pmullw (f_y_factor), %mm3;
	pmullw (f_cb_factor_bgr), %mm1;
	pmullw (f_cb_factor_bgr), %mm4;
	pmullw (f_cr_factor_bgr), %mm2;
	pmullw (f_cr_factor_bgr), %mm5;
	
	movd %mm0, %eax;
	psrlq $16, %mm0;
	movd %mm0, %ebx;
	addw %bx, %ax;
	psrlq $16, %mm0;
	movd %mm0, %ebx;
	addw %bx, %ax;
	sarw $6, %ax;
	subl $128, %eax;
	movl ARG2, %ebx;
	movw %ax, (%ebx);

	movd %mm1, %eax;
	psrlq $16, %mm1;
	movd %mm1, %ebx;
	addw %bx, %ax;
	psrlq $16, %mm1;
	movd %mm1, %ebx;
	addw %bx, %ax;
	sarw $6, %ax;
	movl ARG3, %ebx;
	movw %ax, (%ebx);
	
	movd %mm2, %eax;
	psrlq $16, %mm2;
	movd %mm2, %ebx;
	addw %bx, %ax;
	psrlq $16, %mm2;
	movd %mm2, %ebx;
	addw %bx, %ax;
	sarw $6, %ax;
	movl ARG4, %ebx;
	movw %ax, (%ebx);
	
	movd %mm3, %eax;
	psrlq $16, %mm3;
	movd %mm3, %ebx;
	addw %bx, %ax;
	psrlq $16, %mm3;
	movd %mm3, %ebx;
	addw %bx, %ax;
	sarw $6, %ax;
	subl $128, %eax;
	movl ARG5, %ebx;
	movw %ax, (%ebx);

	movd %mm4, %eax;
	psrlq $16, %mm4;
	movd %mm4, %ebx;
	addw %bx, %ax;
	psrlq $16, %mm4;
	movd %mm4, %ebx;
	addw %bx, %ax;
	sarw $6, %ax;
	movl ARG6, %ebx;
	movw %ax, (%ebx);

	movd %mm5, %eax;
	psrlq $16, %mm5;
	movd %mm5, %ebx;
	addw %bx, %ax;
	psrlq $16, %mm5;
	movd %mm5, %ebx;
	addw %bx, %ax;
	sarw $6, %ax;
	movl ARG7, %ebx;
	movw %ax, (%ebx);
	
	emms;
	
	popl %ebx;
	popl %eax;
	popl %edi;
	popl %ebp;
	ret;


#endif
