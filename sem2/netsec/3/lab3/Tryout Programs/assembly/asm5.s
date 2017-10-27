.text
    .global _start

    _start:

    	movl $3, 0xC(%esp) #does this change esp value?
	addl $0x7, 0xC(%esp)

	jmp _decr
	movl $3, %eax
	inc %eax	#increment

    _decr: decb 0xc(%esp)  #decrement, what is the error here?

	movl $1, %eax
        movl $0, %ebx
	int $0x80
