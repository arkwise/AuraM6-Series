	.file	"desktop.c"
	.section .text
	.p2align 4,,15
	.globl	_PanelEventHandler
_PanelEventHandler:
LFB187:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
LFE187:
	.p2align 4,,15
	.globl	_TraybarEventHandler
_TraybarEventHandler:
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
LFE191:
	.p2align 2
LC0:
	.ascii "Are you sure you want to exit oZone?\0"
LC1:
	.ascii "Codename Phoenix\0"
LC2:
	.ascii "/Control Panel/\0"
LC3:
	.ascii "xapps/nav.app\0"
LC4:
	.ascii "about\0"
LC5:
	.ascii "xapps/about.app\0"
	.p2align 4,,15
	.globl	_AppEventHandler
_AppEventHandler:
LFB200:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$120, %esp
	.cfi_def_cfa_offset 128
	movl	132(%esp), %edx
	movl	4(%edx), %eax
	cmpl	$-267452413, %eax
	je	L25
	jbe	L32
	cmpl	$-267452412, %eax
	je	L26
	cmpl	$-65535, %eax
	je	L33
L29:
	xorl	%eax, %eax
L22:
	addl	$120, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L32:
	.cfi_restore_state
	cmpl	$-267452415, %eax
	je	L23
	cmpl	$-267452414, %eax
	jne	L29
	movl	$0, 4(%esp)
	movl	8(%edx), %ecx
	movl	48(%ecx), %ebx
	movl	%ebx, (%esp)
	call	_FileRun
	addl	$120, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L33:
	.cfi_restore_state
	movl	_GSStart, %eax
	leal	68(%esp), %ebx
	movl	32(%eax), %edx
	incl	%edx
	movl	%edx, 8(%esp)
	movl	20(%eax), %ecx
	movl	%ebx, (%esp)
	subl	$5, %ecx
	movl	%ecx, 4(%esp)
	call	_PointAssign
	movl	72(%esp), %ebx
	movl	68(%esp), %ecx
	movl	_GSStart, %eax
	movl	_Menu, %edx
	movl	%ebx, 8(%esp)
	movl	$0, 20(%esp)
	movl	%eax, 16(%esp)
	movl	%edx, 12(%esp)
	movl	%ecx, 4(%esp)
	movl	$_Me, (%esp)
	call	_PopUpMenu
	addl	$120, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L26:
	.cfi_restore_state
	movl	$LC4, 4(%esp)
	movl	$LC5, (%esp)
	call	_FileRun
	addl	$120, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L23:
	.cfi_restore_state
	movl	$1026, 12(%esp)
	movl	$LC0, 8(%esp)
	movl	$LC1, 4(%esp)
	movl	$_Me, (%esp)
	call	_MessageBox
	movl	%eax, %edx
	movl	$1, %eax
	cmpl	$-32764, %edx
	jne	L22
	movl	_Mouse, %ebx
	movl	(%ebx), %ecx
	movl	%ecx, 24(%esp)
	movl	4(%ebx), %edx
	movl	%edx, 28(%esp)
	movl	_KState, %edx
	movl	8(%ebx), %ecx
	movl	%ecx, 32(%esp)
	movl	_KState+4, %ecx
	movl	12(%ebx), %ebx
	movl	%eax, 60(%esp)
	movl	%ebx, 36(%esp)
	leal	76(%esp), %ebx
	movl	%edx, 16(%esp)
	movl	%ecx, 20(%esp)
	movl	$0, 12(%esp)
	movl	$1, 8(%esp)
	movl	$1, 4(%esp)
	movl	%ebx, (%esp)
	call	_EventAssign
	movl	%ebx, (%esp)
	call	_HandleEvent
	movl	60(%esp), %eax
	jmp	L22
	.p2align 4,,7
L25:
	movl	$LC2, 4(%esp)
	movl	$LC3, (%esp)
	call	_FileRun
	addl	$120, %esp
	.cfi_def_cfa_offset 8
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE200:
	.p2align 4,,15
	.globl	_GenerateSubMenu
