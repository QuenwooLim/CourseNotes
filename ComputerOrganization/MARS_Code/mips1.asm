.data
array:	.word 7,8,9,10,8,1,1,1 	##define array
length:	.word 8			##define length
result:	.asciiz "The average is:"	##define output

.text
.globl main

main:
	li	$t0,0	##sum
	li	$t1,0	##index
	lw	$t2,length	##length

loop:
	beq	$t1,$t2,cal
	
	la	$a0,array
	lw	$t3,0($a0)
	##0($a0): 这是内存地址的计算方式。$a0 是寄存器，包含数组的基地址，0 表示偏移量。这意味着将从 $a0 指向的地址开始加载数据。
	add	$t0,$t0,$t3
	
	addiu	$t1,$t1,1	##index++
	addiu	$a0,$a0,4	##a0跳到下一个元素
	j	loop
	
cal:
	li	$t4,8
	div	$t0,$t4
	mflo	$t0		##获得商lo
	
	li	$v0,4		##syscall for print string
	la	$a0,result	##load result string
	syscall
	
	li	$v0,1		##syscall for print integer
	move	$a0,$t0		##确保要打印的值在$a0存储器中
	syscall
	
	li	$v0,10		##syscall for exit
	syscall