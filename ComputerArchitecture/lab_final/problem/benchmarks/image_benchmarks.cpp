#include <benchmark/benchmark.h>
#include "image_algorithms.hpp"
#include <random>
#include <iostream>
#include <iomanip>

using namespace ImageAlgorithms;

// 测试图像尺寸
constexpr size_t SMALL_SIZE = 1024;
constexpr size_t MEDIUM_SIZE = 4096;
constexpr size_t LARGE_SIZE = 16384;

// 固定迭代次数
constexpr int FIXED_ITERATIONS = 5;

/**
 * @brief 生成随机图像
 */
Image generateRandomImage(size_t width, size_t height, unsigned int seed = 42) {
    Image img(width, height);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, 255);

    for (size_t i = 0; i < img.size(); ++i) {
        img.data()[i] = static_cast<unsigned char>(dis(gen));
    }

    return img;
}

/**
 * @brief 验证算法正确性
 */
struct CorrectnessResult {
    std::string algorithm;
    bool passed;
    std::string message;
};

std::vector<CorrectnessResult> verifyCorrectness() {
    std::vector<CorrectnessResult> results;
    const size_t test_size = 512;
    Image input = generateRandomImage(test_size, test_size, 2333);
    Image output(test_size, test_size);

    // 1. 测试高斯滤波
    try {
        gaussianFilter(input, output);
        // 基本检查：输出不应该全是0或全是255
        bool all_zero = true, all_max = true;
        for (size_t i = 0; i < output.size(); ++i) {
            if (output.data()[i] != 0) all_zero = false;
            if (output.data()[i] != 255) all_max = false;
        }
        bool passed = !all_zero && !all_max;
        results.push_back({"GaussianFilter", passed, passed ? "PASS" : "FAIL: Output is constant"});
    } catch (const std::exception& e) {
        results.push_back({"GaussianFilter", false, std::string("FAIL: Exception - ") + e.what()});
    }

    // 2. 测试幂次变换
    try {
        powerLawTransformation(input, output, 0.5f);
        bool passed = true;
        // 检查输出是否有效（不全为0，且有变化）
        bool all_zero = true;
        bool has_variation = false;
        uint8_t first_val = output.data()[0];
        for (size_t i = 0; i < output.size(); ++i) {
            if (output.data()[i] != 0) all_zero = false;
            if (output.data()[i] != first_val) has_variation = true;
        }
        passed = !all_zero && has_variation;
        results.push_back({"PowerLawTransformation", passed, passed ? "PASS" : "FAIL: Invalid output"});
    } catch (const std::exception& e) {
        results.push_back({"PowerLawTransformation", false, std::string("FAIL: Exception - ") + e.what()});
    }

    // 3. 测试Sobel边缘检测
    try {
        sobelEdgeDetection(input, output);
        bool all_zero = true;
        for (size_t i = 0; i < output.size(); ++i) {
            if (output.data()[i] != 0) {
                all_zero = false;
                break;
            }
        }
        bool passed = !all_zero;
        results.push_back({"SobelEdgeDetection", passed, passed ? "PASS" : "FAIL: Output is all zeros"});
    } catch (const std::exception& e) {
        results.push_back({"SobelEdgeDetection", false, std::string("FAIL: Exception - ") + e.what()});
    }

    // 4. 测试转置
    try {
        Image transposed(test_size, test_size);
        transpose(input, transposed);
        // 验证转置：检查几个点
        bool passed = true;
        for (size_t i = 0; i < 10; ++i) {
            for (size_t j = 0; j < 10; ++j) {
                if (input.at(i, j) != transposed.at(j, i)) {
                    passed = false;
                    break;
                }
            }
            if (!passed) break;
        }
        results.push_back({"Transpose", passed, passed ? "PASS" : "FAIL: Transpose verification failed"});
    } catch (const std::exception& e) {
        results.push_back({"Transpose", false, std::string("FAIL: Exception - ") + e.what()});
    }

    // 5. 测试均值滤波
    try {
        boxFilter(input, output, 5);
        bool all_zero = true, all_max = true;
        for (size_t i = 0; i < output.size(); ++i) {
            if (output.data()[i] != 0) all_zero = false;
            if (output.data()[i] != 255) all_max = false;
        }
        bool passed = !all_zero && !all_max;
        results.push_back({"BoxFilter", passed, passed ? "PASS" : "FAIL: Output is constant"});
    } catch (const std::exception& e) {
        results.push_back({"BoxFilter", false, std::string("FAIL: Exception - ") + e.what()});
    }

    // 6. 测试旋转
    try {
        Image rotated(test_size, test_size);
        rotate90Clockwise(input, rotated);
        // 验证旋转：检查尺寸和几个点
        bool passed = (rotated.width() == test_size && rotated.height() == test_size);
        if (passed) {
            // 检查角点
            passed = (input.at(0, 0) == rotated.at(0, test_size - 1));
        }
        results.push_back({"Rotate90Clockwise", passed, passed ? "PASS" : "FAIL: Rotation verification failed"});
    } catch (const std::exception& e) {
        results.push_back({"Rotate90Clockwise", false, std::string("FAIL: Exception - ") + e.what()});
    }

    return results;
}

