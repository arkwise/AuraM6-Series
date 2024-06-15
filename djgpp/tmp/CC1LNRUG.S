	.file	"console.c"
	.section .text
	.p2align 4,,15
	.globl	___farsetsel
___farsetsel:
LFB0:
	.cfi_startproc
	movl	_Platform, %edx
	movl	4(%esp), %eax
	testl	%edx, %edx
	jne	L1
/APP
# 92 "include/platform.h" 1
	movw %ax,%fs
# 0 "" 2
/NO_APP
L1:
	ret
	.cfi_endproc
LFE0:
	.p2align 4,,15
	.globl	___farnspokeb
___farnspokeb:
LFB1:
	.cfi_startproc
	movl	4(%esp), %edx
	movl	8(%esp), %eax
	cmpl	$0, _Platform
	je	L7
	movb	%al, (%edx)
	ret
	.p2align 4,,7
L7:
/APP
# 110 "include/platform.h" 1
	.byte 0x64
	movb %al,(%edx)
# 0 "" 2
/NO_APP
	ret
	.cfi_endproc
LFE1:
	.p2align 4,,15
	.globl	___farnspokew
___farnspokew:
LFB2:
	.cfi_startproc
	movl	4(%esp), %edx
	movl	8(%esp), %eax
	cmpl	$0, _Platform
	je	L11
	movw	%ax, (%edx)
	ret
	.p2align 4,,7
L11:
/APP
# 128 "include/platform.h" 1
	.byte 0x64
	movw %ax,(%edx)
# 0 "" 2
/NO_APP
	ret
	.cfi_endproc
LFE2:
	.p2align 4,,15
	.globl	___farnspokel
___farnspokel:
LFB3:
	.cfi_startproc
	movl	_Platform, %ecx
	movl	4(%esp), %eax
	movl	8(%esp), %edx
	testl	%ecx, %ecx
	je	L15
	movl	%edx, (%eax)
	ret
	.p2align 4,,7
L15:
/APP
# 146 "include/platform.h" 1
	.byte 0x64
	movl %edx,(%eax)
# 0 "" 2
/NO_APP
	ret
	.cfi_endproc
LFE3:
	.p2align 4,,15
	.globl	___farnspeekb
___farnspeekb:
LFB4:
	.cfi_startproc
	movl	_Platform, %edx
	movl	4(%esp), %eax
	testl	%edx, %edx
	je	L19
	movb	(%eax), %al
	ret
	.p2align 4,,7
L19:
/APP
# 165 "include/platform.h" 1
	.byte 0x64
	movb (%eax),%al
# 0 "" 2
/NO_APP
	ret
	.cfi_endproc
LFE4:
	.p2align 4,,15
	.globl	___farnspeekw
___farnspeekw:
LFB5:
	.cfi_startproc
	movl	_Platform, %edx
	movl	4(%esp), %eax
	testl	%edx, %edx
	je	L23
	movw	(%eax), %ax
	ret
	.p2align 4,,7
L23:
/APP
# 185 "include/platform.h" 1
	.byte 0x64
	movw (%eax),%ax
# 0 "" 2
/NO_APP
	ret
	.cfi_endproc
LFE5:
	.p2align 4,,15
	.globl	___farnspeekl
___farnspeekl:
LFB6:
	.cfi_startproc
	movl	_Platform, %edx
	movl	4(%esp), %eax
	testl	%edx, %edx
	je	L27
	movl	(%eax), %eax
	ret
	.p2align 4,,7
L27:
/APP
# 205 "include/platform.h" 1
	.byte 0x64
	movl (%eax),%eax
# 0 "" 2
/NO_APP
	ret
	.cfi_endproc
LFE6:
	.p2align 4,,15
	.globl	_install_allegro
_install_allegro:
LFB13:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	movl	40(%esp), %eax
	movl	36(%esp), %edx
	movl	32(%esp), %ecx
	movl	$262658, 12(%esp)
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%ecx, (%esp)
	call	__install_allegro_version_check
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE13:
	.p2align 4,,15
	.globl	_set_window_title
