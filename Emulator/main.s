	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 12, 0	sdk_version 13, 1
	.intel_syntax noprefix
	.globl	_main_loop                      ## -- Begin function main_loop
	.p2align	4, 0x90
_main_loop:                             ## @main_loop
	.cfi_startproc
## %bb.0:
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset rbp, -16
	mov	rbp, rsp
	.cfi_def_cfa_register rbp
	xor	edx, edx
	mov	edi, edx
	mov	esi, edx
	call	_ngColor
	call	_ngClear
	mov	edx, 255
	mov	edi, edx
	mov	esi, edx
	call	_ngColor
	mov	edi, 202
	mov	esi, 100
	call	_ngDrawPixel
	xor	esi, esi
	mov	ecx, 100
	mov	edi, esi
	mov	edx, ecx
	call	_ngDrawRectangle
	pop	rbp
	ret
	.cfi_endproc
                                        ## -- End function
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset rbp, -16
	mov	rbp, rsp
	.cfi_def_cfa_register rbp
	sub	rsp, 16
	mov	dword ptr [rbp - 4], 0
	mov	edi, dword ptr [rip + _WIDTH]
	mov	esi, dword ptr [rip + _HEIGHT]
	lea	rdx, [rip + L_.str]
	call	_ngCreateWindow
	call	_ngLoadOpenGL
	call	_ngSetupScreen
	xor	edi, edi
	mov	esi, 1
	call	_ngFlipScreen
	lea	rdi, [rip + _main_loop]
	call	_ngSetMainLoopFunction
	call	_ngMainLoop
	call	_ngTerminate
	xor	eax, eax
	add	rsp, 16
	pop	rbp
	ret
	.cfi_endproc
                                        ## -- End function
	.section	__DATA,__data
	.globl	_WIDTH                          ## @WIDTH
	.p2align	2
_WIDTH:
	.long	800                             ## 0x320

	.globl	_HEIGHT                         ## @HEIGHT
	.p2align	2
_HEIGHT:
	.long	600                             ## 0x258

	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"OPENGL"

.subsections_via_symbols
