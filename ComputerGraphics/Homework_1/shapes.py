"""
图形类模块 - 定义基本二维图形
"""
import math
import json
from abc import ABC, abstractmethod
from typing import List, Tuple, Dict, Any, Optional


class Shape(ABC):
    """抽象图形基类"""
    
    def __init__(self, x: float, y: float, color: str = "black", 
                 line_width: int = 1, fill_color: str = None):
        self.x = x
        self.y = y
        self.color = color  # 线条颜色
        self.line_width = line_width
        self.fill_color = fill_color  # 填充颜色
        self.selected = False
        
    @abstractmethod
    def draw(self, canvas) -> None:
        """在画布上绘制图形"""
        pass
    
    @abstractmethod
    def contains_point(self, x: float, y: float) -> bool:
        """判断点是否在图形内"""
        pass
    
    @abstractmethod
    def move(self, dx: float, dy: float) -> None:
        """移动图形"""
        pass
    
    @abstractmethod
    def get_bounds(self) -> Tuple[float, float, float, float]:
        """获取图形边界 (min_x, min_y, max_x, max_y)"""
        pass
    
    @abstractmethod
    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        """缩放图形"""
        pass
    
    @abstractmethod
    def to_dict(self) -> Dict[str, Any]:
        """转换为字典格式用于序列化"""
        pass
    
    @classmethod
    @abstractmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Shape':
        """从字典创建图形对象"""
        pass


class Point(Shape):
    """点"""
    
    def __init__(self, x: float, y: float, color: str = "black", size: int = 3):
        super().__init__(x, y, color)
        self.size = size
    
    def draw(self, canvas):
        """绘制点"""
        x1, y1 = self.x - self.size, self.y - self.size
        x2, y2 = self.x + self.size, self.y + self.size
        outline_color = "red" if self.selected else self.color
        canvas.create_oval(x1, y1, x2, y2, 
                          fill=self.color, outline=outline_color, width=2)
    
    def contains_point(self, x: float, y: float) -> bool:
        """判断点击位置是否在点附近"""
        distance = math.sqrt((x - self.x)**2 + (y - self.y)**2)
        return distance <= self.size + 3
    
    def move(self, dx: float, dy: float):
        """移动点"""
        self.x += dx
        self.y += dy
    
    def get_bounds(self) -> Tuple[float, float, float, float]:
        """获取点的边界"""
        return (self.x - self.size, self.y - self.size, 
                self.x + self.size, self.y + self.size)
    
    def scale(self, factor: float, center_x: float, center_y: float):
        """缩放点（改变大小）"""
        self.size = max(1, int(self.size * factor))
        # 相对于中心点缩放位置
        dx = self.x - center_x
        dy = self.y - center_y
        self.x = center_x + dx * factor
        self.y = center_y + dy * factor
    
    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "Point",
            "x": self.x,
            "y": self.y,
            "color": self.color,
            "size": self.size
        }
    
    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Point':
        return cls(data["x"], data["y"], data["color"], data["size"])


class Line(Shape):
    """直线"""
    
    def __init__(self, x1: float, y1: float, x2: float, y2: float, 
                 color: str = "black", line_width: int = 1):
        super().__init__(x1, y1, color, line_width)
        self.x2 = x2
        self.y2 = y2
    
    def draw(self, canvas):
        """绘制直线"""
        outline_color = "red" if self.selected else self.color
        canvas.create_line(self.x, self.y, self.x2, self.y2, 
                          fill=outline_color, width=self.line_width)
    
    def contains_point(self, x: float, y: float) -> bool:
        """判断点是否在线段附近"""
        # 计算点到线段的距离
        A = self.y2 - self.y
        B = self.x - self.x2
        C = self.x2 * self.y - self.x * self.y2
        
        distance = abs(A * x + B * y + C) / math.sqrt(A**2 + B**2 + 1e-10)
        
        # 检查点是否在线段范围内
        min_x, max_x = min(self.x, self.x2), max(self.x, self.x2)
        min_y, max_y = min(self.y, self.y2), max(self.y, self.y2)
        
        return (distance <= 3 and 
                min_x - 3 <= x <= max_x + 3 and 
                min_y - 3 <= y <= max_y + 3)
    
    def move(self, dx: float, dy: float):
        """移动直线"""
        self.x += dx
        self.y += dy
        self.x2 += dx
        self.y2 += dy
    
    def get_bounds(self) -> Tuple[float, float, float, float]:
        """获取直线边界"""
        return (min(self.x, self.x2), min(self.y, self.y2),
                max(self.x, self.x2), max(self.y, self.y2))
    
    def scale(self, factor: float, center_x: float, center_y: float):
        """缩放直线"""
        # 相对于中心点缩放两个端点
        dx1 = self.x - center_x
        dy1 = self.y - center_y
        dx2 = self.x2 - center_x
        dy2 = self.y2 - center_y
        
        self.x = center_x + dx1 * factor
        self.y = center_y + dy1 * factor
        self.x2 = center_x + dx2 * factor
        self.y2 = center_y + dy2 * factor
    
    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "Line",
            "x1": self.x,
            "y1": self.y,
            "x2": self.x2,
            "y2": self.y2,
            "color": self.color,
            "line_width": self.line_width
        }
    
    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Line':
        return cls(data["x1"], data["y1"], data["x2"], data["y2"], 
                  data["color"], data["line_width"])


