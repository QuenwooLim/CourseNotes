# 并行程序设计与算法实验
## 6-Pthreads并行构造

### 1. 构造基于Pthreads的并行for循环分解、分配、执行机制
模仿OpenMP的omp_parallel_for构造基于Pthreads的并行for循环分解、分配及执行机制。此内容延续上次实验，在本次实验中完成。

**问题描述：** 生成一个包含parallel_for函数的动态链接库（.so）文件，该函数创建多个Pthreads线程，并行执行parallel_for函数的参数所指定的内容。

**函数参数：** parallel_for函数的参数应当指明被并行循环的索引信息，循环中所需要执行的内容，并行构造等。以下为parallel_for函数的基础定义，实验实现应包括但不限于以下内容：

```c
parallel_for(int start, int end, int inc,
             void *(*functor)(int, void*), void *arg, int num_threads)
```

1. start, end, inc 分别为循环的开始、结束及索引自增量；
2. functor 为函数指针，定义了每次循环所执行的内容；
3. arg 为functor的参数指针，给出了functor执行所需的数据；
4. num_threads 为期望产生的线程数量。
5. 选做：除上述内容外，还可以考虑调度方式等额外参数。

**示例：** 给定functor及参数如下：

```c
struct functor_args {
    float *A, *B, *C;
};

void *functor(int idx, void* args){
    functor_args *args_data = (functor_args*) args;
    args_data->C[idx] = args_data->A[idx] + args_data->B[idx];
}
```

调用方式如下：

```c
functor_args args = {A, B, C};
parallel_for(0, 10, 1, functor, (void*)&args, 2)
```

该调用方式应当能产生两个线程，并行执行functor完成数组求和（）。当不考虑调度方式时，可由前一个线程执行任务{0,1,2,3,4}，后一个线程执行任务{5,6,7,8,9}。也可以实现对调度方式的定义。

**要求：** 完成parallel_for函数实现并生成动态链接库文件，并以矩阵乘法为例，测试其实现的正确性及效率。

### 2. parallel_for并行应用
使用此前构造的parallel_for并行结构，将heated_plate_openmp改造为基于Pthreads的并行应用。

**heated plate问题描述：** 规则网格上的热传导模拟，其具体过程为每次循环中通过对邻域内热量平均模拟热传导过程，即：
，
其OpenMP实现见课程资料中的heated_plate_openmp.c。

**要求：** 使用此前构造的parallel_for并行结构，将heated_plate_openmp实现改造为基于Pthreads的并行应用。测试不同线程、调度方式下的程序并行性能，并与原始heated_plate_openmp.c实现对比。

### 附录1：环境配置及软件安装
本实验提供两种环境配置方式，你可以根据个人需求选择：

**方式1：本地虚拟机环境安装**
1. 下载安装虚拟机软件（推荐VMware或VirtualBox）
   - VMware: https://www.vmware.com/products/desktop-hypervisor/workstation-and-fusion
   - VirtualBox: https://www.virtualbox.org/wiki/Downloads
2. 下载Ubuntu 22.04镜像文件
   - 官方: http://releases.ubuntu.com/22.04/
   - 国内: https://mirrors.sustech.edu.cn/ubuntu-releases/22.04/
3. 在VMware或VirtualBox中创建虚拟机实例，并调整其资源，如CPU核、内存、硬盘容量等。
4. 在虚拟机实例中的虚拟光驱中加载Ubuntu镜像文件(.iso文件)，并安装Ubuntu操作系统。
5. 软件安装
   Pthreads是Linux/glibc自带线程库，不需要单独安装开发包：
   ```bash
   sudo apt-get update
   sudo apt-get install build-essential git
   ```

**方式2：使用智算习堂实验环境**
使用"并行编程Web-IDE"容器，其中预装了GCC、Make等C语言编译工具。Pthreads为Linux系统自带线程库，编译时添加-pthread选项即可。

可通过以下指令验证编译工具:
- 检查编译器：`gcc --version`
- 检查构建工具：`make --version`
- 检查Pthread可用性：
  ```bash
  cat > check_pthread.c << 'EOF'
  #include <pthread.h>
  int main() { return 0; }
  EOF
  gcc -std=c11 -pthread check_pthread.c -o check_pthread
  ./check_pthread && echo Pthreads 环境可用
  ```

### 附录2：示例程序编译与运行验证
本次实验示例代码包含了make_example目录（动态库构建示例）和heated_plate_openmp目录（OpenMP并行示例）。

**编译运行动态库示例程序时，可使用以下两种方式：**

**方式一：手动编译与运行**
```bash
# 进入示例代码目录
cd make_example
# 第一步：编译目标文件（使用 -fPIC 生成位置无关代码）
gcc -fPIC -c mylib.c
# 第二步：创建动态库（使用 -shared）
gcc -shared -Wl,-soname,libmylib.so.1 -o libmylib.so.1.0 mylib.o
# 第三步：创建符号链接（便于库版本管理）
ln -sf libmylib.so.1.0 libmylib.so.1
ln -sf libmylib.so.1 libmylib.so
# 第四步：编译主程序并动态链接（-L. 指定当前目录寻找库，-lmylib 指定库名，-Wl,-rpath 指定运行时库路径）
gcc main.c -L. -lmylib -Wl,-rpath,. -o main
# 第五步：运行程序
./main
```

**方式二：使用Makefile**
```bash
# 进入示例代码目录
cd make_example
# 第一步：编译目标文件
make
# 第二步：运行程序
./main
```

**通过以下指令编译运行OpenMP的热传导方程求解示例程序：**
```bash
# 进入示例代码目录
cd heated_plate_openmp
# 编译器gcc使用-fopenmp选项启用OpenMP支持，并链接数学库-lm
gcc -fopenmp -O2 heated_plate_openmp.c -o heated_plate_openmp -lm
# 启动OpenMP程序运行
./heated_plate_openmp
```

其中：
- `-fopenmp` 用于开启编译器的OpenMP并行指令支持，
- `-lm` 用于链接包含绝对方差运算等所需的数学库，
- `-fPIC` 与 `-shared` 结合使用是构建自定义动态链接库的必备选项。