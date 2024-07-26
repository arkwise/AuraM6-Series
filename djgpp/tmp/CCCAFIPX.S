	.file	"setup.c"
	.section .text
	.p2align 4,,15
	.globl	_Draw1
_Draw1:
LFB173:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$20, %esp
	.cfi_def_cfa_offset 32
	movl	_Bmp1, %ecx
	movl	32(%esp), %edx
	movl	36(%esp), %esi
	movl	32(%edx), %ebx
	movl	24(%edx), %eax
	addl	%ebx, %eax
	movl	4(%ecx), %ebx
	subl	%ebx, %eax
	movl	%eax, %ebx
	shrl	$31, %ebx
	addl	%eax, %ebx
	movl	20(%edx), %eax
	sarl	%ebx
	addl	28(%edx), %eax
	movl	(%ecx), %edx
	subl	%edx, %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	movl	28(%ecx), %edx
	sarl	%eax
	cmpl	$8, (%edx)
	movl	28(%esi), %edx
	movl	%ebx, 12(%esp)
	movl	%eax, 8(%esp)
	movl	%ecx, 4(%esp)
	movl	%esi, (%esp)
	je	L6
	call	*68(%edx)
	addl	$20, %esp
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
L6:
	.cfi_restore_state
	call	*72(%edx)
	addl	$20, %esp
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE173:
	.p2align 4,,15
	.globl	_SysAppEventHandler
_SysAppEventHandler:
LFB174:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	xorl	%eax, %eax
	movl	36(%esp), %edx
	cmpl	$1, (%edx)
	je	L13
L8:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L13:
	.cfi_restore_state
	cmpl	$17000, 4(%edx)
	jne	L8
	movl	_w, %eax
	movl	%eax, (%esp)
	call	_WidgetDispose
	movl	$1, %eax
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE174:
LC0:
	.ascii "Aura Setup cunt\0"
	.p2align 2
LC1:
	.ascii "Where do you want Aura Installed?\0"
LC2:
	.ascii "/system/System/install/\0"
	.p2align 2
LC3:
	.ascii "Drag Aura to the desired diretory\0"
LC4:
	.ascii "Done\0"
	.p2align 4,,15
	.globl	_CollectingData