_set_window_title:
LFB14:
	.cfi_startproc
	movl	_system_driver, %eax
	movl	32(%eax), %edx
	testl	%edx, %edx
	je	L30
	jmp	*%edx
	.p2align 4,,7
L30:
	ret
	.cfi_endproc
LFE14:
	.p2align 4,,15
	.globl	_desktop_color_depth
_desktop_color_depth:
LFB15:
	.cfi_startproc
	movl	_system_driver, %eax
	movl	96(%eax), %eax
	testl	%eax, %eax
	je	L33
	jmp	*%eax
	.p2align 4,,7
L33:
	xorl	%eax, %eax
	ret
	.cfi_endproc
LFE15:
	.p2align 4,,15
	.globl	_get_desktop_resolution
_get_desktop_resolution:
LFB16:
	.cfi_startproc
	movl	_system_driver, %eax
	movl	100(%eax), %eax
	testl	%eax, %eax
	je	L35
	jmp	*%eax
	.p2align 4,,7
L35:
	orl	$-1, %eax
	ret
	.cfi_endproc
LFE16:
	.p2align 4,,15
	.globl	__default_ds
__default_ds:
LFB17:
	.cfi_startproc
/APP
# 28 "c:/djgpp/include/allegro/platform/al386gcc.h" 1
	  movw %ds, %ax 
# 0 "" 2
/NO_APP
	cwtl
	ret
	.cfi_endproc
LFE17:
	.p2align 4,,15
	.globl	_bmp_write_line
_bmp_write_line:
LFB18:
	.cfi_startproc
	movl	4(%esp), %edx
	movl	8(%esp), %eax
	movl	32(%edx), %ecx
/APP
# 46 "c:/djgpp/include/allegro/platform/al386gcc.h" 1
	  call *%ecx 
# 0 "" 2
/NO_APP
	ret
	.cfi_endproc
LFE18:
	.p2align 4,,15
	.globl	_bmp_read_line
_bmp_read_line:
LFB19:
	.cfi_startproc
	movl	4(%esp), %edx
	movl	8(%esp), %eax
	movl	36(%edx), %ecx
/APP
# 68 "c:/djgpp/include/allegro/platform/al386gcc.h" 1
	  call *%ecx 
# 0 "" 2
/NO_APP
	ret
	.cfi_endproc
LFE19:
	.p2align 4,,15
	.globl	_bmp_unwrite_line
_bmp_unwrite_line:
LFB20:
	.cfi_startproc
	movl	4(%esp), %edx
	movl	28(%edx), %eax
	movl	8(%eax), %ecx
/APP
# 90 "c:/djgpp/include/allegro/platform/al386gcc.h" 1
	  call *%ecx 
# 0 "" 2
/NO_APP
	ret
	.cfi_endproc
LFE20:
	.p2align 4,,15
	.globl	_is_windowed_mode
_is_windowed_mode:
LFB21:
	.cfi_startproc
	movl	_gfx_driver, %eax
	movl	136(%eax), %eax
	ret
	.cfi_endproc
LFE21:
	.p2align 4,,15
	.globl	_clear_to_color
_clear_to_color:
LFB22:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	28(%eax), %edx
	movl	160(%edx), %ecx
	jmp	*%ecx
	.cfi_endproc
LFE22:
	.p2align 4,,15
	.globl	_bitmap_color_depth
_bitmap_color_depth:
LFB23:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	28(%eax), %edx
	movl	(%edx), %eax
	ret
	.cfi_endproc
LFE23:
	.p2align 4,,15
	.globl	_bitmap_mask_color
_bitmap_mask_color:
LFB24:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	28(%eax), %edx
	movl	4(%edx), %eax
	ret
	.cfi_endproc
LFE24:
	.p2align 4,,15
	.globl	_is_same_bitmap
