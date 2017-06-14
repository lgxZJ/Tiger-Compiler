.section .text

.type print, @function
.globl print
print:
    pushl %ebp
	movl %esp, %ebp
	subl $0, %esp
	pushl %ebx
	pushl %ebp
	pushl %esi
	pushl %edi

    movl %ebp, %eax
	addl $8, %eax
	movl (%eax), %eax
	pushl %eax
	call puts
	addl $4, %esp

    popl %edi
	popl %esi
	popl %ebp
	popl %ebx
	movl %ebp, %esp
	popl %ebp

	ret