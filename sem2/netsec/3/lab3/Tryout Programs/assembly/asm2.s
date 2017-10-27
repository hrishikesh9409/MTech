.text
    .global _start

    _start:

    	push $1
    	movl $3, 0xC(%esp) #does this change esp value?

	movl 0xC(%esp), %eax #move value in esp+0xc to eax
	leal 0xC(%esp), %ebx #move value in esp+0xc to eax

	movl $1, %eax
        movl $0, %ebx
	int $0x80
