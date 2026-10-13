[BITS 16]
[ORG 0x7C00]

start:
	;获取光标位置并存储光标位置
	mov ah, 0x03        ; 功能号 03h：获取光标位置
	mov bh, 0x00        ; 指定页面号 0 
	int 0x10            ; BIOS中断，返回光标的行号和列号
	mov [cur_row], dh   ; 存储当前光标的行号
	mov [cur_col], dl   ; 存储当前光标的列号

    ; 3. 进入无限循环，等待 GDB 调试
hang:
    jmp hang            ; 无限循环

; 预留变量存储光标位置
cur_row db 0  ; 存储光标行号
cur_col db 0  ; 存储光标列号

; 填充至 512 字节，使其成为引导扇区
times 510-($-$$) db 0
dw 0xAA55

