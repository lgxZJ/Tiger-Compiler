.section .text
.globl _start
_start:
	pushl %ebp
	movl %esp, %ebp
	subl $8, %esp
	pushl %ebx
	pushl %ebp
	pushl %esi
	pushl %edi
Label14:
	movl %ebp, %eax
	addl $-4, %eax
	movl $1, %ebx
	movl %ebx, (%eax)
	movl (%eax), %eax
	movl %ebp, %eax
	addl $-8, %eax
	movl $Label1, %ebx
	movl %ebx, (%eax)
	movl (%eax), %eax
	movl $6, %ecx
	movl $Label5, %edx
	movl %ebp, %eax
	addl $-4, %eax
	movl (%eax), %eax
	movl $1, %ebx
	subl %ebx, %eax
	cmpl $0, %eax
	je Label3
Label15:
	movl $0, %eax
Label4:
	pushl %ecx
	pushl %edx
	pushl %eax
	call assertt
	addl $12, %esp

	movl %eax, %eax
	movl $7, %ebx
	movl $Label9, %esi
	movl %ebp, %eax
	addl $-8, %eax
	movl (%eax), %eax
	movl $Label6, %ecx
	pushl %ecx
	pushl %eax
	call strcmp
	addl $8, %esp

	movl %eax, %eax
	cmpl $0, %eax
	je Label7
Label16:
	movl $0, %eax
Label8:
	pushl %ebx
	pushl %esi
	pushl %eax
	call assertt
	addl $12, %esp

	movl %eax, %eax
	jmp Label17
Label3:
	movl $1, %eax
	jmp Label4
Label7:
	movl $1, %eax
	jmp Label8
Label17:	#the epilogue label
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
Label1:
	.asciz "abc"
Label2:
	.asciz "abc"
Label5:
	.asciz "test1.tig"
Label6:
	.asciz "abc"
Label9:
	.asciz "test1.tig"
Label10:
	.asciz "abc"
Label13:
	.asciz "test1.tig"
