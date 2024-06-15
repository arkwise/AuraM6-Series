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
	call	_NewMenu
	movl	_Mouse, %ecx
	movl	_DeskTop, %edx
	movl	$0, 20(%esp)
	movl	%edx, 16(%esp)
	movl	%eax, 12(%esp)
	movl	4(%ecx), %ebx
	movl	(%ecx), %eax
	movl	%eax, 4(%esp)
	movl	%ebx, 8(%esp)
	movl	$_Me, (%esp)
	call	_PopUpMenu
	movl	$1, %eax
	jmp	L186
	.p2align 4,,7
L189:
	cmpl	$-267452408, 4(%ebx)
	jne	L187
	movl	$0, 4(%esp)
	movl	$LC10, (%esp)
	call	_FileRun
	movl	(%ebx), %eax
	jmp	L184
	.cfi_endproc
LFE178:
	.p2align 4,,15
	.globl	_TraybarRemove
_TraybarRemove:
LFB199:
	.cfi_startproc
	subl	$44, %esp
	.cfi_def_cfa_offset 48
	movl	_TraybarIconList, %edx
	movl	48(%esp), %eax
	movl	%edx, (%esp)
	movl	%eax, 4(%esp)
	call	_ListKeyItem
	movl	_TraybarIconList, %ecx
	movl	%ecx, (%esp)
	movl	%eax, 4(%esp)
	call	_ListRemoveItem
	movl	_TraybarIconList, %eax
	movl	(%eax), %edx
	movl	_GSTraybar, %ecx
	testl	%edx, %edx
	je	L196
	movl	32(%ecx), %eax
	movl	%eax, 16(%esp)
	movl	28(%ecx), %edx
	movl	%edx, 12(%esp)
	movl	24(%ecx), %eax
	movl	%eax, 8(%esp)
	movl	20(%ecx), %edx
	movl	%ecx, (%esp)
	addl	$18, %edx
	movl	%edx, 4(%esp)
	call	_WidgetSetMetrics
	movl	_GSTraybar, %ecx
	movl	$0, 4(%esp)
	movl	%ecx, (%esp)
	call	_WidgetDraw
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L196:
	.cfi_restore_state
	movl	%ecx, (%esp)
	call	_WidgetDispose
	movl	$0, _GSTraybar
	addl	$44, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE199:
	.p2align 4,,15
	.globl	_TaskbarEventHandler
_TaskbarEventHandler:
LFB186:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	xorl	%eax, %eax
	subl	$40, %esp
	.cfi_def_cfa_offset 48
	movl	52(%esp), %edx
	cmpl	$4, (%edx)
	je	L212
L198:
	addl	$40, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L212:
	.cfi_restore_state
	movl	4(%edx), %ecx
	cmpl	$-60924, %ecx
	je	L213
	cmpl	$-60927, %ecx
	jne	L198
	movl	_WindowList, %eax
	movl	(%eax), %ebx
	movl	8(%ebx), %edx
	movl	%edx, (%esp)
	call	_MinimizeWindow
L211:
	movl	48(%esp), %eax
	movl	$0, 4(%esp)
	movl	%eax, (%esp)
	call	_WidgetDraw
	addl	$40, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L213:
	.cfi_restore_state
	movl	_WindowList, %ecx
	movl	(%ecx), %ebx
	testl	%ebx, %ebx
	jne	L207
	jmp	L214
	.p2align 4,,7
L200:
	movl	_WindowList, %edx
	movl	4(%ebx), %ebx
	cmpl	%ebx, (%edx)
	je	L211
L207:
	movl	24(%ebx), %eax
	movl	%eax, 8(%esp)
	movl	28(%ebx), %edx
	movl	%edx, 12(%esp)
	movl	32(%ebx), %ecx
	movl	%ecx, 16(%esp)
	movl	_Mouse, %ecx
	movl	36(%ebx), %eax
	movl	%eax, 20(%esp)
	movl	4(%ecx), %edx
	movl	(%ecx), %eax
	movl	%eax, (%esp)
	movl	%edx, 4(%esp)
	call	_PointInRect
	testl	%eax, %eax
	je	L200
	movl	8(%ebx), %ecx
	movl	%ecx, (%esp)
	call	_WidgetSetFirst
	jmp	L200
L214:
	movl	$1, %eax
	jmp	L198
	.cfi_endproc
LFE186:
	.p2align 4,,15
	.globl	_DesktopWidgetSetFirstEx
_DesktopWidgetSetFirstEx:
LFB204:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$24, %esp
	.cfi_def_cfa_offset 32
	movl	36(%esp), %ebx
	movl	32(%esp), %eax
	movl	%eax, (%esp)
	movl	%ebx, 4(%esp)
	call	__WidgetSetFirstEx
	movl	$1466527264, 4(%esp)
	movl	%ebx, (%esp)
	call	_WidgetIsClassId
	testl	%eax, %eax
	je	L215
	movl	204(%ebx), %eax
	testb	$2, %al
	je	L217
	andl	$-3, %eax
	movl	%eax, 204(%ebx)
L217:
	movl	_GSTaskbar, %edx
	movl	$0, 36(%esp)
	movl	%edx, 32(%esp)
	addl	$24, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	jmp	_WidgetDraw
	.p2align 4,,7
L215:
	.cfi_restore_state
	addl	$24, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE204:
LC12:
	.ascii "TaskbarRemove %x...\0"
