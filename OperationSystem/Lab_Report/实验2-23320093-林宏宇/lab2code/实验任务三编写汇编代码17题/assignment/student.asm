section .data
    string db "Hello, world!", 0  ; 字符串并以 0 结尾
    temp_char db 0                ; 临时存储要输出的字符

section .bss
    a1 resd 1                     ; 预留 a1 空间
    a2 resd 1                     ; 预留 a2 空间
    if_flag resd 1                ; 预留 if_flag 空间
    while_flag resd 256           ; 预留空间存储 while_flag 数组

section .text
global student_main              ; 让 C++ 代码能调用

student_main:
    ; 初始化 a1 和 a2
    mov dword [a1], 15           ; 设定 a1 的值（可修改）
    mov dword [a2], 16           ; 设定 a2 的值（可修改）

    ; 执行分支逻辑
    call your_if

    ; 执行循环逻辑
    call your_while

    ; 执行遍历字符串函数
    call your_function

    ; 退出程序
    mov eax, 1                   ; sys_exit
    xor ebx, ebx                 ; 返回码 0
    int 0x80

your_if:
    mov eax, [a1]                ; 获取 a1 的值
    cmp eax, 12
    jl if_case_1                 ; a1 < 12

    cmp eax, 24
    jl if_case_2                 ; a1 < 24

    ; else: if_flag = a1 << 4
    shl eax, 4                   ; a1 左移 4 位
    mov [if_flag], eax           ; 存储结果到 if_flag
    ret

if_case_1:
    ; if_flag = a1 / 2 + 1
    shr eax, 1                   ; a1 / 2
    add eax, 1                   ; +1
    mov [if_flag], eax           ; 存储结果到 if_flag
    ret

if_case_2:
    ; if_flag = (24 - a1) * a1
    mov ebx, 24
    sub ebx, eax                 ; ebx = 24 - a1
    imul eax, ebx                ; eax = (24 - a1) * a1
    mov [if_flag], eax           ; 存储结果到 if_flag
    ret

your_while:
    while_loop:
        mov eax, [a2]
        cmp eax, 12
        jl end_while              ; a2 < 12 时跳出循环

        call my_random           ; 获取随机数，返回值存储在 eax 中

        sub eax, 12
        mov ebx, [a2]
        sub ebx, 12
        cmp ebx, 255             ; 防止数组越界
        jg skip_store

        shl ebx, 2
        mov [while_flag + ebx], eax  ; 存储随机数到数组中

skip_store:
        dec dword [a2]            ; a2 减 1
        jmp while_loop            ; 继续循环

end_while:
    ret

my_random:
    ; 这里的随机数逻辑可以自行修改
    mov eax, 42                  ; 返回固定值 42 作为示例
    ret

your_function:
    xor ecx, ecx                 ; 初始化索引 i = 0
string_loop:
    mov al, [string + ecx]
    cmp al, 0                    ; 判断是否到达字符串末尾
    je end_string_loop

    mov [temp_char], al          ; 存储字符到 temp_char
    push ecx
    call print_a_char            ; 调用 print_a_char 输出字符
    pop ecx

    inc ecx                       ; 索引加 1
    jmp string_loop               ; 继续遍历字符串

end_string_loop:
    ret

print_a_char:
    mov eax, 4                   ; sys_write
    mov ebx, 1                   ; 标准输出
    mov ecx, temp_char           ; 字符的地址
    mov edx, 1                   ; 输出一个字符
    int 0x80                     ; 调用系统中断输出字符
    ret

