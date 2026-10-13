.data 
Z:	.space 200#为数组Z开辟200的空间
Y:	.word 56#将Y初始化为56

.text
.globl main
main:
	li	$t0,0 #k	
	la	$t1,Z #z

loop_begin:
	bge	$t0,50,end
	
	move	$t2,$t0 #k
	srl	$t2,$t2,2 #k/4
	addi	$t2,$t2,210 #(k/4+210)
	sll	$t2,$t2,4 #16*(k/4+210)
	
	lw	$t3,Y #Y
	sub	$t3,$t3,$t2 #Y-16*(k/4+210)
	
	sw	$t3,0($t1) #存储当前数组指向的的元素
	addi	$t1,$t1,4 #指向下一个元素
	
	add	$t0,$t0,1 #k+
	j	loop_begin 

end:
	li	$v0,10 # 系统调用：10 是 exit
	syscall 