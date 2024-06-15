	.file	"desktop.c"
	.section .text
	.p2align 4,,15
	.globl	_PanelEventHandler
_PanelEventHandler:
LFB189:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
LFE189:
	.p2align 4,,15
	.globl	_TraybarEventHandler
_TraybarEventHandler:
LFB197:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	xorl	%eax, %eax
	movl	68(%esp), %edx
	cmpl	$4, (%edx)
	je	L18
L2:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L18:
	.cfi_restore_state
	movl	4(%edx), %ecx
	cmpl	$-60415, %ecx
	jne	L19
L10:
	movl	_TraybarIconList, %eax
	movl	(%eax), %ebx
	testl	%ebx, %ebx
	je	L11
	movl	%ebx, %edi
	.p2align 4,,7
L7:
	movl	8(%ebx), %eax
	movl	8(%eax), %ecx
	testl	%ecx, %ecx
	je	L6
	movl	_Mouse, %edx
	movl	(%edx), %esi
	cmpl	12(%eax), %esi
	jl	L6
	movl	4(%edx), %ebp
	cmpl	16(%eax), %ebp
	jl	L6
	cmpl	20(%eax), %esi
	jg	L6
	cmpl	24(%eax), %ebp
	jle	L20
	.p2align 4,,7
L6:
	movl	4(%ebx), %ebx
	cmpl	%edi, %ebx
	jne	L7
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L11:
	.cfi_restore_state
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L19:
	.cfi_restore_state
	cmpl	$-60412, %ecx
	je	L10
	jmp	L2
	.p2align 4,,7
L20:
	movl	$0, 20(%esp)
	movl	28(%eax), %edi
	movl	%ecx, 12(%esp)
	movl	%edi, 16(%esp)
	movl	(%edx), %eax
	movl	4(%edx), %ecx
	movl	%ecx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	$_Me, (%esp)
	call	_PopUpMenu
	movl	_TraybarIconList, %edx
	movl	(%edx), %edi
	jmp	L6
	.cfi_endproc
LFE197:
	.p2align 4,,15
	.globl	_GenerateSubMenu
_GenerateSubMenu:
LFB202:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$76, %esp
	.cfi_def_cfa_offset 96
	movl	100(%esp), %eax
	movl	$0, 4(%esp)
	movl	%eax, (%esp)
	call	_ListDir
	movl	%eax, 44(%esp)
	testl	%eax, %eax
	je	L33
	movl	(%eax), %edx
	movl	%edx, 40(%esp)
	testl	%edx, %edx
	je	L34
	movl	%edx, %ebx
	xorl	%esi, %esi
	.p2align 4,,7
L28:
	movl	8(%ebx), %ecx
	movl	(%ecx), %edi
	cmpb	$46, (%edi)
	je	L24
	movl	$46, 4(%esp)
	movl	%edi, (%esp)
	call	_TextRChr
	testl	%eax, %eax
	je	L25
	movl	$46, 4(%esp)
	movl	8(%ebx), %edx
	movl	(%edx), %ecx
	movl	%ecx, (%esp)
	call	_TextRChr
	movl	8(%ebx), %ebp
	movl	0(%ebp), %edi
	subl	%edi, %eax
	movl	%edi, (%esp)
	movl	%eax, 4(%esp)
	call	_TextNDup
	movl	%eax, %edi
L26:
	leal	60(%esp), %eax
	movl	$16, 4(%esp)
	movl	%eax, 8(%esp)
	movl	8(%ebx), %edx
	movl	%edx, (%esp)
	call	_FileGetIcon2
	movl	8(%ebx), %ecx
	movl	%eax, %ebp
	movl	4(%ecx), %eax
	movl	%eax, (%esp)
	call	_TextDup
	movl	%esi, 28(%esp)
	cmpl	$1, 60(%esp)
	sbbl	%edx, %edx
	movl	%eax, 24(%esp)
	xorl	$-1, %edx
	movl	$0, 20(%esp)
	andl	$8, %edx
	movl	$0, 16(%esp)
	movl	%edx, 12(%esp)
	movl	$-267452414, 8(%esp)
	movl	%ebp, 4(%esp)
	movl	%edi, (%esp)
	call	_NewMenuItemEx
	movl	%edi, (%esp)
	movl	%eax, %esi
	call	_free
L24:
	movl	4(%ebx), %ebx
	cmpl	%ebx, 40(%esp)
	jne	L28
L23:
	movl	44(%esp), %edi
	movl	%edi, (%esp)
	call	_FreeList
L22:
	movl	100(%esp), %ebp
	movl	$24, 4(%esp)
	movl	%ebp, (%esp)
	call	_ListDir
	movl	%eax, %edi
	testl	%eax, %eax
	je	L29
	movl	(%eax), %ebp
	testl	%ebp, %ebp
	je	L30
	movl	%ebp, %ebx
	movl	%eax, 40(%esp)
	jmp	L32
	.p2align 4,,7
L31:
	movl	4(%ebx), %ebx
	cmpl	%ebx, %ebp
	je	L48
L32:
	movl	8(%ebx), %ecx
	testb	$16, 12(%ecx)
	je	L31
	leal	60(%esp), %eax
	movl	%ecx, (%esp)
	movl	%eax, 8(%esp)
	movl	$16, 4(%esp)
	call	_FileGetIcon2
	movl	8(%ebx), %edx
	movl	4(%edx), %ecx
	movl	%eax, %edi
	movl	%ecx, (%esp)
	call	_TextDup
	movl	%esi, 28(%esp)
	movl	%eax, 24(%esp)
	movl	$_GenerateSubMenu, 20(%esp)
	movl	$0, 16(%esp)
	movl	$0, 12(%esp)
	movl	$0, 8(%esp)
	movl	%edi, 4(%esp)
	movl	8(%ebx), %esi
	movl	(%esi), %eax
	movl	%eax, (%esp)
	call	_NewMenuItemEx
	movl	4(%ebx), %ebx
	movl	%eax, %esi
	cmpl	%ebx, %ebp
	jne	L32