_GenerateSubMenu:
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
	subl	$76, %esp
	.cfi_def_cfa_offset 96
	movl	100(%esp), %eax
	movl	$0, 4(%esp)
	movl	%eax, (%esp)
	call	_ListDir
	movl	%eax, 44(%esp)
	testl	%eax, %eax
	je	L46
	movl	(%eax), %edx
	movl	%edx, 40(%esp)
	testl	%edx, %edx
	je	L47
	movl	%edx, %ebx
	xorl	%esi, %esi
	.p2align 4,,7
L41:
	movl	8(%ebx), %ecx
	movl	(%ecx), %edi
	cmpb	$46, (%edi)
	je	L37
	movl	$46, 4(%esp)
	movl	%edi, (%esp)
	call	_TextRChr
	testl	%eax, %eax
	je	L38
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
L39:
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
L37:
	movl	4(%ebx), %ebx
	cmpl	%ebx, 40(%esp)
	jne	L41
L36:
	movl	44(%esp), %edi
	movl	%edi, (%esp)
	call	_FreeList
L35:
	movl	100(%esp), %ebp
	movl	$24, 4(%esp)
	movl	%ebp, (%esp)
	call	_ListDir
	movl	%eax, %edi
	testl	%eax, %eax
	je	L42
	movl	(%eax), %ebp
	testl	%ebp, %ebp
	je	L43
	movl	%ebp, %ebx
	movl	%eax, 40(%esp)
	jmp	L45
	.p2align 4,,7
L44:
	movl	4(%ebx), %ebx
	cmpl	%ebx, %ebp
	je	L61
L45:
	movl	8(%ebx), %ecx
	testb	$16, 12(%ecx)
	je	L44
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
	jne	L45
L61:
	movl	40(%esp), %edi
L43:
	movl	%edi, (%esp)
	call	_FreeList
L42:
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
L38:
	.cfi_restore_state
	movl	8(%ebx), %ebp
	movl	0(%ebp), %eax
	movl	%eax, (%esp)
	call	_TextDup
	movl	%eax, %edi
	jmp	L39
	.p2align 4,,7
L46:
	xorl	%esi, %esi
	jmp	L35
	.p2align 4,,7
L47:
	xorl	%esi, %esi
	jmp	L36
	.cfi_endproc
LFE196:
LC6:
	.ascii "12:00\0"
	.p2align 4,,15
	.globl	_TraybarAdd
_TraybarAdd:
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
	subl	$60, %esp
	.cfi_def_cfa_offset 80
	movl	_GSTraybar, %eax
	testl	%eax, %eax
	je	L68
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
L64:
	movl	$32, (%esp)
	call	_malloc
	movl	%eax, %ebp
	testl	%eax, %eax
	je	L66
	movl	80(%esp), %ecx
	testl	%ecx, %ecx
	je	L66
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
L66:
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
L68:
	.cfi_restore_state
	movl	_GSPanel, %edi
	movl	_default_font, %eax
	movl	32(%edi), %ebp
	movl	28(%edi), %edi
	movl	$LC6, 4(%esp)
	movl	%eax, (%esp)
	call	_text_length
	movl	_GSPanel, %edx
	movl	_default_font, %ecx
	movl	28(%edx), %ebx
	movl	%ecx, (%esp)
	movl	$LC6, 4(%esp)
	movl	%eax, %esi
	call	_text_length
	subl	$7, %edi
	subl	$7, %ebx
	subl	%eax, %ebx
	decl	%ebp
	subl	%esi, %edi
	movl	%ebp, 16(%esp)
	subl	$10, %edi
	subl	$32, %ebx
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
	jmp	L64
	.cfi_endproc
LFE192:
	.p2align 4,,15
	.globl	_TraybarDraw
_TraybarDraw:
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
	je	L69
	movl	20(%ebp), %edi
	addl	$3, %edi
	.p2align 4,,7
L71:
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
	jne	L71
L69:
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
LFE190:
LC7:
	.ascii "pretime\0"
LC8:
	.ascii "%d:%02d\0"
LC9:
	.ascii "posttime\0"
	.p2align 4,,15
	.globl	_RefreshPanelClock
