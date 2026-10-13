"""
绘图画布组件 - 处理图形绘制和交互
"""
import tkinter as tk
from tkinter import messagebox
from typing import List, Optional, Tuple
from shapes import Shape, Point, Line, Rectangle, Circle, Polygon, Cube


class DrawingCanvas:
    """绘图画布类，处理图形绘制和交互"""
    
    def __init__(self, parent, width=800, height=600):
        self.parent = parent
        self.width = width
        self.height = height
        
        # 创建画布
        self.canvas = tk.Canvas(parent, width=width, height=height, 
                               bg="white", relief=tk.SUNKEN, borderwidth=2)
        
        # 图形存储
        self.shapes: List[Shape] = []
        self.selected_shape: Optional[Shape] = None
        
        # 绘制状态
        self.current_tool = "select"  # select, point, line, rectangle, circle, polygon, cube
        self.is_drawing = False
        self.start_x = 0
        self.start_y = 0
        self.temp_shape = None
        self.polygon_points = []
        
        # 图形属性
        self.current_color = "black"
        self.current_line_width = 1
        self.current_fill_color = None
        
        # 拖拽状态
        self.is_dragging = False
        self.drag_start_x = 0
        self.drag_start_y = 0
        
        # 绑定事件
        self.bind_events()
    
    def bind_events(self):
        """绑定鼠标事件"""
        self.canvas.bind("<Button-1>", self.on_mouse_down)
        self.canvas.bind("<B1-Motion>", self.on_mouse_drag)
        self.canvas.bind("<ButtonRelease-1>", self.on_mouse_up)
        self.canvas.bind("<Double-Button-1>", self.on_double_click)
        self.canvas.bind("<KeyPress>", self.on_key_press)
        self.canvas.focus_set()
    
    def pack(self, **kwargs):
        """打包画布"""
        self.canvas.pack(**kwargs)
    
    def grid(self, **kwargs):
        """网格布局画布"""
        self.canvas.grid(**kwargs)
    
    def set_tool(self, tool: str):
        """设置当前绘制工具"""
        self.current_tool = tool
        self.is_drawing = False
        self.polygon_points = []
        self.deselect_all()
        self.redraw()
    
    def set_color(self, color: str):
        """设置绘制颜色"""
        self.current_color = color
        if self.selected_shape:
            self.selected_shape.color = color
            self.redraw()
    
    def set_line_width(self, width: int):
        """设置线宽"""
        self.current_line_width = width
        if self.selected_shape:
            self.selected_shape.line_width = width
            self.redraw()
    
    def set_fill_color(self, color: str):
        """设置填充色"""
        self.current_fill_color = color if color != "none" else None
        if self.selected_shape:
            self.selected_shape.fill_color = self.current_fill_color
            self.redraw()
    
    def on_mouse_down(self, event):
        """鼠标按下事件"""
        x, y = event.x, event.y
        
        if self.current_tool == "select":
            self.handle_select_tool(x, y)
        elif self.current_tool == "point":
            self.create_point(x, y)
        elif self.current_tool == "line":
            self.start_line(x, y)
        elif self.current_tool == "rectangle":
            self.start_rectangle(x, y)
        elif self.current_tool == "circle":
            self.start_circle(x, y)
        elif self.current_tool == "polygon":
            self.add_polygon_point(x, y)
        elif self.current_tool == "cube":
            self.create_cube(x, y)
    
    def on_mouse_drag(self, event):
        """鼠标拖拽事件"""
        x, y = event.x, event.y
        
        if self.current_tool == "select" and self.is_dragging:
            self.handle_drag(x, y)
        elif self.is_drawing and self.current_tool in ["line", "rectangle", "circle"]:
            self.update_temp_shape(x, y)
    
    def on_mouse_up(self, event):
        """鼠标释放事件"""
        x, y = event.x, event.y
        
        if self.current_tool == "select":
            self.is_dragging = False
        elif self.is_drawing and self.current_tool in ["line", "rectangle", "circle"]:
            self.finish_shape(x, y)
    
    def on_double_click(self, event):
        """双击事件 - 完成多边形绘制"""
        if self.current_tool == "polygon" and len(self.polygon_points) >= 3:
            self.finish_polygon()
    
    def on_key_press(self, event):
        """键盘事件"""
        if event.keysym == "Delete" and self.selected_shape:
            self.delete_selected()
        elif event.keysym == "Escape":
            if self.current_tool == "polygon":
                self.polygon_points = []
                self.redraw()
        elif event.keysym in ("Up", "Down", "Left", "Right"):
            # 方向键微移选中图形
            step = 10
            if self.selected_shape:
                dx = dy = 0
                if event.keysym == "Up":
                    dy = -step
                elif event.keysym == "Down":
                    dy = step
                elif event.keysym == "Left":
                    dx = -step
                elif event.keysym == "Right":
                    dx = step
                self.selected_shape.move(dx, dy)
                self.redraw()
    
    def handle_select_tool(self, x: float, y: float):
        """处理选择工具"""
        # 查找点击的图形
        clicked_shape = self.find_shape_at_point(x, y)
        
        if clicked_shape:
            if clicked_shape != self.selected_shape:
                self.deselect_all()
                self.selected_shape = clicked_shape
                clicked_shape.selected = True
            
            # 开始拖拽
            self.is_dragging = True
            self.drag_start_x = x
            self.drag_start_y = y
        else:
            self.deselect_all()
        
        self.redraw()
    
    def handle_drag(self, x: float, y: float):
        """处理拖拽"""
        if self.selected_shape:
            dx = x - self.drag_start_x
            dy = y - self.drag_start_y
            
            if isinstance(self.selected_shape, Cube):
                # 对立方体，拖拽是旋转
                self.selected_shape.rotate(dx, dy)
            else:
                # 对其他图形，拖拽是移动
                self.selected_shape.move(dx, dy)
                
            self.drag_start_x = x
            self.drag_start_y = y
            self.redraw()
    
    def find_shape_at_point(self, x: float, y: float) -> Optional[Shape]:
        """查找指定点处的图形"""
        # 从后往前查找（最后绘制的在最上层）
        for shape in reversed(self.shapes):
            if shape.contains_point(x, y):
                return shape
        return None

    def nudge_selected(self, dx: float, dy: float):
        """通过按钮或快捷键微移当前选中图形"""
        if self.selected_shape:
            self.selected_shape.move(dx, dy)
            self.redraw()
    
    def create_point(self, x: float, y: float):
        """创建点"""
        point = Point(x, y, self.current_color)
        self.shapes.append(point)
        self.redraw()
    
    def create_cube(self, x: float, y: float):
        """创建立方体"""
        # z=0, size=50 for default
        cube = Cube(x, y, 0, 50, self.current_color, self.current_line_width)
        self.shapes.append(cube)
        self.redraw()

    def start_line(self, x: float, y: float):
        """开始绘制直线"""
        self.is_drawing = True
        self.start_x = x
        self.start_y = y
        self.temp_shape = Line(x, y, x, y, self.current_color, self.current_line_width)
    
    def start_rectangle(self, x: float, y: float):
        """开始绘制矩形"""
        self.is_drawing = True
        self.start_x = x
        self.start_y = y
        self.temp_shape = Rectangle(x, y, 0, 0, self.current_color, 
                                   self.current_line_width, self.current_fill_color)
    
    def start_circle(self, x: float, y: float):
        """开始绘制圆形"""
        self.is_drawing = True
        self.start_x = x
        self.start_y = y
        self.temp_shape = Circle(x, y, 0, self.current_color, 
                                self.current_line_width, self.current_fill_color)
    
    def add_polygon_point(self, x: float, y: float):
        """添加多边形顶点"""
        self.polygon_points.append((x, y))
        self.redraw()
        
        # 绘制临时点
        self.canvas.create_oval(x-3, y-3, x+3, y+3, fill="red", outline="red")
        
        # 绘制临时线段
        if len(self.polygon_points) > 1:
            prev_x, prev_y = self.polygon_points[-2]
            self.canvas.create_line(prev_x, prev_y, x, y, fill="gray", dash=(2, 2))
    
    def update_temp_shape(self, x: float, y: float):
        """更新临时图形"""
        if not self.temp_shape:
            return
        
        if isinstance(self.temp_shape, Line):
            self.temp_shape.x2 = x
            self.temp_shape.y2 = y
        elif isinstance(self.temp_shape, Rectangle):
            width = abs(x - self.start_x)
            height = abs(y - self.start_y)
            self.temp_shape.x = min(self.start_x, x)
            self.temp_shape.y = min(self.start_y, y)
            self.temp_shape.width = width
            self.temp_shape.height = height
        elif isinstance(self.temp_shape, Circle):
            radius = ((x - self.start_x)**2 + (y - self.start_y)**2)**0.5
            self.temp_shape.radius = radius
        
        self.redraw()
        # 绘制临时图形
        if self.temp_shape:
            self.temp_shape.draw(self.canvas)
    
    def finish_shape(self, x: float, y: float):
        """完成图形绘制"""
        if self.temp_shape:
            self.update_temp_shape(x, y)
            
            # 检查图形是否有效
            if self.is_valid_shape(self.temp_shape):
                self.shapes.append(self.temp_shape)
            
            self.temp_shape = None
            self.is_drawing = False
            self.redraw()
    
    def finish_polygon(self):
        """完成多边形绘制"""
        if len(self.polygon_points) >= 3:
            polygon = Polygon(self.polygon_points.copy(), self.current_color,
                             self.current_line_width, self.current_fill_color)
            self.shapes.append(polygon)
        
        self.polygon_points = []
        self.redraw()
    
    def is_valid_shape(self, shape: Shape) -> bool:
        """检查图形是否有效"""
        if isinstance(shape, Line):
            return (abs(shape.x2 - shape.x) > 1 or abs(shape.y2 - shape.y) > 1)
        elif isinstance(shape, Rectangle):
            return shape.width > 1 and shape.height > 1
        elif isinstance(shape, Circle):
            return shape.radius > 1
        return True
    
    def deselect_all(self):
        """取消所有选择"""
        for shape in self.shapes:
            shape.selected = False
        self.selected_shape = None
    
    def delete_selected(self):
        """删除选中的图形"""
        if self.selected_shape and self.selected_shape in self.shapes:
            self.shapes.remove(self.selected_shape)
            self.selected_shape = None
            self.redraw()
    
    def clear_canvas(self):
        """清空画布"""
        self.shapes.clear()
        self.selected_shape = None
        self.polygon_points = []
        self.is_drawing = False
        self.temp_shape = None
        self.redraw()
    
    def scale_selected(self, factor: float):
        """缩放选中的图形"""
        if self.selected_shape:
            bounds = self.selected_shape.get_bounds()
            center_x = (bounds[0] + bounds[2]) / 2
            center_y = (bounds[1] + bounds[3]) / 2
            self.selected_shape.scale(factor, center_x, center_y)
            self.redraw()
    
    def redraw(self):
        """重新绘制所有图形"""
        self.canvas.delete("all")
        
        # 绘制所有图形
        for shape in self.shapes:
            shape.draw(self.canvas)
        
        # 绘制多边形临时点和线段
        if self.current_tool == "polygon" and self.polygon_points:
            for i, (x, y) in enumerate(self.polygon_points):
                self.canvas.create_oval(x-3, y-3, x+3, y+3, fill="red", outline="red")
                if i > 0:
                    prev_x, prev_y = self.polygon_points[i-1]
                    self.canvas.create_line(prev_x, prev_y, x, y, fill="gray", dash=(2, 2))
    
    def get_shapes(self) -> List[Shape]:
        """获取所有图形"""
        return self.shapes.copy()
    
    def set_shapes(self, shapes: List[Shape]):
        """设置图形列表"""
        self.shapes = shapes
        self.selected_shape = None
        self.redraw()
    
    def export_to_image(self, filename: str):
        """导出为图片"""
        try:
            from PIL import Image, ImageDraw
            
            # 创建PIL图像
            img = Image.new('RGB', (self.width, self.height), 'white')
            draw = ImageDraw.Draw(img)
            
            # 绘制所有图形
            for shape in self.shapes:
                self._draw_shape_on_pil(draw, shape)
            
            img.save(filename)
            return True
        except Exception as e:
            messagebox.showerror("导出错误", f"导出图片失败: {str(e)}")
            return False
    
    def _draw_shape_on_pil(self, draw, shape: Shape):
        """在PIL图像上绘制图形"""
        try:
            if isinstance(shape, Point):
                x, y = shape.x, shape.y
                draw.ellipse([x-shape.size, y-shape.size, 
                             x+shape.size, y+shape.size], 
                            fill=shape.color, outline=shape.color)
            
            elif isinstance(shape, Line):
                draw.line([shape.x, shape.y, shape.x2, shape.y2], 
                         fill=shape.color, width=shape.line_width)
            
            elif isinstance(shape, Rectangle):
                x1, y1 = shape.x, shape.y
                x2, y2 = shape.x + shape.width, shape.y + shape.height
                draw.rectangle([x1, y1, x2, y2], 
                              fill=shape.fill_color, outline=shape.color,
                              width=shape.line_width)
            
            elif isinstance(shape, Circle):
                x, y, r = shape.x, shape.y, shape.radius
                draw.ellipse([x-r, y-r, x+r, y+r], 
                            fill=shape.fill_color, outline=shape.color,
                            width=shape.line_width)
            
            elif isinstance(shape, Polygon):
                if len(shape.points) >= 3:
                    points = [(p[0], p[1]) for p in shape.points]
                    draw.polygon(points, 
                               fill=shape.fill_color, outline=shape.color,
                               width=shape.line_width)
        except Exception as e:
            print(f"绘制图形时出错: {e}")