;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                              ;
; Author    :   Arvind                                                         ;
; Date      :   30/07/2017                                                     ;
; Program   :   Input 2 numbers using scanf and print sum using printf         ;
; Note      :   This program was tested on Ubuntu 16.04 64 bit using           ;
;               nasm 2.11.08 and gcc 5.4.0 multilib.                           ;
;                                                                              ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

BITS 32

extern printf
extern scanf

section .rodata
    inp   :    db "%d"                  ; input format for scanf
    out   :    db "Sum is %d", 10, 0    ; output format for printf
    prompt:    db "Enter number: ", 0   ; prompt to enter input

section .bss
    num1: resd 1                        ; number 1
    num2: resd 1                        ; number 2

section .text
    global main

    main:
        push ebp                    ; setup stack frame
        mov ebp, esp

        push prompt                 ; display prompt
        call printf
        add esp, 4

        push num1                   ; 2nd arg of scanf: address of num1
        push inp                    ; 1st arg of scanf: input format
        call scanf                  ; call scanf
        add esp, 8

        push prompt                 ; display prompt
        call printf
        add esp, 4

        push num2                   ; 2nd arg of scanf: address of num2
        push inp                    ; 1st arg of scanf: input format
        call scanf                  ; call scanf
        add esp, 8

        mov edx, DWORD [num1]       ; Copy num1's value to edx
        add edx, DWORD [num2]       ; Add num2's value to edx
        push edx                    ; and push it on to the stack
        push out                    ; 1st arg of printf: output format to display sum
        call printf                 ; call printf
        add esp, 8

        xor eax, eax                ; same as "mov eax, 0"
        leave                       ; same as "mov esp, ebp; pop ebp"
        ret
