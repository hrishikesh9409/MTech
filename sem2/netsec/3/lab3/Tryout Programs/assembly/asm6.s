.text
    .global _start

    _start:

	movl $0x3, %eax
	cmp $0x3, %eax
	je _decr
  	
	inc %eax	#increment

    _decr: dec %eax  #decrement, what is the error here?

	movl $1, %eax
        movl $0, %ebx
	int $0x80