_is_same_bitmap:
LFB25:
	.cfi_startproc
	movl	8(%esp), %eax
	movl	4(%esp), %edx
	testl	%eax, %eax
	je	L46
	testl	%edx, %edx
	je	L46
	cmpl	%eax, %edx
	je	L47
	movl	44(%eax), %ecx
	movl	44(%edx), %edx
	andl	$33554431, %edx
	andl	$33554431, %ecx
	cmpl	%ecx, %edx
	sete	%cl
	testl	%edx, %edx
	setne	%al
	andl	$255, %eax
	andl	%ecx, %eax
	ret
	.p2align 4,,7
L46:
	xorl	%eax, %eax
	ret
L47:
	orl	$-1, %eax
	ret
	.cfi_endproc
LFE25:
	.p2align 4,,15
	.globl	_is_linear_bitmap
_is_linear_bitmap:
LFB26:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	44(%eax), %eax
	shrl	$28, %eax
	xorl	$1, %eax
	andl	$1, %eax
	ret
	.cfi_endproc
LFE26:
	.p2align 4,,15
	.globl	_is_planar_bitmap
_is_planar_bitmap:
LFB27:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	44(%eax), %eax
	shrl	$28, %eax
	andl	$1, %eax
	ret
	.cfi_endproc
LFE27:
	.p2align 4,,15
	.globl	_is_memory_bitmap
_is_memory_bitmap:
LFB28:
	.cfi_startproc
	movl	4(%esp), %eax
	testl	$-1073741824, 44(%eax)
	sete	%al
	andl	$255, %eax
	ret
	.cfi_endproc
LFE28:
	.p2align 4,,15
	.globl	_is_screen_bitmap
_is_screen_bitmap:
LFB29:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	_screen, %edx
	testl	%eax, %eax
	je	L53
	testl	%edx, %edx
	je	L53
	cmpl	%eax, %edx
	je	L54
	movl	44(%eax), %ecx
	movl	44(%edx), %eax
	andl	$33554431, %ecx
	andl	$33554431, %eax
	cmpl	%eax, %ecx
	sete	%dl
	testl	%ecx, %ecx
	setne	%al
	andl	$255, %eax
	andl	%edx, %eax
	ret
	.p2align 4,,7
L53:
	xorl	%eax, %eax
	ret
L54:
	orl	$-1, %eax
	ret
	.cfi_endproc
LFE29:
	.p2align 4,,15
	.globl	_is_video_bitmap
_is_video_bitmap:
LFB30:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	44(%eax), %eax
	shrl	$31, %eax
	ret
	.cfi_endproc
LFE30:
	.p2align 4,,15
	.globl	_is_system_bitmap
_is_system_bitmap:
LFB31:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	44(%eax), %eax
	shrl	$30, %eax
	andl	$1, %eax
	ret
	.cfi_endproc
LFE31:
	.p2align 4,,15
	.globl	_is_sub_bitmap
_is_sub_bitmap:
LFB32:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	44(%eax), %eax
	shrl	$29, %eax
	andl	$1, %eax
	ret
	.cfi_endproc
LFE32:
	.p2align 4,,15
	.globl	_acquire_bitmap
_acquire_bitmap:
LFB33:
	.cfi_startproc
	movl	4(%esp), %edx
	movl	28(%edx), %eax
	movl	16(%eax), %ecx
	testl	%ecx, %ecx
	je	L58
	jmp	*%ecx
	.p2align 4,,7
L58:
	ret
	.cfi_endproc
LFE33:
	.p2align 4,,15
	.globl	_release_bitmap
_release_bitmap:
LFB34:
	.cfi_startproc
	movl	4(%esp), %edx
	movl	28(%edx), %eax
	movl	20(%eax), %ecx
	testl	%ecx, %ecx
	je	L60
	jmp	*%ecx
	.p2align 4,,7
L60:
	ret
	.cfi_endproc
LFE34:
	.p2align 4,,15
	.globl	_acquire_screen
_acquire_screen:
LFB35:
	.cfi_startproc
	movl	_screen, %edx
	movl	28(%edx), %eax
	movl	16(%eax), %ecx
	testl	%ecx, %ecx
	je	L69
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	movl	%edx, (%esp)
	call	*%ecx
	addl	$28, %esp
	.cfi_def_cfa_offset 4
