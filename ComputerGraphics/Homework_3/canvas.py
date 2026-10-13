"""
绘图画布组件 - 处理图形绘制和交互
"""
import math
import sys
import tkinter as tk
from tkinter import messagebox
from typing import List, Optional, Tuple, Dict
from shapes import Shape, Point, Line, Rectangle, Circle, Polygon, Cube, Ellipse, BezierCurve, BSplineCurve, BezierSurface, BSplineSurface, HermiteCurve, CatmullRomCurve


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
        self.curve_points = []
        
        # 图形属性
        self.current_color = "black"
        self.current_line_width = 1
        self.current_fill_color = None
        
        # 拖拽状态
        self.is_dragging = False
        self.dragging_handle_index = None # Tuple or int or None
        self.drag_start_x = 0
        self.drag_start_y = 0
        self.is_rotating = False
        self.rotate_last_angle = None
        self.rotation_center = None

        # 显示设置
        self.background_color = "white"
        self.antialiasing = True
        # 像素缓存用于正确的 alpha 复合 (key: (x,y) -> (r,g,b))
        self._pixel_map: Dict[Tuple[int, int], Tuple[int, int, int]] = {}
        # 绘制模式：True=光栅化算法，False=Tk库函数
        self.use_rasterization = True
        self.bind_events()
    
    def bind_events(self):
        """绑定鼠标事件"""
        self.canvas.bind("<Button-1>", self.on_mouse_down)
        self.canvas.bind("<B1-Motion>", self.on_mouse_drag)
        self.canvas.bind("<ButtonRelease-1>", self.on_mouse_up)
        self.canvas.bind("<Double-Button-1>", self.on_double_click)
        self.canvas.bind("<KeyPress>", self.on_key_press)
        
        # 右键菜单
        if sys.platform == "darwin":
            self.canvas.bind("<Button-2>", self.show_context_menu)
            self.canvas.bind("<Control-Button-1>", self.show_context_menu)
        else:
            self.canvas.bind("<Button-3>", self.show_context_menu)
            
        self.canvas.focus_set()
    
    def show_context_menu(self, event):
        """显示上下文菜单"""
        x, y = event.x, event.y
        shape = self.find_shape_at_point(x, y)
        if shape:
            self.deselect_all()
            self.selected_shape = shape
            shape.selected = True
            self.redraw()
            
            if isinstance(shape, (BezierSurface, BSplineSurface)):
                menu = tk.Menu(self.canvas, tearoff=0)
                
                menu.add_command(label="切换网格显示 (" + ("开" if shape.show_mesh else "关") + ")", 
                                 command=lambda: self.toggle_surface_mesh(shape))
                
                fill_state = "关" if shape.fill_method == "none" else "开"
                menu.add_command(label="切换填充模式 (" + fill_state + ")", 
                                 command=lambda: self.toggle_surface_fill(shape))
                
                menu.post(event.x_root, event.y_root)

    def toggle_surface_mesh(self, shape):
        shape.show_mesh = not shape.show_mesh
        self.redraw()

    def toggle_surface_fill(self, shape):
        if shape.fill_method == "none":
            shape.fill_method = "scanline"
            if not shape.fill_colors:
                shape.fill_colors = ("#ff0000", "#0000ff")
        else:
            shape.fill_method = "none"
        self.redraw()

    
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
        self.curve_points = []
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
            # 特殊处理曲面类型
            if isinstance(self.selected_shape, (BezierSurface, BSplineSurface)):
                if self.current_fill_color:
                    self.selected_shape.fill_method = "scanline"
                    # 生成渐变色，模拟光照或颜色衰减
                    # 计算一个变种颜色，例如更暗的颜色
                    try:
                       r, g, b = self.winfo_rgb(self.current_fill_color)
                       # winfo_rgb returns 16-bit values (0-65535)
                       r = r // 256
                       g = g // 256
                       b = b // 256
                       # 创建一个明显更暗的颜色作为渐变终点 (0.3x brightness)，以增强立体感
                       r2 = int(r * 0.3)
                       g2 = int(g * 0.3)
                       b2 = int(b * 0.3)
                       color2 = f"#{r2:02x}{g2:02x}{b2:02x}"
                    except Exception:
                       color2 = "#000000" # fallback to black

                    self.selected_shape.fill_colors = (self.current_fill_color, color2) 
                    
                    # 注意：shapes.py 中的 draw() 方法已更新为当 filling 为 True 时自动隐藏 mesh
                else:
                    self.selected_shape.fill_method = "none"
                    self.selected_shape.fill_colors = None
            else:
                self.selected_shape.fill_color = self.current_fill_color
            
            self.redraw()
    
    def on_mouse_down(self, event):
        """鼠标按下事件"""
        x, y = event.x, event.y
        
        if self.current_tool == "select":
            self.handle_select_tool(x, y, event)
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
        elif self.current_tool == "ellipse":
            self.start_ellipse(x, y)
        elif self.current_tool in ["bezier_curve", "bspline_curve", "catmull_rom_curve"]:
            self.add_curve_point(x, y)
        elif self.current_tool == "hermite_curve":
            self.create_hermite_curve(x, y)
        elif self.current_tool == "bezier_surface":
            self.create_bezier_surface(x, y)
        elif self.current_tool == "bspline_surface":
            self.create_bspline_surface(x, y)
    
    def on_mouse_drag(self, event):
        """鼠标拖拽事件"""
        x, y = event.x, event.y
        
        if self.current_tool == "select" and self.is_dragging:
            self.handle_drag(x, y, event)
        elif self.is_drawing and self.current_tool in ["line", "rectangle", "circle", "ellipse"]:
            self.update_temp_shape(x, y)
    
    def on_mouse_up(self, event):
        """鼠标释放事件"""
        x, y = event.x, event.y
        
        if self.current_tool == "select":
            self.is_dragging = False
            self.dragging_handle_index = None # Reset
            self.is_rotating = False
            self.rotate_last_angle = None
            self.rotation_center = None
        elif self.is_drawing and self.current_tool in ["line", "rectangle", "circle", "ellipse"]:
            self.finish_shape(x, y)
    
    def on_double_click(self, event):
        """双击事件 - 完成多边形绘制"""
        if self.current_tool == "polygon" and len(self.polygon_points) >= 3:
            self.finish_polygon()
        elif self.current_tool in ["bezier_curve", "bspline_curve", "catmull_rom_curve"] and len(self.curve_points) >= 2:
            self.finish_curve()
    
    def on_key_press(self, event):
        """键盘事件"""
        if event.keysym == "Delete" and self.selected_shape:
            self.delete_selected()
        elif event.keysym == "Escape":
            if self.current_tool == "polygon":
                self.polygon_points = []
                self.redraw()
            elif self.current_tool in ["bezier_curve", "bspline_curve", "catmull_rom_curve"]:
                self.curve_points = []
                self.redraw()
        elif event.keysym in ("Up", "Down", "Left", "Right"):
            return
    
    def handle_select_tool(self, x: float, y: float, event):
        """处理选择工具"""
        # 0. 检查是否点击了已选中图形的控制点
        self.dragging_handle_index = None
        if self.selected_shape and hasattr(self.selected_shape, "get_handle_index"):
            idx = self.selected_shape.get_handle_index(x, y)
            # handle index could be int or tuple (-1, -1) or -1
            is_hit = False
            if isinstance(idx, int) and idx != -1:
                is_hit = True
            elif isinstance(idx, tuple) and idx != (-1, -1):
                is_hit = True
            
            if is_hit:
                self.dragging_handle_index = idx
                self.is_dragging = True
                self.drag_start_x = x
                self.drag_start_y = y
                return

        # 1. 查找点击的图形
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
            self.rotation_center = self._get_shape_center(clicked_shape)
            self.rotate_last_angle = self._calculate_pointer_angle(self.rotation_center, x, y)
            self.is_rotating = self._is_rotation_modifier(event)
        else:
            self.deselect_all()
            self.is_rotating = False
            self.rotate_last_angle = None
            self.rotation_center = None
        
        self.redraw()
    
    def handle_drag(self, x: float, y: float, event):
        """处理拖拽"""
        if self.selected_shape:
            # 如果正在拖拽控制点
            if self.dragging_handle_index is not None and hasattr(self.selected_shape, "update_handle"):
                self.selected_shape.update_handle(self.dragging_handle_index, x, y)
                self.redraw()
                return

            dx = x - self.drag_start_x
            dy = y - self.drag_start_y
            
            if self.is_rotating:
                if isinstance(self.selected_shape, Cube):
                    self.selected_shape.rotate(dx, dy)
                else:
                    if not self.rotation_center:
                        self.rotation_center = self._get_shape_center(self.selected_shape)
                    current_angle = self._calculate_pointer_angle(self.rotation_center, x, y)
                    if self.rotate_last_angle is not None and current_angle is not None:
                        delta = current_angle - self.rotate_last_angle
                        if delta > math.pi:
                            delta -= 2 * math.pi
                        elif delta < -math.pi:
                            delta += 2 * math.pi
                        self.selected_shape.rotate(delta, pivot=self.rotation_center)
                    self.rotate_last_angle = current_angle
            else:
                self.selected_shape.move(dx, dy)
                
            self.drag_start_x = x
            self.drag_start_y = y
            self.redraw()

    def _is_rotation_modifier(self, event) -> bool:
        """判断当前事件是否触发旋转模式（按住Shift）"""
        if event is None:
            return False
        # Tk事件state中的最低位表示Shift键
        return bool(event.state & 0x0001)

    def _calculate_pointer_angle(self, center: Optional[Tuple[float, float]], x: float, y: float) -> Optional[float]:
        """计算指针相对中心的角度"""
        if center is None:
            return None
        cx, cy = center
        dx = x - cx
        dy = y - cy
        if abs(dx) < 1e-6 and abs(dy) < 1e-6:
            return None
        return math.atan2(dy, dx)

    def _get_shape_center(self, shape: Shape) -> Tuple[float, float]:
        """获取图形的旋转中心"""
        return shape.get_center()
    
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
    
    def add_curve_point(self, x: float, y: float):
        """添加曲线控制点"""
        self.curve_points.append((x, y))
        self.redraw()
        # 绘制临时标记
        for i, (px, py) in enumerate(self.curve_points):
            self.canvas.create_oval(px-3, py-3, px+3, py+3, fill="blue")
            if i > 0:
                prev = self.curve_points[i-1]
                self.canvas.create_line(prev[0], prev[1], px, py, fill="gray", dash=(2,2))

    def finish_curve(self):
        """完成曲线绘制"""
        if len(self.curve_points) >= 2:
            if self.current_tool == "bezier_curve":
                curve = BezierCurve(list(self.curve_points), self.current_color, self.current_line_width)
                self.shapes.append(curve)
            elif self.current_tool == "bspline_curve":
                curve = BSplineCurve(list(self.curve_points), 3, self.current_color, self.current_line_width)
                self.shapes.append(curve)
            elif self.current_tool == "catmull_rom_curve":
                 if len(self.curve_points) >= 4:
                    curve = CatmullRomCurve(list(self.curve_points), self.current_color, self.current_line_width)
                    self.shapes.append(curve)
                 else:
                     messagebox.showwarning("提示", "Catmull-Rom曲线至少需要4个控制点")

        self.curve_points = []
        self.redraw()

    def create_hermite_curve(self, x: float, y: float):
        """创建Hermite曲线（默认形状）"""
        # Create a default 'S' shape or similar
        p1 = (x, y)
        p2 = (x + 200, y - 50)
        t1 = (100, -100) # Vector
        t2 = (100, 100)  # Vector
        curve = HermiteCurve(p1, p2, t1, t2, self.current_color, self.current_line_width)
        self.shapes.append(curve)
        self.set_tool("select")
        self.redraw()

    def create_bezier_surface(self, x: float, y: float):
        """创建Bézier曲面"""
        # 创建一个默认的 4x4 网格
        step = 60
        start_x = x - 1.5 * step
        start_y = y - 1.5 * step
        points = []
        for i in range(4):
            row = []
            for j in range(4):
                # z 值设为0，或者稍微有些起伏
                z = 0
                if 1 <= i <= 2 and 1 <= j <= 2:
                    z = 50 # 中间隆起
                row.append((start_x + i*step, start_y + j*step, z))
            points.append(row)
        
        surface = BezierSurface(points, self.current_color)
        self.shapes.append(surface)
        self.set_tool("select") # 创建后自动切换到选择工具以便调整
        self.redraw()

    def create_bspline_surface(self, x: float, y: float):
        """创建B样条曲面"""
        # 创建一个默认的 4x4 网格
        step = 60
        start_x = x - 1.5 * step
        start_y = y - 1.5 * step
        points = []
        for i in range(4):
            row = []
            for j in range(4):
                # z 值设为0，或者稍微有些起伏
                z = 0
                if 1 <= i <= 2 and 1 <= j <= 2:
                    z = 50 # 中间隆起
                row.append((start_x + i*step, start_y + j*step, z))
            points.append(row)
        
        surface = BSplineSurface(points, self.current_color)
        self.shapes.append(surface)
        self.set_tool("select") 
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
    
    def start_ellipse(self, x: float, y: float):
        """开始绘制椭圆"""
        self.is_drawing = True
        self.start_x = x
        self.start_y = y
        self.temp_shape = Ellipse(x, y, 0, 0, self.current_color,
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
        elif isinstance(self.temp_shape, Ellipse):
            rx = abs(x - self.start_x) / 2
            ry = abs(y - self.start_y) / 2
            cx = (self.start_x + x) / 2
            cy = (self.start_y + y) / 2
            self.temp_shape.x = cx
            self.temp_shape.y = cy
            self.temp_shape.rx = rx
            self.temp_shape.ry = ry
        
        self.redraw()
        # 绘制临时图形
        if self.temp_shape:
            self.temp_shape.draw(self)
    
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
        elif isinstance(shape, Ellipse):
            return shape.rx > 1 and shape.ry > 1
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
        # 清空像素缓存，防止残留半透明像素影响后续绘制
        self._pixel_map = {}
        self.redraw()
    
    def scale_selected(self, factor: float):
        """缩放选中的图形"""
        if self.selected_shape:
            bounds = self.selected_shape.get_bounds()
            center_x = (bounds[0] + bounds[2]) / 2
            center_y = (bounds[1] + bounds[3]) / 2
            self.selected_shape.scale(factor, center_x, center_y)
            self.redraw()

    def rotate_selected(self, angle: float) -> bool:
        """围绕自身中心旋转选中的图形"""
        if not self.selected_shape:
            return False

        shape = self.selected_shape
        if isinstance(shape, Cube):
            # Cube.rotate 使用像素位移模拟角度，这里进行简单换算
            delta = angle / 0.01
            shape.rotate(delta, 0)
        else:
            pivot = self._get_shape_center(shape)
            shape.rotate(angle, pivot=pivot)

        self.redraw()
        return True
    
    def redraw(self):
        """重新绘制所有图形"""
        # 在每次重绘前清空画布及像素缓存（像素缓存在绘制过程中被逐像素更新）
        self.canvas.delete("all")
        self._pixel_map = {}
        
        # 绘制所有图形
        for shape in self.shapes:
            shape.draw(self)
        
        # 绘制多边形临时点和线段
        points_to_draw = []
        if self.current_tool == "polygon":
            points_to_draw = self.polygon_points
        elif self.current_tool in ["bezier_curve", "bspline_curve", "catmull_rom_curve"]:
            points_to_draw = self.curve_points
            
        if points_to_draw:
            for i, (x, y) in enumerate(points_to_draw):
                self.canvas.create_oval(x-3, y-3, x+3, y+3, fill="red", outline="red")
                if i > 0:
                    prev_x, prev_y = points_to_draw[i-1]
                    self.canvas.create_line(prev_x, prev_y, x, y, fill="gray", dash=(2, 2))
    
    def get_shapes(self) -> List[Shape]:
        """获取所有图形"""
        return self.shapes.copy()
    
    def set_shapes(self, shapes: List[Shape]):
        """设置图形列表"""
        self.shapes = shapes
        self.selected_shape = None
        # 重置像素缓存并重绘
        self._pixel_map = {}
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
    
    def winfo_rgb(self, color):
        """代理调用 tk.Canvas 的 winfo_rgb"""
        return self.canvas.winfo_rgb(color)

    def draw_pixel(self, x, y, color, alpha: float = 1.0):
        """在指定坐标绘制一个像素点，可选透明度用于反走样"""
        if alpha <= 0:
            return
        # 使用画布的实际当前尺寸判断边界（支持窗口/画布被放大）
        cw = self.canvas.winfo_width()
        ch = self.canvas.winfo_height()
        if cw <= 1:
            cw = self.width
        if ch <= 1:
            ch = self.height

        if x < 0 or y < 0 or x >= cw or y >= ch:
            return

        opacity = max(0.0, min(1.0, alpha))

        # 将颜色与当前像素进行正确的 alpha 复合（而不是每次都与背景混合）
        fg_r, fg_g, fg_b = self._color_to_rgb(color)
        key = (int(x), int(y))
        existing = self._pixel_map.get(key)
        if existing is None:
            existing = self._color_to_rgb(self.background_color)

        r = int(round(fg_r * opacity + existing[0] * (1 - opacity)))
        g = int(round(fg_g * opacity + existing[1] * (1 - opacity)))
        b = int(round(fg_b * opacity + existing[2] * (1 - opacity)))

        self._pixel_map[key] = (r, g, b)
        hexcol = f"#{r:02x}{g:02x}{b:02x}"
        self.canvas.create_rectangle(key[0], key[1], key[0] + 1, key[1] + 1, fill=hexcol, outline=hexcol)

    def _blend_with_background(self, color: str, alpha: float) -> str:
        fg_r, fg_g, fg_b = self._color_to_rgb(color)
        bg_r, bg_g, bg_b = self._color_to_rgb(self.background_color)
        r = int(round(fg_r * alpha + bg_r * (1 - alpha)))
        g = int(round(fg_g * alpha + bg_g * (1 - alpha)))
        b = int(round(fg_b * alpha + bg_b * (1 - alpha)))
        return f"#{r:02x}{g:02x}{b:02x}"

    def _color_to_rgb(self, color: str) -> Tuple[int, int, int]:
        r, g, b = self.canvas.winfo_rgb(color)
        return r // 256, g // 256, b // 256