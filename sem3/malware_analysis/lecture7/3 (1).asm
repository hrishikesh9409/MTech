BITS 32

extern printf
extern scanf

section .data
    i   :    dd 1

section .rodata
    out :    db "%d", 10, 0

section .text
    global main

    main:
        push ebp
        mov ebp, esp

        .loop:
            push DWORD [i]
            push out
            call printf
            add esp, 8
            inc DWORD [i]
            cmp DWORD [i], 10
            jg .final
            jmp .loop

        .final:
            xor eax, eax
            leave
            ret
