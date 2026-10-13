#ifndef IMAGE_ALGORITHMS_HPP
#define IMAGE_ALGORITHMS_HPP

#include <vector>
#include <cstddef>
#include <cmath>
#include <algorithm>

namespace ImageAlgorithms {

/**
 * @brief 图像类 - 使用连续内存存储
 */
class Image {
private:
    std::vector<unsigned char> data_;
    size_t width_;
    size_t height_;

public:
    Image() : width_(0), height_(0) {}
    
    Image(size_t w, size_t h) : data_(w * h), width_(w), height_(h) {}
    
    void resize(size_t w, size_t h) {
        width_ = w;
        height_ = h;
        data_.resize(w * h);
    }
    
    unsigned char& at(size_t i, size_t j) {
        return data_[i * width_ + j];
    }
    
    const unsigned char& at(size_t i, size_t j) const {
        return data_[i * width_ + j];
    }
    
    unsigned char* data() { return data_.data(); }
    const unsigned char* data() const { return data_.data(); }
    
    size_t width() const { return width_; }
    size_t height() const { return height_; }
    size_t size() const { return data_.size(); }
};

/**
 * @brief 高斯滤波 - 基准实现
 */
void gaussianFilter(const Image& input, Image& output);

/**
 * @brief 幂次变换 - 基准实现
 */
void powerLawTransformation(const Image& input, Image& output, float gamma = 0.5f);

/**
 * @brief Sobel边缘检测 - 基准实现
 */
void sobelEdgeDetection(const Image& input, Image& output);

/**
 * @brief 图像转置 - 基准实现
 */
void transpose(const Image& input, Image& output);

/**
 * @brief 均值滤波 - 基准实现
 */
void boxFilter(const Image& input, Image& output, int kernel_size = 5);

/**
 * @brief 图像旋转90度 - 基准实现
 */
void rotate90Clockwise(const Image& input, Image& output);

/**
 * @brief 计算校验和（用于验证正确性）
 */
unsigned int calcChecksum(const Image& img);

/**
 * @brief 比较两个图像是否相等
 */
bool compareImages(const Image& img1, const Image& img2, unsigned char tolerance = 0);

} // namespace ImageAlgorithms

#endif // IMAGE_ALGORITHMS_HPP