// ============================================================================
// Benchmark Fixtures
// ============================================================================

template<size_t SIZE>
class ImageFixture : public benchmark::Fixture {
public:
    Image input;
    Image output;

    void SetUp(const ::benchmark::State&) override {
        input = generateRandomImage(SIZE, SIZE, 2333);
        output.resize(SIZE, SIZE);
    }
};

// ============================================================================
// 详细的分部测试（所有测试固定5次迭代）
// ============================================================================

// ============================================================================
// Gaussian Filter Benchmarks
// ============================================================================

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, GaussianFilter_Small, SMALL_SIZE)
(benchmark::State& state) {
    for (auto _ : state) {
        gaussianFilter(input, output);
        benchmark::DoNotOptimize(output.data());
    }
    state.SetItemsProcessed(state.iterations() * SMALL_SIZE * SMALL_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, GaussianFilter_Small)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, GaussianFilter_Medium, MEDIUM_SIZE)
(benchmark::State& state) {
    for (auto _ : state) {
        gaussianFilter(input, output);
        benchmark::DoNotOptimize(output.data());
    }
    state.SetItemsProcessed(state.iterations() * MEDIUM_SIZE * MEDIUM_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, GaussianFilter_Medium)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, GaussianFilter_Large, LARGE_SIZE)
(benchmark::State& state) {
    for (auto _ : state) {
        gaussianFilter(input, output);
        benchmark::DoNotOptimize(output.data());
    }
    state.SetItemsProcessed(state.iterations() * LARGE_SIZE * LARGE_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, GaussianFilter_Large)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

// ============================================================================
// Power Law Transformation Benchmarks
// ============================================================================

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, PowerLaw_Small, SMALL_SIZE)
(benchmark::State& state) {
    for (auto _ : state) {
        powerLawTransformation(input, output, 0.5f);
        benchmark::DoNotOptimize(output.data());
    }
    state.SetItemsProcessed(state.iterations() * SMALL_SIZE * SMALL_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, PowerLaw_Small)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, PowerLaw_Medium, MEDIUM_SIZE)
(benchmark::State& state) {
    for (auto _ : state) {
        powerLawTransformation(input, output, 0.5f);
        benchmark::DoNotOptimize(output.data());
    }
    state.SetItemsProcessed(state.iterations() * MEDIUM_SIZE * MEDIUM_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, PowerLaw_Medium)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, PowerLaw_Large, LARGE_SIZE)
(benchmark::State& state) {
    for (auto _ : state) {
        powerLawTransformation(input, output, 0.5f);
        benchmark::DoNotOptimize(output.data());
    }
    state.SetItemsProcessed(state.iterations() * LARGE_SIZE * LARGE_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, PowerLaw_Large)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

// ============================================================================
// Sobel Edge Detection Benchmarks
// ============================================================================

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, Sobel_Small, SMALL_SIZE)
(benchmark::State& state) {
    for (auto _ : state) {
        sobelEdgeDetection(input, output);
        benchmark::DoNotOptimize(output.data());
    }
    state.SetItemsProcessed(state.iterations() * SMALL_SIZE * SMALL_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, Sobel_Small)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, Sobel_Medium, MEDIUM_SIZE)
(benchmark::State& state) {
    for (auto _ : state) {
        sobelEdgeDetection(input, output);
        benchmark::DoNotOptimize(output.data());
    }
    state.SetItemsProcessed(state.iterations() * MEDIUM_SIZE * MEDIUM_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, Sobel_Medium)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, Sobel_Large, LARGE_SIZE)
(benchmark::State& state) {
    for (auto _ : state) {
        sobelEdgeDetection(input, output);
        benchmark::DoNotOptimize(output.data());
    }
    state.SetItemsProcessed(state.iterations() * LARGE_SIZE * LARGE_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, Sobel_Large)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

// ============================================================================
// Transpose Benchmarks
// ============================================================================

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, Transpose_Small, SMALL_SIZE)
(benchmark::State& state) {
    Image transposed(SMALL_SIZE, SMALL_SIZE);
    for (auto _ : state) {
        transpose(input, transposed);
        benchmark::DoNotOptimize(transposed.data());
    }
    state.SetItemsProcessed(state.iterations() * SMALL_SIZE * SMALL_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, Transpose_Small)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, Transpose_Medium, MEDIUM_SIZE)
