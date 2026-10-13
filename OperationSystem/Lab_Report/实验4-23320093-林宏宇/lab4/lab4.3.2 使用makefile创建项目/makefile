# 编译器和目标文件
CC = gcc
TARGET = my_program
SRC = main.c mathlib.c

# 默认规则
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET)

# 运行规则
run: $(TARGET)
	./$(TARGET)

# 清理规则
clean:
	rm -f $(TARGET)