L48:
	movl	40(%esp), %edi
L30:
	movl	%edi, (%esp)
	call	_FreeList
L29:
	movl	%esi, (%esp)
	call	_NewMenu
	movl	96(%esp), %ebp
	movl	%eax, 28(%ebp)
	addl	$76, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L25:
	.cfi_restore_state
	movl	8(%ebx), %ebp
	movl	0(%ebp), %eax
	movl	%eax, (%esp)
	call	_TextDup
	movl	%eax, %edi
	jmp	L26
	.p2align 4,,7
L33:
	xorl	%esi, %esi
	jmp	L22
	.p2align 4,,7
L34:
	xorl	%esi, %esi
	jmp	L23
	.cfi_endproc
LFE202:
LC0:
	.ascii "12:00\0"
	.p2align 4,,15
	.globl	_TraybarAdd
_TraybarAdd:
LFB198:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$60, %esp
	.cfi_def_cfa_offset 80
	movl	_GSTraybar, %eax
	testl	%eax, %eax
	je	L55
	movl	32(%eax), %edx
	movl	%edx, 16(%esp)
	movl	28(%eax), %ecx
	movl	%ecx, 12(%esp)
	movl	24(%eax), %ebx
	movl	%ebx, 8(%esp)
	movl	20(%eax), %esi
	movl	%eax, (%esp)
	subl	$18, %esi
	movl	%esi, 4(%esp)
	call	_WidgetSetMetrics
L51:
	movl	$32, (%esp)
	call	_malloc
	movl	%eax, %ebp
	testl	%eax, %eax
	je	L53
	movl	80(%esp), %ecx
	testl	%ecx, %ecx
	je	L53
	xorl	%eax, %eax
	movl	%ebp, %edi
	movl	$8, %ecx
	rep stosl
	movl	84(%esp), %edx
	movl	80(%esp), %esi
	movl	%esi, 0(%ebp)
	movl	%edx, (%esp)
	call	_TextDup
	movl	88(%esp), %ebx
	movl	92(%esp), %ecx
	movl	%eax, 4(%ebp)
	movl	%ebx, 8(%ebp)
	movl	%ecx, 28(%ebp)
	movl	_TraybarIconList, %eax
	movl	%ebp, 8(%esp)
	movl	84(%esp), %ebp
	movl	$_free, 12(%esp)
	movl	%ebp, 4(%esp)
	movl	%eax, (%esp)
	call	_ListAdd
	movl	_GSTraybar, %edi
	movl	%edi, (%esp)
	movl	$0, 4(%esp)
	call	_WidgetDraw
	addl	$60, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L53:
	.cfi_restore_state
	addl	$60, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L55:
	.cfi_restore_state
	movl	_GSPanel, %edi
	movl	_default_font, %eax
	movl	32(%edi), %ebp
	movl	28(%edi), %edi
	movl	$LC0, 4(%esp)
	movl	%eax, (%esp)
	call	_text_length
	movl	_GSPanel, %edx
	movl	_default_font, %ecx
	movl	28(%edx), %ebx
	movl	%ecx, (%esp)
	movl	$LC0, 4(%esp)
	movl	%eax, %esi
	call	_text_length
	subl	$7, %edi
	subl	$7, %ebx
	subl	%eax, %ebx
	decl	%ebp
	subl	%esi, %edi
	movl	%ebp, 16(%esp)
	subl	$15, %edi
	subl	$37, %ebx
	leal	32(%esp), %ebp
	movl	%edi, 12(%esp)
	movl	%ebx, 4(%esp)
	movl	$1, 8(%esp)
	movl	%ebp, (%esp)
	call	_RectAssign
	movl	32(%esp), %edi
	movl	36(%esp), %eax
	movl	40(%esp), %esi
	movl	44(%esp), %edx
	movl	%edi, 4(%esp)
	movl	%eax, 8(%esp)
	movl	%esi, 12(%esp)
	movl	%edx, 16(%esp)
	movl	$_Me, (%esp)
	call	_CreateWidget
	movl	_GSPanel, %ebx
	movl	%eax, _GSTraybar
	movl	$_TraybarDraw, 128(%eax)
	movl	$_TraybarEventHandler, 116(%eax)
	movl	%eax, 4(%esp)
	movl	%ebx, (%esp)
	call	_WidgetInsert
	jmp	L51
	.cfi_endproc
LFE198:
	.p2align 4,,15
	.globl	_TraybarDraw
