cmake_minimum_required(VERSION 3.14)
project(ImageProcessingOptimization CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O0")  # 禁用编译器优化，突出手工优化效果

# 包含目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 基准版本库（未优化）
add_library(image_algorithms_baseline STATIC src/image_algorithms.cpp)

# 优化版本库（学生提交的优化代码）
add_library(image_algorithms_optimized STATIC src/image_algorithms_optimize.cpp)

# ============================================================================
# Google Benchmark 集成
# ============================================================================

include(FetchContent)

FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.3
)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark testing" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Disable gtest" FORCE)

FetchContent_MakeAvailable(benchmark)

# ============================================================================
# Benchmark 可执行文件
# ============================================================================

# 基准版本（-O0，无优化）
add_executable(image_benchmarks_baseline
    benchmarks/image_benchmarks.cpp
)
target_link_libraries(image_benchmarks_baseline
    image_algorithms_baseline
    benchmark::benchmark
)
target_compile_options(image_benchmarks_baseline PRIVATE -O0)

# 优化版本（使用学生的image_algorithms_optimize.cpp）
add_executable(image_benchmarks_optimized
    benchmarks/image_benchmarks.cpp
)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(image_algorithms_optimized PUBLIC OpenMP::OpenMP_CXX)
endif()
target_link_libraries(image_benchmarks_optimized
    image_algorithms_optimized
    benchmark::benchmark
)

# 根据架构选择优化选项
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    # x86/64 架构 - 支持 AVX2/SSE
    target_compile_options(image_benchmarks_optimized PRIVATE
        -O0 -march=native -mavx2 -mfma -fopenmp
    )
    target_link_options(image_benchmarks_optimized PRIVATE -fopenmp)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    # ARM64 架构 - 支持 NEON
    target_compile_options(image_benchmarks_optimized PRIVATE
        -O0 -march=native -fopenmp
    )
    target_link_options(image_benchmarks_optimized PRIVATE -fopenmp)
else()
    # 其他架构 - 仅使用 OpenMP
    target_compile_options(image_benchmarks_optimized PRIVATE
        -O0 -fopenmp
    )
    target_link_options(image_benchmarks_optimized PRIVATE -fopenmp)
endif()

# ============================================================================
# 自定义目标
# ============================================================================

# 运行基准测试
add_custom_target(run_baseline
    COMMAND image_benchmarks_baseline
    DEPENDS image_benchmarks_baseline
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running baseline benchmarks..."
)

add_custom_target(run_optimized
    COMMAND image_benchmarks_optimized
    DEPENDS image_benchmarks_optimized
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running optimized benchmarks..."
)

# 对比测试（生成JSON输出）
add_custom_target(compare
    COMMAND ${CMAKE_COMMAND} -E echo "Running baseline..."
    COMMAND image_benchmarks_baseline --benchmark_format=json --benchmark_out=baseline.json
    COMMAND ${CMAKE_COMMAND} -E echo "Running optimized..."
    COMMAND image_benchmarks_optimized --benchmark_format=json --benchmark_out=optimized.json
    COMMAND ${CMAKE_COMMAND} -E echo "Results saved to baseline.json and optimized.json"
    DEPENDS image_benchmarks_baseline image_benchmarks_optimized
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Comparing baseline vs optimized..."
)

# ============================================================================
# 安装
# ============================================================================

install(TARGETS image_benchmarks_baseline image_benchmarks_optimized
    RUNTIME DESTINATION bin
)

# ============================================================================
# 帮助信息
# ============================================================================

add_custom_target(show_help
    COMMAND ${CMAKE_COMMAND} -E echo "Available targets:"
    COMMAND ${CMAKE_COMMAND} -E echo "  all                  - Build all targets"
    COMMAND ${CMAKE_COMMAND} -E echo "  image_benchmarks_baseline  - Build baseline version"
    COMMAND ${CMAKE_COMMAND} -E echo "  image_benchmarks_optimized - Build optimized version"
    COMMAND ${CMAKE_COMMAND} -E echo "  run_baseline         - Run baseline benchmarks"
    COMMAND ${CMAKE_COMMAND} -E echo "  run_optimized        - Run optimized benchmarks"
    COMMAND ${CMAKE_COMMAND} -E echo "  compare              - Compare baseline vs optimized"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "Usage:"
    COMMAND ${CMAKE_COMMAND} -E echo "  mkdir build && cd build"
    COMMAND ${CMAKE_COMMAND} -E echo "  cmake .."
    COMMAND ${CMAKE_COMMAND} -E echo "  make image_benchmarks_baseline"
    COMMAND ${CMAKE_COMMAND} -E echo "  make run_baseline"
)