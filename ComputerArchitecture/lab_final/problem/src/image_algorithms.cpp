#include "image_algorithms.hpp"
#include <cmath>

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
    
    output.resize(width, height);
    
    // 处理内部像素
    for (size_t i = 1; i < height - 1; ++i) {
        for (size_t j = 1; j < width - 1; ++j) {
            int sum = 
                input.at(i-1, j-1) + 2*input.at(i-1, j) + input.at(i-1, j+1) +
                2*input.at(i, j-1) + 4*input.at(i, j) + 2*input.at(i, j+1) +
                input.at(i+1, j-1) + 2*input.at(i+1, j) + input.at(i+1, j+1);
            output.at(i, j) = static_cast<unsigned char>(sum / 16);
        }
    }
    
    // 处理边界（简单复制）
    for (size_t j = 0; j < width; ++j) {
        output.at(0, j) = input.at(0, j);
        output.at(height-1, j) = input.at(height-1, j);
    }
    for (size_t i = 0; i < height; ++i) {
        output.at(i, 0) = input.at(i, 0);
        output.at(i, width-1) = input.at(i, width-1);
    }
}

/**
 * @brief 幂次变换 - 对比度调整
 * 
 * 公式: output = 255 * (input/255)^gamma
 */
void powerLawTransformation(const Image& input, Image& output, float gamma) {
    const size_t size = input.size();
    output.resize(input.width(), input.height());
    
    for (size_t i = 0; i < size; ++i) {
        float normalized = input.data()[i] / 255.0f;
        float transformed = std::pow(normalized, gamma);
        output.data()[i] = static_cast<unsigned char>(transformed * 255.0f + 0.5f);
    }
}

/**
 * @brief Sobel边缘检测
 * 
 * Gx = [-1 0 1]    Gy = [-1 -2 -1]
 *      [-2 0 2]         [ 0  0  0]
 *      [-1 0 1]         [ 1  2  1]
 */
void sobelEdgeDetection(const Image& input, Image& output) {
    const size_t width = input.width();
    const size_t height = input.height();
    
    output.resize(width, height);
    
    for (size_t i = 1; i < height - 1; ++i) {
        for (size_t j = 1; j < width - 1; ++j) {
            int gx = -input.at(i-1, j-1) + input.at(i-1, j+1)
                     -2*input.at(i, j-1) + 2*input.at(i, j+1)
                     -input.at(i+1, j-1) + input.at(i+1, j+1);
            
            int gy = -input.at(i-1, j-1) - 2*input.at(i-1, j) - input.at(i-1, j+1)
                     +input.at(i+1, j-1) + 2*input.at(i+1, j) + input.at(i+1, j+1);
            
            int magnitude = static_cast<int>(std::sqrt(gx*gx + gy*gy));
            output.at(i, j) = static_cast<unsigned char>(std::min(magnitude, 255));
        }
    }
    
    // 边界置零
    for (size_t j = 0; j < width; ++j) {
        output.at(0, j) = 0;
        output.at(height-1, j) = 0;
    }
    for (size_t i = 0; i < height; ++i) {
        output.at(i, 0) = 0;
        output.at(i, width-1) = 0;
    }
}

/**
 * @brief 图像转置
 */
void transpose(const Image& input, Image& output) {
    const size_t width = input.width();
    const size_t height = input.height();
    
    output.resize(height, width);
    
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            output.at(j, i) = input.at(i, j);
        }
    }
}

/**
 * @brief 均值滤波
 */
void boxFilter(const Image& input, Image& output, int kernel_size) {
    const size_t width = input.width();
    const size_t height = input.height();
    const int radius = kernel_size / 2;
    
    output.resize(width, height);
    
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            int sum = 0;
            int count = 0;
            
            for (int di = -radius; di <= radius; ++di) {
                for (int dj = -radius; dj <= radius; ++dj) {
                    int ni = static_cast<int>(i) + di;
                    int nj = static_cast<int>(j) + dj;
                    
                    if (ni >= 0 && ni < static_cast<int>(height) &&
                        nj >= 0 && nj < static_cast<int>(width)) {
                        sum += input.at(ni, nj);
                        count++;
                    }
                }
            }
            
            output.at(i, j) = static_cast<unsigned char>(sum / count);
        }
    }
}

/**
 * @brief 图像旋转90度（顺时针）
 */
void rotate90Clockwise(const Image& input, Image& output) {
    const size_t width = input.width();
    const size_t height = input.height();
    
    output.resize(height, width);
    
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            output.at(j, height - 1 - i) = input.at(i, j);
        }
    }
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