class Rectangle(Shape):
    """矩形"""
    
    def __init__(self, x: float, y: float, width: float, height: float,
                 color: str = "black", line_width: int = 1, fill_color: str = None):
        super().__init__(x, y, color, line_width, fill_color)
        self.width = width
        self.height = height
    
    def draw(self, canvas):
        """绘制矩形"""
        x1, y1 = self.x, self.y
        x2, y2 = self.x + self.width, self.y + self.height
        outline_color = "red" if self.selected else self.color
        
        canvas.create_rectangle(x1, y1, x2, y2, 
                               outline=outline_color, 
                               fill=self.fill_color,
                               width=self.line_width)
    
    def contains_point(self, x: float, y: float) -> bool:
        """判断点是否在矩形内"""
        return (self.x <= x <= self.x + self.width and 
                self.y <= y <= self.y + self.height)
    
    def move(self, dx: float, dy: float):
        """移动矩形"""
        self.x += dx
        self.y += dy
    
    def get_bounds(self) -> Tuple[float, float, float, float]:
        """获取矩形边界"""
        return (self.x, self.y, self.x + self.width, self.y + self.height)
    
    def scale(self, factor: float, center_x: float, center_y: float):
        """缩放矩形"""
        # 计算矩形中心
        rect_center_x = self.x + self.width / 2
        rect_center_y = self.y + self.height / 2
        
        # 缩放尺寸
        self.width *= factor
        self.height *= factor
        
        # 缩放位置
        dx = rect_center_x - center_x
        dy = rect_center_y - center_y
        new_center_x = center_x + dx * factor
        new_center_y = center_y + dy * factor
        
        self.x = new_center_x - self.width / 2
        self.y = new_center_y - self.height / 2
    
    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "Rectangle",
            "x": self.x,
            "y": self.y,
            "width": self.width,
            "height": self.height,
            "color": self.color,
            "line_width": self.line_width,
            "fill_color": self.fill_color
        }
    
    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Rectangle':
        return cls(data["x"], data["y"], data["width"], data["height"],
                  data["color"], data["line_width"], data.get("fill_color"))


class Circle(Shape):
    """圆形"""
    
    def __init__(self, x: float, y: float, radius: float,
                 color: str = "black", line_width: int = 1, fill_color: str = None):
        super().__init__(x, y, color, line_width, fill_color)
        self.radius = radius
    
    def draw(self, canvas):
        """绘制圆形"""
        x1, y1 = self.x - self.radius, self.y - self.radius
        x2, y2 = self.x + self.radius, self.y + self.radius
        outline_color = "red" if self.selected else self.color
        
        canvas.create_oval(x1, y1, x2, y2, 
                          outline=outline_color, 
                          fill=self.fill_color,
                          width=self.line_width)
    
    def contains_point(self, x: float, y: float) -> bool:
        """判断点是否在圆内"""
        distance = math.sqrt((x - self.x)**2 + (y - self.y)**2)
        return distance <= self.radius
    
    def move(self, dx: float, dy: float):
        """移动圆形"""
        self.x += dx
        self.y += dy
    
    def get_bounds(self) -> Tuple[float, float, float, float]:
        """获取圆形边界"""
        return (self.x - self.radius, self.y - self.radius,
                self.x + self.radius, self.y + self.radius)
    
    def scale(self, factor: float, center_x: float, center_y: float):
        """缩放圆形"""
        # 缩放半径
        self.radius *= factor
        
        # 缩放位置
        dx = self.x - center_x
        dy = self.y - center_y
        self.x = center_x + dx * factor
        self.y = center_y + dy * factor
    
    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "Circle",
            "x": self.x,
            "y": self.y,
            "radius": self.radius,
            "color": self.color,
            "line_width": self.line_width,
            "fill_color": self.fill_color
        }
    
    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Circle':
        return cls(data["x"], data["y"], data["radius"],
                  data["color"], data["line_width"], data.get("fill_color"))


