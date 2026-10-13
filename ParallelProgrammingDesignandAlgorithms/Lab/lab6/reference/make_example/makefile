# 编译器设置
CC = gcc
CFLAGS = -Wall -fPIC

# 库名设置
LIB_NAME = mylib
LIB_SONAME_VERSION = 1
LIB_VERSION = 1.0

# 文件命名
LIB_REAL = lib$(LIB_NAME).so.$(LIB_VERSION)
LIB_SONAME = lib$(LIB_NAME).so.$(LIB_SONAME_VERSION)
LIB_LINKER = lib$(LIB_NAME).so

TARGET = main

# 默认目标
all: $(LIB_REAL) symlinks $(TARGET)

# 编译共享库
$(LIB_REAL): mylib.o
	$(CC) -shared -Wl,-soname,$(LIB_SONAME) -o $@ $^

# 创建符号链接
symlinks:
	ln -sf $(LIB_REAL) $(LIB_SONAME)
	ln -sf $(LIB_SONAME) $(LIB_LINKER)

# 编译主程序（-l 直接使用 LIB_NAME）
$(TARGET): main.o
	$(CC) -o $@ $< -L. -l$(LIB_NAME) -Wl,-rpath,.

# 生成 .o 文件
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# 清理
clean:
	rm -f *.o $(LIB_REAL) $(LIB_SONAME) $(LIB_LINKER) $(TARGET)

.PHONY: all clean symlinks