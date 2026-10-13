org 0x7c00
[bits 16]
xor ax, ax
; 初始化段寄存器
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

call read_disk
jmp 0x0000:0x7e00

jmp $

read_disk:
	mov ax, 0
	mov es, ax
	mov bx, 0x7e00
	mov ah, 0x02	
	mov al, 0x05	
times 510-($-$$) db 0
db 0x55, 0xaa