_RefreshPanelClock:
LFB189:
	.cfi_startproc
	subl	$44, %esp
	.cfi_def_cfa_offset 48
	movl	$0, (%esp)
	call	_time
	movl	$LC7, (%esp)
	movl	%eax, 28(%esp)
	call	_DebugMessage
	leal	28(%esp), %eax
	movl	%eax, (%esp)
	call	_localtime
	testl	%eax, %eax
	je	L76
	movl	4(%eax), %edx
	movl	%edx, 12(%esp)
	movl	8(%eax), %ecx
	movl	_GSClock, %eax
	movl	%ecx, 8(%esp)
	movl	$LC8, 4(%esp)
	movl	%eax, (%esp)
	call	_LabelSetText
L76:
	movl	$LC9, (%esp)
	call	_DebugMessage
	addl	$44, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE189:
	.p2align 4,,15
	.globl	_StartButtonDraw
_StartButtonDraw:
LFB188:
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
	movl	64(%esp), %ebx
	movl	4(%edx), %ecx
	movl	28(%esi), %eax
	movl	%ecx, 20(%esp)
	movl	32(%ebx), %edi
	movl	%edi, 16(%esp)
	movl	28(%ebx), %ebp
	movl	%ebp, 12(%esp)
	movl	24(%ebx), %edx
	movl	%edx, 8(%esp)
	movl	20(%ebx), %ecx
	movl	%esi, (%esp)
	movl	%ecx, 4(%esp)
	call	*60(%eax)
	movl	_THMPanelStartDown, %eax
	testl	%eax, %eax
	je	L82
	movl	_THMPanelStartOver, %ebp
	testl	%ebp, %ebp
	je	L82
	movl	_THMPanelStart, %ecx
	testl	%ecx, %ecx
	je	L82
	movl	_UseSkins, %edi
	testl	%edi, %edi
	je	L82
	movl	212(%ebx), %edx
	cmpw	$1, %dx
	je	L106
	cmpw	$2, %dx
	je	L85
	movl	_Mouse, %edx
	testb	$1, 12(%edx)
	je	L86
	movl	72(%esp), %ebp
	movl	(%edx), %edi
	cmpl	0(%ebp), %edi
	jl	L86
	movl	4(%edx), %edx
	cmpl	4(%ebp), %edx
	jl	L86
	cmpl	8(%ebp), %edi
	jg	L86
	cmpl	12(%ebp), %edx
	jg	L86
L85:
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
	call	_blit
	jmp	L81
	.p2align 4,,7
L82:
	movl	212(%ebx), %eax
	cmpw	$1, %ax
	je	L107
L87:
	cmpw	$2, %ax
	movl	_SysColors, %ecx
	je	L88
	movl	12(%ecx), %ebp
	movl	%ebp, 24(%esp)
	movl	8(%ecx), %edx
L105:
	movl	%edx, 20(%esp)
	movl	32(%ebx), %eax
	movl	%eax, 16(%esp)
	movl	28(%ebx), %ecx
	movl	%ecx, 12(%esp)
	movl	24(%ebx), %edi
	movl	%edi, 8(%esp)
	movl	20(%ebx), %ebx
	movl	%esi, (%esp)
	movl	%ebx, 4(%esp)
	call	_Rect3D
L81:
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
L88:
	.cfi_restore_state
	movl	8(%ecx), %edi
	movl	%edi, 24(%esp)
	movl	12(%ecx), %edx
	jmp	L105
	.p2align 4,,7
L86:
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
	call	_blit
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
L107:
	.cfi_restore_state
	movl	_SysColors, %edx
	movl	28(%esi), %eax
	movl	16(%edx), %ebp
	movl	%ebp, 20(%esp)
	movl	32(%ebx), %ecx
	movl	%ecx, 16(%esp)
	movl	28(%ebx), %edi
	movl	%edi, 12(%esp)
	movl	24(%ebx), %edx
	movl	%edx, 8(%esp)
	movl	20(%ebx), %ebp
	movl	%esi, (%esp)
	movl	%ebp, 4(%esp)
	call	*60(%eax)
	movl	212(%ebx), %eax
	jmp	L87
	.p2align 4,,7
L106:
	movl	32(%ebx), %ecx
	movl	%ecx, 28(%esp)
	movl	28(%ebx), %edx
	movl	%edx, 24(%esp)
	movl	24(%ebx), %edi
	movl	%edi, 20(%esp)
	movl	20(%ebx), %ebx
	movl	$0, 12(%esp)
	movl	%ebx, 16(%esp)
	movl	$0, 8(%esp)
	movl	%esi, 4(%esp)
	movl	0(%ebp), %esi
	movl	%esi, (%esp)
	call	_blit
	jmp	L81
	.cfi_endproc
