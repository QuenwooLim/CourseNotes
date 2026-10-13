.data
.text
.globl main
main:
    # 初始化被乘数和乘数
    li $a0, -5           # 被乘数（例如 -5）
    li $a1, 4            # 乘数（例如 4）
    li $v1, 0            # 初始化积为 0

    # 确定结果的最终符号并取绝对值
    # 记录原始被乘数和乘数的符号
    # 若符号不同，则最终结果应为负

    li $t3, 0            # $t3 用于保存最终符号（0 = 正，1 = 负）

    # 如果 $a0 < 0，将其取反，并设置 $t3 为 1 表示负数
    bltz $a0, neg_a0
    j check_a1
neg_a0:
    sub $a0, $zero, $a0  # 取 $a0 的绝对值
    xori $t3, $t3, 1     # 切换最终符号为负

check_a1:
    # 如果 $a1 < 0，将其取反，并切换最终符号
    bltz $a1, neg_a1
    j multiply
neg_a1:
    sub $a1, $zero, $a1  # 取 $a1 的绝对值
    xori $t3, $t3, 1     # 切换最终符号

    # 二进制无符号乘法循环
multiply:
loop_begin:
    beq $a1, 0, end      # 如果乘数为 0，跳转到 end 结束

    # 取出乘数的最后一位
    andi $t0, $a1, 1
    beqz $t0, shift_num  # 如果最后一位为 0，跳到移位步骤

    add $v1, $v1, $a0    # 若最后一位为 1，将被乘数加到积上

shift_num:
    sll $a0, $a0, 1      # 被乘数左移 1 位
    srl $a1, $a1, 1      # 乘数右移 1 位
    j loop_begin         # 跳转回循环开始

end:
    # 如果最终结果应该是负数，则取反 $v1
    bnez $t3, negate_result
    j print_result

negate_result:
    sub $v1, $zero, $v1  # 将积 $v1 取反

print_result:
    li $v0, 1            # 系统调用号 1，用于打印整数
    move $a0, $v1        # 将积的值移动到 $a0
    syscall              # 打印结果

    li $v0, 10           # 系统调用号 10，用于退出程序
    syscall              # 退出程序