_TraybarDraw:
LFB196:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	_SysColors, %edx
	movl	68(%esp), %esi
	movl	64(%esp), %ebp
	movl	4(%edx), %ecx
	movl	28(%esi), %eax
	movl	%ecx, 20(%esp)
	movl	32(%ebp), %ebx
	movl	%ebx, 16(%esp)
	movl	28(%ebp), %edi
	movl	%edi, 12(%esp)
	movl	24(%ebp), %edx
	movl	%edx, 8(%esp)
	movl	20(%ebp), %ecx
	movl	%esi, (%esp)
	movl	%ecx, 4(%esp)
	call	*60(%eax)
	movl	_SysColors, %ebx
	movl	12(%ebx), %edi
	movl	28(%esi), %eax
	movl	%edi, 20(%esp)
	movl	32(%ebp), %edx
	movl	%edx, 16(%esp)
	movl	28(%ebp), %ecx
	movl	%ecx, 12(%esp)
	movl	24(%ebp), %ebx
	movl	%ebx, 8(%esp)
	movl	20(%ebp), %edi
	movl	%esi, (%esp)
	movl	%edi, 4(%esp)
	call	*188(%eax)
	movl	_TraybarIconList, %eax
	movl	(%eax), %ebx
	testl	%ebx, %ebx
	je	L56
	movl	20(%ebp), %edi
	addl	$3, %edi
	.p2align 4,,7
L58:
	movl	24(%ebp), %edx
	movl	32(%ebp), %eax
	subl	%edx, %eax
	movl	%edi, 4(%esp)
	movl	%eax, %ecx
	shrl	$31, %ecx
	addl	%ecx, %eax
	leal	16(%edi), %ecx
	sarl	%eax
	movl	%ecx, 12(%esp)
	addl	%edx, %eax
	addl	$18, %edi
	leal	8(%eax), %edx
	subl	$8, %eax
	movl	%edx, 16(%esp)
	movl	%eax, 8(%esp)
	movl	8(%ebx), %eax
	addl	$12, %eax
	movl	%eax, (%esp)
	call	_RectAssign
	movl	8(%ebx), %ecx
	movl	24(%ecx), %edx
	movl	%edx, 28(%esp)
	movl	20(%ecx), %eax
	movl	%eax, 24(%esp)
	movl	16(%ecx), %edx
	movl	%edx, 20(%esp)
	movl	12(%ecx), %eax
	movl	$0, 12(%esp)
	movl	%eax, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	(%ecx), %ecx
	movl	%ecx, (%esp)
	call	_masked_blit
	movl	_TraybarIconList, %edx
	movl	4(%ebx), %ebx
	cmpl	%ebx, (%edx)
	jne	L58
L56:
	addl	$44, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE196:
LC1:
	.ascii "pretime\0"
LC2:
	.ascii "%d:%02d %s\0"
LC3:
	.ascii "posttime\0"
	.p2align 4,,15
	.globl	_RefreshPanelClock
_RefreshPanelClock:
LFB195:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$56, %esp
	.cfi_def_cfa_offset 64
	movl	$0, (%esp)
	call	_time
	movl	%eax, 44(%esp)
	leal	44(%esp), %eax
	movl	%eax, (%esp)
	call	_localtime
	movw	$19792, 41(%esp)
	movl	%eax, %ebx
	movb	$0, 43(%esp)
	movl	$0, 8(%eax)
	movl	$LC1, (%esp)
	call	_DebugMessage
	leal	41(%esp), %edx
	movl	%edx, 16(%esp)
	movl	4(%ebx), %ecx
	movl	%ecx, 12(%esp)
	movl	8(%ebx), %eax
	movl	_GSClock, %ebx
	movl	%eax, 8(%esp)
	movl	%ebx, (%esp)
	movl	$LC2, 4(%esp)
	call	_LabelSetText
	movl	$LC3, (%esp)
	call	_DebugMessage
	addl	$56, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE195:
LC4:
	.ascii "X\0"
	.p2align 4,,15
	.globl	_navButtonDraw
_navButtonDraw:
LFB194:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	_THMPanelnavDown, %eax
	movl	64(%esp), %ebx
	movl	68(%esp), %esi
	testl	%eax, %eax
	je	L65
	movl	_THMPanelnavOver, %edx
	testl	%edx, %edx
	je	L65
	movl	_THMPanelnav, %ecx
	testl	%ecx, %ecx
	je	L65
	movl	_UseSkins, %ebp
	testl	%ebp, %ebp
	je	L65
	movl	212(%ebx), %edi
	cmpw	$1, %di
	je	L89
	cmpw	$2, %di
	je	L68
	movl	_Mouse, %edx
	testb	$1, 12(%edx)
	je	L69
	movl	72(%esp), %ebp
	movl	(%edx), %edi
	cmpl	0(%ebp), %edi
	jl	L69
	movl	4(%edx), %edx
	cmpl	4(%ebp), %edx
	jl	L69
	cmpl	8(%ebp), %edi
	jg	L69
	cmpl	12(%ebp), %edx
	jg	L69
L68:
	movl	32(%ebx), %ecx
	movl	%ecx, 28(%esp)
	movl	28(%ebx), %edx
	movl	%edx, 24(%esp)
	movl	24(%ebx), %edi
	movl	%edi, 20(%esp)
	movl	20(%ebx), %ebp
	movl	$0, 12(%esp)
	movl	%ebp, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_masked_blit
	jmp	L64
	.p2align 4,,7
L65:
	movl	212(%ebx), %edi
	cmpw	$1, %di
	je	L90