_CollectingData:
LFB175:
	.cfi_startproc
	subl	$44, %esp
	.cfi_def_cfa_offset 48
	movl	$255, 16(%esp)
	movl	$400, 12(%esp)
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %eax
	movl	_r+8, %ecx
	movl	%eax, 4(%esp)
	movl	_r+4, %edx
	movl	_r+12, %eax
	movl	%ecx, 12(%esp)
	movl	%edx, 8(%esp)
	movl	$523, 24(%esp)
	movl	$LC0, 20(%esp)
	movl	%eax, 16(%esp)
	movl	$_Me, (%esp)
	call	_CreateWindow
	movl	_DeskTop, %edx
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	movl	%eax, _w
	call	_WidgetInsert
	movl	_w, %ecx
	movl	$_SysAppEventHandler, 120(%ecx)
	movl	$41, 16(%esp)
	movl	$400, 12(%esp)
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %eax
	movl	_r+8, %ecx
	movl	%eax, 4(%esp)
	movl	_r+4, %edx
	movl	_r+12, %eax
	movl	%ecx, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%eax, 16(%esp)
	movl	$_Me, (%esp)
	call	_CreateCanvas
	movl	_w, %edx
	movl	%eax, _c
	orl	$2, 104(%eax)
	movl	$_Draw1, 128(%eax)
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_WidgetInsert
	movl	$60, 16(%esp)
	movl	$250, 12(%esp)
	movl	$45, 8(%esp)
	movl	$5, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %ecx
	movl	%ecx, 4(%esp)
	movl	_r+8, %edx
	movl	_r+12, %ecx
	movl	_r+4, %eax
	movl	%edx, 12(%esp)
	movl	%ecx, 16(%esp)
	movl	$LC1, 20(%esp)
	movl	%eax, 8(%esp)
	movl	$_Me, (%esp)
	call	_CreateLabel
	movl	_SysColors, %edx
	movl	4(%edx), %ecx
	movl	%eax, _j
	movl	%ecx, 100(%eax)
	movl	%eax, 4(%esp)
	movl	_w, %eax
	movl	%eax, (%esp)
	call	_WidgetInsert
	movl	$190, 16(%esp)
	movl	$200, 12(%esp)
	movl	$65, 8(%esp)
	movl	$5, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %edx
	movl	%edx, 4(%esp)
	movl	_r+4, %ecx
	movl	_r+12, %edx
	movl	_r+8, %eax
	movl	%edx, 16(%esp)
	movl	%ecx, 8(%esp)
	movl	$3, 28(%esp)
	movl	$0, 24(%esp)
	movl	$LC2, 20(%esp)
	movl	%eax, 12(%esp)
	movl	$_Me, (%esp)
	call	_CreateDirview
	movl	_w, %ecx
	movl	%eax, _s
	orl	$26214400, 104(%eax)
	movb	$0, 280(%eax)
	movl	%eax, 4(%esp)
	movl	%ecx, (%esp)
	call	_WidgetInsert
	movl	$190, 16(%esp)
	movl	$204, 12(%esp)
	movl	$65, 8(%esp)
	movl	$201, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %eax
	movl	_r+8, %ecx
	movl	%eax, 4(%esp)
	movl	_r+4, %edx
	movl	_r+12, %eax
	movl	%ecx, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%eax, 16(%esp)
	movl	$_Me, (%esp)
	call	_CreateVSpliter
	movl	_w, %edx
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	movl	%eax, _spl
	call	_WidgetInsert
	movl	$190, 16(%esp)
	movl	$395, 12(%esp)
	movl	$65, 8(%esp)
	movl	$205, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %ecx
	movl	_r+8, %edx
	movl	%ecx, 4(%esp)
	movl	_r+4, %eax
	movl	_r+12, %ecx
	movl	%edx, 12(%esp)
	movl	%ecx, 16(%esp)
	movl	%eax, 8(%esp)
	movl	$_Me, (%esp)
	call	_CreateTreeDirview
	movl	%eax, _t
	orl	$26214400, 104(%eax)
	movl	%eax, 4(%esp)
	movl	_w, %eax
	movl	%eax, (%esp)
	call	_WidgetInsert
	movl	$225, 16(%esp)
	movl	$195, 12(%esp)
	movl	$195, 8(%esp)
	movl	$5, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %edx
	movl	%edx, 4(%esp)
	movl	_r+4, %ecx
	movl	_r+12, %edx
	movl	_r+8, %eax
	movl	%ecx, 8(%esp)
	movl	%edx, 16(%esp)
	movl	$LC3, 20(%esp)
	movl	%eax, 12(%esp)
	movl	$_Me, (%esp)
	call	_CreateLabel
	movl	_SysColors, %ecx
	movl	4(%ecx), %edx
	movl	%eax, _j
	movl	%edx, 100(%eax)
	movl	%eax, 4(%esp)
	movl	_w, %eax
	movl	%eax, (%esp)
	call	_WidgetInsert
	movl	$250, 16(%esp)
	movl	$400, 12(%esp)
	movl	$230, 8(%esp)
	movl	$350, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %ecx
	movl	%ecx, 4(%esp)
	movl	_r+4, %edx
	movl	_r+12, %ecx
	movl	_r+8, %eax
	movl	%ecx, 16(%esp)
	movl	%edx, 8(%esp)
	movl	$17000, 24(%esp)
	movl	$LC4, 20(%esp)
	movl	%eax, 12(%esp)
	movl	$_Me, (%esp)
	call	_CreateButton
	movl	_w, %edx
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	movl	%eax, _b
	call	_WidgetInsert
	movl	_w, %eax
	movl	%eax, (%esp)
	call	_WidgetDrawAll
	addl	$44, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE175:
	.p2align 2
LC5:
	.ascii "Setup is now complete, You must now reboot.\0"
LC6:
	.ascii "Its That Easy!\0"
LC7:
	.ascii "/v/system/websurf/fdisk.bat\0"
LC8:
	.ascii "xapps/rundos.app\0"
LC9:
	.ascii "/v/system/websurf/install.bat\0"
LC10:
	.ascii "/v/system/websurf/update.bat\0"
	.p2align 4,,15
	.globl	_AppEventHandler
_AppEventHandler:
LFB176:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	xorl	%eax, %eax
	subl	$120, %esp
	.cfi_def_cfa_offset 128
	movl	132(%esp), %edx
	cmpl	$1, (%edx)
	je	L29
L17:
	addl	$120, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L29:
	.cfi_restore_state
	movl	4(%edx), %eax
	cmpl	$16997, %eax
	je	L20
	jbe	L30
	cmpl	$16999, %eax
	je	L22
	jb	L21
	cmpl	$-48895, %eax
	jne	L26
	movl	128(%esp), %ecx
	movl	%ecx, (%esp)
	call	_WidgetDispose
	movl	$_Me, (%esp)
	call	_CloseApp
	movl	$1, %eax
	jmp	L17
	.p2align 4,,7
L30:
	cmpl	$16995, %eax
	je	L18
	cmpl	$16996, %eax
	je	L31