(benchmark::State& state) {
    Image transposed(MEDIUM_SIZE, MEDIUM_SIZE);
    for (auto _ : state) {
        transpose(input, transposed);
        benchmark::DoNotOptimize(transposed.data());
    }
    state.SetItemsProcessed(state.iterations() * MEDIUM_SIZE * MEDIUM_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, Transpose_Medium)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, Transpose_Large, LARGE_SIZE)
(benchmark::State& state) {
    Image transposed(LARGE_SIZE, LARGE_SIZE);
    for (auto _ : state) {
        transpose(input, transposed);
        benchmark::DoNotOptimize(transposed.data());
    }
    state.SetItemsProcessed(state.iterations() * LARGE_SIZE * LARGE_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, Transpose_Large)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

// ============================================================================
// Box Filter Benchmarks
// ============================================================================

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, BoxFilter_Small, SMALL_SIZE)
(benchmark::State& state) {
    for (auto _ : state) {
        boxFilter(input, output, 5);
        benchmark::DoNotOptimize(output.data());
    }
    state.SetItemsProcessed(state.iterations() * SMALL_SIZE * SMALL_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, BoxFilter_Small)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, BoxFilter_Medium, MEDIUM_SIZE)
(benchmark::State& state) {
    for (auto _ : state) {
        boxFilter(input, output, 5);
        benchmark::DoNotOptimize(output.data());
    }
    state.SetItemsProcessed(state.iterations() * MEDIUM_SIZE * MEDIUM_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, BoxFilter_Medium)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, BoxFilter_Large, LARGE_SIZE)
(benchmark::State& state) {
    for (auto _ : state) {
        boxFilter(input, output, 5);
        benchmark::DoNotOptimize(output.data());
    }
    state.SetItemsProcessed(state.iterations() * LARGE_SIZE * LARGE_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, BoxFilter_Large)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

// ============================================================================
// Rotate90 Benchmarks
// ============================================================================

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, Rotate90_Small, SMALL_SIZE)
(benchmark::State& state) {
    Image rotated(SMALL_SIZE, SMALL_SIZE);
    for (auto _ : state) {
        rotate90Clockwise(input, rotated);
        benchmark::DoNotOptimize(rotated.data());
    }
    state.SetItemsProcessed(state.iterations() * SMALL_SIZE * SMALL_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, Rotate90_Small)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, Rotate90_Medium, MEDIUM_SIZE)
(benchmark::State& state) {
    Image rotated(MEDIUM_SIZE, MEDIUM_SIZE);
    for (auto _ : state) {
        rotate90Clockwise(input, rotated);
        benchmark::DoNotOptimize(rotated.data());
    }
    state.SetItemsProcessed(state.iterations() * MEDIUM_SIZE * MEDIUM_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, Rotate90_Medium)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

BENCHMARK_TEMPLATE_DEFINE_F(ImageFixture, Rotate90_Large, LARGE_SIZE)
(benchmark::State& state) {
    Image rotated(LARGE_SIZE, LARGE_SIZE);
    for (auto _ : state) {
        rotate90Clockwise(input, rotated);
        benchmark::DoNotOptimize(rotated.data());
    }
    state.SetItemsProcessed(state.iterations() * LARGE_SIZE * LARGE_SIZE);
}
BENCHMARK_REGISTER_F(ImageFixture, Rotate90_Large)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(FIXED_ITERATIONS);

// ============================================================================
// 主函数：运行benchmark并显示正确性验证结果
// ============================================================================

int main(int argc, char** argv) {
    // 运行正确性验证
    std::cout << "\n========================================\n";
    std::cout << "正确性验证\n";
    std::cout << "========================================\n";

    auto correctness_results = verifyCorrectness();

    // 显示每个算法的详细结果
    std::cout << "\n详细测试结果：\n";
    std::cout << std::left << std::setw(25) << "算法"
              << std::setw(10) << "状态"
              << "信息\n";
    std::cout << std::string(60, '-') << "\n";

    bool all_passed = true;
    for (const auto& result : correctness_results) {
        std::cout << std::left << std::setw(25) << result.algorithm
                  << std::setw(10) << (result.passed ? "✓ PASS" : "✗ FAIL")
                  << result.message << "\n";
        if (!result.passed) all_passed = false;
    }

    std::cout << "\n总体结果: " << (all_passed ? "✓ 所有测试通过" : "✗ 存在失败的测试") << "\n";
    std::cout << "========================================\n\n";

    // 运行性能测试
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();

    return all_passed ? 0 : 1;
}

