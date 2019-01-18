	.file	"compile1.cpp"
	.text
	.p2align 4,,15
	.globl	_Z3addii
	.type	_Z3addii, @function
_Z3addii:
.LFB1:
	.cfi_startproc
	leal	(%rdi,%rsi), %eax
	ret
	.cfi_endproc
.LFE1:
	.size	_Z3addii, .-_Z3addii
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-36)"
	.section	.note.GNU-stack,"",@progbits