class Polygon(Shape):
    """多边形"""
    
    def __init__(self, points: List[Tuple[float, float]], 
                 color: str = "black", line_width: int = 1, fill_color: str = None):
        if points:
            center_x = sum(p[0] for p in points) / len(points)
            center_y = sum(p[1] for p in points) / len(points)
        else:
            center_x = center_y = 0
        
        super().__init__(center_x, center_y, color, line_width, fill_color)
        self.points = points
    
    def draw(self, canvas):
        """绘制多边形"""
        if len(self.points) < 3:
            return
        
        outline_color = "red" if self.selected else self.color
        
        # 将点列表展平为坐标列表
        coords = []
        for point in self.points:
            coords.extend([point[0], point[1]])
        
        canvas.create_polygon(coords, 
                             outline=outline_color, 
                             fill=self.fill_color,
                             width=self.line_width)
    
    def contains_point(self, x: float, y: float) -> bool:
        """使用射线法判断点是否在多边形内"""
        if len(self.points) < 3:
            return False
        
        n = len(self.points)
        inside = False
        
        p1x, p1y = self.points[0]
        for i in range(1, n + 1):
            p2x, p2y = self.points[i % n]
            if y > min(p1y, p2y):
                if y <= max(p1y, p2y):
                    if x <= max(p1x, p2x):
                        if p1y != p2y:
                            xinters = (y - p1y) * (p2x - p1x) / (p2y - p1y) + p1x
                        if p1x == p2x or x <= xinters:
                            inside = not inside
            p1x, p1y = p2x, p2y
        
        return inside
    
    def move(self, dx: float, dy: float):
        """移动多边形"""
        self.x += dx
        self.y += dy
        self.points = [(px + dx, py + dy) for px, py in self.points]
    
    def get_bounds(self) -> Tuple[float, float, float, float]:
        """获取多边形边界"""
        if not self.points:
            return (0, 0, 0, 0)
        
        xs = [p[0] for p in self.points]
        ys = [p[1] for p in self.points]
        return (min(xs), min(ys), max(xs), max(ys))
    
    def scale(self, factor: float, center_x: float, center_y: float):
        """缩放多边形"""
        # 缩放所有点
        new_points = []
        for px, py in self.points:
            dx = px - center_x
            dy = py - center_y
            new_x = center_x + dx * factor
            new_y = center_y + dy * factor
            new_points.append((new_x, new_y))
        
        self.points = new_points
        
        # 更新中心点
        if self.points:
            self.x = sum(p[0] for p in self.points) / len(self.points)
            self.y = sum(p[1] for p in self.points) / len(self.points)
    
    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "Polygon",
            "points": self.points,
            "color": self.color,
            "line_width": self.line_width,
            "fill_color": self.fill_color
        }
    
    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Polygon':
        return cls(data["points"], data["color"], 
                  data["line_width"], data.get("fill_color"))


