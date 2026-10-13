#include "image_algorithms.hpp"
#include <cmath>
#include <immintrin.h> // SIMD
#include <algorithm>
#include <omp.h>       // OpenMP
#pragma GCC target("avx2")  //开启 AVX2 指令集支持
#include <cstdio>
namespace ImageAlgorithms {

/**
 * @brief 高斯滤波 - 3x3卷积核
 * 
 * 卷积核:
 *   1  2  1
 *   2  4  2
 *   1  2  1
 * 除以16进行归一化
 */
void gaussianFilter(const Image& input, Image& output) {
    const size_t width = input.width();
    const size_t height = input.height();
    const unsigned char* in = input.data();
    output.resize(width, height);
    unsigned char* out = output.data();

    // 处理内部像素，SIMD一次处理16个像素（AVX2）
    for (size_t i = 1; i < height - 1; ++i) {
        size_t j = 1;
        for (; j + 15 < width - 1; j += 16) {
            const unsigned char* row0 = in + (i - 1) * width;
            const unsigned char* row1 = in + i * width;
            const unsigned char* row2 = in + (i + 1) * width;
            unsigned char* out_row = out + i * width;

            // 加载相邻行的数据
            __m128i r0_l = _mm_loadu_si128((__m128i*)(row0 + j - 1));
            __m128i r0_m = _mm_loadu_si128((__m128i*)(row0 + j));
            __m128i r0_r = _mm_loadu_si128((__m128i*)(row0 + j + 1));
            __m128i r1_l = _mm_loadu_si128((__m128i*)(row1 + j - 1));
            __m128i r1_m = _mm_loadu_si128((__m128i*)(row1 + j));
            __m128i r1_r = _mm_loadu_si128((__m128i*)(row1 + j + 1));
            __m128i r2_l = _mm_loadu_si128((__m128i*)(row2 + j - 1));
            __m128i r2_m = _mm_loadu_si128((__m128i*)(row2 + j));
            __m128i r2_r = _mm_loadu_si128((__m128i*)(row2 + j + 1));

            // 转为16位，避免溢出
            __m256i r0_l_16 = _mm256_cvtepu8_epi16(r0_l);
            __m256i r0_m_16 = _mm256_cvtepu8_epi16(r0_m);
            __m256i r0_r_16 = _mm256_cvtepu8_epi16(r0_r);
            __m256i r1_l_16 = _mm256_cvtepu8_epi16(r1_l);
            __m256i r1_m_16 = _mm256_cvtepu8_epi16(r1_m);
            __m256i r1_r_16 = _mm256_cvtepu8_epi16(r1_r);
            __m256i r2_l_16 = _mm256_cvtepu8_epi16(r2_l);
            __m256i r2_m_16 = _mm256_cvtepu8_epi16(r2_m);
            __m256i r2_r_16 = _mm256_cvtepu8_epi16(r2_r);

            // sum = r0_l + 2*r0_m + r0_r + 2*r1_l + 4*r1_m + 2*r1_r + r2_l + 2*r2_m + r2_r
            __m256i sum = _mm256_add_epi16(r0_l_16, r0_r_16);
            sum = _mm256_add_epi16(sum, r2_l_16);
            sum = _mm256_add_epi16(sum, r2_r_16);

            __m256i tmp2 = _mm256_slli_epi16(r0_m_16, 1); // 2*r0_m
            sum = _mm256_add_epi16(sum, tmp2);
            tmp2 = _mm256_slli_epi16(r1_l_16, 1); // 2*r1_l
            sum = _mm256_add_epi16(sum, tmp2);
            tmp2 = _mm256_slli_epi16(r1_r_16, 1); // 2*r1_r
            sum = _mm256_add_epi16(sum, tmp2);
            tmp2 = _mm256_slli_epi16(r2_m_16, 1); // 2*r2_m
            sum = _mm256_add_epi16(sum, tmp2);

            tmp2 = _mm256_slli_epi16(r1_m_16, 2); // 4*r1_m
            sum = _mm256_add_epi16(sum, tmp2);

            // 除以16（右移4位）
            sum = _mm256_srli_epi16(sum, 4);

            // 压缩回8位
            __m128i result = _mm_packus_epi16(_mm256_castsi256_si128(sum), _mm256_extracti128_si256(sum, 1));
            _mm_storeu_si128((__m128i*)(out_row + j), result);
        }
        // 处理剩余像素
        for (; j < width - 1; ++j) {
            const unsigned char* row0 = in + (i - 1) * width;
            const unsigned char* row1 = in + i * width;
            const unsigned char* row2 = in + (i + 1) * width;
            int sum =
                row0[j - 1] + 2 * row0[j] + row0[j + 1] +
                2 * row1[j - 1] + 4 * row1[j] + 2 * row1[j + 1] +
                row2[j - 1] + 2 * row2[j] + row2[j + 1];
            out[i * width + j] = static_cast<unsigned char>(sum / 16);
        }
    }

    // 处理边界（简单复制）
    for (size_t j = 0; j < width; ++j) {
        out[j] = in[j];
        out[(height - 1) * width + j] = in[(height - 1) * width + j];
    }
    for (size_t i = 0; i < height; ++i) {
        out[i * width] = in[i * width];
        out[i * width + (width - 1)] = in[i * width + (width - 1)];
    }

    // 校验和输出
    unsigned int checksum = calcChecksum(output);
    printf("[gaussianFilter] optimized checksum: %u\n", checksum);
}

/**
 * @brief 幂次变换 - 对比度调整
 * 
 * 公式: output = 255 * (input/255)^gamma
 */

void powerLawTransformation(const Image& input, Image& output, float gamma) {
    const size_t size = input.size();
    const unsigned char* __restrict in = input.data();
    output.resize(input.width(), input.height());
    unsigned char* __restrict out = output.data();

    // 预计算 LUT（完全在 L1 cache）
    alignas(64) unsigned char lut[256];
    const float inv255 = 1.0f / 255.0f;
    for (int i = 0; i < 256; ++i) {
        float x = i * inv255;
        lut[i] = static_cast<unsigned char>(
            std::pow(x, gamma) * 255.0f + 0.5f
        );
    }

    // 主循环：顺序访问 + 编译器自动向量化
    size_t i = 0;

    // 手动展开，降低 loop overhead
    for (; i + 7 < size; i += 8) {
        out[i + 0] = lut[in[i + 0]];
        out[i + 1] = lut[in[i + 1]];
        out[i + 2] = lut[in[i + 2]];
        out[i + 3] = lut[in[i + 3]];
        out[i + 4] = lut[in[i + 4]];
        out[i + 5] = lut[in[i + 5]];
        out[i + 6] = lut[in[i + 6]];
        out[i + 7] = lut[in[i + 7]];
    }

    // 尾部
    for (; i < size; ++i) {
        out[i] = lut[in[i]];
    }

    // 校验和输出
    unsigned int checksum = calcChecksum(output);
    printf("[powerLawTransformation] optimized checksum: %u\n", checksum);
}


/**
 * @brief Sobel边缘检测
 * 
 * Gx = [-1 0 1]    Gy = [-1 -2 -1]
 *      [-2 0 2]         [ 0  0  0]
 *      [-1 0 1]         [ 1  2  1]
 */
static int SOBEL_SQ[2041];
static unsigned char SOBEL_MAG[2 * 1020 * 1020 + 1];
static bool LUT_INIT = false;

static inline void initSobelLUT() {
    if (LUT_INIT) return;

    for (int i = -1020; i <= 1020; ++i)
        SOBEL_SQ[i + 1020] = i * i;

    const int maxSum = 2 * 1020 * 1020;
    for (int s = 0; s <= maxSum; ++s)
        SOBEL_MAG[s] =
            static_cast<unsigned char>(std::min((int)std::sqrt(s), 255));

    LUT_INIT = true;
}
static inline void sobelRowAVX2(
    const unsigned char* __restrict__ r0,
    const unsigned char* __restrict__ r1,
    const unsigned char* __restrict__ r2,
    unsigned char* __restrict__ out,
    size_t w
) 
{
    size_t j = 1;

#if defined(__AVX2__)
    __m256i zero = _mm256_setzero_si256();

    for (; j + 33 < w; j += 32) {
        __m256i r0l = _mm256_loadu_si256((__m256i*)(r0 + j - 1));
        __m256i r0m = _mm256_loadu_si256((__m256i*)(r0 + j));
        __m256i r0r = _mm256_loadu_si256((__m256i*)(r0 + j + 1));

        __m256i r1l = _mm256_loadu_si256((__m256i*)(r1 + j - 1));
        __m256i r1r = _mm256_loadu_si256((__m256i*)(r1 + j + 1));

        __m256i r2l = _mm256_loadu_si256((__m256i*)(r2 + j - 1));
        __m256i r2m = _mm256_loadu_si256((__m256i*)(r2 + j));
        __m256i r2r = _mm256_loadu_si256((__m256i*)(r2 + j + 1));

        __m256i gx =
            _mm256_add_epi16(
                _mm256_add_epi16(
                    _mm256_sub_epi16(_mm256_unpacklo_epi8(r0r, zero),
                                     _mm256_unpacklo_epi8(r0l, zero)),
                    _mm256_slli_epi16(
                        _mm256_sub_epi16(_mm256_unpacklo_epi8(r1r, zero),
                                         _mm256_unpacklo_epi8(r1l, zero)), 1)),
                _mm256_sub_epi16(_mm256_unpacklo_epi8(r2r, zero),
                                 _mm256_unpacklo_epi8(r2l, zero)));

        __m256i gy =
            _mm256_add_epi16(
                _mm256_sub_epi16(_mm256_unpacklo_epi8(r2l, zero),
                                 _mm256_unpacklo_epi8(r0l, zero)),
                _mm256_add_epi16(
                    _mm256_slli_epi16(
                        _mm256_sub_epi16(_mm256_unpacklo_epi8(r2m, zero),
                                         _mm256_unpacklo_epi8(r0m, zero)), 1),
                    _mm256_sub_epi16(_mm256_unpacklo_epi8(r2r, zero),
                                     _mm256_unpacklo_epi8(r0r, zero))));

        alignas(32) short gxv[16], gyv[16];
        _mm256_store_si256((__m256i*)gxv, gx);
        _mm256_store_si256((__m256i*)gyv, gy);

        #pragma unroll
        for (int k = 0; k < 16; ++k) {
            out[j + k] =
                SOBEL_MAG[
                    SOBEL_SQ[gxv[k] + 1020] +
                    SOBEL_SQ[gyv[k] + 1020]];
        }
    }
#endif

    // scalar 尾
    for (; j < w - 1; ++j) {
        int gx =
            -r0[j - 1] + r0[j + 1]
            -2 * r1[j - 1] + 2 * r1[j + 1]
            -r2[j - 1] + r2[j + 1];

        int gy =
            -r0[j - 1] - 2 * r0[j] - r0[j + 1]
            + r2[j - 1] + 2 * r2[j] + r2[j + 1];

        out[j] = SOBEL_MAG[
            SOBEL_SQ[gx + 1020] +
            SOBEL_SQ[gy + 1020]];
    }
}
void sobelEdgeDetection(const Image& input, Image& output) {
    initSobelLUT();

    const size_t w = input.width();
    const size_t h = input.height();

    output.resize(w, h);

    const unsigned char* __restrict__ in = input.data();
    unsigned char* __restrict__ out = output.data();

    const bool useOMP = (w * h >= 96 * 96);
    const size_t BLOCK = 16;

#pragma omp parallel for if(useOMP) schedule(static)
    for (size_t bi = 1; bi < h - 1; bi += BLOCK) {
        size_t end = std::min(bi + BLOCK, h - 1);

        for (size_t i = bi; i < end; ++i) {
            __builtin_prefetch(in + (i + 2) * w, 0, 1);
            sobelRowAVX2(
                in + (i - 1) * w,
                in + i * w,
                in + (i + 1) * w,
                out + i * w,
                w
            );
        }
    }

    // 边界完全一致
    std::fill(out, out + w, 0);
    std::fill(out + (h - 1) * w, out + h * w, 0);
    for (size_t i = 0; i < h; ++i)
        out[i * w] = out[i * w + w - 1] = 0;

    unsigned int checksum = calcChecksum(output);
    printf("[sobelEdgeDetection] optimized checksum: %u\n", checksum);
}



/**
 * @brief 图像转置
 */
void transpose(const Image& input, Image& output) {
    const size_t width  = input.width();
    const size_t height = input.height();

    const unsigned char* __restrict in  = input.data();
    output.resize(height, width);
    unsigned char* __restrict out = output.data();

    const size_t out_w = height;

    // 小/中尺寸：标量更快
    if (width * height <= 512 * 512) {
        for (size_t j = 0; j < width; ++j)
            for (size_t i = 0; i < height; ++i)
                out[j * out_w + i] = in[i * width + j];
        // 校验和输出
        unsigned int checksum = calcChecksum(output);
        printf("[transpose] optimized checksum: %u\n", checksum);
        return;
    }

    // 大尺寸：SIMD + blocking
    const size_t block = 32;

    for (size_t i = 0; i < height; i += block) {
        for (size_t j = 0; j < width; j += block) {

            size_t i_max = std::min(i + block, height);
            size_t j_max = std::min(j + block, width);

            size_t ii = i;
            for (; ii + 7 < i_max; ii += 8) {
                size_t jj = j;
                for (; jj + 7 < j_max; jj += 8) {
                    const unsigned char* src = in + ii * width + jj;
                    __m128i r0 = _mm_loadl_epi64((const __m128i*)(src + 0 * width));
                    __m128i r1 = _mm_loadl_epi64((const __m128i*)(src + 1 * width));
                    __m128i r2 = _mm_loadl_epi64((const __m128i*)(src + 2 * width));
                    __m128i r3 = _mm_loadl_epi64((const __m128i*)(src + 3 * width));
                    __m128i r4 = _mm_loadl_epi64((const __m128i*)(src + 4 * width));
                    __m128i r5 = _mm_loadl_epi64((const __m128i*)(src + 5 * width));
                    __m128i r6 = _mm_loadl_epi64((const __m128i*)(src + 6 * width));
                    __m128i r7 = _mm_loadl_epi64((const __m128i*)(src + 7 * width));
                    __m128i t0 = _mm_unpacklo_epi8(r0, r1);
                    __m128i t1 = _mm_unpacklo_epi8(r2, r3);
                    __m128i t2 = _mm_unpacklo_epi8(r4, r5);
                    __m128i t3 = _mm_unpacklo_epi8(r6, r7);
                    __m128i s0 = _mm_unpacklo_epi16(t0, t1);
                    __m128i s1 = _mm_unpackhi_epi16(t0, t1);
                    __m128i s2 = _mm_unpacklo_epi16(t2, t3);
                    __m128i s3 = _mm_unpackhi_epi16(t2, t3);
                    __m128i f0 = _mm_unpacklo_epi32(s0, s2);
                    __m128i f1 = _mm_unpackhi_epi32(s0, s2);
                    __m128i f2 = _mm_unpacklo_epi32(s1, s3);
                    __m128i f3 = _mm_unpackhi_epi32(s1, s3);
                    unsigned char* dst = out + jj * out_w + ii;
                    _mm_storel_epi64((__m128i*)(dst + 0 * out_w), f0);
                    _mm_storel_epi64((__m128i*)(dst + 1 * out_w), _mm_srli_si128(f0, 8));
                    _mm_storel_epi64((__m128i*)(dst + 2 * out_w), f1);
                    _mm_storel_epi64((__m128i*)(dst + 3 * out_w), _mm_srli_si128(f1, 8));
                    _mm_storel_epi64((__m128i*)(dst + 4 * out_w), f2);
                    _mm_storel_epi64((__m128i*)(dst + 5 * out_w), _mm_srli_si128(f2, 8));
                    _mm_storel_epi64((__m128i*)(dst + 6 * out_w), f3);
                    _mm_storel_epi64((__m128i*)(dst + 7 * out_w), _mm_srli_si128(f3, 8));
                }
                for (; jj < j_max; ++jj)
                    for (size_t k = ii; k < ii + 8; ++k)
                        out[jj * out_w + k] = in[k * width + jj];
            }
            for (; ii < i_max; ++ii)
                for (size_t jj = j; jj < j_max; ++jj)
                    out[jj * out_w + ii] = in[ii * width + jj];
        }
    }
    // 校验和输出
    unsigned int checksum = calcChecksum(output);
    printf("[transpose] optimized checksum: %u\n", checksum);
}


/**
 * @brief 均值滤波
 */
void boxFilter(const Image& input, Image& output, int kernel_size) {
    const size_t width = input.width();
    const size_t height = input.height();
    const int radius = kernel_size / 2;
    output.resize(width, height);

    // 构建积分图 (优化：使用指针访问，减少索引计算)
    const size_t int_width = width + 1;
    std::vector<int> integral((height + 1) * int_width, 0);
    
    const unsigned char* in_data = input.data();
    int* int_data = integral.data();

    for (size_t i = 1; i <= height; ++i) {
        int row_sum = 0;
        const unsigned char* row_in = in_data + (i - 1) * width;
        int* row_curr = int_data + i * int_width;
        int* row_prev = int_data + (i - 1) * int_width;
        
        for (size_t j = 1; j <= width; ++j) {
            row_sum += row_in[j - 1];
            row_curr[j] = row_prev[j] + row_sum;
        }
    }

    unsigned char* out_data = output.data();

    // 均值滤波 (优化：SIMD处理中心区域，浮点乘法代替整数除法)
    for (size_t i = 0; i < height; ++i) {
        int top = std::max<int>(static_cast<int>(i) - radius, 0);
        int bottom = std::min<int>(static_cast<int>(i) + radius, static_cast<int>(height) - 1);
        int h = bottom - top + 1;
        // 积分图行指针
        const int* row_top = int_data + top * int_width;
        const int* row_bot = int_data + (bottom + 1) * int_width;
        unsigned char* row_out = out_data + i * width;
        size_t j = 0;
        // 左边界处理 (Scalar)
        size_t limit_left = std::min(width, static_cast<size_t>(radius));
        for (; j < limit_left; ++j) {
            int left = 0;
            int right = std::min<int>(static_cast<int>(j) + radius, static_cast<int>(width) - 1);
            int w = right - left + 1;
            int area = h * w;
            int sum = row_bot[right + 1] - row_top[right + 1] - row_bot[left] + row_top[left];
            row_out[j] = static_cast<unsigned char>(sum / area);
        }
        // 中心区域 (SIMD) 条件：j - radius >= 0 且 j + radius < width
        if (width > static_cast<size_t>(2 * radius)) {
            size_t limit_simd = width - radius;
            int w = 2 * radius + 1;
            float area_inv = 1.0f / (h * w);
            __m256 v_area_inv = _mm256_set1_ps(area_inv);
            // 积分图指针偏移
            const int* p_bot_r = row_bot + j + radius + 1;
            const int* p_top_r = row_top + j + radius + 1;
            const int* p_bot_l = row_bot + j - radius;
            const int* p_top_l = row_top + j - radius;
            for (; j + 7 < limit_simd; j += 8) {
                // 加载数据 (连续内存加载)
                __m256i br = _mm256_loadu_si256((const __m256i*)p_bot_r);
                __m256i tr = _mm256_loadu_si256((const __m256i*)p_top_r);
                __m256i bl = _mm256_loadu_si256((const __m256i*)p_bot_l);
                __m256i tl = _mm256_loadu_si256((const __m256i*)p_top_l);
                // 计算区域和: sum = (br - tr) - (bl - tl)
                __m256i diff_r = _mm256_sub_epi32(br, tr);
                __m256i diff_l = _mm256_sub_epi32(bl, tl);
                __m256i sum = _mm256_sub_epi32(diff_r, diff_l);
                // 转换为浮点数并乘以倒数 (代替除法)
                __m256 sum_ps = _mm256_cvtepi32_ps(sum);
                __m256 res_ps = _mm256_mul_ps(sum_ps, v_area_inv);
                // 截断回整数
                __m256i res_i = _mm256_cvttps_epi32(res_ps);
                // 压缩 32位 -> 8位
                __m128i lo = _mm256_castsi256_si128(res_i);
                __m128i hi = _mm256_extracti128_si256(res_i, 1);
                __m128i p16 = _mm_packus_epi32(lo, hi); // 32->16
                __m128i p8 = _mm_packus_epi16(p16, _mm_setzero_si128()); // 16->8
                _mm_storel_epi64((__m128i*)(row_out + j), p8);
                p_bot_r += 8; p_top_r += 8;
                p_bot_l += 8; p_top_l += 8;
            }
        }
        // 右边界处理 (Scalar)
        for (; j < width; ++j) {
            int left = std::max<int>(static_cast<int>(j) - radius, 0);
            int right = std::min<int>(static_cast<int>(j) + radius, static_cast<int>(width) - 1);
            int w = right - left + 1;
            int area = h * w;
            int sum = row_bot[right + 1] - row_top[right + 1] - row_bot[left] + row_top[left];
            row_out[j] = static_cast<unsigned char>(sum / area);
        }
    }
    // 校验和输出
    unsigned int checksum = calcChecksum(output);
    printf("[boxFilter] optimized checksum: %u\n", checksum);
}

/**
 * @brief 图像旋转90度（顺时针）
 */
void rotate90Clockwise(const Image& input, Image& output) {
    const size_t width = input.width();
    const size_t height = input.height();
    output.resize(height, width);

    const unsigned char* in_base = input.data();
    unsigned char* out_base = output.data();

    // 缓存分块 (Tiling) 32x32 的块大小通常能很好地适应 L1 Cache
    const size_t TILE_SIZE = 32; 

    // 循环交换 (Loop Interchange) 交换外层循环顺序：先 j 后 i
    for (size_t j = 0; j < width; j += TILE_SIZE) {
        for (size_t i = 0; i < height; i += TILE_SIZE) {
            // 确定当前 Tile 的边界
            size_t i_limit = std::min(i + TILE_SIZE, height);
            size_t j_limit = std::min(j + TILE_SIZE, width);
            // 在 Tile 内部进行 8x8 的 SIMD 处理
            for (size_t ii = i; ii < i_limit; ii += 8) {
                for (size_t jj = j; jj < j_limit; jj += 8) {
                    if (ii + 8 <= i_limit && jj + 8 <= j_limit) {
                        __m128i row[8];
                        const unsigned char* src_ptr = in_base + ii * width + jj;
                        _mm_prefetch((const char*)(src_ptr + 8 * width), _MM_HINT_T0);
                        row[0] = _mm_loadu_si128((const __m128i*)(src_ptr + 0 * width));
                        row[1] = _mm_loadu_si128((const __m128i*)(src_ptr + 1 * width));
                        row[2] = _mm_loadu_si128((const __m128i*)(src_ptr + 2 * width));
                        row[3] = _mm_loadu_si128((const __m128i*)(src_ptr + 3 * width));
                        row[4] = _mm_loadu_si128((const __m128i*)(src_ptr + 4 * width));
                        row[5] = _mm_loadu_si128((const __m128i*)(src_ptr + 5 * width));
                        row[6] = _mm_loadu_si128((const __m128i*)(src_ptr + 6 * width));
                        row[7] = _mm_loadu_si128((const __m128i*)(src_ptr + 7 * width));
                        __m128i t0 = _mm_unpacklo_epi8(row[0], row[1]);
                        __m128i t1 = _mm_unpacklo_epi8(row[2], row[3]);
                        __m128i t2 = _mm_unpacklo_epi8(row[4], row[5]);
                        __m128i t3 = _mm_unpacklo_epi8(row[6], row[7]);
                        __m128i t4 = _mm_unpackhi_epi8(row[0], row[1]);
                        __m128i t5 = _mm_unpackhi_epi8(row[2], row[3]);
                        __m128i t6 = _mm_unpackhi_epi8(row[4], row[5]);
                        __m128i t7 = _mm_unpackhi_epi8(row[6], row[7]);
                        __m128i s0 = _mm_unpacklo_epi16(t0, t1);
                        __m128i s1 = _mm_unpacklo_epi16(t2, t3);
                        __m128i s2 = _mm_unpackhi_epi16(t0, t1);
                        __m128i s3 = _mm_unpackhi_epi16(t2, t3);
                        __m128i s4 = _mm_unpacklo_epi16(t4, t5);
                        __m128i s5 = _mm_unpacklo_epi16(t6, t7);
                        __m128i s6 = _mm_unpackhi_epi16(t4, t5);
                        __m128i s7 = _mm_unpackhi_epi16(t6, t7);
                        __m128i u[8];
                        u[0] = _mm_unpacklo_epi32(s0, s1);
                        u[1] = _mm_unpackhi_epi32(s0, s1);
                        u[2] = _mm_unpacklo_epi32(s2, s3);
                        u[3] = _mm_unpackhi_epi32(s2, s3);
                        u[4] = _mm_unpacklo_epi32(s4, s5);
                        u[5] = _mm_unpackhi_epi32(s4, s5);
                        u[6] = _mm_unpacklo_epi32(s6, s7);
                        u[7] = _mm_unpackhi_epi32(s6, s7);
                        size_t base_dst_idx = (height - ii - 8); 
                        unsigned char* dst_base = out_base + jj * height + base_dst_idx;
                        _mm_storel_epi64((__m128i*)(dst_base + 0 * height), u[7]);
                        _mm_storel_epi64((__m128i*)(dst_base + 1 * height), u[6]);
                        _mm_storel_epi64((__m128i*)(dst_base + 2 * height), u[5]);
                        _mm_storel_epi64((__m128i*)(dst_base + 3 * height), u[4]);
                        _mm_storel_epi64((__m128i*)(dst_base + 4 * height), u[3]);
                        _mm_storel_epi64((__m128i*)(dst_base + 5 * height), u[2]);
                        _mm_storel_epi64((__m128i*)(dst_base + 6 * height), u[1]);
                        _mm_storel_epi64((__m128i*)(dst_base + 7 * height), u[0]);
                    } else {
                        // 边界处理 (Scalar Fallback)
                        size_t i_end = std::min(ii + 8, i_limit);
                        size_t j_end = std::min(jj + 8, j_limit);
                        for (size_t r = ii; r < i_end; ++r) {
                            for (size_t c = jj; c < j_end; ++c) {
                                output.at(c, height - 1 - r) = input.at(r, c);
                            }
                        }
                    }
                }
            }
        }
    }
    // 校验和输出
    unsigned int checksum = calcChecksum(output);
    printf("[rotate90Clockwise] optimized checksum: %u\n", checksum);
}


/**
 * @brief 计算校验和
 */
unsigned int calcChecksum(const Image& img) {
    unsigned int sum = 0;
    constexpr size_t mod = 1000000007;
    const size_t size = img.size();
    const unsigned char* data = img.data();
    
    for (size_t i = 0; i < size; ++i) {
        sum = (sum + data[i]) % mod;
    }
    
    return sum;
}

/**
 * @brief 比较两个图像
 */
bool compareImages(const Image& img1, const Image& img2, unsigned char tolerance) {
    if (img1.width() != img2.width() || img1.height() != img2.height()) {
        return false;
    }
    
    const size_t size = img1.size();
    for (size_t i = 0; i < size; ++i) {
        int diff = std::abs(static_cast<int>(img1.data()[i]) - static_cast<int>(img2.data()[i]));
        if (diff > tolerance) {
            return false;
        }
    }
    
    return true;
}

} // namespace ImageAlgorithms

