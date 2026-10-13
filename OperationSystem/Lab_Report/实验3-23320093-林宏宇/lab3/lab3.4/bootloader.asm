%include "boot.inc"
org 0x7e00
[bits 16]


mov ax, 0xb800
mov gs, ax
mov ah, 0x03 ;青色
mov ecx, bootloader_tag_end - bootloader_tag
xor ebx, ebx
mov esi, bootloader_tag
output_bootloader_tag:
    mov al, [esi]
    mov word[gs:bx], ax
    inc esi
    add ebx,2
    loop output_bootloader_tag

bootloader_tag db 'run bootloader'
bootloader_tag_end:


;空描述符
mov dword [GDT_START_ADDRESS+0x00],0x00
mov dword [GDT_START_ADDRESS+0x04],0x00  

;创建描述符，这是一个数据段，对应0~4GB的线性地址空间
mov dword [GDT_START_ADDRESS+0x08],0x0000ffff    ; 基地址为0，段界限为0xFFFFF
mov dword [GDT_START_ADDRESS+0x0c],0x00cf9200    ; 粒度为4KB，存储器段描述符 

;建立保护模式下的堆栈段描述符      
mov dword [GDT_START_ADDRESS+0x10],0x00000000    ; 基地址为0x00000000，界限0x0 
mov dword [GDT_START_ADDRESS+0x14],0x00409600    ; 粒度为1个字节

;建立保护模式下的显存描述符   
mov dword [GDT_START_ADDRESS+0x18],0x80007fff    ; 基地址为0x000B8000，界限0x07FFF 
mov dword [GDT_START_ADDRESS+0x1c],0x0040920b    ; 粒度为字节

;创建保护模式下平坦模式代码段描述符
mov dword [GDT_START_ADDRESS+0x20],0x0000ffff    ; 基地址为0，段界限为0xFFFFF
mov dword [GDT_START_ADDRESS+0x24],0x00cf9800    ; 粒度为4kb，代码段描述符 

pgdt dw 0 
    dd GDT_START_ADDRESS

;初始化描述符表寄存器GDTR
mov word [pgdt], 39      ;描述符表的界限   
lgdt [pgdt]

; _____________Selector_____________
;平坦模式数据段选择子
DATA_SELECTOR equ 0x8
;平坦模式栈段选择子
STACK_SELECTOR equ 0x10
;平坦模式视频段选择子
VIDEO_SELECTOR equ 0x18
VIDEO_NUM equ 0x18
;平坦模式代码段选择子
CODE_SELECTOR equ 0x20

in al,0x92                         ;南桥芯片内的端口 
or al,0000_0010B
out 0x92,al                        ;打开A20

cli                                ;中断机制尚未工作
mov eax,cr0
or eax,1
mov cr0,eax                        ;设置PE位

jmp dword CODE_SELECTOR:protect_mode_begin

;16位的描述符选择子：32位偏移
;清流水线并串行化处理器
[bits 32]           
protect_mode_begin:
    ;设置数据段、栈段和视频段寄存器
    mov eax, DATA_SELECTOR             ;加载数据段选择子（0..4GB）
    mov ds, eax                        ;将选择子加载到 ds
    mov es, eax                        ;将选择子加载到 es
    mov eax, STACK_SELECTOR            ;加载栈段选择子
    mov ss, eax                        ;将选择子加载到 ss
    mov eax, VIDEO_SELECTOR            ;加载视频段选择子
    mov gs, eax                        ;将选择子加载到 gs

    ;设置输出字符的起始位置
    mov esi, student_id_name          ;esi指向学生ID的字符串
    mov ebx, (12 * 80 + 12) * 2        ;计算显示位置，12行12列，80列屏幕，每个字符2字节（16位）

    ;计算学生ID字符串的长度
    mov ecx, student_id_name_end - student_id_name  ;计算字符串长度
    mov edx, 0                         ;用于颜色切换的计数器（切换前景和背景颜色）
    
output_student_info:
    mov al, [esi]                     ;取字符
    test edx, 1                        ;检查颜色切换
    jz even_color                      ;如果edx为偶数，使用一个颜色
    mov ah, 0x1F                       ;奇数位置，白色前景，蓝色背景
    jmp set_char                       ;跳转到设置字符

even_color:
    mov ah, 0xF1                       ;偶数位置，蓝色前景，白色背景

set_char:
    mov word [gs:ebx], ax              ;将字符和颜色写入显存
    add ebx, 2                         ;移动到下一个字符位置（每个字符占2字节）
    inc esi                            ;指向下一个字符
    inc edx                            ;增加计数器，用于控制颜色切换
    loop output_student_info           ;循环直到字符串输出完

jmp $  ;进入死循环，防止程序退出

student_id_name db '23320093LHY'      ;学生ID字符串
student_id_name_end:                   ;字符串结束标记


times 2560 - ($ - $$) db 0 ;手动填充字节
