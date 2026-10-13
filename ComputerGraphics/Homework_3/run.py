#!/usr/bin/env python3
"""
启动脚本 - 用于测试图形绘制系统
"""

import os
import sys
import traceback

# 添加当前目录到Python路径
current_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, current_dir)

def check_dependencies():
    """检查依赖包"""
    try:
        import tkinter
        print("✓ tkinter已安装")
    except ImportError:
        print("✗ tkinter未安装")
        return False
    
    try:
        import PIL
        print("✓ Pillow已安装")
    except ImportError:
        print("✗ Pillow未安装，图像导出功能将不可用")
    
    return True

def run_application():
    """运行应用程序"""
    try:
        print("正在启动图形绘制系统...")
        print("=" * 50)
        
        # 检查依赖
        if not check_dependencies():
            print("请先安装必要的依赖包")
            return False
        
        print("=" * 50)
        
        # 导入并运行主程序
        from main import main
        main()
        
        return True
        
    except ImportError as e:
        print(f"导入错误: {e}")
        print("请检查所有源文件是否存在")
        return False
        
    except Exception as e:
        print(f"运行时错误: {e}")
        print("详细错误信息:")
        traceback.print_exc()
        return False

if __name__ == "__main__":
    print("图形绘制系统启动器")
    print("=" * 50)
    print(f"Python版本: {sys.version}")
    print(f"工作目录: {current_dir}")
    print("=" * 50)
    
    success = run_application()
    
    if not success:
        print("=" * 50)
        print("程序未能正常启动")
        input("按Enter键退出...")
    else:
        print("程序已退出")