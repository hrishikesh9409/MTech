
.data
	HelloWorldStr: 
		.ascii "Hello World!\n"
	ByteVar:
		.byte 10
	Int32:
		.int 2
	Int16:
		.short 3
	Float:	
		.float 10.22
	IntArray:
		.int 10,20,30,40
.bss
	.comm LargeBuff 10000

.text
.global _start

_start:
	nop
	
	movl $1, %eax
        movl $0, %ebx
	int $0x80

