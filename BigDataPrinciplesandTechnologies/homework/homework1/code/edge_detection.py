import cv2
import numpy as np
import os
import matplotlib.pyplot as plt

def main():
    # 设置图片路径
    img_path = os.path.join(os.path.dirname(__file__), 'data', 'test.png')
    if not os.path.exists(img_path):
        print(f"未找到测试图片: {img_path}")
        return

    # 读取原图
    img = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)
    if img is None:
        print("图片读取失败！")
        return

    # Sobel 边缘检测
    sobelx = cv2.Sobel(img, cv2.CV_64F, 1, 0, ksize=3)
    sobely = cv2.Sobel(img, cv2.CV_64F, 0, 1, ksize=3)
    sobel = cv2.magnitude(sobelx, sobely)
    sobel = np.uint8(np.clip(sobel, 0, 255))

    # 展示原图和边缘图
    plt.figure(figsize=(10, 4))
    plt.subplot(1, 2, 1)
    plt.title('Original')
    plt.imshow(img, cmap='gray')
    plt.axis('off')
    plt.subplot(1, 2, 2)
    plt.title('Sobel Edge')
    plt.imshow(sobel, cmap='gray')
    plt.axis('off')
    plt.tight_layout()
    plt.show()

    # 保存边缘检测结果
    out_path = os.path.join(os.path.dirname(__file__), 'data', 'test_sobel.png')
    cv2.imwrite(out_path, sobel)
    print(f"边缘检测结果已保存: {out_path}")

if __name__ == '__main__':
    main()
