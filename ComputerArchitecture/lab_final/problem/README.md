# 图像处理算法优化实验

同学们可以在本文件中查看实验步骤和预期的输出效果，以及一些可能会遇到的问题。

## 快速开始

### 1. 构建项目

```bash
mkdir build && cd build
cmake ..
make
```

### 2. 运行基准测试

```bash
./image_benchmarks_baseline
```

输出说明：
- 显示每个算法的详细正确性验证结果
- 显示每个算法在不同图像尺寸下的性能测试（固定5次迭代）
- 便于调试和逐个优化算法

示例输出：
```
========================================
正确性验证
========================================

详细测试结果：
算法                   状态    信息
------------------------------------------------------------
GaussianFilter           ✓ PASS  PASS
PowerLawTransformation   ✓ PASS  PASS
...

总体结果: ✓ 所有测试通过
========================================

性能测试：
GaussianFilter_Small/iterations:5      57.6 ms
GaussianFilter_Medium/iterations:5     939 ms
...
```

### 3. 优化代码

**重要：只需修改 `src/image_algorithms_optimize.cpp` 文件。**

同学们需要对这个 cpp 文件进行算法的优化，这也是同学们最后需要提交的唯一源代码文件（当然还要提交实验报告）。

### 4. 运行优化测试

```bash
make image_benchmarks_optimized
./image_benchmarks_optimized
```

### 5. 对比性能

```bash
make compare
```

这将生成 `baseline.json` 和 `optimized.json` 两个文件，包含详细的性能数据。

## 目录结构

```
problem/
├── include/                         	# 头文件
│   └── image_algorithms.hpp         	# 算法接口定义
├── src/                             	# 源文件
│   ├── image_algorithms.cpp         	# 基准实现（不要修改）
│   └── image_algorithms_optimize.cpp 	# 优化实现（同学们进行修改并提交）
├── benchmarks/                      	# 性能测试
│   └── image_benchmarks.cpp         	# Google Benchmark 测试
├── CMakeLists.txt                   	# 构建配置
└── README.md                          	# 本文件
```

## 提交说明

提交压缩包 `[学号]-[姓名].zip`，包含：

1. `[学号]-[姓名].pdf` - 实验报告
2. `[(avx2|sse|neon)]-[(g++|clang++)]-[学号]-[姓名].cpp` - 优化后的代码（即`image_algorithms_optimize.cpp`）

详细要求请参考 PDF 文件 `2025秋计算机体系结构 大作业`。

## 优化提示

### 数据结构优化

基准实现使用连续内存存储图像数据，已经具有良好的缓存局部性。进一步优化可以考虑内存对齐。

### SIMD向量化

x86/64 平台可以使用AVX2指令集一次处理32字节数据：

```cpp
#include <immintrin.h>

__m256i data = _mm256_loadu_si256((__m256i*)&input[i]);
// 处理...
_mm256_storeu_si256((__m256i*)&output[i], result);
```

ARM64 平台可以使用NEON指令集：

```cpp
#include <arm_neon.h>

uint8x16_t data = vld1q_u8(&input[i]);
// 处理...
vst1q_u8(&output[i], result);
```

### 缓存优化

对于转置等操作，使用分块技术提高缓存命中率：

```cpp
const size_t BLOCK_SIZE = 64;
for (size_t i = 0; i < height; i += BLOCK_SIZE) {
    for (size_t j = 0; j < width; j += BLOCK_SIZE) {
        // 处理一个块
    }
}
```

### 多线程并行

使用 OpenMP 并行化循环：

```cpp
#pragma omp parallel for
for (size_t i = 0; i < height; ++i) {
    // 处理第i行
}
```

### 算法优化

对于幂次变换，使用查找表避免重复计算：

```cpp
unsigned char lut[256];
for (int i = 0; i < 256; ++i) {
    lut[i] = static_cast<unsigned char>(255 * std::pow(i / 255.0f, gamma));
}

// 使用查找表
for (size_t i = 0; i < size; ++i) {
    output[i] = lut[input[i]];
}
```

## 性能分析工具

### 使用 perf 分析

```bash
perf stat -e cache-misses,cache-references,instructions,cycles ./image_benchmarks_optimized
```

### 使用 valgrind 分析缓存

```bash
valgrind --tool=cachegrind ./image_benchmarks_optimized
```

## 常见问题

**Q: 编译时提示找不到 benchmark 库**

A: CMake 会自动下载 Google Benchmark。如果网络问题导致失败，可以手动安装：

```bash
# Ubuntu
sudo apt install libbenchmark-dev

# macOS
brew install google-benchmark
```

**Q: 如何只运行特定算法的测试？**

A: 使用 filter 参数：

```bash
./image_benchmarks_baseline --benchmark_filter=GaussianFilter
```

**Q: 如何调整测试迭代次数？**

A: 使用min_time参数：

```bash
./image_benchmarks_baseline --benchmark_min_time=5.0
```

## 技术支持

详细的实验要求和评分标准请参考 PDF 文件 `2025秋计算机体系结构 大作业`。

有问题欢迎同学们随时联系，在 QQ 群或私聊提问。