class Cube(Shape):
    """立方体 (3D)"""
    
    def __init__(self, x: float, y: float, z: float, size: float,
                 color: str = "black", line_width: int = 1, fill_color: str = None):
        super().__init__(x, y, color, line_width, fill_color)
        self.z = z
        self.size = size
        
        # 定义8个局部坐标的顶点（以立方体中心为原点）
        s = self.size / 2
        self.vertices = [
            (-s, -s, -s), (s, -s, -s),
            (s, s, -s), (-s, s, -s),
            (-s, -s, s), (s, -s, s),
            (s, s, s), (-s, s, s)
        ]
        
        # 定义12条边
        self.edges = [
            (0, 1), (1, 2), (2, 3), (3, 0),  # back face
            (4, 5), (5, 6), (6, 7), (7, 4),  # front face
            (0, 4), (1, 5), (2, 6), (3, 7)   # connecting edges
        ]
        
        # 旋转参数
        self.rotation_x = self.rotation_y = self.rotation_z = 0
        # 记录最近一次绘制时的投影边界（用于命中测试）
        self._last_bounds = None  # type: Optional[Tuple[float, float, float, float]]

    def _project_local(self, x, y, z):
        """等轴测投影（局部坐标 -> 屏幕偏移）"""
        cos30 = 0.8660254037844386
        iso_x = (x - z) * cos30
        iso_y = y + (x + z) * 0.5
        return iso_x, -iso_y

    def draw(self, canvas):
        """绘制立方体"""
        outline_color = "red" if self.selected else self.color
        
        # 旋转顶点
        rotated_vertices = self._get_rotated_vertices()
        
        # 投影到2D并平移到屏幕中心 (self.x, self.y)
        projected_points = []
        for lx, ly, lz in rotated_vertices:
            px, py = self._project_local(lx, ly, lz)
            projected_points.append((self.x + px, self.y + py))
            
        # 绘制边
        for edge in self.edges:
            p1 = projected_points[edge[0]]
            p2 = projected_points[edge[1]]
            canvas.create_line(p1[0], p1[1], p2[0], p2[1], 
                              fill=outline_color, width=self.line_width)
        # 计算并缓存当前画布尺寸下的投影边界，供 contains_point 使用
        if projected_points:
            xs = [p[0] for p in projected_points]
            ys = [p[1] for p in projected_points]
            self._last_bounds = (min(xs), min(ys), max(xs), max(ys))

    def _get_rotated_vertices(self):
        """获取旋转后的顶点坐标"""
        rotated_vertices = []
        
        # 旋转矩阵
        cos_x, sin_x = math.cos(self.rotation_x), math.sin(self.rotation_x)
        cos_y, sin_y = math.cos(self.rotation_y), math.sin(self.rotation_y)
        cos_z, sin_z = math.cos(self.rotation_z), math.sin(self.rotation_z)

        for x, y, z in self.vertices:
            # Y-axis rotation
            x_new = x * cos_y - z * sin_y
            z_new = x * sin_y + z * cos_y
            x, z = x_new, z_new
            
            # X-axis rotation
            y_new = y * cos_x - z * sin_x
            z_new = y * sin_x + z * cos_x
            y, z = y_new, z_new
            
            # Z-axis rotation
            x_new = x * cos_z - y * sin_z
            y_new = x * sin_z + y * cos_z
            x, y = x_new, y_new
            
            rotated_vertices.append((x, y, z))
            
        return rotated_vertices

    def contains_point(self, x: float, y: float) -> bool:
        """判断点是否在图形内 (使用最近一次绘制的投影边界，并给少量容差)"""
        min_x, min_y, max_x, max_y = self.get_bounds()
        # 提高可选中性：边界外扩一定像素
        margin = 6.0
        return (min_x - margin) <= x <= (max_x + margin) and (min_y - margin) <= y <= (max_y + margin)

    def move(self, dx: float, dy: float):
        """移动立方体中心（屏幕坐标，右/下为正）"""
        self.x += dx
        self.y += dy

    def get_bounds(self) -> Tuple[float, float, float, float]:
        """获取投影后的边界。优先返回最近一次绘制缓存的边界。"""
        if self._last_bounds is not None:
            return self._last_bounds
        # 后备：按当前位置估算（首次绘制前）
        rotated_vertices = self._get_rotated_vertices()
        projected_points = []
        for lx, ly, lz in rotated_vertices:
            px, py = self._project_local(lx, ly, lz)
            projected_points.append((self.x + px, self.y + py))
        if not projected_points:
            return (0, 0, 0, 0)
        xs = [p[0] for p in projected_points]
        ys = [p[1] for p in projected_points]
        return (min(xs), min(ys), max(xs), max(ys))

    def scale(self, factor: float, center_x: float, center_y: float):
        """缩放立方体"""
        self.size *= factor
        s = self.size / 2
        self.vertices = [
            (-s, -s, -s), (s, -s, -s),
            (s, s, -s), (-s, s, -s),
            (-s, -s, s), (s, -s, s),
            (s, s, s), (-s, s, s)
        ]

    def rotate(self, dx: float, dy: float):
        """根据鼠标拖动旋转"""
        self.rotation_y += dx * 0.01
        self.rotation_x += dy * 0.01

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "Cube",
            "x": self.x, "y": self.y, "z": self.z,
            "size": self.size, "color": self.color,
            "line_width": self.line_width,
            "rotation_x": self.rotation_x,
            "rotation_y": self.rotation_y,
            "rotation_z": self.rotation_z
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Cube':
        cube = cls(data["x"], data["y"], data["z"], data["size"],
                   data["color"], data["line_width"])
        cube.rotation_x = data.get("rotation_x", 0)
        cube.rotation_y = data.get("rotation_y", 0)
        cube.rotation_z = data.get("rotation_z", 0)
        return cube


# 图形工厂函数
def create_shape_from_dict(data: Dict[str, Any]) -> Shape:
    """从字典数据创建图形对象"""
    shape_type = data.get("type")
    
    if shape_type == "Point":
        return Point.from_dict(data)
    elif shape_type == "Line":
        return Line.from_dict(data)
    elif shape_type == "Rectangle":
        return Rectangle.from_dict(data)
    elif shape_type == "Circle":
        return Circle.from_dict(data)
    elif shape_type == "Polygon":
        return Polygon.from_dict(data)
    elif shape_type == "Cube":
        return Cube.from_dict(data)
    else:
        raise ValueError(f"Unknown shape type: {shape_type}")