LFE188:
	.p2align 4,,15
	.globl	_WDraw
_WDraw:
LFB176:
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
	je	L108
	movl	_Style, %edx
	cmpl	$1, %edx
	je	L116
	cmpl	$2, %edx
	je	L117
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
L108:
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
L117:
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
	movl	%ebx, 4(%esp)
	movl	%edx, 16(%esp)
	movl	$0, 12(%esp)
	movl	$0, 8(%esp)
	movl	%eax, (%esp)
	call	_stretch_blit
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
L116:
	.cfi_restore_state
	movl	_GSScreenHeight, %ecx
	movl	_GSScreenWidth, %esi
	movl	%ecx, 20(%esp)
	movl	%esi, 16(%esp)
	movl	$0, 12(%esp)
	movl	$0, 8(%esp)
	movl	%ebx, 4(%esp)
	movl	%eax, (%esp)
	call	_draw_sprite_tile
	jmp	L108
	.cfi_endproc
LFE176:
LC10:
	.ascii "xapps/setwall.app\0"
LC11:
	.ascii "Properties\0"
	.p2align 4,,15
	.globl	_WEventHandler
_WEventHandler:
LFB177:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$40, %esp
	.cfi_def_cfa_offset 48
	movl	52(%esp), %ebx
	movl	(%ebx), %eax
	cmpl	$1, %eax
	je	L124
L119:
	cmpl	$4, %eax
	je	L125
L122:
	xorl	%eax, %eax
L121:
	addl	$40, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L125:
	.cfi_restore_state
	cmpl	$-60927, 4(%ebx)
	jne	L122
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
	movl	$1, 20(%esp)
	movl	%edx, 16(%esp)
	movl	%eax, 12(%esp)
	movl	4(%ecx), %ebx
	movl	(%ecx), %eax
	movl	%eax, 4(%esp)
	movl	%ebx, 8(%esp)
	movl	$_Me, (%esp)
	call	_PopUpMenu
	movl	$1, %eax
	jmp	L121
	.p2align 4,,7
L124:
	cmpl	$-267452408, 4(%ebx)
	jne	L122
	movl	$0, 4(%esp)
	movl	$LC10, (%esp)
	call	_FileRun
	movl	(%ebx), %eax
	jmp	L119
	.cfi_endproc
LFE177:
	.p2align 4,,15
	.globl	_TraybarRemove
_TraybarRemove:
LFB193:
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
	je	L131
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
L131:
	.cfi_restore_state
	movl	%ecx, (%esp)
	call	_WidgetDispose
	movl	$0, _GSTraybar
	addl	$44, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE193:
	.p2align 4,,15
	.globl	_TaskbarEventHandler
_TaskbarEventHandler:
LFB185:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	xorl	%eax, %eax
	subl	$40, %esp
	.cfi_def_cfa_offset 48
	movl	52(%esp), %edx
	cmpl	$4, (%edx)
	je	L145
L133:
	addl	$40, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L145:
	.cfi_restore_state
	cmpl	$-60924, 4(%edx)
	jne	L133
	movl	_WindowList, %eax
	movl	(%eax), %ebx
	testl	%ebx, %ebx
	jne	L141
	jmp	L147
	.p2align 4,,7
L134:
	movl	_WindowList, %edx
	movl	4(%ebx), %ebx
	cmpl	(%edx), %ebx
	je	L146
L141:
	movl	24(%ebx), %ecx
	movl	%ecx, 8(%esp)
	movl	28(%ebx), %edx
	movl	%edx, 12(%esp)
	movl	32(%ebx), %eax
	movl	%eax, 16(%esp)
	movl	_Mouse, %eax
	movl	36(%ebx), %ecx
	movl	%ecx, 20(%esp)
	movl	4(%eax), %edx
	movl	(%eax), %ecx
	movl	%ecx, (%esp)
	movl	%edx, 4(%esp)
	call	_PointInRect
	testl	%eax, %eax
	je	L134
	movl	8(%ebx), %eax
	movl	%eax, (%esp)
	call	_WidgetSetFirst
	movl	_WindowList, %edx
	movl	4(%ebx), %ebx
	cmpl	(%edx), %ebx
	jne	L141
