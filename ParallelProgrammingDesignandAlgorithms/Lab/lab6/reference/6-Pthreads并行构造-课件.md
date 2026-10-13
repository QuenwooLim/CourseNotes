并行程序设计与算法（实验）
6 - Pthreads并行构造

授课教师： 吴迪、刘学正  
单位： 中山大学计算机学院

实验概要

实验内容
动态库的构建与链接使用
基于 Pthreads 的 OpenMP parallel for 并行循环实现
典型问题的并行化改造：矩阵乘法，热传导平板问题

实验目的
深入理解 C/C++ 程序编译、链接与构建过程
提高 Pthreads 多线程编程综合能力
并行库开发与性能验证

C/C++ 程序的编译

从源代码到可执行文件的完整编译流程：

预处理（Preprocessing）
      gcc -E hello.c -o hello.i
   

编译（Compilation）
      gcc -S hello.i -o hello.s
   

汇编（Assemble）
      gcc -c hello.s -o hello.o
   

链接（Linking）
      gcc hello.o -o hello
   

参考资料： https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

从源码到目标代码

gcc -c hello.c

目标代码符号表
  已定义的代码段符号（main 函数）
  已定义的数据段符号（全局变量 x）
  未定义的外部符号（需要链接时解析）

从目标代码到可执行程序

链接过程与标准库的使用

gcc hello.o -o hello

链接阶段会解析 hello.o 中的外部符号（如 printf）
默认链接标准 C 库 libc.so，生成可执行文件

静态和动态链接

静态链接（Static Linking）
发生在程序运行加载前
链接器将程序用到的库目标代码完整拷贝到可执行文件中
生成的可执行文件体积较大，运行时不依赖任何外部库文件
若库版本更新，必须重新编译链接整个程序

动态链接（Dynamic Linking）
发生在程序加载时或运行时
链接阶段仅做符号引用登记，不拷贝库代码
可执行文件体积更小，多个进程可共享同一份动态库内存镜像
库更新后无需重新编译链接程序
平台   静态库   动态库
Linux   lib.a   lib.so

Windows   lib.lib   lib.dll

库的使用：从编译到运行

预处理阶段：包含库头文件（-I）
      gcc ... -Iinclude_path ...
   

链接阶段：链接库文件（-L/-l）
      gcc ... -Llibrary_path -llibrary_name ...
   

运行阶段：加载动态库（rpath）
      gcc ... -Wl,rpath=library_path ...
   

完整命令示例：
gcc main.c -I./include -L./lib -lmylib -Wl,rpath=./lib -o main

动态库制作流程

编译位置无关的目标文件
      gcc -c -fPIC source_file.c ...
   

链接生成动态库
      gcc -shared -o libfoo.so.x.y -Wl,-soname,libfoo.so.x ...
   

安装库文件
      cp libfoo.so.x.y /usr/lib64
   

创建符号链接
      ln -s libfoo.so.x.y libfoo.so.x
   ln -s libfoo.so.x libfoo.so

使用 Makefile 自动构建动态库

项目文件结构
变量   含义   示例值
CC   编译器   gcc

CFLAGS   编译选项   -Wall -fPIC

LIB_NAME   库名   mylib

LIB_VERSION   完整版本   1.0

LIB_SONAME_VERSION   soname 主版本   1

示例代码见： make_example.zip

实验内容

使用 Pthreads 构建并行 for 循环分解、分配、执行机制

模仿 OpenMP 的 omp_parallel_for 构造
生成包含 parallel_for 函数的动态链接库（.so）文件

parallel_for(int start, int end, int inc,
             void(functor)(int, void*), void *arg, int num_threads)

start, end, inc 分别为循环的开始、结束及索引自增量
functor 为函数指针，定义了每次循环所执行的内容
arg 为 functor 的参数指针，给出了 functor 执行所需的数据
num_threads 为期望产生的线程数量

对于以下 functor 及参数定义

struct functor_args {
    float *A, *B, *C;
};

void *functor(int idx, void *args) {
    functor_args *args_data = (functor_args *) args;
    args_data->C[idx] = args_data->A[idx] + args_data->B[idx];
}

functor_args args = {A, B, C};
parallel_for(0, 10, 1, functor, (void*)&args, 2)

如上代码应产生两个线程，对两个包含 10 个元素的数组并行求和
要求： 完成 parallel_for 函数实现，并以矩阵乘法为例进行验证

Pthreads 并行应用

使用自定义的 parallel_for 替代 heated_plate_openmp 中的并行构造
该应用模拟规则网格上的热传导，每次循环中对邻域内热量平均

T_{i,j}^{n+1} = frac{1}{4}(T_{i-1,j}^n + T_{i,j+1}^n + T_{i+1,j}^n + T_{i+1,j+1}^n)

测试不同线程、调度方式下的程序并行性能
与原始 heated_plate_openmp.c 实现进行对比
