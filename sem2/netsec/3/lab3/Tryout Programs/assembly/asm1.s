.text
    .global _start

    _start:

    	push $1  #notice esp value
	push $2  #notice esp value again. does the value change? 
    	movl $3, 0xC(%esp) #does this change esp value?

	movl 0xC(%esp), %eax #move value in esp+0xc to eax

	movb $0xdeadbeef, %eax

	movl $1, %eax
        movl $0, %ebx
	int $0x80