L146:
	movl	48(%esp), %ecx
	movl	$0, 4(%esp)
	movl	%ecx, (%esp)
	call	_WidgetDraw
	addl	$40, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
L147:
	.cfi_restore_state
	movl	$1, %eax
	jmp	L133
	.cfi_endproc
LFE185:
	.p2align 4,,15
	.globl	_DesktopWidgetSetFirstEx
_DesktopWidgetSetFirstEx:
LFB198:
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
	je	L148
	movl	204(%ebx), %eax
	testb	$2, %al
	je	L150
	andl	$-3, %eax
	movl	%eax, 204(%ebx)
L150:
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
L148:
	.cfi_restore_state
	addl	$24, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE198:
	.p2align 4,,15
	.globl	_DesktopWidgetRemove
_DesktopWidgetRemove:
LFB199:
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
	jne	L163
L155:
	addl	$24, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L163:
	.cfi_restore_state
	movl	_WindowList, %edx
	movl	%ebx, 4(%esp)
	movl	%edx, (%esp)
	call	_ListFoundItem
	testl	%eax, %eax
	je	L155
	movl	_WindowList, %ecx
	movl	%eax, 4(%esp)
	movl	%ecx, (%esp)
	call	_ListRemoveItem
	movl	_GSTaskbar, %ebx
	movl	%ebx, 32(%esp)
	movl	$0, 36(%esp)
	addl	$24, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	jmp	_WidgetDraw
	.cfi_endproc
LFE199:
	.p2align 4,,15
	.globl	_DesktopWidgetInsert
_DesktopWidgetInsert:
LFB197:
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
	jne	L171
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
L171:
	.cfi_restore_state
	movl	$40, (%esp)
	call	_malloc
	movl	%eax, %edx
	testl	%eax, %eax
	je	L166
	movl	$10, %ecx
	xorl	%eax, %eax
	movl	%edx, %edi
	rep stosl
L166:
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
LFE197:
LC12:
	.ascii "\0"
LC13:
	.ascii "/USER/DESKTOP/wallpaper\0"
LC14:
	.ascii "/USER/DESKTOP/alignment\0"
LC15:
	.ascii "3D61AD\0"
LC16:
	.ascii "/USER/DESKTOP/color\0"
	.p2align 4,,15
	.globl	_WallpaperReLoad
_WallpaperReLoad:
LFB179:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$24, %esp
	.cfi_def_cfa_offset 32
	movl	_WllBmp, %eax
	testl	%eax, %eax
	je	L173
	movl	%eax, (%esp)
	call	_destroy_bitmap
L173:
	movl	$LC12, 4(%esp)
	movl	$LC13, (%esp)
	call	_KeyGetText
	movl	$0, 4(%esp)
	movl	$LC14, (%esp)
	movl	%eax, %ebx
	call	_KeyGetInt
	movl	%ebx, (%esp)
	movl	%eax, _Style
	call	_TextLen
	testl	%eax, %eax
	jne	L182
	movl	$0, _WllBmp
L175:
	movl	$LC15, 4(%esp)
	movl	$LC16, (%esp)
	movl	_DeskTop, %ebx
	call	_KeyGetText
	movl	%eax, (%esp)
	call	_GetColorFromHex
	movl	_WllBmp, %edx
	movl	%eax, 100(%ebx)
	movl	_DeskTop, %ecx
	testl	%edx, %edx
	je	L176
	movl	$_WDraw, 128(%ecx)
L177:
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
L182:
	.cfi_restore_state
	movl	%ebx, (%esp)
	call	_LoadImage
	movl	%eax, _WllBmp
	jmp	L175
	.p2align 4,,7
L176:
	movl	$0, 128(%ecx)
	jmp	L177
	.cfi_endproc
LFE179:
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
	je	L186
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
L186:
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
	.globl	_PanelDraw
_PanelDraw:
LFB186:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$52, %esp
	.cfi_def_cfa_offset 64
	movl	_SysColors, %edx
	movl	68(%esp), %esi
	movl	64(%esp), %ebx
	movl	4(%edx), %ecx
	movl	28(%esi), %eax
	movl	%ecx, 20(%esp)
	movl	32(%e