[BITS16]
[ORG0x7C00]

start:
    movah,0x03
    movbh,0x00
    int0x10
    mov[cur_row],dh
    mov[cur_col],dl
    ;获取光标初始位置并存储，用于后续光标移动

keyboard_loop:
    movah,0x00
    int0x16
    cmpal,'q'
    jeexit
    ;等待键盘输入，按'q'退出

    movah,0x03
    movbh,0x00
    int0x10
    mov[cur_row],dh
    mov[cur_col],dl
    ;读取光标当前位置

    cmpal,'w'
    jemove_up
    cmpal,'s'
    jemove_down
    cmpal,'a'
    jemove_left
    cmpal,'d'
    jemove_right
    jmpkeyboard_loop
    ;检测输入字符并跳转到相应方向的移动逻辑

move_up:
    decbyte[cur_row]
    jmpupdate_cursor
    ;光标上移

move_down:
    incbyte[cur_row]
    jmpupdate_cursor
    ;光标下移

move_left:
    decbyte[cur_col]
    jmpupdate_cursor
    ;光标左移

move_right:
    incbyte[cur_col]
    ;光标右移

update_cursor:
    movah,0x02
    movdh,[cur_row]
    movdl,[cur_col]
    movbh,0x00
    int0x10
    ;设置光标新位置

    movah,0x09
    moval,' '
    movbh,0x00
    movbl,0x07
    movcx,1
    int0x10
    ;清除原字符

    movah,0x02
    movdh,[cur_row]
    movdl,[cur_col]
    movbh,0x00
    int0x10
    ;重新设置光标位置

    movah,0x09
    moval,'X'
    movbh,0x00
    movbl,0x0F
    movcx,1
    int0x10
    ;在新位置显示'X'，表示移动成功

    jmpkeyboard_loop

exit:
    jmpexit
    ;程序进入死循环，等待重启或退出

cur_rowdb10
cur_coldb10
;存储光标当前位置，初始设定在(10,10)

times510-($-$$)db0
dw0xAA55
;填充到512字节，并写入引导扇区标志


