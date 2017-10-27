.text
    .global _start

    _start:

    	movl $3, 0xC(%esp) #does this change esp value?
	addl $0x7, 0xC(%esp)

	subl $0x5, 0xC(%esp)


	movl $1, %eax
        movl $0, %ebx
	int $0x80