LC13:
	.ascii "...finished\0"
	.p2align 4,,15
	.globl	_DesktopWidgetRemove
_DesktopWidgetRemove:
LFB205:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$24, %esp
	.cfi_def_cfa_offset 32
	movl	36(%esp), %ebx
	movl	32(%esp), %eax
	movl	%eax, (%esp)
	movl	%ebx, 4(%esp)
	call	__WidgetRemove
	movl	$1466527264, 4(%esp)
	movl	%ebx, (%esp)
	call	_WidgetIsClassId
	testl	%eax, %eax
	jne	L229
	addl	$24, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L229:
	.cfi_restore_state
	movl	%ebx, 4(%esp)
	movl	$LC12, (%esp)
	call	_DebugMessage
	movl	_WindowList, %edx
	movl	%ebx, 4(%esp)
	movl	%edx, (%esp)
	call	_ListFoundItem
	testl	%eax, %eax
	je	L224
	movl	_WindowList, %ecx
	movl	%eax, 4(%esp)
	movl	%ecx, (%esp)
	call	_ListRemoveItem
	movl	_GSTaskbar, %ebx
	movl	$0, 4(%esp)
	movl	%ebx, (%esp)
	call	_WidgetDraw
L224:
	movl	$LC13, 32(%esp)
	addl	$24, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	jmp	_DebugMessage
	.cfi_endproc
LFE205:
LC14:
	.ascii "TaskbarAdd %x\0"
	.p2align 4,,15
	.globl	_DesktopWidgetInsert
_DesktopWidgetInsert:
LFB203:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$20, %esp
	.cfi_def_cfa_offset 32
	movl	36(%esp), %ebx
	movl	32(%esp), %eax
	movl	%eax, (%esp)
	movl	%ebx, 4(%esp)
	call	__WidgetInsert
	movl	$1466527264, 4(%esp)
	movl	%ebx, (%esp)
	call	_WidgetIsClassId
	testl	%eax, %eax
	jne	L237
	addl	$20, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L237:
	.cfi_restore_state
	movl	%ebx, 4(%esp)
	movl	$LC14, (%esp)
	call	_DebugMessage
	movl	$40, (%esp)
	call	_malloc
	movl	%eax, %edx
	testl	%eax, %eax
	je	L232
	movl	$10, %ecx
	xorl	%eax, %eax
	movl	%edx, %edi
	rep stosl
L232:
	movl	%ebx, 8(%edx)
	movl	_WindowList, %ebx
	movl	$0, 20(%edx)
	movl	%edx, 4(%esp)
	movl	%ebx, (%esp)
	call	_ListAddItem
	movl	_GSTaskbar, %edx
	movl	$0, 36(%esp)
	movl	%edx, 32(%esp)
	addl	$20, %esp
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	jmp	_WidgetDraw
	.cfi_endproc
LFE203:
LC15:
	.ascii "./DESKTOP/start.bmp\0"
LC16:
	.ascii "/USER/DESKTOP/wallpaper\0"
LC17:
	.ascii "/USER/DESKTOP/alignment\0"
LC18:
	.ascii "3D61AD\0"
LC19:
	.ascii "/USER/DESKTOP/color\0"
	.p2align 4,,15
	.globl	_WallpaperReLoad
_WallpaperReLoad:
LFB180:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$24, %esp
	.cfi_def_cfa_offset 32
	movl	_WllBmp, %eax
	testl	%eax, %eax
	je	L239
	movl	%eax, (%esp)
	call	_destroy_bitmap
L239:
	movl	$LC15, 4(%esp)
	movl	$LC16, (%esp)
	call	_KeyGetText
	movl	$0, 4(%esp)
	movl	$LC17, (%esp)
	movl	%eax, %ebx
	call	_KeyGetInt
	movl	%ebx, (%esp)
	movl	%eax, _Style
	call	_TextLen
	testl	%eax, %eax
	jne	L248
	movl	$0, _WllBmp
L241:
	movl	$LC18, 4(%esp)
	movl	$LC19, (%esp)
	movl	_DeskTop, %ebx
	call	_KeyGetText
	movl	%eax, (%esp)
	call	_GetColorFromHex
	movl	_WllBmp, %edx
	movl	%eax, 100(%ebx)
	movl	_DeskTop, %ecx
	testl	%edx, %edx
	je	L242
	movl	$_WDraw, 128(%ecx)
L243:
	movl	$_WEventHandler, 116(%ecx)
	movl	$0, 4(%esp)
	movl	%ecx, (%esp)
	call	_WidgetDraw
	addl	$24, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L248:
	.cfi_restore_state
	movl	%ebx, (%esp)
	call	_LoadImage
	movl	%eax, _WllBmp
	jmp	L241
	.p2align 4,,7
L242:
	movl	$0, 128(%ecx)
	jmp	L243
	.cfi_endproc
LFE180:
	.p2align 4,,15
	.globl	_DrawLikeSkin
