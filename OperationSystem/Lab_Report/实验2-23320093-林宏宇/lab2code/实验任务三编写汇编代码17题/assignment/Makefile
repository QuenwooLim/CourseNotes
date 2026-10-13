all: run

run: student.o test.o
	g++ -m32 -o test test.o student.o  # 连接生成的目标文件
	./test

student.o: student.asm
	nasm -f elf32 student.asm -o student.o

test.o: test.cpp
	g++ -m32 -c test.cpp -o test.o  # 将 cpp 文件编译成目标文件

clean:
	rm -f *.o test