L26:
	xorl	%eax, %eax
	jmp	L17
	.p2align 4,,7
L21:
	movl	$LC10, 4(%esp)
	movl	$LC8, (%esp)
	call	_FileRun
	movl	$1, %eax
	jmp	L17
	.p2align 4,,7
L31:
	movl	$LC9, 4(%esp)
	movl	$LC8, (%esp)
	call	_FileRun
	movl	$1, %eax
	jmp	L17
	.p2align 4,,7
L18:
	movl	$LC7, 4(%esp)
	movl	$LC8, (%esp)
	call	_FileRun
	movl	$1, %eax
	jmp	L17
	.p2align 4,,7
L20:
	call	_CollectingData
	movl	$1, %eax
	jmp	L17
	.p2align 4,,7
L22:
	movl	$256, 12(%esp)
	movl	$LC5, 8(%esp)
	movl	$LC6, 4(%esp)
	movl	$_Me, (%esp)
	call	_MessageBox
	movl	%eax, %ebx
	movl	$1, %eax
	cmpl	$-32767, %ebx
	jne	L17
	movl	_Mouse, %edx
	movl	(%edx), %ecx
	movl	%ecx, 24(%esp)
	movl	4(%edx), %ebx
	movl	%ebx, 28(%esp)
	movl	_KState, %ebx
	movl	8(%edx), %ecx
	movl	%ecx, 32(%esp)
	movl	_KState+4, %ecx
	movl	12(%edx), %edx
	movl	%ebx, 16(%esp)
	leal	76(%esp), %ebx
	movl	%eax, 60(%esp)
	movl	%edx, 36(%esp)
	movl	%ecx, 20(%esp)
	movl	$0, 12(%esp)
	movl	$1, 8(%esp)
	movl	$1, 4(%esp)
	movl	%ebx, (%esp)
	call	_EventAssign
	movl	%ebx, (%esp)
	call	_HandleEvent
	movl	60(%esp), %eax
	jmp	L17
	.cfi_endproc
LFE176:
LC11:
	.ascii "1\0"
LC12:
	.ascii "SETUP\0"
LC13:
	.ascii "Verified and initiated\0"
LC14:
	.ascii "YAYY\0"
LC15:
	.ascii "This didnt work\0"
LC16:
	.ascii "BOO\0"
LC17:
	.ascii "Aura Setup\0"
LC18:
	.ascii "fdisk\0"
	.p2align 2
LC19:
	.ascii "Prepare your partition for installation with fdisk\0"
LC20:
	.ascii "freeDOS\0"
	.p2align 2
LC21:
	.ascii "Install freedos 1.1 with doscore networking and filesharing settings after optional format\0"
LC22:
	.ascii "Aura m3\0"
LC23:
	.ascii "Install the Aura m3 System\0"
LC24:
	.ascii "Update\0"
LC25:
	.ascii "Check for and install updates\0"
LC26:
	.ascii "All Done\0"
	.p2align 4,,15
	.globl	_Main
_Main:
LFB177:
	.cfi_startproc
	subl	$44, %esp
	.cfi_def_cfa_offset 48
	movl	_Me+24, %eax
	movl	$LC11, 4(%esp)
	movl	%eax, (%esp)
	call	_ListKey
	movl	_Me+24, %edx
	movl	$LC12, 4(%esp)
	testl	%eax, %eax
	movl	%edx, (%esp)
	je	L33
	call	_ListKey
	movl	$256, 12(%esp)
	movl	$LC13, 8(%esp)
	movl	$LC14, 4(%esp)
	movl	$_Me, (%esp)
	movl	%eax, _Bmp1
	call	_MessageBox
