### 20190439, Gyubin Lee, Assignment #4
### --------------------------------------------------------------------
### mydc.s
###
### Desk Calculator (dc)
### --------------------------------------------------------------------

	.equ   ARRAYSIZE, 20
	.equ   EOF, -1
	.equ   MINIMUM_ITEM, 2
	
.section ".rodata"

scanfFormat:
	.asciz "%s"

printfFormat:
	.asciz "%d\n"

errorFormat:
	.asciz "dc: stack empty\n"
### --------------------------------------------------------------------

        .section ".data"

stackSize:
	.long 0

### --------------------------------------------------------------------

        .section ".bss"
buffer:
        .skip  ARRAYSIZE

### --------------------------------------------------------------------

	.section ".text"

	## -------------------------------------------------------------
	## int main(void)
	## Runs desk calculator program.  Returns 0.
	## -------------------------------------------------------------

	.globl  main
	.type   main,@function

main:

	pushl   %ebp
	movl    %esp, %ebp

input:

	## dc number stack initialized. %esp = %ebp
	
	## scanf("%s", buffer)
	pushl	$buffer
	pushl	$scanfFormat
	call    scanf
	addl    $8, %esp

	## check if user input EOF
	cmp	$EOF, %eax
	je	quit
	
	##check whether the input is digit
	xor %ecx, %ecx
	movl $buffer, %ebx
	movb (%ebx), %cl
	
	pushl %ecx
	call isdigit
	add $4, %esp
	
	cmpl $0, %eax ##if input is not digit, eax is 0x0
	je operator

pushvalue:
	## push the value if input is unsigned digit
	pushl $buffer
	call atoi
	add $4, %esp

	pushl %eax

	jmp incSize
	
operator:
	## all of these operation should jump returns back to input loop
	cmp $'_', %cl
	je pushsignedvalue

	cmp $'p', %cl
	je top

	cmp $'+', %cl
	je sum

	cmp $'-', %cl
	je subs

	cmp $'*', %cl
	je multi

	cmp $'/', %cl
	je divi

	cmp $'%', %cl
	je modul

	cmp $'^', %cl
	je power

	cmp $'q', %cl
	je quit

	cmp $'f', %cl
	je print

	cmp $'c', %cl
	je clear

	cmp $'d', %cl
	je dupl

	cmp $'r', %cl
	je rev

	jmp input

pushsignedvalue:
	## push signed int value
	movl %ebx, %edi
	inc %edi

	pushl %edi
	call atoi
	add $4, %esp

	neg %eax
	pushl %eax

	jmp incSize 

top:
	## print top of the stack
	cmpl $0, (stackSize)
	je stackEmptyError

	movl (%esp), %ecx
	pushl %ecx
	pushl $printfFormat
	call printf
	addl $8, %esp
	jmp input

sum:
	cmpl $MINIMUM_ITEM, (stackSize)
	jb stackEmptyError

	popl %edi
	popl %esi
	addl %edi, %esi
	pushl %esi
	jmp decSize

subs:
	cmpl $MINIMUM_ITEM, (stackSize)
	jb stackEmptyError

	popl %edi
	popl %esi
	subl %edi, %esi
	pushl %esi
	jmp decSize

multi:
	cmpl $MINIMUM_ITEM, (stackSize)
	jb stackEmptyError

	popl %eax
	popl %esi
	imul %esi
	pushl %eax
	jmp decSize

divi:
	cmpl $MINIMUM_ITEM, (stackSize)
	jb stackEmptyError

	popl %esi
	popl %eax
	cdq
	idivl %esi
	pushl %eax
	jmp decSize

modul:
	cmpl $MINIMUM_ITEM, (stackSize)
	jb stackEmptyError

	popl %esi
	popl %eax
	cdq
	idivl %esi
	pushl %edx
	jmp decSize

power:
	cmpl $MINIMUM_ITEM, (stackSize)
	jb stackEmptyError

	call powerFunction
	addl $8, %esp
	pushl %eax
	jmp decSize

print:
	cmpl $0, (stackSize)
	je input
		
	xor %esi, %esi
	print_loop:
		cmpl %esi, (stackSize)
		je input
		movl (%esp, %esi, 4), %ecx
		pushl %ecx
		pushl $printfFormat
		call printf
		addl $8, %esp
		addl $1, %esi
		jmp print_loop

clear:
	cmpl $0, (stackSize)
	je input

	movl (stackSize), %eax
	movl $4, %esi
	mul %esi
	addl %eax, %esp
	movl $0, (stackSize)
	jmp input

dupl:
	cmpl $0, (stackSize)
	je stackEmptyError

	xor %ecx, %ecx
	movl (%esp), %ecx
	pushl %ecx
	jmp incSize

rev:
	movl (stackSize), %ebx
	cmpl $MINIMUM_ITEM, %ebx
	jb stackEmptyError

	popl %esi
	popl %edi
	pushl %esi
	pushl %edi
	jmp input

incSize:
	addl $1, (stackSize)
	jmp input

decSize:
	subl $1, (stackSize)
	jmp input

stackEmptyError:
	pushl $errorFormat
	call printf
	addl $4, %esp
	jmp input

powerFunction:
	pushl %ebp
	movl %esp, %ebp
	
	movl 8(%esp), %edi ##exponent
	movl 12(%esp), %esi ##base

	xor %ecx, %ecx
	movl $1, %eax
	power_loop:
		cmpl %ecx, %edi
		je power_end
		imul %esi
		addl $1, %ecx
		jmp power_loop
	power_end:
		movl %ebp, %esp
		popl %ebp
		ret

quit:	
	## return 0
	movl    $0, %eax
	movl    %ebp, %esp
	popl    %ebp
	ret