L70:
	cmpw	$2, %di
	movl	_SysColors, %ecx
	je	L71
	movl	12(%ecx), %eax
	movl	%eax, 24(%esp)
	movl	8(%ecx), %edx
	movl	%edx, 20(%esp)
	movl	32(%ebx), %ecx
	movl	%ecx, 16(%esp)
	movl	28(%ebx), %ebp
	movl	%ebp, 12(%esp)
	movl	24(%ebx), %edi
	movl	%edi, 8(%esp)
	movl	20(%ebx), %eax
	movl	%esi, (%esp)
	movl	%eax, 4(%esp)
	call	_Rect3D
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	movl	$0, (%esp)
	call	_makecol
	movl	24(%ebx), %edx
	movl	32(%ebx), %ecx
	movl	%eax, %ebp
	addl	%ecx, %edx
	movl	_default_font, %eax
	movl	%edx, %edi
	movl	%eax, (%esp)
	shrl	$31, %edi
	addl	%edx, %edi
	call	_text_height
	movl	%ebp, 20(%esp)
	movl	%eax, %ebp
	shrl	$31, %ebp
	movl	$-1, 24(%esp)
	sarl	%edi
	addl	%ebp, %eax
	sarl	%eax
	subl	%eax, %edi
	movl	%edi, 16(%esp)
	movl	20(%ebx), %ebp
	movl	28(%ebx), %ebx
	addl	%ebx, %ebp
	movl	%ebp, %edx
	shrl	$31, %edx
	addl	%edx, %ebp
	sarl	%ebp
L88:
	movl	_default_font, %ecx
	movl	%ebp, 12(%esp)
	movl	$LC4, 8(%esp)
	movl	%ecx, 4(%esp)
	movl	%esi, (%esp)
	call	_textout_centre_ex
