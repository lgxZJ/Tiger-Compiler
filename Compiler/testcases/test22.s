.section .text
.globl _start
_start:
	pushl %ebp
	movl %esp, %ebp
	subl $4, %esp
	pushl %ebx
	pushl %ebp
	pushl %esi
	pushl %edi
Label27:
	movl $Label1, %eax
	pushl %eax
	call prints
	addl $4, %esp

	movl %eax, %eax
	movl $Label2, %eax
	pushl %eax
	call prints
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $Label3, %eax
	pushl %eax
	call ord
	addl $4, %esp

	movl %eax, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $Label4, %eax
	pushl %eax
	call ord
	addl $4, %esp

	movl %eax, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $33, %eax
	pushl %eax
	call chr
	addl $4, %esp

	movl %eax, %eax
	pushl %eax
	call prints
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $126, %eax
	pushl %eax
	call chr
	addl $4, %esp

	movl %eax, %eax
	pushl %eax
	call prints
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $Label5, %eax
	pushl %eax
	call size
	addl $4, %esp

	movl %eax, %eax
	pushl %eax
	call printn
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $Label6, %eax
	pushl %eax
	call size
	addl $4, %esp

	movl %eax, %eax
	pushl %eax
	call printn
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $Label7, %eax
	pushl %eax
	call size
	addl $4, %esp

	movl %eax, %eax
	pushl %eax
	call printn
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $1211, %eax
	pushl %eax
	call printn
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $6, %eax
	movl $0, %ebx
	movl $Label8, %ecx
	pushl %eax
	pushl %ebx
	pushl %ecx
	call substring
	addl $12, %esp

	movl %eax, %eax
	pushl %eax
	call prints
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $3, %eax
	movl $3, %ebx
	movl $Label9, %ecx
	pushl %eax
	pushl %ebx
	pushl %ecx
	call substring
	addl $12, %esp

	movl %eax, %eax
	pushl %eax
	call prints
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $1, %eax
	movl $5, %ebx
	movl $Label10, %ecx
	pushl %eax
	pushl %ebx
	pushl %ecx
	call substring
	addl $12, %esp

	movl %eax, %eax
	pushl %eax
	call prints
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $0, %eax
	movl $5, %ebx
	movl $Label11, %ecx
	pushl %eax
	pushl %ebx
	pushl %ecx
	call substring
	addl $12, %esp

	movl %eax, %eax
	pushl %eax
	call prints
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $Label13, %eax
	movl $Label12, %ebx
	pushl %eax
	pushl %ebx
	call concat
	addl $8, %esp

	movl %eax, %eax
	pushl %eax
	call prints
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $Label15, %eax
	movl $Label14, %ebx
	pushl %eax
	pushl %ebx
	call concat
	addl $8, %esp

	movl %eax, %eax
	pushl %eax
	call prints
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $Label17, %eax
	movl $Label16, %ebx
	pushl %eax
	pushl %ebx
	call concat
	addl $8, %esp

	movl %eax, %eax
	pushl %eax
	call prints
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $1, %eax
	pushl %eax
	call not
	addl $4, %esp

	movl %eax, %eax
	pushl %eax
	call printn
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $9, %eax
	pushl %eax
	call not
	addl $4, %esp

	movl %eax, %eax
	pushl %eax
	call printn
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $1, %eax
	movl $0, %ebx
	subl %eax, %ebx
	pushl %ebx
	call not
	addl $4, %esp

	movl %eax, %eax
	pushl %eax
	call printn
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $0, %eax
	pushl %eax
	call not
	addl $4, %esp

	movl %eax, %eax
	pushl %eax
	call printn
	addl $4, %esp

	movl %eax, %eax
	movl $10, %eax
	pushl %eax
	call printc
	addl $4, %esp

	movl %eax, %eax
	movl $22, %eax
	movl $Label18, %ebx
	movl $1, %ecx
	pushl %eax
	pushl %ebx
	pushl %ecx
	call assertt
	addl $12, %esp

	movl %eax, %eax
	movl $23, %ebx
	movl $Label19, %ecx
	movl $1, %eax
	movl $90, %edx
	movl %eax, %eax
	movl %eax, %eax
	imull %edx
	pushl %ebx
	pushl %ecx
	pushl %eax
	call assertt
	addl $12, %esp

	movl %eax, %eax
	movl $24, %ebx
	movl $Label20, %ecx
	movl $1, %eax
	movl $34, %edx
	movl %eax, %eax
	addl %edx, %eax
	pushl %ebx
	pushl %ecx
	pushl %eax
	call assertt
	addl $12, %esp

	movl %eax, %eax
	movl $25, %ebx
	movl $Label21, %ecx
	movl $1, %eax
	movl $1, %esi
	movl %eax, %eax
	movl $0, %edx
	movl %eax, %eax
	idivl %esi
	pushl %ebx
	pushl %ecx
	pushl %eax
	call assertt
	addl $12, %esp

	movl %eax, %eax
	movl %ebp, %eax
	addl $-4, %eax
	movl $10, %ebx
	movl %ebx, (%eax)
	movl (%eax), %eax
Label23:
	movl %ebp, %eax
	addl $-4, %eax
	movl (%eax), %eax
	cmpl $0, %eax
	je Label22
Label28:
	call getcharr
	addl $0, %esp

	movl %eax, %eax
	pushl %eax
	call prints
	addl $4, %esp

	movl %eax, %eax
	movl %ebp, %eax
	addl $-4, %eax
	movl %ebp, %ebx
	addl $-4, %ebx
	movl (%ebx), %ebx
	movl $1, %ecx
	movl %ebx, %ebx
	subl %ecx, %ebx
	movl %ebx, (%eax)
	movl (%eax), %eax
	jmp Label23
Label22:
	movl $Label26, %eax
	pushl %eax
	call prints
	addl $4, %esp

	movl %eax, %eax
	jmp Label29
Label29:	#the epilogue label
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
	.asciz "==Following is a output/input test==\n"
Label2:
	.asciz "aaa"
Label3:
	.asciz "12"
Label4:
	.asciz "21"
Label5:
	.asciz "a"
Label6:
	.asciz "aa"
Label7:
	.asciz "aaa"
Label8:
	.asciz "123456"
Label9:
	.asciz "123456"
Label10:
	.asciz "123456"
Label11:
	.asciz "123456"
Label12:
	.asciz "12"
Label13:
	.asciz "21"
Label14:
	.asciz "12"
Label15:
	.asciz "458"
Label16:
	.asciz "12"
Label17:
	.asciz ""
Label18:
	.asciz "test22.tig"
Label19:
	.asciz "test22.tig"
Label20:
	.asciz "test22.tig"
Label21:
	.asciz "test22.tig"
Label26:
	.asciz "==Tests successfully ends==\n"
