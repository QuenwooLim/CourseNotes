org 0x7c00
[bits 16]
xor ax, ax           ; 清除 ax
; 初始化段寄存器, 段地址全部设为0
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

; 初始化栈指针
mov sp, 0x7c00
mov ax, 1                ; 逻辑扇区号第0~15位
mov cx, 0                ; 逻辑扇区号第16~31位
mov bx, 0x7e00           ; bootloader的加载地址
load_bootloader:
    call asm_read_hard_disk  ; 读取硬盘
    inc ax
    cmp ax, 5
    jle load_bootloader
jmp 0x0000:0x7e00        ; 跳转到bootloader

jmp $ ; 死循环

asm_read_hard_disk:
    mov ah, 0x02      ;功能号 0x02 -> 读取扇区
    mov al, 1         ;读取 1 个扇区
    mov ch, 0         ;柱面号为 0
    mov cl, 2         ;扇区号为 2
    mov dh, 0         ;磁头号为 0
    mov dl, 0x80      ;驱动器号为 0x80，表示第一个硬盘
    int 0x13          ;调用 BIOS int 13h 进行硬盘操作
    add bx, 512       ;更新缓冲区地址，增加 512 字节
    ret               


times 510 - ($ - $$) db 0
db 0x55, 0xaa             ; MBR 标志


