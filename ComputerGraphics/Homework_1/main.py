"""
图形绘制系统主程序
计算机图形学第一次作业

支持功能：
- 绘制基本二维图形（点、直线、矩形、圆形、多边形）
- 图形属性设置（颜色、线宽、填充）
- 图形操作（选择、移动、缩放、删除）
- 文件保存和加载
- 图像导出
- 前端代码生成（HTML Canvas、SVG）
"""

import sys
import os

# 添加当前目录到Python路径
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from gui import DrawingApplication


def main():
    """主函数"""
    print("正在启动图形绘制系统...")
    
    try:
        # 创建并运行应用程序
        app = DrawingApplication()
        print("系统启动成功！")
        app.run()
        
    except ImportError as e:
        print(f"导入错误: {e}")
        print("请确保已安装所有必要的依赖包")
        
    except Exception as e:
        print(f"程序运行出错: {e}")
        

if __name__ == "__main__":
    main()