_DrawLikeSkin:
LFB173:
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
	subl	$108, %esp
	.cfi_def_cfa_offset 128
	movl	136(%esp), %edx
	movl	140(%esp), %ecx
	movl	144(%esp), %eax
	movl	132(%esp), %ebx
	movl	%edx, 72(%esp)
	movl	%ecx, 76(%esp)
	movl	156(%esp), %edx
	movl	164(%esp), %ecx
	movl	%eax, 68(%esp)
	movl	128(%esp), %edi
	movl	148(%esp), %ebp
	movl	152(%esp), %esi
	movl	%edx, 60(%esp)
	movl	160(%esp), %eax
	movl	%ecx, 64(%esp)
	testl	%ebx, %ebx
	je	L252
	movl	76(%esp), %edx
	movl	72(%esp), %ecx
	movl	%eax, 28(%esp)
	movl	%esi, 24(%esp)
	movl	%edx, 20(%esp)
	movl	%ecx, 16(%esp)
	movl	%edi, 4(%esp)
	movl	%ebx, (%esp)
	movl	$0, 12(%esp)
	movl	$0, 8(%esp)
	movl	%eax, 52(%esp)
	call	_masked_blit
	movl	64(%esp), %eax
	movl	%ebp, %edx
	subl	%eax, %edx
	movl	64(%esp), %ecx
	incl	%edx
	movl	72(%esp), %eax
	movl	%edx, 80(%esp)
	movl	%ecx, 28(%esp)
	movl	80(%esp), %edx
	movl	%esi, 24(%esp)
	movl	%eax, 16(%esp)
	movl	%edx, 20(%esp)
	movl	4(%ebx), %edx
	movl	64(%esp), %ecx
	subl	%ecx, %edx
	movl	%edi, 4(%esp)
	movl	%edx, 12(%esp)
	movl	%ebx, (%esp)
	movl	$0, 8(%esp)
	call	_masked_blit
	movl	60(%esp), %eax
	movl	68(%esp), %edx
	movl	52(%esp), %ecx
	subl	%eax, %edx
	movl	%ecx, 28(%esp)
	incl	%edx
	movl	76(%esp), %eax
	movl	%edx, 84(%esp)
	movl	60(%esp), %edx
	movl	84(%esp), %ecx
	movl	%eax, 20(%esp)
	movl	%ecx, 16(%esp)
	movl	%edx, 24(%esp)
	movl	$0, 12(%esp)
	movl	60(%esp), %eax
	movl	(%ebx), %edx
	movl	%edi, 4(%esp)
	subl	%eax, %edx
	movl	%ebx, (%esp)
	movl	%edx, 8(%esp)
	call	_masked_blit
	movl	64(%esp), %ecx
	movl	%ecx, 28(%esp)
	movl	60(%esp), %edx
	movl	80(%esp), %eax
	movl	84(%esp), %ecx
	movl	%eax, 20(%esp)
	movl	%edx, 24(%esp)
	movl	%ecx, 16(%esp)
	movl	64(%esp), %eax
	movl	4(%ebx), %edx
	subl	%eax, %edx
	movl	%edx, 12(%esp)
	movl	60(%esp), %edx
	movl	(%ebx), %ecx
	movl	%edi, 4(%esp)
	subl	%edx, %ecx
	movl	%ebx, (%esp)
	movl	%ecx, 8(%esp)
	call	_masked_blit
	movl	76(%esp), %eax
	subl	%eax, %ebp
	movl	52(%esp), %eax
	movl	%ebp, 92(%esp)
	incl	%ebp
	subl	%eax, %ebp
	movl	64(%esp), %ecx
	movl	76(%esp), %edx
	subl	%ecx, %ebp
	addl	%eax, %edx
	movl	72(%esp), %ecx
	movl	%ebp, 36(%esp)
	movl	%esi, 32(%esp)
	movl	%ecx, 24(%esp)
	movl	%edx, 28(%esp)
	movl	%edx, 88(%esp)
	movl	4(%ebx), %edx
	subl	%eax, %edx
	movl	64(%esp), %ecx
	subl	%ecx, %edx
	movl	%esi, 16(%esp)
	movl	%edx, 20(%esp)
	movl	%eax, 12(%esp)
	movl	%edi, 4(%esp)
	movl	%ebx, (%esp)
	movl	$0, 8(%esp)
	call	_masked_stretch_blit
	movl	%ebp, 36(%esp)
	movl	88(%esp), %eax
	movl	60(%esp), %ebp
	movl	84(%esp), %edx
	movl	%ebp, 32(%esp)
	movl	%eax, 28(%esp)
	movl	%edx, 24(%esp)
	movl	4(%ebx), %ecx
	movl	52(%esp), %eax
	subl	%eax, %ecx
	movl	64(%esp), %ebp
	subl	%ebp, %ecx
	movl	%eax, 12(%esp)
	movl	%ecx, 20(%esp)
	movl	60(%esp), %ecx
	movl	%ecx, 16(%esp)
	movl	(%ebx), %edx
	movl	%edi, 4(%esp)
	subl	%ecx, %edx
	movl	%ebx, (%esp)
	movl	%edx, 8(%esp)
	call	_masked_stretch_blit
	movl	68(%esp), %edx
	movl	72(%esp), %eax
	movl	60(%esp), %ebp
	subl	%eax, %edx
	movl	76(%esp), %ecx
	movl	%edx, 68(%esp)
	incl	%edx
	subl	%esi, %edx
	movl	52(%esp), %eax
	subl	%ebp, %edx
	movl	72(%esp), %ebp
	addl	%esi, %ebp
	movl	%eax, 36(%esp)
	movl	%edx, 32(%esp)
	movl	%ebp, 24(%esp)
	movl	%eax, 20(%esp)
	movl	%ecx, 28(%esp)
	movl	(%ebx), %ecx
	movl	60(%esp), %eax
	subl	%esi, %ecx
	movl	%esi, 8(%esp)
	subl	%eax, %ecx
	movl	%edi, 4(%esp)
	movl	%ecx, 16(%esp)
	movl	%ebx, (%esp)
	movl	$0, 12(%esp)
	movl	%edx, 56(%esp)
	call	_masked_stretch_blit
	movl	64(%esp), %ecx
	movl	56(%esp), %edx
	movl	80(%esp), %eax
	movl	%ecx, 36(%esp)
	movl	%eax, 28(%esp)
	movl	%ebp, 24(%esp)
	movl	%ecx, 20(%esp)
	movl	%edx, 32(%esp)
	movl	60(%esp), %eax
	movl	(%ebx), %edx
	subl	%esi, %edx
	subl	%eax, %edx
	movl	%edx, 16(%esp)
	movl	4(%ebx), %edx
	movl	%esi, 8(%esp)
	subl	%ecx, %edx
	movl	%edi, 4(%esp)
	movl	%edx, 12(%esp)
	movl	%ebx, (%esp)
	call	_masked_stretch_blit
	movl	52(%esp), %eax
	movl	92(%esp), %edx
	movl	64(%esp), %ecx
	subl	%eax, %edx
	movl	%ebp, 152(%esp)
	subl	%ecx, %edx
	movl	60(%esp), %ecx
	incl	%edx
	movl	64(%esp), %ebp
	movl	%edx, 164(%esp)
	movl	68(%esp), %edx
	subl	%esi, %edx
	subl	%ecx, %edx
	incl	%edx
	movl	%edx, 160(%esp)
	movl	88(%esp), %edx
	movl	%edx, 156(%esp)
	movl	4(%ebx), %ecx
	subl	%eax, %ecx
	subl	%ebp, %ecx
	movl	%ecx, 148(%esp)
	movl	60(%esp), %ecx
	movl	(%ebx), %edx
	movl	%esi, 136(%esp)
	subl	%esi, %edx
	movl	%edi, 132(%esp)
	subl	%ecx, %edx
	movl	%ebx, 128(%esp)
	movl	%edx, 144(%esp)
	movl	%eax, 140(%esp)
	addl	$108, %esp
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
	jmp	_masked_stretch_blit
	.p2align 4,,7
