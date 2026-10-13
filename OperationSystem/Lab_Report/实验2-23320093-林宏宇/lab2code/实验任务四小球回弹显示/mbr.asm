org 0x7C00  ;MBR加载地址

;BIOS颜色表
%define COLOR_BLUE 1
%define COLOR_GREEN 2
%define COLOR_CYAN 3
%define COLOR_RED 4
%define COLOR_MAGENTA 5
%define COLOR_YELLOW 6
%define COLOR_WHITE 7

;变量存储
x_pos db 2     ;初始X坐标
y_pos db 0     ;初始Y坐标
x_dir db 1     ;X方向(1右,-1左)
y_dir db 1     ;Y方向(1下,-1上)
char db '*'    ;显示字符
color db COLOR_RED  ;颜色

start:
    ;清屏（只执行一次）
    mov ah, 0x06   ;BIOS中断服务，清除屏幕
    mov al, 0      ;清除屏幕
    mov bh, 0x07   ;颜色属性：白色
    mov cx, 0      ;左上角（坐标0,0）
    mov dh, 24     ;屏幕高度(25行)
    mov dl, 79     ;屏幕宽度(80列)
    int 0x10       ;调用BIOS中断，清除屏幕

main_loop:
    call draw_char  ;画字符
    call delay      ;延迟
    call move_char  ;移动字符
    jmp main_loop   ;回到主循环继续执行

;画字符（不擦除，留下轨迹）
draw_char:
    mov ah, 0x02   ;设置光标位置
    mov bh, 0      ;页号，0为当前页面
    mov dh, [y_pos] ;获取当前Y坐标
    mov dl, [x_pos] ;获取当前X坐标
    int 0x10       ;调用BIOS中断，设置光标位置

    mov ah, 0x09   ;写字符
    mov al, [char] ;读取字符
    mov bh, 0      ;页号，0为当前页面
    mov bl, [color] ;读取颜色
    mov cx, 1      ;只写一个字符
    int 0x10       ;调用BIOS中断，输出字符
    ret

;计算新坐标
move_char:
    mov al, [x_pos]
    add al, [x_dir]  ;更新X坐标
    mov [x_pos], al

    mov al, [y_pos]
    add al, [y_dir]  ;更新Y坐标
    mov [y_pos], al

    ;处理边界反弹
    cmp byte [x_pos], 79  ;检查是否到达右边界
    jg bounce_x_left      ;如果到达，左反弹
    cmp byte [x_pos], 0    ;检查是否到达左边界
    jl bounce_x_right     ;如果到达，右反弹

    cmp byte [y_pos], 24  ;检查是否到达下边界
    jg bounce_y_up        ;如果到达，上反弹
    cmp byte [y_pos], 0    ;检查是否到达上边界
    jl bounce_y_down      ;如果到达，下反弹
    ret

bounce_x_left:
    mov byte [x_dir], -1   ;X方向反向
    call change_color      ;改变颜色
    call change_char       ;改变字符
    ret

bounce_x_right:
    mov byte [x_dir], 1    ;X方向正向
    call change_color      ;改变颜色
    call change_char       ;改变字符
    ret

bounce_y_up:
    mov byte [y_dir], -1   ;Y方向反向
    call change_color      ;改变颜色
    call change_char       ;改变字符
    ret

bounce_y_down:
    mov byte [y_dir], 1    ;Y方向正向
    call change_color      ;改变颜色
    call change_char       ;改变字符
    ret

;变换颜色
change_color:
    mov al, [color]        ;读取当前颜色
    inc al                 ;增加颜色
    cmp al, COLOR_WHITE    ;如果颜色超出白色
    jle set_color          ;不超过白色时设置新颜色
    mov al, COLOR_BLUE     ;超过白色时，回到蓝色
set_color:
    mov [color], al        ;存储新的颜色
    ret

;变换字符
change_char:
    mov al, [char]         ;读取当前字符
    inc al                 ;字符递增
    cmp al, '9'            ;如果字符大于'9'
    jle set_char           ;小于等于'9'时设置字符
    mov al, '0'            ;超过'9'时从'0'开始
set_char:
    mov [char], al         ;存储新的字符
    ret

;延迟
delay:
    mov cx, 0xFFFF         ;设置延迟循环的计数器
delay_loop:
    loop delay_loop        ;执行延迟
    ret

;MBR需要填充到510字节，并以0x55AA结束
times 510-($-$$) db 0      ;填充空字节
dw 0xAA55                  ;MBR标志(0xAA55)