L69:
	ret
	.cfi_endproc
LFE35:
	.p2align 4,,15
	.globl	_release_screen
_release_screen:
LFB36:
	.cfi_startproc
	movl	_screen, %edx
	movl	28(%edx), %eax
	movl	20(%eax), %ecx
	testl	%ecx, %ecx
	je	L77
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	movl	%edx, (%esp)
	call	*%ecx
	addl	$28, %esp
	.cfi_def_cfa_offset 4
L77:
	ret
	.cfi_endproc
LFE36:
	.p2align 4,,15
	.globl	_is_inside_bitmap
_is_inside_bitmap:
LFB37:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	20(%esp), %ebx
	movl	8(%esp), %edx
	movl	12(%esp), %ecx
	testl	%ebx, %ebx
	je	L79
	movl	8(%edx), %ebx
	testl	%ebx, %ebx
	je	L81
	xorl	%eax, %eax
	cmpl	%ecx, 12(%edx)
	jg	L80
	movl	16(%esp), %ebx
	cmpl	%ebx, 20(%edx)
	jg	L80
	cmpl	16(%edx), %ecx
	jge	L80
	cmpl	24(%edx), %ebx
	setl	%al
	andl	$255, %eax
	.p2align 4,,7
L80:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L79:
	.cfi_restore_state
	xorl	%eax, %eax
	cmpl	(%edx), %ecx
	jae	L80
	movl	16(%esp), %eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	cmpl	4(%edx), %eax
	setb	%al
	andl	$255, %eax
	ret
	.p2align 4,,7
L81:
	.cfi_restore_state
	orl	$-1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE37:
	.p2align 4,,15
	.globl	_get_clip_rect
_get_clip_rect:
LFB38:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	8(%esp), %edx
	movl	12(%eax), %ecx
	movl	%ecx, (%edx)
	movl	12(%esp), %edx
	movl	20(%eax), %ecx
	movl	%ecx, (%edx)
	movl	16(%esp), %edx
	movl	16(%eax), %ecx
	decl	%ecx
	movl	%ecx, (%edx)
	movl	24(%eax), %ecx
	movl	20(%esp), %eax
	decl	%ecx
	movl	%ecx, (%eax)
	ret
	.cfi_endproc
LFE38:
	.p2align 4,,15
	.globl	_set_clip_state
_set_clip_state:
LFB39:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	8(%esp), %edx
	movl	%edx, 8(%eax)
	ret
	.cfi_endproc
LFE39:
	.p2align 4,,15
	.globl	_get_clip_state
_get_clip_state:
LFB40:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	ret
	.cfi_endproc
LFE40:
	.p2align 4,,15
	.globl	_makecol15
_makecol15:
LFB41:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	__rgb_g_shift_15, %ecx
	movl	12(%esp), %eax
	movl	8(%esp), %ebx
	sarl	$3, %eax
	movl	16(%esp), %edx
	sall	%cl, %eax
	sarl	$3, %ebx
	movl	__rgb_r_shift_15, %ecx
	sall	%cl, %ebx
	sarl	$3, %edx
	movl	__rgb_b_shift_15, %ecx
	orl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	sall	%cl, %edx
	orl	%edx, %eax
	ret
	.cfi_endproc
LFE41:
	.p2align 4,,15
	.globl	_makecol16
_makecol16:
LFB42:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	__rgb_g_shift_16, %ecx
	movl	12(%esp), %eax
	movl	8(%esp), %ebx
	sarl	$2, %eax
	movl	16(%esp), %edx
	sall	%cl, %eax
	sarl	$3, %ebx
	movl	__rgb_r_shift_16, %ecx
	sall	%cl, %ebx
	sarl	$3, %edx
	movl	__rgb_b_shift_16, %ecx
	orl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	sall	%cl, %edx
	orl	%edx, %eax
	ret
	.cfi_endproc
LFE42:
	.p2align 4,,15
	.globl	_makecol24
