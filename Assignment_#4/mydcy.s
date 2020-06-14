### --------------------------------------------------------------------
### mydc.s
###
### Desk Calculator (dc)
### --------------------------------------------------------------------

	.equ   ARRAYSIZE, 20
	.equ   EOF, -1
	.equ   IINDEX, 0
	.equ   IPOWER, 4	
.section ".rodata"

emptystr:
	.asciz "dc: stack empty\n"
Print_one:
	.asciz "%d\n"
scanfFormat:
	.asciz "%s"
### --------------------------------------------------------------------

        .section ".data"

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
	
	## check buffer[0] is digit
	movl $buffer, %eax
	movl (%eax), %eax
	pushl %eax
	call isdigit
	addl $4, %esp
	cmpl $0, %eax
	je alphabet

digit:
	## push the new digit
	pushl $buffer
	call atoi
	add $4, %esp
	pushl %eax
	
	jmp input 

change_sign:
	## change sign
	movl $buffer, %ecx
	movl $'-', %ecx
	pushl %ecx
	call atoi
	add $4, %esp
	pushl %eax
	jmp input 
	
alphabet:
	movl $buffer, %ecx
	movb (%ecx), %al
	
	## p appear
	cmpb $'p', %al
	je print_top
	
	## q appear
	cmpb $'q', %al
	je quit
	
	## '_' change sign	
	cmpb $'_', %al
	je change_sign
	
	## plus
	cmpb $'+', %al
	je plus
	
	## minus
	cmpb $'-', %al
	je minus

	## multiplication
	cmpb $'*', %al
	je multiplication
	
	## remain 
	cmpb $'%', %al
	je remain
	
	## division 
	cmpb $'/', %al
	je division
	
	## power 	
	cmpb $'^', %al
	je power
	
	## 'f' print all
	cmpb $'f', %al
	je print_all
	
	## 'c'	clear
	cmpb $'c', %al
	je clear
	
	##  'r' reverse
	cmpb $'r', %al
	je reverse
	
	## 'd' duplicate
	cmpb $'d', %al
	je dup
	
	## egnore other alphabet	
	jmp input

print_all:
	movl %esp, %esi 
print_all_loop: 
	cmpl %esi, %ebp
	je input
	
	movl (%esi), %ecx
	pushl %ecx
	pushl $Print_one
	call printf
	addl $8, %esp
	addl $4, %esi
	jmp print_all
	

print_top:
	## check stack enough
	cmpl %esp, %ebp
	je empty
	movl (%esp), %ecx
	pushl %ecx
	pushl $Print_one
	call printf
	add $8, %esp
	jmp input

empty:

	pushl $emptystr
	call printf
	add $4, %esp
	jmp input

plus:
	cmpl %esp, %ebp
	je empty
	/*movl (%esp), %ebx ## protect to reject the %esp 
	addl $4, %esp ## pop
	cmpl %esp, %ebp
	je empty 
	movl (%esp), %ecx
	addl $4, %esp ## pop 
	*/

	popl %esi
	popl %edi
	addl %esi, %edi
	pushl %edi
	jmp input
	
minus:

	cmpl %esp, %ebp
	je empty
	movl (%esp), %edx ## protect to reject the %esp 
	addl $4, %esp
	cmpl %esp, %ebp
	je empty 
	movl (%esp), %eax
	addl $4, %esp ## pop
	subl %edx, %eax
	pushl %eax
	jmp input

multiplication:

	cmpl %esp, %ebp
	je empty
	movl (%esp), %edx ## protect to reject the %esp 
	addl $4, %esp
	cmpl %esp, %ebp
	je empty 
	movl (%esp), %eax
	addl $4, %esp ## pop
	mull %edx
	pushl %eax
	jmp input
	
remain:
	cmpl %esp, %ebp
	je empty
	movl (%esp), %ebx ## protect to reject the %esp 
	addl $4, %esp
	cmpl %esp, %ebp
	je empty 
	movl (%esp), %eax
	addl $4, %esp ## pop
	cdq ##check the divided and divider bit 
	idiv %ebx
	pushl %edx
	jmp input

division:

	cmpl %esp, %ebp
	je empty
	movl (%esp), %edx ## protect to reject the %esp 
	addl $4, %esp
	cmpl %esp, %ebp
	je empty 
	movl (%esp), %eax
	addl $4, %esp ## pop
	cdq ##check the divided and divider bit 
	idiv %ebx
	pushl %eax
	jmp input
	
power:
	cmpl %esp, %ebp
	je empty
	movl (%esp), %ebx ## protect to reject the %esp 
	addl $4, %esp
	cmpl %esp, %ebp
	je empty 
	movl (%esp), %ecx
	addl $4, %esp ## pop
	## precept 8 copy
	push $1
	subl $4, %esp
	movl $1, IINDEX(%esp)
power_loop:
	movl IINDEX(%esp), %eax
	cmpl %eax, %ebx 
	jg power_loopend
	movl IPOWER(%esp), %eax
	mull %ecx
	movl %eax, IPOWER(%esp)
	incl IINDEX(%esp)
	jmp power_loop
power_loopend:
	movl IPOWER(%esp), %eax
	addl $8, %esp
	push %eax
	jmp input
	
clear:
clear_loop:
	cmpl %esp, %ebp
	je input
	addl $4, %esp
	jmp clear_loop

dup:
	cmpl %esp, %ebp
	je empty
	movl (%esp), %eax
	pushl %eax
	jmp input
reverse:
	cmpl %esp, %ebp
	je empty
	pop %eax
	pop %ebx
	push %eax
	push %ebx
	jmp input
	
	
quit:	
	## return 0
	movl    $0, %eax
	movl    %ebp, %esp
	popl    %ebp
	ret
