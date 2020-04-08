	.file	"wc209.c"
	.globl	line
	.data
	.align 4
	.type	line, @object
	.size	line, 4
line:
	.long	1
	.globl	word
	.bss
	.align 4
	.type	word, @object
	.size	word, 4
word:
	.zero	4
	.globl	charac
	.align 4
	.type	charac, @object
	.size	charac, 4
charac:
	.zero	4
	.globl	temp
	.data
	.align 4
	.type	temp, @object
	.size	temp, 4
temp:
	.long	1
	.text
	.globl	newline
	.type	newline, @function
newline:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	line(%rip), %eax
	addl	$1, %eax
	movl	%eax, line(%rip)
	movl	charac(%rip), %eax
	addl	$1, %eax
	movl	%eax, charac(%rip)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	newline, .-newline
	.globl	space
	.type	space, @function
space:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8, %rsp
	movl	%edi, -4(%rbp)
	cmpl	$10, -4(%rbp)
	jne	.L3
	call	newline
	jmp	.L5
.L3:
	movl	charac(%rip), %eax
	addl	$1, %eax
	movl	%eax, charac(%rip)
.L5:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	space, .-space
	.section	.rodata
.LC0:
	.string	"wc209.c"
.LC1:
	.string	"0"
	.align 8
.LC2:
	.string	"Error: line %d: unterminated comment\n"
.LC3:
	.string	"%d %d %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, -8(%rbp)
	jmp	.L7
.L34:
	cmpl	$6, -8(%rbp)
	ja	.L8
	movl	-8(%rbp), %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L10(%rip), %rax
	movl	(%rdx,%rax), %eax
	movslq	%eax, %rdx
	leaq	.L10(%rip), %rax
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L10:
	.long	.L9-.L10
	.long	.L11-.L10
	.long	.L12-.L10
	.long	.L13-.L10
	.long	.L14-.L10
	.long	.L15-.L10
	.long	.L16-.L10
	.text
.L9:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L17
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	space
	jmp	.L7
.L17:
	cmpl	$47, -4(%rbp)
	jne	.L18
	movl	word(%rip), %eax
	addl	$1, %eax
	movl	%eax, word(%rip)
	movl	charac(%rip), %eax
	addl	$1, %eax
	movl	%eax, charac(%rip)
	movl	$3, -8(%rbp)
	jmp	.L7
.L18:
	movl	word(%rip), %eax
	addl	$1, %eax
	movl	%eax, word(%rip)
	movl	charac(%rip), %eax
	addl	$1, %eax
	movl	%eax, charac(%rip)
	movl	$1, -8(%rbp)
	jmp	.L7
.L11:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L19
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	space
	movl	$0, -8(%rbp)
	jmp	.L7
.L19:
	cmpl	$47, -4(%rbp)
	jne	.L20
	movl	charac(%rip), %eax
	addl	$1, %eax
	movl	%eax, charac(%rip)
	movl	$2, -8(%rbp)
	jmp	.L7
.L20:
	movl	charac(%rip), %eax
	addl	$1, %eax
	movl	%eax, charac(%rip)
	jmp	.L7
.L12:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L21
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	space
	movl	$0, -8(%rbp)
	jmp	.L7
.L21:
	cmpl	$42, -4(%rbp)
	jne	.L22
	movl	charac(%rip), %eax
	subl	$1, %eax
	movl	%eax, charac(%rip)
	movl	line(%rip), %eax
	movl	%eax, temp(%rip)
	movl	$4, -8(%rbp)
	jmp	.L7
.L22:
	cmpl	$47, -4(%rbp)
	jne	.L23
	movl	charac(%rip), %eax
	addl	$1, %eax
	movl	%eax, charac(%rip)
	jmp	.L7
.L23:
	movl	charac(%rip), %eax
	addl	$1, %eax
	movl	%eax, charac(%rip)
	movl	$1, -8(%rbp)
	jmp	.L7
.L14:
	cmpl	$42, -4(%rbp)
	jne	.L24
	movl	$5, -8(%rbp)
	jmp	.L7
.L24:
	cmpl	$10, -4(%rbp)
	jne	.L7
	call	newline
	jmp	.L7
.L15:
	cmpl	$47, -4(%rbp)
	jne	.L26
	movl	charac(%rip), %eax
	addl	$1, %eax
	movl	%eax, charac(%rip)
	movl	$6, -8(%rbp)
	jmp	.L7
.L26:
	cmpl	$42, -4(%rbp)
	jne	.L27
	jmp	.L7
.L27:
	cmpl	$10, -4(%rbp)
	jne	.L28
	call	newline
.L28:
	movl	$4, -8(%rbp)
	jmp	.L7
.L16:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L29
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	space
	movl	$0, -8(%rbp)
	jmp	.L7
.L29:
	cmpl	$47, -4(%rbp)
	jne	.L30
	movl	charac(%rip), %eax
	addl	$1, %eax
	movl	%eax, charac(%rip)
	movl	word(%rip), %eax
	addl	$1, %eax
	movl	%eax, word(%rip)
	movl	$3, -8(%rbp)
	jmp	.L7
.L30:
	movl	word(%rip), %eax
	addl	$1, %eax
	movl	%eax, word(%rip)
	movl	charac(%rip), %eax
	addl	$1, %eax
	movl	%eax, charac(%rip)
	movl	$1, -8(%rbp)
	jmp	.L7
.L13:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L31
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	space
	movl	$0, -8(%rbp)
	jmp	.L7
.L31:
	cmpl	$42, -4(%rbp)
	jne	.L32
	movl	charac(%rip), %eax
	subl	$1, %eax
	movl	%eax, charac(%rip)
	movl	word(%rip), %eax
	subl	$1, %eax
	movl	%eax, word(%rip)
	movl	line(%rip), %eax
	movl	%eax, temp(%rip)
	movl	$4, -8(%rbp)
	jmp	.L7
.L32:
	cmpl	$47, -4(%rbp)
	jne	.L33
	movl	charac(%rip), %eax
	addl	$1, %eax
	movl	%eax, charac(%rip)
	jmp	.L7
.L33:
	movl	charac(%rip), %eax
	addl	$1, %eax
	movl	%eax, charac(%rip)
	movl	$1, -8(%rbp)
	jmp	.L7
.L8:
	leaq	__PRETTY_FUNCTION__.2263(%rip), %rcx
	movl	$121, %edx
	leaq	.LC0(%rip), %rsi
	leaq	.LC1(%rip), %rdi
	call	__assert_fail@PLT
.L7:
	call	getchar@PLT
	movl	%eax, -4(%rbp)
	cmpl	$-1, -4(%rbp)
	jne	.L34
	movl	charac(%rip), %eax
	testl	%eax, %eax
	jne	.L35
	movl	$0, line(%rip)
.L35:
	cmpl	$4, -8(%rbp)
	je	.L36
	cmpl	$5, -8(%rbp)
	jne	.L37
.L36:
	movl	temp(%rip), %edx
	movq	stderr(%rip), %rax
	leaq	.LC2(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %eax
	jmp	.L38
.L37:
	movl	charac(%rip), %ecx
	movl	word(%rip), %edx
	movl	line(%rip), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
.L38:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.section	.rodata
	.type	__PRETTY_FUNCTION__.2263, @object
	.size	__PRETTY_FUNCTION__.2263, 5
__PRETTY_FUNCTION__.2263:
	.string	"main"
	.ident	"GCC: (Ubuntu 5.5.0-1ubuntu2) 5.4.1 20171010"
	.section	.note.GNU-stack,"",@progbits