L34:
	movl	$255, 16(%esp)
	movl	$400, 12(%esp)
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %ecx
	movl	_r+8, %edx
	movl	%ecx, 4(%esp)
	movl	_r+4, %eax
	movl	_r+12, %ecx
	movl	%ecx, 16(%esp)
	movl	%edx, 12(%esp)
	movl	$523, 24(%esp)
	movl	$LC17, 20(%esp)
	movl	%eax, 8(%esp)
	movl	$_Me, (%esp)
	call	_CreateWindow
	movl	%eax, 4(%esp)
	movl	%eax, _w
	movl	_DeskTop, %eax
	movl	%eax, (%esp)
	call	_WidgetInsert
	movl	_w, %edx
	movl	$_AppEventHandler, 120(%edx)
	movl	$41, 16(%esp)
	movl	$400, 12(%esp)
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %ecx
	movl	_r+8, %edx
	movl	%ecx, 4(%esp)
	movl	_r+4, %eax
	movl	_r+12, %ecx
	movl	%edx, 12(%esp)
	movl	%ecx, 16(%esp)
	movl	%eax, 8(%esp)
	movl	$_Me, (%esp)
	call	_CreateCanvas
	movl	%eax, _c
	orl	$2, 104(%eax)
	movl	$_Draw1, 128(%eax)
	movl	%eax, 4(%esp)
	movl	_w, %eax
	movl	%eax, (%esp)
	call	_WidgetInsert
	movl	$75, 16(%esp)
	movl	$55, 12(%esp)
	movl	$55, 8(%esp)
	movl	$5, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %edx
	movl	%edx, 4(%esp)
	movl	_r+4, %ecx
	movl	_r+12, %edx
	movl	_r+8, %eax
	movl	%edx, 16(%esp)
	movl	%ecx, 8(%esp)
	movl	$16995, 24(%esp)
	movl	$LC18, 20(%esp)
	movl	%eax, 12(%esp)
	movl	$_Me, (%esp)
	call	_CreateButton
	movl	_w, %ecx
	movl	%eax, 4(%esp)
	movl	%ecx, (%esp)
	movl	%eax, _b1
	call	_WidgetInsert
	movl	$75, 16(%esp)
	movl	$400, 12(%esp)
	movl	$55, 8(%esp)
	movl	$60, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %eax
	movl	%eax, 4(%esp)
	movl	_r+4, %edx
	movl	_r+8, %ecx
	movl	_r+12, %eax
	movl	%edx, 8(%esp)
	movl	%ecx, 12(%esp)
	movl	$LC19, 20(%esp)
	movl	%eax, 16(%esp)
	movl	$_Me, (%esp)
	call	_CreateLabel
	movl	_SysColors, %edx
	movl	4(%edx), %ecx
	movl	%eax, _j11
	movl	%ecx, 100(%eax)
	movl	%eax, 4(%esp)
	movl	_w, %eax
	movl	%eax, (%esp)
	call	_WidgetInsert
	movl	$110, 16(%esp)
	movl	$55, 12(%esp)
	movl	$90, 8(%esp)
	movl	$5, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %edx
	movl	%edx, 4(%esp)
	movl	_r+4, %ecx
	movl	_r+12, %edx
	movl	_r+8, %eax
	movl	%edx, 16(%esp)
	movl	%ecx, 8(%esp)
	movl	$16996, 24(%esp)
	movl	$LC20, 20(%esp)
	movl	%eax, 12(%esp)
	movl	$_Me, (%esp)
	call	_CreateButton
	movl	_w, %ecx
	movl	%eax, 4(%esp)
	movl	%ecx, (%esp)
	movl	%eax, _b2
	call	_WidgetInsert
	movl	$110, 16(%esp)
	movl	$400, 12(%esp)
	movl	$90, 8(%esp)
	movl	$60, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %eax
	movl	%eax, 4(%esp)
	movl	_r+4, %edx
	movl	_r+8, %ecx
	movl	_r+12, %eax
	movl	%edx, 8(%esp)
	movl	%ecx, 12(%esp)
	movl	$LC21, 20(%esp)
	movl	%eax, 16(%esp)
	movl	$_Me, (%esp)
	call	_CreateLabel
	movl	_SysColors, %edx
	movl	4(%edx), %ecx
	movl	%eax, _j2
	movl	%ecx, 100(%eax)
	movl	%eax, 4(%esp)
	movl	_w, %eax
	movl	%eax, (%esp)
	call	_WidgetInsert
	movl	$145, 16(%esp)
	movl	$55, 12(%esp)
	movl	$125, 8(%esp)
	movl	$5, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %edx
	movl	%edx, 4(%esp)
	movl	_r+4, %ecx
	movl	_r+12, %edx
	movl	_r+8, %eax
	movl	%edx, 16(%esp)
	movl	%ecx, 8(%esp)
	movl	$16997, 24(%esp)
	movl	$LC22, 20(%esp)
	movl	%eax, 12(%esp)
	movl	$_Me, (%esp)
	call	_CreateButton
	movl	_w, %ecx
	movl	%eax, 4(%esp)
	movl	%ecx, (%esp)
	movl	%eax, _b3
	call	_WidgetInsert
	movl	$145, 16(%esp)
	movl	$400, 12(%esp)
	movl	$125, 8(%esp)
	movl	$60, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %eax
	movl	%eax, 4(%esp)
	movl	_r+4, %edx
	movl	_r+8, %ecx
	movl	_r+12, %eax
	movl	%edx, 8(%esp)
	movl	%ecx, 12(%esp)
	movl	$LC23, 20(%esp)
	movl	%eax, 16(%esp)
	movl	$_Me, (%esp)
	call	_CreateLabel
	movl	_SysColors, %edx
	movl	4(%edx), %ecx
	movl	%eax, _j3
	movl	%ecx, 100(%eax)
	movl	%eax, 4(%esp)
	movl	_w, %eax
	movl	%eax, (%esp)
	call	_WidgetInsert
	movl	$180, 16(%esp)
	movl	$55, 12(%esp)
	movl	$160, 8(%esp)
	movl	$5, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %edx
	movl	%edx, 4(%esp)
	movl	_r+4, %ecx
	movl	_r+12, %edx
	movl	_r+8, %eax
	movl	%edx, 16(%esp)
	movl	%ecx, 8(%esp)
	movl	$16998, 24(%esp)
	movl	$LC24, 20(%esp)
	movl	%eax, 12(%esp)
	movl	$_Me, (%esp)
	call	_CreateButton
	movl	_w, %ecx
	movl	%eax, 4(%esp)
	movl	%ecx, (%esp)
	movl	%eax, _b4
	call	_WidgetInsert
	movl	$180, 16(%esp)
	movl	$400, 12(%esp)
	movl	$160, 8(%esp)
	movl	$60, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %eax
	movl	%eax, 4(%esp)
	movl	_r+4, %edx
	movl	_r+8, %ecx
	movl	_r+12, %eax
	movl	%edx, 8(%esp)
	movl	%ecx, 12(%esp)
	movl	$LC25, 20(%esp)
	movl	%eax, 16(%esp)
	movl	$_Me, (%esp)
	call	_CreateLabel
	movl	_SysColors, %edx
	movl	4(%edx), %ecx
	movl	%eax, _j4
	movl	%ecx, 100(%eax)
	movl	%eax, 4(%esp)
	movl	_w, %eax
	movl	%eax, (%esp)
	call	_WidgetInsert
	movl	$250, 16(%esp)
	movl	$400, 12(%esp)
	movl	$230, 8(%esp)
	movl	$350, 4(%esp)
	movl	$_r, (%esp)
	call	_RectAssign
	movl	_r, %edx
	movl	%edx, 4(%esp)
	movl	_r+4, %ecx
	movl	_r+12, %edx
	movl	_r+8, %eax
	movl	%edx, 16(%esp)
	movl	%ecx, 8(%esp)
	movl	$16999, 24(%esp)
	movl	$LC26, 20(%esp)
	movl	%eax, 12(%esp)
	movl	$_Me, (%esp)
	call	_CreateButton
	movl	_w, %ecx
	movl	%eax, 4(%esp)
	movl	%ecx, (%esp)
	movl	%eax, _b
	call	_WidgetInsert
	movl	_w, %eax
	movl	%eax, (%esp)
	call	_WidgetDrawAll
	movl	$1, %eax
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 4
	ret
	.p2align 4,,7
