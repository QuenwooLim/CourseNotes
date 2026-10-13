.data
.text
.globl main
main:
	li	$a0,5#被乘数
	li	$a1,4#乘数
	li	$v1,0
	
loop_begin:
	beq	$a1,0,end #乘数为0 结束
	
	andi	$t0,$a1,1 #取出乘数的最后一位
	beqz	$t0,shift_num #最后一位为0 直接跳到移位步骤
	
	add	$v1,$v1,$a0 #积=被乘数+积

shift_num:
	sll	$a0,$a0,1 #被乘数左移1
	srl	$a1,$a1,1 #乘数右移1
	j 	loop_begin
	
end:
	li $v0,1 #系统调用号1 用于打印整数
	move $a0,$v1
	syscall
	
	li $v0,10
	syscall
