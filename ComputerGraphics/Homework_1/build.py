"""
图形绘制系统 - PyInstaller 构建脚本
用于将Python项目打包成exe可执行文件
"""

import os
import sys
import shutil
import subprocess
import platform
from pathlib import Path


class ProjectBuilder:
    """项目构建器"""
    
    def __init__(self):
        self.project_root = Path(__file__).parent
        self.dist_dir = self.project_root / "dist"
        self.build_dir = self.project_root / "build"
        self.exe_name = "图形绘制系统"
        self.main_script = "main.py"
        
    def check_dependencies(self):
        """检查构建依赖"""
        print("检查构建依赖...")
        
        try:
            import PyInstaller
            print(f"✓ PyInstaller 已安装 (版本: {PyInstaller.__version__})")
        except ImportError:
            print("✗ 未找到 PyInstaller，正在安装...")
            self.install_pyinstaller()
            
        # 检查项目依赖
        required_modules = ['tkinter', 'PIL']
        for module in required_modules:
            try:
                __import__(module)
                print(f"✓ {module} 可用")
            except ImportError:
                print(f"✗ {module} 未找到")
                if module == 'PIL':
                    print("请运行: pip install Pillow")
                
    def install_pyinstaller(self):
        """安装PyInstaller"""
        try:
            subprocess.run([sys.executable, "-m", "pip", "install", "pyinstaller"], 
                         check=True)
            print("✓ PyInstaller 安装成功")
        except subprocess.CalledProcessError as e:
            print(f"✗ PyInstaller 安装失败: {e}")
            sys.exit(1)
            
    def clean_build(self):
        """清理之前的构建文件"""
        print("清理构建目录...")
        
        dirs_to_clean = [self.dist_dir, self.build_dir]
        for dir_path in dirs_to_clean:
            if dir_path.exists():
                shutil.rmtree(dir_path)
                print(f"✓ 已删除 {dir_path}")
                
        # 删除spec文件
        spec_files = list(self.project_root.glob("*.spec"))
        for spec_file in spec_files:
            spec_file.unlink()
            print(f"✓ 已删除 {spec_file}")
            
    def create_spec_file(self):
        """创建PyInstaller spec文件"""
        print("创建PyInstaller配置文件...")
        
        spec_content = f'''# -*- mode: python ; coding: utf-8 -*-

import sys
from pathlib import Path

# 项目根目录
project_root = Path(r"{self.project_root}")

a = Analysis(
    [r'{self.project_root / self.main_script}'],
    pathex=[r'{self.project_root}'],
    binaries=[],
    datas=[
        # 如果有其他资源文件，在这里添加
        # (r'path/to/resource', 'destination/in/exe'),
    ],
    hiddenimports=[
        'tkinter',
        'tkinter.ttk',
        'tkinter.colorchooser',
        'tkinter.messagebox',
        'tkinter.filedialog',
        'PIL',
        'PIL.Image',
        'PIL.ImageTk',
        'PIL.ImageDraw',
    ],
    hookspath=[],
    hooksconfig={{}},
    runtime_hooks=[],
    excludes=[
        'matplotlib',
        'numpy',
        'pandas',
        'scipy',
        'jupyter',
        'notebook',
        'IPython',
    ],
    win_no_prefer_redirects=False,
    win_private_assemblies=False,
    cipher=None,
    noarchive=False,
)

pyz = PYZ(a.pure, a.zipped_data, cipher=None)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.zipfiles,
    a.datas,
    [],
    name='{self.exe_name}',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=False,  # 设为False隐藏控制台窗口
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon=None,  # 如果有图标文件，在这里设置路径
)
'''
        
        spec_path = self.project_root / f"{self.exe_name}.spec"
        with open(spec_path, 'w', encoding='utf-8') as f:
            f.write(spec_content)
            
        print(f"✓ 已创建配置文件: {spec_path}")
        return spec_path
        
    def build_exe(self):
        """构建exe文件"""
        print("开始构建exe文件...")
        
        spec_path = self.create_spec_file()
        
        # 构建命令
        cmd = [
            sys.executable, "-m", "PyInstaller",
            "--clean",  # 清理临时文件
            "--noconfirm",  # 覆盖输出目录
            str(spec_path)
        ]
        
        print(f"执行命令: {' '.join(cmd)}")
        
        try:
            # 在项目根目录执行构建
            result = subprocess.run(cmd, cwd=self.project_root, 
                                  capture_output=True, text=True, 
                                  encoding='utf-8')
            
            if result.returncode == 0:
                print("✓ 构建成功!")
                
                # 查找生成的exe文件
                exe_path = self.dist_dir / f"{self.exe_name}.exe"
                if exe_path.exists():
                    file_size = exe_path.stat().st_size / (1024 * 1024)
                    print(f"✓ 可执行文件: {exe_path}")
                    print(f"✓ 文件大小: {file_size:.1f} MB")
                    return exe_path
                else:
                    print("✗ 未找到生成的exe文件")
                    return None
                    
            else:
                print("✗ 构建失败!")
                print("错误信息:")
                print(result.stderr)
                return None
                
        except Exception as e:
            print(f"✗ 构建过程出错: {e}")
            return None
            
    def test_exe(self, exe_path):
        """测试生成的exe文件"""
        if not exe_path or not exe_path.exists():
            print("✗ 无法测试：exe文件不存在")
            return False
            
        print("测试exe文件...")
        print(f"文件路径: {exe_path}")
        print("提示：exe文件已生成，可以双击运行测试")
        print("注意：首次运行可能需要较长时间加载")
        
        return True
        
    def create_launcher_script(self):
        """创建启动脚本"""
        launcher_content = f'''@echo off
echo 正在启动图形绘制系统...
echo 请等待程序加载...

cd /d "%~dp0"
if exist "{self.exe_name}.exe" (
    start "" "{self.exe_name}.exe"
) else (
    echo 错误：未找到可执行文件 {self.exe_name}.exe
    pause
)
'''
        
        launcher_path = self.dist_dir / "启动程序.bat"
        with open(launcher_path, 'w', encoding='gbk') as f:
            f.write(launcher_content)
            
        print(f"✓ 已创建启动脚本: {launcher_path}")
        
    def build(self):
        """完整构建流程"""
        print("="*50)
        print("图形绘制系统 - 构建exe可执行文件")
        print("="*50)
        
        try:
            # 1. 检查依赖
            self.check_dependencies()
            
            # 2. 清理旧文件
            self.clean_build()
            
            # 3. 构建exe
            exe_path = self.build_exe()
            
            if exe_path:
                # 4. 创建启动脚本
                self.create_launcher_script()
                
                # 5. 测试exe
                self.test_exe(exe_path)
                
                print("\n" + "="*50)
                print("构建完成！")
                print(f"可执行文件位置: {exe_path}")
                print(f"启动脚本位置: {self.dist_dir / '启动程序.bat'}")
                print("="*50)
                
                return True
            else:
                print("\n构建失败，请检查错误信息")
                return False
                
        except Exception as e:
            print(f"\n构建过程出现错误: {e}")
            return False


def main():
    """主函数"""
    builder = ProjectBuilder()
    
    if len(sys.argv) > 1:
        command = sys.argv[1].lower()
        if command == "clean":
            builder.clean_build()
        elif command == "spec":
            builder.create_spec_file()
        elif command == "test":
            exe_path = builder.dist_dir / f"{builder.exe_name}.exe"
            builder.test_exe(exe_path)
        else:
            print("可用命令: clean, spec, test")
    else:
        # 执行完整构建
        success = builder.build()
        
        if not success:
            input("\n按回车键退出...")


if __name__ == "__main__":
    main()