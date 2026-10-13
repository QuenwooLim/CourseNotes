"""
文件管理器 - 处理图形的保存和加载
"""
import json
import os
from typing import List, Dict, Any
from tkinter import filedialog, messagebox
from shapes import Shape, create_shape_from_dict


class FileManager:
    """文件管理器类"""
    
    def __init__(self):
        self.current_file = None
        self.file_types = [
            ("图形文件", "*.json"),
            ("所有文件", "*.*")
        ]
        self.image_types = [
            ("PNG图像", "*.png"),
            ("JPEG图像", "*.jpg"),
            ("所有文件", "*.*")
        ]
    
    def save_shapes(self, shapes: List[Shape], filename: str = None) -> bool:
        """保存图形到文件"""
        try:
            if filename is None:
                filename = filedialog.asksaveasfilename(
                    title="保存图形文件",
                    filetypes=self.file_types,
                    defaultextension=".json"
                )
            
            if not filename:
                return False
            
            # 转换图形为字典格式
            shapes_data = []
            for shape in shapes:
                shapes_data.append(shape.to_dict())
            
            # 创建文件数据
            file_data = {
                "version": "1.0",
                "shapes": shapes_data,
                "metadata": {
                    "created_by": "图形绘制系统",
                    "total_shapes": len(shapes)
                }
            }
            
            # 保存到文件
            with open(filename, 'w', encoding='utf-8') as f:
                json.dump(file_data, f, ensure_ascii=False, indent=2)
            
            self.current_file = filename
            messagebox.showinfo("保存成功", f"图形已保存到: {os.path.basename(filename)}")
            return True
            
        except Exception as e:
            messagebox.showerror("保存失败", f"保存文件时出错: {str(e)}")
            return False
    
    def load_shapes(self, filename: str = None) -> List[Shape]:
        """从文件加载图形"""
        try:
            if filename is None:
                filename = filedialog.askopenfilename(
                    title="打开图形文件",
                    filetypes=self.file_types
                )
            
            if not filename:
                return []
            
            # 读取文件
            with open(filename, 'r', encoding='utf-8') as f:
                file_data = json.load(f)
            
            # 验证文件格式
            if "shapes" not in file_data:
                raise ValueError("无效的文件格式")
            
            # 创建图形对象
            shapes = []
            for shape_data in file_data["shapes"]:
                try:
                    shape = create_shape_from_dict(shape_data)
                    shapes.append(shape)
                except Exception as e:
                    print(f"加载图形时出错: {e}")
                    continue
            
            self.current_file = filename
            messagebox.showinfo("加载成功", 
                              f"已从 {os.path.basename(filename)} 加载 {len(shapes)} 个图形")
            return shapes
            
        except Exception as e:
            messagebox.showerror("加载失败", f"加载文件时出错: {str(e)}")
            return []
    
    def save_as_image(self, canvas, filename: str = None) -> bool:
        """保存画布为图像"""
        try:
            if filename is None:
                filename = filedialog.asksaveasfilename(
                    title="导出图像",
                    filetypes=self.image_types,
                    defaultextension=".png"
                )
            
            if not filename:
                return False
            
            # 调用画布的导出方法
            success = canvas.export_to_image(filename)
            if success:
                messagebox.showinfo("导出成功", f"图像已导出到: {os.path.basename(filename)}")
            
            return success
            
        except Exception as e:
            messagebox.showerror("导出失败", f"导出图像时出错: {str(e)}")
            return False
    
    def new_file(self):
        """新建文件"""
        self.current_file = None
    
    def get_current_file(self) -> str:
        """获取当前文件名"""
        return self.current_file
    
    def has_current_file(self) -> bool:
        """是否有当前文件"""
        return self.current_file is not None
    
    def quick_save(self, shapes: List[Shape]) -> bool:
        """快速保存（如果有当前文件则直接保存，否则另存为）"""
        if self.current_file:
            return self.save_shapes(shapes, self.current_file)
        else:
            return self.save_shapes(shapes)


