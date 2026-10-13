.data
array:      .word 7, 8, 9, 10, 8    
length:     .word 5                 
element_b:  .word 0                 

.text
.globl main

main:
    la      $t4, array               # 加载数组的地址到 $t4
    lw      $t0, 0($t4)              # 将数组第一个元素加载到 $t0 作为初始最小值
    li      $t1, 1                    # 下标初始化为 1
    lw      $t2, length              

loop:
    beq     $t1, $t2, complete        # 如果下标等于长度，跳转到完成

    lw      $t3, 0($t4)               # 使用偏移量加载当前元素
    mul     $t5, $t1, 4                # 计算当前元素的字节偏移量
    lw      $t3, 0($t4)               # 读取当前元素

    blt     $t3, $t0, updatemin       # 如果当前值小于最小值，更新

    addiu   $t1, $t1, 1                # 下标加 1
    j       loop                       # 继续循环

updatemin:
    move    $t0, $t3                  
    j       loop                       # 返回循环

complete:
    sw      $t0, element_b            
    li      $v0, 10                    # syscall for exit
    syscall
