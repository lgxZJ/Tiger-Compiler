.section .text
.globl _start
_start:
	pushl %ebp
	movl %esp, %ebp
	subl $12, %esp
	pushl %ebx
	pushl %ebp
	pushl %esi
	pushl %edi
Label19:
	movl %ebp, %eax
	addl $-4, %eax
	movl $1, %ebx
	movl %ebx, (%eax)
	movl (%eax), %eax
	movl %ebp, %eax
	addl $-8, %eax
	movl $2, %ebx
	movl %ebx, (%eax)
	movl (%eax), %eax
	movl %ebp, %eax
	addl $-12, %eax
	movl $3, %ebx
	movl %ebx, (%eax)
	movl (%eax), %eax
	movl %ebp, %ebx
	addl $-12, %ebx
	movl %ebp, %eax
	addl $-12, %eax
	movl (%eax), %eax
	movl $1, %ecx
	movl %eax, %eax
	addl %ecx, %eax
	movl %eax, (%ebx)
	movl (%ebx), %eax
	movl %ebp, %eax
	addl $-8, %eax
	movl (%eax), %eax
	movl $2, %ebx
	movl %eax, %eax
	addl %ebx, %eax
	movl $12, %ecx
	movl $Label3, %edx
	movl %ebp, %eax
	addl $-8, %eax
	movl (%eax), %eax
	movl $2, %ebx
	subl %ebx, %eax
	cmpl $0, %eax
	je Label1
Label20:
	movl $0, %eax
Label2:
	pushl %ecx
	pushl %edx
	pushl %eax
	call assertt
	addl $12, %esp

	movl %eax, %eax
	movl $16, %ecx
	movl $Label9, %edx
	movl %ebp, %eax
	addl $-12, %eax
	movl (%eax), %eax
	movl $4, %ebx
	subl %ebx, %eax
	cmpl $0, %eax
	je Label7
Label21:
	movl $0, %eax
Label8:
	pushl %ecx
	pushl %edx
	pushl %eax
	call assertt
	addl $12, %esp

	movl %eax, %eax
	movl $19, %ecx
	movl $Label15, %edx
	movl %ebp, %eax
	addl $-4, %eax
	movl (%eax), %eax
	movl $1, %ebx
	subl %ebx, %eax
	cmpl $0, %eax
	je Label13
Label22:
	movl $0, %eax
Label14:
	pushl %ecx
	pushl %edx
	pushl %eax
	call assertt
	addl $12, %esp

	movl %eax, %eax
	jmp Label23
Label1:
	movl $1, %eax
	jmp Label2
Label7:
	movl $1, %eax
	jmp Label8
Label13:
	movl $1, %eax
	jmp Label14
Label23:	#the epilogue label
	popl %edi
	popl %esi
	popl %ebp
	popl %ebx
	movl %ebp, %esp
	popl %ebp

	movl $0, %ebx
	movl $1, %eax
	int $0x80


.section .data
Label3:
	.asciz "test21.tig"
Label6:
	.asciz "test21.tig"
Label9:
	.asciz "test21.tig"
Label12:
	.asciz "test21.tig"
Label15:
	.asciz "test21.tig"
Label18:
	.asciz "test21.tig"