L33:
	.cfi_restore_state
	call	_ListKey
	movl	$256, 12(%esp)
	movl	$LC15, 8(%esp)
	movl	$LC16, 4(%esp)
	movl	$_Me, (%esp)
	movl	%eax, _Bmp1
	call	_MessageBox
	jmp	L34
	.cfi_endproc
LFE177:
	.p2align 4,,15
	.globl	_Close
_Close:
LFB178:
	.cfi_startproc
	ret
	.cfi_endproc
LFE178:
.comm _p,16
.comm _ext,16
	.globl	_t
	.section	.bss
	.p2align 2
_t:
	.space 4
	.globl	_w
	.p2align 2
_w:
	.space 4
.comm _j4,16
.comm _j3,16
.comm _j2,16
.comm _j11,16
.comm _j,16
	.globl	_a
	.p2align 2
_a:
	.space 4
.comm _b5,16
.comm _b4,16
.comm _b3,16
.comm _b2,16
.comm _b1,16
.comm _b,16
.comm _wr,16
	.globl	_Frame
	.p2align 2
_Frame:
	.space 4
	.globl	_c
	.p2align 2
_c:
	.space 4
.comm _Bmp1,16
	.globl	_nUID
	.section .data
_nUID:
	.ascii "app:aossetup\0"
	.globl	_AppName
_AppName:
	.ascii "Aura Setup\0"
	.globl	_AppVersion
	.p2align 2
_AppVersion:
	.long	1
	.globl	_UID
	.p2align 2
_UID:
	.long	1330791240
.comm _spl,16
	.globl	_s
	.section	.bss
	.p2align 2
_s:
	.space 4
.comm _r,16
	.ident	"GCC: (GNU) 4.7.4"
