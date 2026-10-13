org 0x7c00;引导扇区的起始地址
[bits 16]

start:
    xor ax,ax;清空AX寄存器
    mov ds,ax;数据段寄存器
    mov ss,ax;栈段寄存器
    mov es,ax;附加段寄存器
    mov fs,ax;额外段寄存器
    mov gs,ax;全局段寄存器
    ;初始化段寄存器，实模式下所有段地址设为0

    mov sp,0x7c00;设置栈指针到0x7C00
    ;传统上引导扇区使用这块内存作为栈

    mov ax,0xb800
    mov gs,ax;视频内存段地址，文本模式下从0xB8000开始

main_loop:
    mov ah,0x00;功能号0：读取键盘输入字符
    int 0x16;调用BIOS中断，等待按键输入
    ;AL中返回ASCII码字符

    cmp al,27;检查是否按下ESC键（ASCII=27）
    je exit;如果是ESC键，退出循环

    mov ah,0x03;设置颜色属性：前景色青色（3），背景色黑色（0）

    mov [gs:0x0000],al;存储字符
    mov [gs:0x0001],ah;存储颜色属性
    ;在屏幕左上角(0x0000)显示输入的字符，青色字体

    jmp main_loop;跳回主循环继续读取键盘输入

exit:
    jmp $;死循环，防止程序退出

times 510-($-$$) db 0
db 0x55,0xaa;引导扇区结束标志