_makecol24:
LFB43:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	__rgb_g_shift_24, %ecx
	movl	12(%esp), %eax
	movl	8(%esp), %ebx
	sall	%cl, %eax
	movl	__rgb_r_shift_24, %ecx
	movl	16(%esp), %edx
	sall	%cl, %ebx
	movl	__rgb_b_shift_24, %ecx
	orl	%ebx, %eax
	sall	%cl, %edx
	orl	%edx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE43:
	.p2align 4,,15
	.globl	_makecol32
_makecol32:
LFB44:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	__rgb_g_shift_32, %ecx
	movl	12(%esp), %eax
	movl	8(%esp), %ebx
	sall	%cl, %eax
	movl	__rgb_r_shift_32, %ecx
	movl	16(%esp), %edx
	sall	%cl, %ebx
	movl	__rgb_b_shift_32, %ecx
	orl	%ebx, %eax
	sall	%cl, %edx
	orl	%edx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE44:
	.p2align 4,,15
	.globl	_makeacol32
_makeacol32:
LFB45:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	16(%esp), %eax
	movl	__rgb_g_shift_32, %ecx
	movl	12(%esp), %esi
	movl	20(%esp), %ebx
	sall	%cl, %eax
	movl	__rgb_r_shift_32, %ecx
	movl	24(%esp), %edx
	sall	%cl, %esi
	movl	__rgb_b_shift_32, %ecx
	orl	%esi, %eax
	sall	%cl, %ebx
	movl	__rgb_a_shift_32, %ecx
	orl	%ebx, %eax
	sall	%cl, %edx
	orl	%edx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE45:
	.p2align 4,,15
	.globl	_getr8
_getr8:
LFB46:
	.cfi_startproc
	movl	4(%esp), %eax
	movb	__current_palette(,%eax,4), %dl
	andl	$255, %edx
	movl	__rgb_scale_6(,%edx,4), %eax
	ret
	.cfi_endproc
LFE46:
	.p2align 4,,15
	.globl	_getg8
_getg8:
LFB47:
	.cfi_startproc
	movl	4(%esp), %eax
	movb	__current_palette+1(,%eax,4), %dl
	andl	$255, %edx
	movl	__rgb_scale_6(,%edx,4), %eax
	ret
	.cfi_endproc
LFE47:
	.p2align 4,,15
	.globl	_getb8
_getb8:
LFB48:
	.cfi_startproc
	movl	4(%esp), %eax
	movb	__current_palette+2(,%eax,4), %dl
	andl	$255, %edx
	movl	__rgb_scale_6(,%edx,4), %eax
	ret
	.cfi_endproc
LFE48:
	.p2align 4,,15
	.globl	_getr15
_getr15:
LFB49:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	__rgb_r_shift_15, %ecx
	sarl	%cl, %eax
	andl	$31, %eax
	movl	__rgb_scale_5(,%eax,4), %eax
	ret
	.cfi_endproc
LFE49:
	.p2align 4,,15
	.globl	_getg15
_getg15:
LFB50:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	__rgb_g_shift_15, %ecx
	sarl	%cl, %eax
	andl	$31, %eax
	movl	__rgb_scale_5(,%eax,4), %eax
	ret
	.cfi_endproc
LFE50:
	.p2align 4,,15
	.globl	_getb15
_getb15:
LFB51:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	__rgb_b_shift_15, %ecx
	sarl	%cl, %eax
	andl	$31, %eax
	movl	__rgb_scale_5(,%eax,4), %eax
	ret
	.cfi_endproc
LFE51:
	.p2align 4,,15
	.globl	_getr16
_getr16:
LFB52:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	__rgb_r_shift_16, %ecx
	sarl	%cl, %eax
	andl	$31, %eax
	movl	__rgb_scale_5(,%eax,4), %eax
	ret
	.cfi_endproc
LFE52:
	.p2align 4,,15
	.globl	_getg16
_getg16:
LFB53:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	__rgb_g_shift_16, %ecx
	sarl	%cl, %eax
	andl	$63, %eax
	movl	__rgb_scale_6(,%eax,4), %eax
	ret
	.cfi_endproc