L252:
	.cfi_restore_state
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	movl	$0, (%esp)
	call	_makecol
	movl	28(%edi), %ebx
	movl	68(%esp), %esi
	movl	%eax, 148(%esp)
	movl	%ebp, 144(%esp)
	movl	76(%esp), %eax
	movl	72(%esp), %ebp
	movl	%esi, 140(%esp)
	movl	%ebp, 132(%esp)
	movl	%edi, 128(%esp)
	movl	%eax, 136(%esp)
	movl	60(%ebx), %edx
	addl	$108, %esp
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
	jmp	*%edx
	.cfi_endproc
LFE173:
	.p2align 4,,15
	.globl	_DrawTransTHEMESkin
_DrawTransTHEMESkin:
LFB174:
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
	subl	$92, %esp
	.cfi_def_cfa_offset 112
	movl	120(%esp), %eax
	movl	116(%esp), %ebx
	movl	%eax, 64(%esp)
	movl	124(%esp), %edx
	movl	136(%esp), %esi
	movl	140(%esp), %eax
	movl	112(%esp), %ecx
	movl	%edx, 68(%esp)
	movl	128(%esp), %ebp
	movl	132(%esp), %edi
	movl	%esi, 72(%esp)
	movl	%eax, 76(%esp)
	testl	%ebx, %ebx
	je	L258
	movl	64(%esp), %esi
	movl	68(%esp), %edx
	subl	%esi, %ebp
	subl	%edx, %edi
	movl	%edi, 4(%esp)
	movl	%ecx, 60(%esp)
	leal	1(%ebp), %eax
	movl	%eax, (%esp)
	call	_create_bitmap
	movl	$255, 8(%esp)
	movl	$0, 4(%esp)
	movl	$255, (%esp)
	movl	%eax, %esi
	call	_makecol
	movl	28(%esi), %ecx
	movl	%eax, 4(%esp)
	movl	%esi, (%esp)
	call	*160(%ecx)
	movl	%edi, 36(%esp)
	movl	$10, 32(%esp)
	movl	$0, 28(%esp)
	movl	$0, 24(%esp)
	movl	4(%ebx), %edx
	movl	%ebx, (%esp)
	movl	%edx, 20(%esp)
	movl	$10, 16(%esp)
	movl	$0, 12(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	call	_masked_stretch_blit
	leal	-19(%ebp), %eax
	movl	%eax, 32(%esp)
	movl	%edi, 36(%esp)
	movl	$0, 28(%esp)
	movl	$10, 24(%esp)
	movl	4(%ebx), %ecx
	subl	$9, %ebp
	movl	%ecx, 20(%esp)
	movl	(%ebx), %edx
	movl	%ebx, (%esp)
	subl	$20, %edx
	movl	$0, 12(%esp)
	movl	%edx, 16(%esp)
	movl	$10, 8(%esp)
	movl	%esi, 4(%esp)
	call	_masked_stretch_blit
	movl	%edi, 36(%esp)
	movl	$10, 32(%esp)
	movl	$0, 28(%esp)
	movl	%ebp, 24(%esp)
	movl	4(%ebx), %ebp
	movl	$10, 16(%esp)
	movl	%ebp, 20(%esp)
	movl	$0, 12(%esp)
	movl	(%ebx), %edi
	movl	%ebx, (%esp)
	subl	$10, %edi
	movl	%esi, 4(%esp)
	movl	%edi, 8(%esp)
	call	_masked_stretch_blit
	movl	72(%esp), %eax
	movl	72(%esp), %ebx
	movl	$0, 8(%esp)
	sall	$8, %eax
	movl	$0, 4(%esp)
	subl	%ebx, %eax
	movl	$0, (%esp)
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	76(%esp)
	movl	%eax, 12(%esp)
	call	_set_trans_blender
	movl	28(%esi), %ecx
	movl	60(%esp), %ebp
	movl	68(%esp), %eax
	movl	64(%esp), %ebx
	cmpl	$32, (%ecx)
	movl	28(%ebp), %edi
	movl	%eax, 12(%esp)
	movl	%ebx, 8(%esp)
	movl	%esi, 4(%esp)
	movl	%ebp, (%esp)
	je	L259
	call	*88(%edi)
L256:
	call	_solid_mode
	movl	%esi, 112(%esp)
	addl	$92, %esp
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
	jmp	_destroy_bitmap
	.p2align 4,,7
L259:
	.cfi_restore_state
	call	*92(%edi)
	jmp	L256
	.p2align 4,,7
L258:
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	movl	$0, (%esp)
	movl	%ecx, 60(%esp)
	call	_makecol
	movl	60(%esp), %esi
	movl	68(%esp), %ecx
	movl	28(%esi), %edx
	movl	%ebp, 124(%esp)
	movl	64(%esp), %ebp
	movl	%edi, 128(%esp)
	movl	%ebp, 116(%esp)
	movl	%esi, 112(%esp)
	movl	%eax, 132(%esp)
	movl	%ecx, 120(%esp)
	movl	60(%edx), %eax
	addl	$92, %esp
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
	jmp	*%eax
	.cfi_endproc
LFE174:
LC20:
	.ascii "Settings\0"
	.p2align 4,,15
	.globl	_CPButtonDraw
_CPButtonDraw:
LFB190:
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
	movl	_THMPanelCPDown, %eax
	movl	64(%esp), %ebx
	movl	68(%esp), %esi
	testl	%eax, %eax
	je	L261
	movl	_THMPanelCPOver, %edx
	testl	%edx, %edx
	je	L261
	movl	_THMPanelCP, %ecx
	testl	%ecx, %ecx
	je	L261
	movl	_UseSkins, %ebp
	testl	%ebp, %ebp
	je	L261
	movl	212(%ebx), %edi
	cmpw	$1, %di
	je	L285
	cmpw	$2, %di
	je	L264
	movl	_Mouse, %edx
	testb	$1, 12(%edx)
	je	L265
	movl	72(%esp), %ebp
	movl	(%edx), %edi
	cmpl	0(%ebp), %edi
	jl	L265
	movl	4(%edx), %edx
	cmpl	4(%ebp), %edx
	jl	L265
	cmpl	8(%ebp), %edi
	jg	L265
	cmpl	12(%ebp), %edx
	jg	L265
L264:
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
	jmp	L260
	.p2align 4,,7
L261:
	movl	212(%ebx), %edi
	cmpw	$1, %di
	je	L286
L266:
	cmpw	$2, %di
	movl	_SysColors, %ecx
	je	L267
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
L284:
	movl	_default_font, %ecx
	movl	%ebp, 12(%esp)
	movl	$LC20, 8(%esp)
	movl	%ecx, 4(%esp)
	movl	%esi, (%esp)
	call	_textout_centre_ex
L260:
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
L267:
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
	jmp	L284
	.p2align 4,,7
L265:
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
L286:
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
	jmp	L266
	.p2align 4,,7
L285:
	movl	$255, 28(%esp)
	movl	$255, 24(%esp)
	movl	32(%ebx), %ecx
	movl	%ecx, 20(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 16(%esp)
	movl	24(%ebx), %ebp
	movl	%ebp, 12(%esp)
	movl	20(%ebx), %ebx
	movl	%ebx, 8(%esp)
	movl	(%edx), %edx
	movl	%esi, (%esp)
	movl	%edx, 4(%esp)
	call	_DrawTransTHEMESkin
	jmp	L260
	.cfi_endproc
LFE190:
	.p2align 2
LC21:
	.ascii "/USER/DESKTOP/transparency/dock\0"
	.p2align 4,,15
	.globl	_PanelDraw
_PanelDraw:
LFB187:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$40, %esp
	.cfi_def_cfa_offset 48
	movl	$255, 4(%esp)
	movl	$LC21, (%esp)
	movl	48(%esp), %ebx
	call	_KeyGetInt
	movl	_THMDockFace, %edx
	movl	%eax, _dock
	testl	%edx, %edx
	je	L288
	movl	_UseSkins, %ecx
	testl	%ecx, %ecx
	jne	L294
L288:
	movl	_SysColors, %eax
	movl	12(%eax), %edx
	movl	%edx, 24(%esp)
	movl	8(%eax), %ecx
	movl	%ecx, 20(%esp)
	movl	32(%ebx), %eax
	movl	%eax, 16(%esp)
	movl	52(%esp), %eax
	movl	28(%ebx), %edx
	movl	%edx, 12(%esp)
	movl	24(%ebx), %ecx
	movl	%ecx, 8(%esp)
	movl	20(%ebx), %ebx
	movl	%eax, (%esp)
	movl	%ebx, 4(%esp)
	call	_Rect3D
	addl	$40, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L294:
	.cfi_restore_state
	movl	$255, 28(%esp)
	movl	%eax, 24(%esp)
	movl	32(%ebx), %ecx
	movl	%ecx, 20(%esp)
	movl	28(%ebx), %eax
	movl	%eax, 16(%esp)
	movl	52(%esp), %eax
	movl	24(%ebx), %ecx
	movl	%ecx, 12(%esp)
	movl	20(%ebx), %ebx
	movl	%ebx, 8(%esp)
	movl	(%edx), %edx
	movl	%eax, (%esp)
	movl	%edx, 4(%esp)
	call	_DrawTransTHEMESkin
	addl	$40, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE187:
	.p2align 2
LC22:
	.ascii "/USER/DESKTOP/transparency/window_list\0"
	.p2align 4,,15
	.globl	_TaskbarDraw
_TaskbarDraw:
LFB185:
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
	subl	$92, %esp
	.cfi_def_cfa_offset 112
	movl	$255, 4(%esp)
	movl	$LC22, (%esp)
	movl	112(%esp), %esi
	movl	116(%esp), %ebp
	call	_KeyGetInt
	movl	_SysColors, %edx
	movl	4(%edx), %ecx
	movl	%eax, _windowlst
	movl	28(%ebp), %eax
	movl	%ecx, 20(%esp)
	movl	32(%esi), %ebx
	movl	%ebx, 16(%esp)
	movl	28(%esi), %edi
	movl	%edi, 12(%esp)
	movl	24(%esi), %edx
	movl	%edx, 8(%esp)
	movl	20(%esi), %ecx
	movl	%ebp, (%esp)
	movl	%ecx, 4(%esp)
	call	*60(%eax)
	movl	_THMPanelFace, %eax
	testl	%eax, %eax
	je	L296
	movl	_UseSkins, %ebx
	testl	%ebx, %ebx
	jne	L336
L296:
	movl	_SysColors, %edi
	movl	12(%edi), %edx
	movl	%edx, 24(%esp)
	movl	8(%edi), %ecx
	movl	%ecx, 20(%esp)
	movl	32(%esi), %eax
	movl	%eax, 16(%esp)
	movl	28(%esi), %ebx
	movl	%ebx, 12(%esp)
	movl	24(%esi), %edi
	movl	%edi, 8(%esp)
	movl	20(%esi), %edx
	movl	%ebp, (%esp)
	movl	%edx, 4(%esp)
	call	_Rect3D
L297:
	movl	_WindowList, %ebx
	xorl	%edi, %edi
	movl	(%ebx), %ebx
	movl	%ebx, %edx
	testl	%ebx, %ebx
	je	L295
	.p2align 4,,7
L325:
	movl	4(%edx), %edx
	incl	%edi
	cmpl	%edx, %ebx
	jne	L325
	movl	20(%esi), %ecx
	movl	28(%esi), %eax
	subl	%ecx, %eax
	subl	$10, %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%edi
	cmpl	$150, %eax
	jg	L337
L299:
	movl	(%ebx), %ebx
	addl	$3, %eax
	xorl	%edx, %edx
	movl	%ebp, %edi
	movl	%ebx, 60(%esp)
	movl	%eax, 56(%esp)
	movl	%edx, %ebp
L309:
	movl	56(%esp), %edx
	movl	32(%esi), %eax
	addl	%ebp, %edx
	subl	$2, %eax
	movl	%eax, 16(%esp)
	leal	-3(%ecx,%edx), %eax
	leal	5(%ebp,%ecx), %ecx
	movl	%eax, 12(%esp)
	leal	64(%esp), %eax
	movl	24(%esi), %edx
	movl	%ecx, 4(%esp)
	addl	$2, %edx
	movl	%eax, (%esp)
	movl	%edx, 8(%esp)
	call	_RectAssign
	movl	68(%esp), %ecx
	movl	64(%esp), %edx
	movl	%ecx, 28(%ebx)
	movl	_Mouse, %ecx
	movl	%edx, 24(%ebx)
	movl	72(%esp), %eax
	movl	76(%esp), %edx
	movl	%eax, 32(%ebx)
	movl	%edx, 36(%ebx)
	testb	$1, 12(%ecx)
	je	L300
	movl	(%ecx), %edx
	cmpl	64(%esp), %edx
	jl	L300
	movl	4(%ecx), %eax
	cmpl	68(%esp), %eax
	jge	L338
	.p2align 4,,7
L300:
	movl	8(%ebx), %ecx
	movl	_Desktop, %edx
	movl	84(%ecx), %eax
	cmpl	%eax, 92(%edx)
	je	L301
	movl	_SysColors, %ecx
	movl	12(%ecx), %edx
	movl	%edx, 24(%esp)
	movl	72(%esp), %edx
	movl	8(%ecx), %eax
	movl	76(%esp), %ecx
	movl	%eax, 20(%esp)
	movl	%ecx, 16(%esp)
	movl	68(%esp), %eax
	movl	64(%esp), %ecx
	movl	%edx, 12(%esp)
	movl	%ecx, 4(%esp)
	movl	%eax, 8(%esp)
	movl	%edi, (%esp)
	call	_Rect3D
	movl	8(%ebx), %ecx
	movl	236(%ecx), %edx
	testl	%edx, %edx
	je	L306
	movl	4(%edx), %edx
	testl	%edx, %edx
	je	L306
	movl	76(%esp), %ecx
	movl	68(%esp), %eax
	addl	%ecx, %eax
	movl	%edx, 4(%esp)
	movl	%eax, %ecx
	movl	%edi, (%esp)
	shrl	$31, %ecx
	addl	%ecx, %eax
	sarl	%eax
	subl	$8, %eax
	movl	%eax, 12(%esp)
	movl	64(%esp), %eax
	addl	$5, %eax
	movl	%eax, 8(%esp)
	movl	28(%edi), %edx
	call	*100(%edx)
	movl	8(%ebx), %ecx
L306:
	movl	196(%ecx), %edx
	testl	%edx, %edx
	je	L305
	movl	_SysColors, %eax
	movl	(%eax), %ecx
	movl	$-26, %eax
	movl	%ecx, 48(%esp)
	movl	64(%esp), %ecx
	subl	%ecx, %eax
	movl	72(%esp), %ecx
	movl	%edx, 44(%esp)
	leal	-5(%ecx,%eax), %eax
	movl	76(%esp), %ecx
	movl	%eax, 52(%esp)
	movl	68(%esp), %eax
	addl	%ecx, %eax
	movl	%eax, %ecx
	shrl	$31, %ecx
	addl	%eax, %ecx
	movl	_default_font, %eax
	sarl	%ecx
	movl	%eax, (%esp)
	movl	%ecx, 36(%esp)
	call	_text_height
	movl	44(%esp), %ecx
	movl	48(%esp), %edx
	movl	%ecx, 20(%esp)
	movl	%eax, %ecx
	shrl	$31, %ecx
	movl	%edx, 24(%esp)
	addl	%ecx, %eax
	movl	52(%esp), %edx
	sarl	%eax
	movl	%edx, 16(%esp)
	movl	36(%esp), %edx
	subl	%eax, %edx
	movl	64(%esp), %eax
	movl	%edx, 12(%esp)
	addl	$26, %eax
L335:
	movl	%eax, 8(%esp)
	movl	_default_font, %eax
	movl	%eax, 4(%esp)
	movl	%edi, (%esp)
	call	_DrawNiceText
L305:
	movl	(%ebx), %ebx
	movl	56(%esp), %ecx
	addl	%ecx, %ebp
	cmpl	%ebx, 60(%esp)
	je	L295
	movl	20(%esi), %ecx
	jmp	L309
	.p2align 4,,7
L338:
	cmpl	72(%esp), %edx
	jg	L300
	cmpl	76(%esp), %eax
	jg	L300
	.p2align 4,,7
L301:
	movl	_SysColors, %ecx
	movl	8(%ecx), %edx
	movl	%edx, 24(%esp)
	movl	72(%esp), %edx
	movl	12(%ecx), %eax
	movl	76(%esp), %ecx
	movl	%eax, 20(%esp)
	movl	%ecx, 16(%esp)
	movl	68(%esp), %eax
	movl	64(%esp), %ecx
	movl	%edx, 12(%esp)
	movl	%ecx, 4(%esp)
	movl	%eax, 8(%esp)
	movl	%edi, (%esp)
	call	_Rect3D
	movl	8(%ebx), %ecx
	movl	236(%ecx), %edx
	testl	%edx, %edx
	je	L303
	movl	4(%edx), %edx
	testl	%edx, %edx
	je	L303
	movl	76(%esp), %ecx
	movl	68(%esp), %eax
	addl	%ecx, %eax
	movl	%edx, 4(%esp)
	movl	%eax, %ecx
	movl	%edi, (%esp)
	shrl	$31, %ecx
	addl	%ecx, %eax
	sarl	%eax
	subl	$7, %eax
	movl	%eax, 12(%esp)
	movl	64(%esp), %eax
	addl	$6, %eax
	movl	%eax, 8(%esp)
	movl	28(%edi), %edx
	call	*100(%edx)
	movl	8(%ebx), %ecx
L303:
	movl	196(%ecx), %edx
	testl	%edx, %edx
	je	L305
	movl	_SysColors, %eax
	movl	(%eax), %ecx
	movl	$-27, %eax
	movl	%ecx, 48(%esp)
	movl	64(%esp), %ecx
	subl	%ecx, %eax
	movl	72(%esp), %ecx
	movl	%edx, 44(%esp)
	leal	-6(%ecx,%eax), %eax
	movl	76(%esp), %ecx
	movl	%eax, 52(%esp)
	movl	68(%esp), %eax
	addl	%ecx, %eax
	movl	%eax, %ecx
	shrl	$31, %ecx
	addl	%eax, %ecx
	movl	_default_font, %eax
	sarl	%ecx
	movl	%eax, (%esp)
	movl	%ecx, 36(%esp)
	call	_text_height
	movl	44(%esp), %ecx
	movl	48(%esp), %edx
	movl	%ecx, 20(%esp)
	movl	%eax, %ecx
	shrl	$31, %ecx
	movl	%edx, 24(%esp)
	addl	%ecx, %eax
	movl	52(%esp), %edx
	sarl	%eax
	movl	%edx, 16(%esp)
	movl	36(%esp), %edx
	subl	%eax, %edx
	movl	64(%esp), %eax
	movl	%edx, 12(%esp)
	addl	$27, %eax
	jmp	L335
	.p2align 4,,7
L295:
	addl	$92, %esp
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
L337:
	.cfi_restore_state
	movl	$150, %eax
	jmp	L299
	.p2align 4,,7
L336:
	movl	_windowlst, %ecx
	movl	$255, 28(%esp)
	movl	%ecx, 24(%esp)
	movl	32(%esi), %ebx
	movl	%ebx, 20(%esp)
	movl	28(%esi), %edi
	movl	%edi, 16(%esp)
	movl	24(%esi), %edx
	movl	%edx, 12(%esp)
	movl	20(%esi), %ecx
	movl	%ecx, 8(%esp)
	movl	(%eax), %eax
	movl	%ebp, (%esp)
	movl	%eax, 4(%esp)
	call	_DrawTransTHEMESkin
	jmp	L297
	.cfi_endproc
LFE185:
LC23:
	.ascii "\0"
LC24:
	.ascii "bmp\0"
LC25:
	.ascii "left\0"
LC26:
	.ascii "right\0"
LC27:
	.ascii "top\0"
LC28:
	.ascii "bottom\0"
	.p2align 4,,15
	.globl	_ThemeLoadItem
_ThemeLoadItem:
LFB175:
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
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	48(%esp), %esi
	movl	52(%esp), %edi
	movl	$LC23, 12(%esp)
	movl	$LC24, 8(%esp)
	movl	%edi, 4(%esp)
	movl	%esi, (%esp)
	call	_INIGetNew
	cmpb	$0, (%eax)
	je	L342
	movl	%eax, (%esp)
	call	_LoadImage
	movl	%eax, %ebp
	testl	%eax, %eax
	je	L342
	movl	$20, (%esp)
	call	_malloc
	movl	%eax, %ebx
	movl	%ebp, (%eax)
	movl	%edi, 4(%esp)
	movl	%esi, (%esp)
	movl	$0, 12(%esp)
	movl	$LC25, 8(%esp)
	call	_INIGetInteger
	movl	%eax, 4(%ebx)
	movl	%edi, 4(%esp)
	movl	%esi, (%esp)
	movl	$0, 12(%esp)
	movl	$LC26, 8(%esp)
	call	_INIGetInteger
	movl	%eax, 8(%ebx)
	movl	%edi, 4(%esp)
	movl	%esi, (%esp)
	movl	$0, 12(%esp)
	movl	$LC27, 8(%esp)
	call	_INIGetInteger
	movl	%eax, 12(%ebx)
	movl	%edi, 4(%esp)
	movl	%esi, (%esp)
	movl	$0, 12(%esp)
	movl	$LC28, 8(%esp)
	call	_INIGetInteger
	movl	%eax, 16(%ebx)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
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
L342:
	.cfi_restore_state
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	xorl	%ebx, %ebx
	movl	%ebx, %eax
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
LFE175:
LC29:
	.ascii "FileName: %s\12\0"
LC30:
	.ascii "sidebar_face\0"
LC31:
	.ascii "sidebar_clock\0"
LC32:
	.ascii "dock_face\0"
LC33:
	.ascii "panel_face\0"
LC34:
	.ascii "panel\0"
LC35:
	.ascii "panel_over\0"
LC36:
	.ascii "panel_down\0"
LC37:
	.ascii "cp\0"
LC38:
	.ascii "cp_over\0"
LC39:
	.ascii "cp_down\0"
LC40:
	.ascii "app\0"
LC41:
	.ascii "app_over\0"
LC42:
	.ascii "app_down\0"
LC43:
	.ascii "usr\0"
LC44:
	.ascii "usr_over\0"
LC45:
	.ascii "usr_down\0"
LC46:
	.ascii "exit\0"
LC47:
	.ascii "exit_over\0"
LC48:
	.ascii "exit_down\0"
LC49:
	.ascii "nav\0"
LC50:
	.ascii "nav_over\0"
LC51:
	.ascii "nav_down\0"
LC52:
	.ascii "panel_left\0"
LC53:
	.ascii "task\0"
LC54:
	.ascii "task_down\0"
LC55:
	.ascii "sysmenu\0"
	.p2align 4,,15
	.globl	_ThemeLoad
_ThemeLoad:
LFB176:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$24, %esp
	.cfi_def_cfa_offset 32
	movl	32(%esp), %ebx
	movl	$LC29, (%esp)
	movl	%ebx, 4(%esp)
	call	_DebugMessage
	movl	%ebx, (%esp)
	movl	$LC30, 4(%esp)
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC31, 4(%esp)
	movl	%eax, _THMSidebarFace
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC32, 4(%esp)
	movl	%eax, _THMSidebarClock
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC33, 4(%esp)
	movl	%eax, _THMDockFace
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC34, 4(%esp)
	movl	%eax, _THMPanelFace
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC35, 4(%esp)
	movl	%eax, _THMPanelStart
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC36, 4(%esp)
	movl	%eax, _THMPanelStartOver
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC37, 4(%esp)
	movl	%eax, _THMPanelStartDown
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC38, 4(%esp)
	movl	%eax, _THMPanelCP
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC39, 4(%esp)
	movl	%eax, _THMPanelCPOver
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC40, 4(%esp)
	movl	%eax, _THMPanelCPDown
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC41, 4(%esp)
	movl	%eax, _THMPanelapp
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC42, 4(%esp)
	movl	%eax, _THMPanelappOver
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC43, 4(%esp)
	movl	%eax, _THMPanelappDown
	call	_ThemeLoadItem
	movl	%ebx, (%esp)
	movl	$LC44, 4(%esp)
	movl	%eax