[bits 32]

global asm_hello_world

asm_hello_world:
    push eax
    xor eax, eax

    mov ah, 0x03 ;青色
    mov al, '2'
    mov [gs:2 * 0], ax

    mov al, '3'
    mov [gs:2 * 1], ax

    mov al, '3'
    mov [gs:2 * 2], ax

    mov al, '2'
    mov [gs:2 * 3], ax

    mov al, '0'
    mov [gs:2 * 4], ax

    mov al, '0'
    mov [gs:2 * 5], ax

    mov al, '9'
    mov [gs:2 * 6], ax

    mov al, '3'
    mov [gs:2 * 7], ax

    pop eax
    ret