LFE53:
	.p2align 4,,15
	.globl	_getb16
_getb16:
LFB54:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	__rgb_b_shift_16, %ecx
	sarl	%cl, %eax
	andl	$31, %eax
	movl	__rgb_scale_5(,%eax,4), %eax
	ret
	.cfi_endproc
LFE54:
	.p2align 4,,15
	.globl	_getr24
_getr24:
LFB55:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	__rgb_r_shift_24, %ecx
	sarl	%cl, %eax
	andl	$255, %eax
	ret
	.cfi_endproc
LFE55:
	.p2align 4,,15
	.globl	_getg24
_getg24:
LFB56:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	__rgb_g_shift_24, %ecx
	sarl	%cl, %eax
	andl	$255, %eax
	ret
	.cfi_endproc
LFE56:
	.p2align 4,,15
	.globl	_getb24
_getb24:
LFB57:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	__rgb_b_shift_24, %ecx
	sarl	%cl, %eax
	andl	$255, %eax
	ret
	.cfi_endproc
LFE57:
	.p2align 4,,15
	.globl	_getr32
_getr32:
LFB58:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	__rgb_r_shift_32, %ecx
	sarl	%cl, %eax
	andl	$255, %eax
	ret
	.cfi_endproc
LFE58:
	.p2align 4,,15
	.globl	_getg32
_getg32:
LFB59:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	__rgb_g_shift_32, %ecx
	sarl	%cl, %eax
	andl	$255, %eax
	ret
	.cfi_endproc
LFE59:
	.p2align 4,,15
	.globl	_getb32
_getb32:
LFB60:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	__rgb_b_shift_32, %ecx
	sarl	%cl, %eax
	andl	$255, %eax
	ret
	.cfi_endproc
LFE60:
	.p2align 4,,15
	.globl	_geta32
_geta32:
LFB61:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	__rgb_a_shift_32, %ecx
	sarl	%cl, %eax
	andl	$255, %eax
	ret
	.cfi_endproc
LFE61:
	.p2align 4,,15
	.globl	_getpixel
_getpixel:
LFB62:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	28(%eax), %edx
	movl	32(%edx), %eax
	jmp	*%eax
	.cfi_endproc
LFE62:
	.p2align 4,,15
	.globl	_putpixel
_putpixel:
LFB63:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	28(%eax), %edx
	movl	36(%edx), %ecx
	jmp	*%ecx
	.cfi_endproc
LFE63:
	.p2align 4,,15
	.globl	__allegro_vline
__allegro_vline:
LFB64:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	28(%eax), %edx
	movl	40(%edx), %ecx
	jmp	*%ecx
	.cfi_endproc
LFE64:
	.p2align 4,,15
	.globl	__allegro_hline
__allegro_hline:
LFB65:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	28(%eax), %edx
	movl	44(%edx), %ecx
	jmp	*%ecx
	.cfi_endproc
LFE65:
	.p2align 4,,15
	.globl	_line
_line:
LFB68:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	28(%eax), %edx
	movl	52(%edx), %ecx
	jmp	*%ecx
	.cfi_endproc
LFE68:
	.p2align 4,,15
	.globl	_fastline
_fastline:
LFB69:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	28(%eax), %edx
	movl	56(%edx), %ecx
	jmp	*%ecx
	.cfi_endproc
LFE69:
	.p2align 4,,15
	.globl	_rectfill
_rectfill:
LFB70:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	28(%eax), %edx
	movl	60(%edx), %ecx
	jmp	*%ecx
	.cfi_endproc
LFE70:
	.p2align 4,,15
	.globl	_triangle
_triangle:
LFB71:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	28(%eax), %edx
	movl	64(%edx), %ecx
	jmp	*%ecx
	.cfi_endproc
LFE71:
	.p2align 4,,15
	.globl	_polygon
_polygon:
LFB72:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	28(%eax), %edx
	movl	184(%edx), %ecx
	jmp	*%ecx
	.cfi_endproc
