	.file	"dl.c"
	.section	.rodata
.LC0:
	.string	"/lib/libm.so.6"
.LC1:
	.string	"cos"
.LC3:
	.string	"%f\n"
	.text
.globl main
	.type	main, @function
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	pushl	%ecx
	subl	$32, %esp
	movl	$1, 4(%esp)
	movl	$.LC0, (%esp)
	call	dlopen
	movl	%eax, -20(%ebp)
	cmpl	$0, -20(%ebp)
	jne	.L2
	movl	stderr, %ebx
	call	dlerror
	movl	%ebx, 4(%esp)
	movl	%eax, (%esp)
	call	fputs
	movl	$1, (%esp)
	call	exit
.L2:
	movl	$.LC1, 4(%esp)
	movl	-20(%ebp), %eax
	movl	%eax, (%esp)
	call	dlsym
	movl	%eax, -16(%ebp)
	call	dlerror
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	je	.L3
	movl	stderr, %eax
	movl	%eax, 4(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	fputs
	movl	$1, (%esp)
	call	exit
.L3:
	fldl	.LC2
	fstpl	(%esp)
	movl	-16(%ebp), %eax
	call	*%eax
	fstpl	4(%esp)
	movl	$.LC3, (%esp)
	call	printf
	movl	-20(%ebp), %eax
	movl	%eax, (%esp)
	call	dlclose
	movl	$0, %eax
	addl	$32, %esp
	popl	%ecx
	popl	%ebx
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
	.size	main, .-main
	.section	.rodata
	.align 8
.LC2:
	.long	0
	.long	1073741824
	.ident	"GCC: (Debian 4.3.2-1.1) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