L64:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L71:
	.cfi_restore_state
	movl	8(%ecx), %ebp
	movl	%ebp, 24(%esp)
	movl	12(%ecx), %eax
	movl	%eax, 20(%esp)
	movl	32(%ebx), %edx
	movl	%edx, 16(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 12(%esp)
	movl	24(%ebx), %ecx
	movl	%ecx, 8(%esp)
	movl	20(%ebx), %ebp
	movl	%esi, (%esp)
	movl	%ebp, 4(%esp)
	call	_Rect3D
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	movl	$0, (%esp)
	call	_makecol
	movl	_default_font, %edx
	movl	24(%ebx), %edi
	movl	%eax, %ebp
	movl	32(%ebx), %eax
	movl	%edx, (%esp)
	addl	%eax, %edi
	call	_text_height
	subl	%eax, %edi
	movl	$-1, 24(%esp)
	movl	%edi, %ecx
	movl	%ebp, 20(%esp)
	shrl	$31, %ecx
	addl	%ecx, %edi
	sarl	%edi
	incl	%edi
	movl	%edi, 16(%esp)
	movl	20(%ebx), %ebp
	movl	28(%ebx), %ebx
	addl	%ebx, %ebp
	movl	%ebp, %edi
	shrl	$31, %edi
	addl	%edi, %ebp
	sarl	%ebp
	incl	%ebp
	jmp	L88
	.p2align 4,,7
L69:
	movl	32(%ebx), %eax
	movl	%eax, 28(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 24(%esp)
	movl	24(%ebx), %ebp
	movl	%ebp, 20(%esp)
	movl	20(%ebx), %ebx
	movl	$0, 12(%esp)
	movl	%ebx, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	(%ecx), %esi
	movl	%esi, (%esp)
	call	_masked_blit
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L90:
	.cfi_restore_state
	movl	_SysColors, %eax
	movl	28(%esi), %edi
	movl	16(%eax), %edx
	movl	%edx, 20(%esp)
	movl	32(%ebx), %ecx
	movl	%ecx, 16(%esp)
	movl	28(%ebx), %ebp
	movl	%ebp, 12(%esp)
	movl	24(%ebx), %eax
	movl	%eax, 8(%esp)
	movl	20(%ebx), %edx
	movl	%esi, (%esp)
	movl	%edx, 4(%esp)
	call	*60(%edi)
	movl	212(%ebx), %edi
	jmp	L70
	.p2align 4,,7
L89:
	movl	32(%ebx), %ecx
	movl	%ecx, 28(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 24(%esp)
	movl	24(%ebx), %ebp
	movl	%ebp, 20(%esp)
	movl	20(%ebx), %ebx
	movl	$0, 12(%esp)
	movl	%ebx, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	(%edx), %esi
	movl	%esi, (%esp)
	call	_masked_blit
	jmp	L64
	.cfi_endproc
LFE194:
LC5:
	.ascii "Exit\0"
	.p2align 4,,15
	.globl	_ExitButtonDraw
_ExitButtonDraw:
LFB193:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	_THMPanelExitDown, %eax
	movl	64(%esp), %ebx
	movl	68(%esp), %esi
	testl	%eax, %eax
	je	L92
	movl	_THMPanelExitOver, %edx
	testl	%edx, %edx
	je	L92
	movl	_THMPanelExit, %ecx
	testl	%ecx, %ecx
	je	L92
	movl	_UseSkins, %ebp
	testl	%ebp, %ebp
	je	L92
	movl	212(%ebx), %edi
	cmpw	$1, %di
	je	L116
	cmpw	$2, %di
	je	L95
	movl	_Mouse, %edx
	testb	$1, 12(%edx)
	je	L96
	movl	72(%esp), %ebp
	movl	(%edx), %edi
	cmpl	0(%ebp), %edi
	jl	L96
	movl	4(%edx), %edx
	cmpl	4(%ebp), %edx
	jl	L96
	cmpl	8(%ebp), %edi
	jg	L96
	cmpl	12(%ebp), %edx
	jg	L96
L95:
	movl	32(%ebx), %ecx
	movl	%ecx, 28(%esp)
	movl	28(%ebx), %edx
	movl	%edx, 24(%esp)
	movl	24(%ebx), %edi
	movl	%edi, 20(%esp)
	movl	20(%ebx), %ebp
	movl	$0, 12(%esp)
	movl	%ebp, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_masked_blit
	jmp	L91
	.p2align 4,,7
L92:
	movl	212(%ebx), %edi
	cmpw	$1, %di
	je	L117
L97:
	cmpw	$2, %di
	movl	_SysColors, %ecx
	je	L98
	movl	12(%ecx), %edx
	movl	%edx, 24(%esp)
	movl	8(%ecx), %ecx
	movl	%ecx, 20(%esp)
	movl	32(%ebx), %eax
	movl	%eax, 16(%esp)
	movl	28(%ebx), %ebp
	movl	%ebp, 12(%esp)
	movl	24(%ebx), %edi
	movl	%edi, 8(%esp)
	movl	20(%ebx), %edx
	movl	%esi, (%esp)
	movl	%edx, 4(%esp)
	call	_Rect3D
	movl	32(%ebx), %edi
	movl	24(%ebx), %eax
	movl	_SysColors, %ecx
	addl	%edi, %eax
	movl	_default_font, %edx
	movl	%eax, %edi
	movl	(%ecx), %ebp
	shrl	$31, %edi
	movl	%edx, (%esp)
	addl	%eax, %edi
	call	_text_height
	movl	%eax, %ecx
	movl	$-1, 24(%esp)
	shrl	$31, %ecx
	movl	%ebp, 20(%esp)
	sarl	%edi
	addl	%ecx, %eax
	sarl	%eax
	subl	%eax, %edi
	movl	%edi, 16(%esp)
	movl	20(%ebx), %ebp
	movl	28(%ebx), %ebx
	addl	%ebx, %ebp
	movl	%ebp, %eax
	shrl	$31, %eax
	addl	%eax, %ebp
	sarl	%ebp
L115:
	movl	%ebp, 12(%esp)
	movl	_default_font, %ebp
	movl	$LC5, 8(%esp)
	movl	%ebp, 4(%esp)
	movl	%esi, (%esp)
	call	_textout_centre_ex
L91:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L98:
	.cfi_restore_state
	movl	8(%ecx), %ebp
	movl	%ebp, 24(%esp)
	movl	12(%ecx), %eax
	movl	%eax, 20(%esp)
	movl	32(%ebx), %edx
	movl	%edx, 16(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 12(%esp)
	movl	24(%ebx), %ecx
	movl	%ecx, 8(%esp)
	movl	20(%ebx), %ebp
	movl	%esi, (%esp)
	movl	%ebp, 4(%esp)
	call	_Rect3D
	movl	_SysColors, %eax
	movl	_default_font, %ecx
	movl	(%eax), %ebp
	movl	24(%ebx), %edi
	movl	32(%ebx), %edx
	movl	%ecx, (%esp)
	addl	%edx, %edi
	call	_text_height
	movl	$-1, 24(%esp)
	subl	%eax, %edi
	movl	%ebp, 20(%esp)
	movl	%edi, %eax
	shrl	$31, %eax
	addl	%eax, %edi
	sarl	%edi
	incl	%edi
	movl	%edi, 16(%esp)
	movl	20(%ebx), %ebp
	movl	28(%ebx), %ebx
	addl	%ebx, %ebp
	movl	%ebp, %edi
	shrl	$31, %edi
	addl	%edi, %ebp
	sarl	%ebp
	incl	%ebp
	jmp	L115
	.p2align 4,,7
L96:
	movl	32(%ebx), %eax
	movl	%eax, 28(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 24(%esp)
	movl	24(%ebx), %ebp
	movl	%ebp, 20(%esp)
	movl	20(%ebx), %ebx
	movl	$0, 12(%esp)
	movl	%ebx, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	(%ecx), %esi
	movl	%esi, (%esp)
	call	_masked_blit
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L117:
	.cfi_restore_state
	movl	_SysColors, %eax
	movl	28(%esi), %edi
	movl	16(%eax), %edx
	movl	%edx, 20(%esp)
	movl	32(%ebx), %ecx
	movl	%ecx, 16(%esp)
	movl	28(%ebx), %ebp
	movl	%ebp, 12(%esp)
	movl	24(%ebx), %eax
	movl	%eax, 8(%esp)
	movl	20(%ebx), %edx
	movl	%esi, (%esp)
	movl	%edx, 4(%esp)
	call	*60(%edi)
	movl	212(%ebx), %edi
	jmp	L97
	.p2align 4,,7
L116:
	movl	32(%ebx), %ecx
	movl	%ecx, 28(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 24(%esp)
	movl	24(%ebx), %ebp
	movl	%ebp, 20(%esp)
	movl	20(%ebx), %ebx
	movl	$0, 12(%esp)
	movl	%ebx, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	(%edx), %esi
	movl	%esi, (%esp)
	call	_masked_blit
	jmp	L91
	.cfi_endproc
LFE193:
LC6:
	.ascii "Apps\0"
	.p2align 4,,15
	.globl	_appButtonDraw
_appButtonDraw:
LFB192:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	_THMPanelappDown, %eax
	movl	64(%esp), %ebx
	movl	68(%esp), %esi
	testl	%eax, %eax
	je	L119
	movl	_THMPanelappOver, %edx
	testl	%edx, %edx
	je	L119
	movl	_THMPanelapp, %ecx
	testl	%ecx, %ecx
	je	L119
	movl	_UseSkins, %ebp
	testl	%ebp, %ebp
	je	L119
	movl	212(%ebx), %edi
	cmpw	$1, %di
	je	L143
	cmpw	$2, %di
	je	L122
	movl	_Mouse, %edx
	testb	$1, 12(%edx)
	je	L123
	movl	72(%esp), %ebp
	movl	(%edx), %edi
	cmpl	0(%ebp), %edi
	jl	L123
	movl	4(%edx), %edx
	cmpl	4(%ebp), %edx
	jl	L123
	cmpl	8(%ebp), %edi
	jg	L123
	cmpl	12(%ebp), %edx
	jg	L123
L122:
	movl	32(%ebx), %ecx
	movl	%ecx, 28(%esp)
	movl	28(%ebx), %edx
	movl	%edx, 24(%esp)
	movl	24(%ebx), %edi
	movl	%edi, 20(%esp)
	movl	20(%ebx), %ebp
	movl	$0, 12(%esp)
	movl	%ebp, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_masked_blit
	jmp	L118
	.p2align 4,,7
L119:
	movl	212(%ebx), %edi
	cmpw	$1, %di
	je	L144
L124:
	cmpw	$2, %di
	movl	_SysColors, %ecx
	je	L125
	movl	12(%ecx), %edx
	movl	%edx, 24(%esp)
	movl	8(%ecx), %ecx
	movl	%ecx, 20(%esp)
	movl	32(%ebx), %eax
	movl	%eax, 16(%esp)
	movl	28(%ebx), %ebp
	movl	%ebp, 12(%esp)
	movl	24(%ebx), %edi
	movl	%edi, 8(%esp)
	movl	20(%ebx), %edx
	movl	%esi, (%esp)
	movl	%edx, 4(%esp)
	call	_Rect3D
	movl	32(%ebx), %edi
	movl	24(%ebx), %eax
	movl	_SysColors, %ecx
	addl	%edi, %eax
	movl	_default_font, %edx
	movl	%eax, %edi
	movl	(%ecx), %ebp
	shrl	$31, %edi
	movl	%edx, (%esp)
	addl	%eax, %edi
	call	_text_height
	movl	%eax, %ecx
	movl	$-1, 24(%esp)
	shrl	$31, %ecx
	movl	%ebp, 20(%esp)
	sarl	%edi
	addl	%ecx, %eax
	sarl	%eax
	subl	%eax, %edi
	movl	%edi, 16(%esp)
	movl	20(%ebx), %ebp
	movl	28(%ebx), %ebx
	addl	%ebx, %ebp
	movl	%ebp, %eax
	shrl	$31, %eax
	addl	%eax, %ebp
	sarl	%ebp
L142:
	movl	%ebp, 12(%esp)
	movl	_default_font, %ebp
	movl	$LC6, 8(%esp)
	movl	%ebp, 4(%esp)
	movl	%esi, (%esp)
	call	_textout_centre_ex
L118:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L125:
	.cfi_restore_state
	movl	8(%ecx), %ebp
	movl	%ebp, 24(%esp)
	movl	12(%ecx), %eax
	movl	%eax, 20(%esp)
	movl	32(%ebx), %edx
	movl	%edx, 16(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 12(%esp)
	movl	24(%ebx), %ecx
	movl	%ecx, 8(%esp)
	movl	20(%ebx), %ebp
	movl	%esi, (%esp)
	movl	%ebp, 4(%esp)
	call	_Rect3D
	movl	_SysColors, %eax
	movl	_default_font, %ecx
	movl	(%eax), %ebp
	movl	24(%ebx), %edi
	movl	32(%ebx), %edx
	movl	%ecx, (%esp)
	addl	%edx, %edi
	call	_text_height
	movl	$-1, 24(%esp)
	subl	%eax, %edi
	movl	%ebp, 20(%esp)
	movl	%edi, %eax
	shrl	$31, %eax
	addl	%eax, %edi
	sarl	%edi
	incl	%edi
	movl	%edi, 16(%esp)
	movl	20(%ebx), %ebp
	movl	28(%ebx), %ebx
	addl	%ebx, %ebp
	movl	%ebp, %edi
	shrl	$31, %edi
	addl	%edi, %ebp
	sarl	%ebp
	incl	%ebp
	jmp	L142
	.p2align 4,,7
L123:
	movl	32(%ebx), %eax
	movl	%eax, 28(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 24(%esp)
	movl	24(%ebx), %ebp
	movl	%ebp, 20(%esp)
	movl	20(%ebx), %ebx
	movl	$0, 12(%esp)
	movl	%ebx, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	(%ecx), %esi
	movl	%esi, (%esp)
	call	_masked_blit
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L144:
	.cfi_restore_state
	movl	_SysColors, %eax
	movl	28(%esi), %edi
	movl	16(%eax), %edx
	movl	%edx, 20(%esp)
	movl	32(%ebx), %ecx
	movl	%ecx, 16(%esp)
	movl	28(%ebx), %ebp
	movl	%ebp, 12(%esp)
	movl	24(%ebx), %eax
	movl	%eax, 8(%esp)
	movl	20(%ebx), %edx
	movl	%esi, (%esp)
	movl	%edx, 4(%esp)
	call	*60(%edi)
	movl	212(%ebx), %edi
	jmp	L124
	.p2align 4,,7
L143:
	movl	32(%ebx), %ecx
	movl	%ecx, 28(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 24(%esp)
	movl	24(%ebx), %ebp
	movl	%ebp, 20(%esp)
	movl	20(%ebx), %ebx
	movl	$0, 12(%esp)
	movl	%ebx, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	(%edx), %esi
	movl	%esi, (%esp)
	call	_masked_blit
	jmp	L118
	.cfi_endproc
LFE192:
LC7:
	.ascii "Home\0"
	.p2align 4,,15
	.globl	_USRButtonDraw
_USRButtonDraw:
LFB191:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	_THMPanelUSRDown, %eax
	movl	64(%esp), %ebx
	movl	68(%esp), %esi
	testl	%eax, %eax
	je	L146
	movl	_THMPanelUSROver, %edx
	testl	%edx, %edx
	je	L146
	movl	_THMPanelUSR, %ecx
	testl	%ecx, %ecx
	je	L146
	movl	_UseSkins, %ebp
	testl	%ebp, %ebp
	je	L146
	movl	212(%ebx), %edi
	cmpw	$1, %di
	je	L170
	cmpw	$2, %di
	je	L149
	movl	_Mouse, %edx
	testb	$1, 12(%edx)
	je	L150
	movl	72(%esp), %ebp
	movl	(%edx), %edi
	cmpl	0(%ebp), %edi
	jl	L150
	movl	4(%edx), %edx
	cmpl	4(%ebp), %edx
	jl	L150
	cmpl	8(%ebp), %edi
	jg	L150
	cmpl	12(%ebp), %edx
	jg	L150
L149:
	movl	32(%ebx), %ecx
	movl	%ecx, 28(%esp)
	movl	28(%ebx), %edx
	movl	%edx, 24(%esp)
	movl	24(%ebx), %edi
	movl	%edi, 20(%esp)
	movl	20(%ebx), %ebp
	movl	$0, 12(%esp)
	movl	%ebp, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_masked_blit
	jmp	L145
	.p2align 4,,7
L146:
	movl	212(%ebx), %edi
	cmpw	$1, %di
	je	L171
L151:
	cmpw	$2, %di
	movl	_SysColors, %ecx
	je	L152
	movl	12(%ecx), %edx
	movl	%edx, 24(%esp)
	movl	8(%ecx), %ecx
	movl	%ecx, 20(%esp)
	movl	32(%ebx), %eax
	movl	%eax, 16(%esp)
	movl	28(%ebx), %ebp
	movl	%ebp, 12(%esp)
	movl	24(%ebx), %edi
	movl	%edi, 8(%esp)
	movl	20(%ebx), %edx
	movl	%esi, (%esp)
	movl	%edx, 4(%esp)
	call	_Rect3D
	movl	32(%ebx), %edi
	movl	24(%ebx), %eax
	movl	_SysColors, %ecx
	addl	%edi, %eax
	movl	_default_font, %edx
	movl	%eax, %edi
	movl	(%ecx), %ebp
	shrl	$31, %edi
	movl	%edx, (%esp)
	addl	%eax, %edi
	call	_text_height
	movl	%eax, %ecx
	movl	$-1, 24(%esp)
	shrl	$31, %ecx
	movl	%ebp, 20(%esp)
	sarl	%edi
	addl	%ecx, %eax
	sarl	%eax
	subl	%eax, %edi
	movl	%edi, 16(%esp)
	movl	20(%ebx), %ebp
	movl	28(%ebx), %ebx
	addl	%ebx, %ebp
	movl	%ebp, %eax
	shrl	$31, %eax
	addl	%eax, %ebp
	sarl	%ebp
L169:
	movl	%ebp, 12(%esp)
	movl	_default_font, %ebp
	movl	$LC7, 8(%esp)
	movl	%ebp, 4(%esp)
	movl	%esi, (%esp)
	call	_textout_centre_ex
L145:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L152:
	.cfi_restore_state
	movl	8(%ecx), %ebp
	movl	%ebp, 24(%esp)
	movl	12(%ecx), %eax
	movl	%eax, 20(%esp)
	movl	32(%ebx), %edx
	movl	%edx, 16(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 12(%esp)
	movl	24(%ebx), %ecx
	movl	%ecx, 8(%esp)
	movl	20(%ebx), %ebp
	movl	%esi, (%esp)
	movl	%ebp, 4(%esp)
	call	_Rect3D
	movl	_SysColors, %eax
	movl	_default_font, %ecx
	movl	(%eax), %ebp
	movl	24(%ebx), %edi
	movl	32(%ebx), %edx
	movl	%ecx, (%esp)
	addl	%edx, %edi
	call	_text_height
	movl	$-1, 24(%esp)
	subl	%eax, %edi
	movl	%ebp, 20(%esp)
	movl	%edi, %eax
	shrl	$31, %eax
	addl	%eax, %edi
	sarl	%edi
	incl	%edi
	movl	%edi, 16(%esp)
	movl	20(%ebx), %ebp
	movl	28(%ebx), %ebx
	addl	%ebx, %ebp
	movl	%ebp, %edi
	shrl	$31, %edi
	addl	%edi, %ebp
	sarl	%ebp
	incl	%ebp
	jmp	L169
	.p2align 4,,7
L150:
	movl	32(%ebx), %eax
	movl	%eax, 28(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 24(%esp)
	movl	24(%ebx), %ebp
	movl	%ebp, 20(%esp)
	movl	20(%ebx), %ebx
	movl	$0, 12(%esp)
	movl	%ebx, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	(%ecx), %esi
	movl	%esi, (%esp)
	call	_masked_blit
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L171:
	.cfi_restore_state
	movl	_SysColors, %eax
	movl	28(%esi), %edi
	movl	16(%eax), %edx
	movl	%edx, 20(%esp)
	movl	32(%ebx), %ecx
	movl	%ecx, 16(%esp)
	movl	28(%ebx), %ebp
	movl	%ebp, 12(%esp)
	movl	24(%ebx), %eax
	movl	%eax, 8(%esp)
	movl	20(%ebx), %edx
	movl	%esi, (%esp)
	movl	%edx, 4(%esp)
	call	*60(%edi)
	movl	212(%ebx), %edi
	jmp	L151
	.p2align 4,,7
L170:
	movl	32(%ebx), %ecx
	movl	%ecx, 28(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 24(%esp)
	movl	24(%ebx), %ebp
	movl	%ebp, 20(%esp)
	movl	20(%ebx), %ebx
	movl	$0, 12(%esp)
	movl	%ebx, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	(%edx), %esi
	movl	%esi, (%esp)
	call	_masked_blit
	jmp	L145
	.cfi_endproc
LFE191:
LC8:
	.ascii "Aura M6\0"
LC9:
	.ascii "Build: 4000 - 20/8/2023\0"
	.p2align 4,,15
	.globl	_WDraw
_WDraw:
LFB177:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$52, %esp
	.cfi_def_cfa_offset 64
	movl	64(%esp), %ecx
	movl	68(%esp), %ebx
	movl	72(%esp), %eax
	movl	100(%ecx), %esi
	movl	28(%ebx), %edx
	movl	%esi, 20(%esp)
	movl	12(%eax), %ecx
	movl	%ecx, 16(%esp)
	movl	8(%eax), %esi
	movl	%esi, 12(%esp)
	movl	4(%eax), %ecx
	movl	%ecx, 8(%esp)
	movl	(%eax), %eax
	movl	%ebx, (%esp)
	movl	%eax, 4(%esp)
	call	*60(%edx)
	movl	_WllBmp, %eax
	testl	%eax, %eax
	je	L172
	movl	_Style, %edx
	cmpl	$1, %edx
	je	L181
	cmpl	$2, %edx
	je	L182
	movl	_GSScreenHeight, %ecx
	movl	_GSScreenWidth, %edx
	movl	%ecx, 28(%esp)
	movl	%edx, 24(%esp)
	movl	4(%eax), %esi
	subl	%esi, %ecx
	movl	%ecx, %esi
	shrl	$31, %esi
	addl	%esi, %ecx
	sarl	%ecx
	movl	%ecx, 20(%esp)
	movl	(%eax), %ecx
	movl	$0, 12(%esp)
	subl	%ecx, %edx
	movl	$0, 8(%esp)
	movl	%edx, %esi
	movl	%ebx, 4(%esp)
	shrl	$31, %esi
	movl	%eax, (%esp)
	addl	%esi, %edx
	sarl	%edx
	movl	%edx, 16(%esp)
	call	_blit
L180:
	movl	$255, 8(%esp)
	movl	$255, 4(%esp)
	movl	$255, (%esp)
	call	_makecol
	movl	_GSScreenWidth, %edx
	movl	%eax, 20(%esp)
	movl	_GSScreenHeight, %eax
	subl	$5, %edx
	subl	$34, %eax
	movl	_default_font, %ecx
	movl	%edx, 12(%esp)
	movl	%eax, 16(%esp)
	movl	%ecx, 4(%esp)
	movl	$-1, 24(%esp)
	movl	$LC8, 8(%esp)
	movl	%ebx, (%esp)
	call	_textout_right_ex
	movl	$255, 8(%esp)
	movl	$255, 4(%esp)
	movl	$255, (%esp)
	call	_makecol
	movl	_GSScreenHeight, %esi
	movl	%eax, 20(%esp)
	movl	_GSScreenWidth, %eax
	subl	$17, %esi
	subl	$5, %eax
	movl	_default_font, %edx
	movl	$-1, 24(%esp)
	movl	%esi, 16(%esp)
	movl	%eax, 12(%esp)
	movl	$LC9, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%ebx, (%esp)
	call	_textout_right_ex
L172:
	addl	$52, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L182:
	.cfi_restore_state
	movl	_GSScreenHeight, %edx
	movl	_GSScreenWidth, %ecx
	movl	%edx, 36(%esp)
	movl	%ecx, 32(%esp)
	movl	$0, 28(%esp)
	movl	$0, 24(%esp)
	movl	4(%eax), %esi
	movl	%esi, 20(%esp)
	movl	(%eax), %edx
	movl	$0, 12(%esp)
	movl	%edx, 16(%esp)
	movl	$0, 8(%esp)
	movl	%ebx, 4(%esp)
	movl	%eax, (%esp)
	call	_stretch_blit
	jmp	L180
	.p2align 4,,7
L181:
	movl	_GSScreenHeight, %ecx
	movl	_GSScreenWidth, %esi
	movl	%ecx, 20(%esp)
	movl	%esi, 16(%esp)
	movl	$0, 12(%esp)
	movl	$0, 8(%esp)
	movl	%ebx, 4(%esp)
	movl	%eax, (%esp)
	call	_draw_sprite_tile
	jmp	L180
	.cfi_endproc
LFE177:
LC10:
	.ascii "xapps/setwall.app\0"
LC11:
	.ascii "Properties\0"
	.p2align 4,,15
	.globl	_WEventHandler
_WEventHandler:
LFB178:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$40, %esp
	.cfi_def_cfa_offset 48
	movl	52(%esp), %ebx
	movl	(%ebx), %eax
	cmpl	$1, %eax
	je	L189
L184:
	cmpl	$4, %eax
	je	L190
L187:
	xorl	%eax, %eax
L186:
	addl	$40, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L190:
	.cfi_restore_state
	cmpl	$-60927, 4(%ebx)
	jne	L187
	movl	$0, 20(%esp)
	movl	$0, 16(%esp)
	movl	$0, 12(%esp)
	movl	$-267452408, 8(%esp)
	movl	$0, 4(%esp)
	movl	$LC11, (%esp)
	call	_NewMenuItem
	movl	%eax, (%esp)
	ca