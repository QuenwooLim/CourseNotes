.data
n1:	.word	14
n2:	.word	27

.text

main:
	la	$a0,n1
	la	$a1,n2
	jal	swap
	li	$v0,1	# print n1 and n2; should be 27 and 14
	lw	$a0,n1
	syscall
	li	$v0,11
	li	$a0,' '
	syscall
	li	$v0,1
	lw	$a0,n2
	syscall
	li	$v0,11
	li	$a0,'\n'
	syscall
	li	$v0,10	# exit
	syscall

swap:	
	#int temp;
	addi $sp, $sp, -4          # 在栈上开辟空间，将栈指针 $sp 向下移动 4 字节
	
	#temp = *px;
	lw $t0,0($a0) #存a0到temp栈中
	sw $t0,0($sp)
	
	#*px = *py;
	lw $t1,0($a1) #存a1到a0中
	sw $t1,0($a0)
	
	#*py = temp;
	lw $t2,0($sp)
	sw $t2,0($a1)
	
	addi $sp, $sp, 4           # 恢复栈指针，将栈指针 $sp 向上移动 4 字节
	
	jr $ra # 返回到调用点