class CodeGenerator:
    """代码生成器 - 特色功能"""
    
    def __init__(self):
        pass
    
    def generate_html_canvas(self, shapes: List[Shape]) -> str:
        """生成HTML Canvas代码"""
        html_template = '''<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>绘图系统生成的图形</title>
    <style>
        body {
            margin: 0;
            padding: 20px;
            font-family: Arial, sans-serif;
        }
        canvas {
            border: 1px solid #ccc;
            display: block;
            margin: 20px auto;
        }
        .info {
            text-align: center;
            color: #666;
            margin-bottom: 20px;
        }
    </style>
</head>
<body>
    <div class="info">
        <h2>图形绘制系统生成的前端代码</h2>
        <p>包含 {shape_count} 个图形</p>
    </div>
    
    <canvas id="canvas" width="800" height="600"></canvas>
    
    <script>
        const canvas = document.getElementById('canvas');
        const ctx = canvas.getContext('2d');
        
        // 设置画布背景
        ctx.fillStyle = 'white';
        ctx.fillRect(0, 0, canvas.width, canvas.height);
        
        // 绘制图形
{draw_code}
    </script>
</body>
</html>'''
        
        draw_code = ""
        for i, shape in enumerate(shapes):
            draw_code += f"        // 图形 {i+1}: {shape.__class__.__name__}\\n"
            draw_code += self._generate_shape_code(shape)
            draw_code += "\\n"
        
        return html_template.format(
            shape_count=len(shapes),
            draw_code=draw_code
        )
    
    def _generate_shape_code(self, shape: Shape) -> str:
        """生成单个图形的Canvas代码"""
        code = ""
        
        if hasattr(shape, 'fill_color') and shape.fill_color:
            code += f"        ctx.fillStyle = '{shape.fill_color}';\\n"
        
        code += f"        ctx.strokeStyle = '{shape.color}';\\n"
        
        if hasattr(shape, 'line_width'):
            code += f"        ctx.lineWidth = {shape.line_width};\\n"
        
        if shape.__class__.__name__ == "Point":
            code += f"        ctx.beginPath();\\n"
            code += f"        ctx.arc({shape.x}, {shape.y}, {shape.size}, 0, 2 * Math.PI);\\n"
            code += f"        ctx.fill();\\n"
            
        elif shape.__class__.__name__ == "Line":
            code += f"        ctx.beginPath();\\n"
            code += f"        ctx.moveTo({shape.x}, {shape.y});\\n"
            code += f"        ctx.lineTo({shape.x2}, {shape.y2});\\n"
            code += f"        ctx.stroke();\\n"
            
        elif shape.__class__.__name__ == "Rectangle":
            if shape.fill_color:
                code += f"        ctx.fillRect({shape.x}, {shape.y}, {shape.width}, {shape.height});\\n"
            code += f"        ctx.strokeRect({shape.x}, {shape.y}, {shape.width}, {shape.height});\\n"
            
        elif shape.__class__.__name__ == "Circle":
            code += f"        ctx.beginPath();\\n"
            code += f"        ctx.arc({shape.x}, {shape.y}, {shape.radius}, 0, 2 * Math.PI);\\n"
            if shape.fill_color:
                code += f"        ctx.fill();\\n"
            code += f"        ctx.stroke();\\n"
            
        elif shape.__class__.__name__ == "Polygon":
            if len(shape.points) >= 3:
                code += f"        ctx.beginPath();\\n"
                first_point = shape.points[0]
                code += f"        ctx.moveTo({first_point[0]}, {first_point[1]});\\n"
                for point in shape.points[1:]:
                    code += f"        ctx.lineTo({point[0]}, {point[1]});\\n"
                code += f"        ctx.closePath();\\n"
                if shape.fill_color:
                    code += f"        ctx.fill();\\n"
                code += f"        ctx.stroke();\\n"
        
        return code
    
    def generate_svg(self, shapes: List[Shape]) -> str:
        """生成SVG代码"""
        svg_template = '''<?xml version="1.0" encoding="UTF-8"?>
<svg width="800" height="600" xmlns="http://www.w3.org/2000/svg">
    <rect width="100%" height="100%" fill="white"/>
    <!-- 图形绘制系统生成的SVG，包含 {shape_count} 个图形 -->
{svg_elements}
</svg>'''
        
        svg_elements = ""
        for i, shape in enumerate(shapes):
            svg_elements += f"    <!-- 图形 {i+1}: {shape.__class__.__name__} -->\\n"
            svg_elements += self._generate_svg_element(shape)
            svg_elements += "\\n"
        
        return svg_template.format(
            shape_count=len(shapes),
            svg_elements=svg_elements
        )
    
    def _generate_svg_element(self, shape: Shape) -> str:
        """生成单个图形的SVG元素"""
        fill = shape.fill_color if hasattr(shape, 'fill_color') and shape.fill_color else "none"
        stroke = shape.color
        stroke_width = getattr(shape, 'line_width', 1)
        
        if shape.__class__.__name__ == "Point":
            return f'    <circle cx="{shape.x}" cy="{shape.y}" r="{shape.size}" fill="{stroke}" stroke="{stroke}"/>'
            
        elif shape.__class__.__name__ == "Line":
            return f'    <line x1="{shape.x}" y1="{shape.y}" x2="{shape.x2}" y2="{shape.y2}" stroke="{stroke}" stroke-width="{stroke_width}"/>'
            
        elif shape.__class__.__name__ == "Rectangle":
            return f'    <rect x="{shape.x}" y="{shape.y}" width="{shape.width}" height="{shape.height}" fill="{fill}" stroke="{stroke}" stroke-width="{stroke_width}"/>'
            
        elif shape.__class__.__name__ == "Circle":
            return f'    <circle cx="{shape.x}" cy="{shape.y}" r="{shape.radius}" fill="{fill}" stroke="{stroke}" stroke-width="{stroke_width}"/>'
            
        elif shape.__class__.__name__ == "Polygon":
            if len(shape.points) >= 3:
                points_str = " ".join([f"{p[0]},{p[1]}" for p in shape.points])
                return f'    <polygon points="{points_str}" fill="{fill}" stroke="{stroke}" stroke-width="{stroke_width}"/>'
        
        return ""
    
    def save_generated_code(self, shapes: List[Shape], code_type: str = "html"):
        """保存生成的代码"""
        try:
            if code_type == "html":
                code = self.generate_html_canvas(shapes)
                filetypes = [("HTML文件", "*.html"), ("所有文件", "*.*")]
                defaultextension = ".html"
                title = "保存HTML代码"
            elif code_type == "svg":
                code = self.generate_svg(shapes)
                filetypes = [("SVG文件", "*.svg"), ("所有文件", "*.*")]
                defaultextension = ".svg"
                title = "保存SVG代码"
            else:
                return False
            
            filename = filedialog.asksaveasfilename(
                title=title,
                filetypes=filetypes,
                defaultextension=defaultextension
            )
            
            if not filename:
                return False
            
            with open(filename, 'w', encoding='utf-8') as f:
                f.write(code)
            
            messagebox.showinfo("保存成功", f"代码已保存到: {os.path.basename(filename)}")
            return True
            
        except Exception as e:
            messagebox.showerror("保存失败", f"保存代码时出错: {str(e)}")
            return False