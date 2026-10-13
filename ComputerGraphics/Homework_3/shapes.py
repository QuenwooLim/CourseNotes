"""
图形类模块 - 定义基本二维图形
"""
import math
from abc import ABC, abstractmethod
from typing import Any, Dict, List, Optional, Tuple

from rasterization import (
    draw_circle_antialiased,
    draw_circle_midpoint,
    draw_line_bresenham,
    draw_line_wu,
    draw_bezier_curve,
    draw_bspline_curve,
    draw_bezier_surface,
    draw_hermite_curve,
    draw_catmull_rom_curve,
    draw_bspline_surface,
)

from rasterization import draw_ellipse_midpoint, draw_ellipse_antialiased


def rotate_point(px: float, py: float, cx: float, cy: float, angle: float) -> Tuple[float, float]:
    """围绕指定中心旋转点"""
    cos_a = math.cos(angle)
    sin_a = math.sin(angle)
    dx = px - cx
    dy = py - cy
    return (cx + dx * cos_a - dy * sin_a, cy + dx * sin_a + dy * cos_a)


class Shape(ABC):
    """抽象图形基类"""

    def __init__(
        self,
        x: float,
        y: float,
        color: str = "black",
        line_width: int = 1,
        fill_color: Optional[str] = None,
    ) -> None:
        self.x = x
        self.y = y
        self.color = color
        self.line_width = line_width
        self.fill_color = fill_color
        self.selected = False

    @abstractmethod
    def draw(self, canvas) -> None:
        """在画布上绘制图形"""

    @abstractmethod
    def contains_point(self, x: float, y: float) -> bool:
        """判断点是否在图形内"""

    @abstractmethod
    def move(self, dx: float, dy: float) -> None:
        """移动图形"""

    @abstractmethod
    def get_bounds(self) -> Tuple[float, float, float, float]:
        """获取图形边界 (min_x, min_y, max_x, max_y)"""

    @abstractmethod
    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        """缩放图形"""

    def get_center(self) -> Tuple[float, float]:
        min_x, min_y, max_x, max_y = self.get_bounds()
        return ((min_x + max_x) / 2, (min_y + max_y) / 2)

    def rotate(self, angle: float, pivot: Optional[Tuple[float, float]] = None) -> None:
        raise NotImplementedError(f"{self.__class__.__name__} 未实现旋转逻辑")

    @abstractmethod
    def to_dict(self) -> Dict[str, Any]:
        """转换为字典格式用于序列化"""

    @classmethod
    @abstractmethod
    def from_dict(cls, data: Dict[str, Any]) -> "Shape":
        """从字典创建图形对象"""


class Point(Shape):
    """点"""

    def __init__(self, x: float, y: float, color: str = "black", size: int = 3):
        super().__init__(x, y, color)
        self.size = size

    def draw(self, canvas) -> None:
        x1, y1 = self.x - self.size, self.y - self.size
        x2, y2 = self.x + self.size, self.y + self.size
        outline_color = "red" if self.selected else self.color
        if getattr(canvas, "use_rasterization", True):
            # 使用光栅化绘制点：绘制一个小的正方形区域
            for py in range(int(y1), int(y2) + 1):
                for px in range(int(x1), int(x2) + 1):
                    canvas.draw_pixel(px, py, self.color)
        else:
            # 使用Tk库函数
            canvas.canvas.create_oval(
                x1,
                y1,
                x2,
                y2,
                fill=self.color,
                outline=outline_color,
                width=2,
            )

    def contains_point(self, x: float, y: float) -> bool:
        distance = math.hypot(x - self.x, y - self.y)
        return distance <= self.size + 3

    def move(self, dx: float, dy: float) -> None:
        self.x += dx
        self.y += dy

    def get_bounds(self) -> Tuple[float, float, float, float]:
        return (self.x - self.size, self.y - self.size, self.x + self.size, self.y + self.size)

    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        self.size = max(1, int(self.size * factor))
        dx = self.x - center_x
        dy = self.y - center_y
        self.x = center_x + dx * factor
        self.y = center_y + dy * factor

    def get_center(self) -> Tuple[float, float]:
        return (self.x, self.y)

    def rotate(self, angle: float, pivot: Optional[Tuple[float, float]] = None) -> None:
        if pivot is None:
            pivot = self.get_center()
        self.x, self.y = rotate_point(self.x, self.y, pivot[0], pivot[1], angle)

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "Point",
            "x": self.x,
            "y": self.y,
            "color": self.color,
            "size": self.size,
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "Point":
        return cls(data["x"], data["y"], data["color"], data["size"])


class Line(Shape):
    """直线"""

    def __init__(self, x1: float, y1: float, x2: float, y2: float, color: str = "black", line_width: int = 1):
        super().__init__(x1, y1, color, line_width)
        self.x2 = x2
        self.y2 = y2

    def draw(self, canvas) -> None:
        if getattr(canvas, "use_rasterization", True):
            if getattr(canvas, "antialiasing", False) and self.line_width == 1:
                draw_line_wu(canvas, self.x, self.y, self.x2, self.y2, self.color)
            else:
                draw_line_bresenham(canvas, int(self.x), int(self.y), int(self.x2), int(self.y2), self.color, self.line_width)
        else:
            # 使用Tk库函数
            outline_color = "red" if self.selected else self.color
            canvas.canvas.create_line(
                self.x, self.y, self.x2, self.y2,
                fill=outline_color, width=self.line_width
            )
        if self.selected:
            min_x, min_y, max_x, max_y = self.get_bounds()
            canvas.canvas.create_rectangle(
                min_x - 2,
                min_y - 2,
                max_x + 2,
                max_y + 2,
                outline="red",
                dash=(2, 2),
            )

    def contains_point(self, x: float, y: float) -> bool:
        A = self.y2 - self.y
        B = self.x - self.x2
        C = self.x2 * self.y - self.x * self.y2
        distance = abs(A * x + B * y + C) / math.sqrt(A * A + B * B + 1e-10)
        min_x, max_x = min(self.x, self.x2), max(self.x, self.x2)
        min_y, max_y = min(self.y, self.y2), max(self.y, self.y2)
        return distance <= 3 and min_x - 3 <= x <= max_x + 3 and min_y - 3 <= y <= max_y + 3

    def move(self, dx: float, dy: float) -> None:
        self.x += dx
        self.y += dy
        self.x2 += dx
        self.y2 += dy

    def get_bounds(self) -> Tuple[float, float, float, float]:
        return (min(self.x, self.x2), min(self.y, self.y2), max(self.x, self.x2), max(self.y, self.y2))

    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        dx1 = self.x - center_x
        dy1 = self.y - center_y
        dx2 = self.x2 - center_x
        dy2 = self.y2 - center_y
        self.x = center_x + dx1 * factor
        self.y = center_y + dy1 * factor
        self.x2 = center_x + dx2 * factor
        self.y2 = center_y + dy2 * factor

    def get_center(self) -> Tuple[float, float]:
        return ((self.x + self.x2) / 2, (self.y + self.y2) / 2)

    def rotate(self, angle: float, pivot: Optional[Tuple[float, float]] = None) -> None:
        if pivot is None:
            pivot = self.get_center()
        self.x, self.y = rotate_point(self.x, self.y, pivot[0], pivot[1], angle)
        self.x2, self.y2 = rotate_point(self.x2, self.y2, pivot[0], pivot[1], angle)

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "Line",
            "x1": self.x,
            "y1": self.y,
            "x2": self.x2,
            "y2": self.y2,
            "color": self.color,
            "line_width": self.line_width,
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "Line":
        return cls(
            data["x1"],
            data["y1"],
            data["x2"],
            data["y2"],
            data["color"],
            data["line_width"],
        )


class Rectangle(Shape):
    """矩形（支持旋转）"""

    def __init__(
        self,
        x: float,
        y: float,
        width: float,
        height: float,
        color: str = "black",
        line_width: int = 1,
        fill_color: Optional[str] = None,
    ) -> None:
        super().__init__(x, y, color, line_width, fill_color)
        self.width = width
        self.height = height
        self.rotation = 0.0

    def draw(self, canvas) -> None:
        outline_color = "red" if self.selected else self.color
        if getattr(canvas, "use_rasterization", True):
            # 使用光栅化算法绘制矩形轮廓（避免直接调用 Tk 的 create_rectangle/create_polygon）
            if abs(self.rotation) < 1e-6:
                # 未旋转：四个角为轴对齐
                p0 = (self.x, self.y)
                p1 = (self.x + self.width, self.y)
                p2 = (self.x + self.width, self.y + self.height)
                p3 = (self.x, self.y + self.height)
                verts = [p0, p1, p2, p3]
            else:
                # 旋转矩形：使用变换后的顶点
                verts = self._get_vertices()

            # 绘制四条边，考虑抗锯齿开关
            for i in range(len(verts)):
                a = verts[i]
                b = verts[(i + 1) % len(verts)]
                if getattr(canvas, "antialiasing", False) and self.line_width == 1:
                    draw_line_wu(canvas, a[0], a[1], b[0], b[1], outline_color)
                else:
                    draw_line_bresenham(canvas, int(round(a[0])), int(round(a[1])), int(round(b[0])), int(round(b[1])), outline_color, self.line_width)

            # 填充：使用扫描线填充（复用 Polygon.scanline_fill 的实现）
            if self.fill_color:
                try:
                    poly = Polygon(verts, self.color, self.line_width, self.fill_color)
                    poly.scanline_fill(canvas, self.fill_color)
                except Exception:
                    # 回退：如果出现问题，使用 Tk 的多边形填充（保证不会丢失填充）
                    flat_points = [coord for point in verts for coord in point]
                    canvas.canvas.create_polygon(
                        flat_points,
                        outline="",
                        fill=self.fill_color,
                    )
        else:
            # 使用Tk库函数
            if abs(self.rotation) < 1e-6:
                x1, y1 = self.x, self.y
                x2, y2 = self.x + self.width, self.y + self.height
                canvas.canvas.create_rectangle(
                    x1,
                    y1,
                    x2,
                    y2,
                    outline=outline_color,
                    fill=self.fill_color,
                    width=self.line_width,
                )
            else:
                points = self._get_vertices()
                flat_points = [coord for point in points for coord in point]
                canvas.canvas.create_polygon(
                    flat_points,
                    outline=outline_color,
                    fill=self.fill_color or "",
                    width=self.line_width,
                )

    def contains_point(self, x: float, y: float) -> bool:
        cx, cy = self.get_center()
        cos_a = math.cos(-self.rotation)
        sin_a = math.sin(-self.rotation)
        dx = x - cx
        dy = y - cy
        local_x = dx * cos_a - dy * sin_a
        local_y = dx * sin_a + dy * cos_a
        half_w = self.width / 2
        half_h = self.height / 2
        return -half_w <= local_x <= half_w and -half_h <= local_y <= half_h

    def move(self, dx: float, dy: float) -> None:
        self.x += dx
        self.y += dy

    def get_bounds(self) -> Tuple[float, float, float, float]:
        points = self._get_vertices()
        xs = [p[0] for p in points]
        ys = [p[1] for p in points]
        return (min(xs), min(ys), max(xs), max(ys))

    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        cx, cy = self.get_center()
        self.width *= factor
        self.height *= factor
        dx = cx - center_x
        dy = cy - center_y
        new_cx = center_x + dx * factor
        new_cy = center_y + dy * factor
        self.x = new_cx - self.width / 2
        self.y = new_cy - self.height / 2

    def get_center(self) -> Tuple[float, float]:
        return (self.x + self.width / 2, self.y + self.height / 2)

    def rotate(self, angle: float, pivot: Optional[Tuple[float, float]] = None) -> None:
        if pivot is None:
            pivot = self.get_center()
        cx, cy = self.get_center()
        if abs(cx - pivot[0]) > 1e-9 or abs(cy - pivot[1]) > 1e-9:
            cx, cy = rotate_point(cx, cy, pivot[0], pivot[1], angle)
            self.x = cx - self.width / 2
            self.y = cy - self.height / 2
        self.rotation += angle
        if self.rotation > math.pi:
            self.rotation -= 2 * math.pi
        elif self.rotation <= -math.pi:
            self.rotation += 2 * math.pi

    def _get_vertices(self) -> List[Tuple[float, float]]:
        cx, cy = self.get_center()
        half_w = self.width / 2
        half_h = self.height / 2
        corners = [
            (-half_w, -half_h),
            (half_w, -half_h),
            (half_w, half_h),
            (-half_w, half_h),
        ]
        cos_a = math.cos(self.rotation)
        sin_a = math.sin(self.rotation)
        transformed: List[Tuple[float, float]] = []
        for dx, dy in corners:
            rx = dx * cos_a - dy * sin_a
            ry = dx * sin_a + dy * cos_a
            transformed.append((cx + rx, cy + ry))
        return transformed

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "Rectangle",
            "x": self.x,
            "y": self.y,
            "width": self.width,
            "height": self.height,
            "color": self.color,
            "line_width": self.line_width,
            "fill_color": self.fill_color,
            "rotation": self.rotation,
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "Rectangle":
        rect = cls(
            data["x"],
            data["y"],
            data["width"],
            data["height"],
            data["color"],
            data["line_width"],
            data.get("fill_color"),
        )
        rect.rotation = data.get("rotation", 0.0)
        return rect


class Circle(Shape):
    """圆形"""

    def __init__(
        self,
        x: float,
        y: float,
        radius: float,
        color: str = "black",
        line_width: int = 1,
        fill_color: Optional[str] = None,
    ) -> None:
        super().__init__(x, y, color, line_width, fill_color)
        self.radius = radius

    def draw(self, canvas) -> None:
        if getattr(canvas, "use_rasterization", True):
            if getattr(canvas, "antialiasing", False) and self.line_width == 1:
                draw_circle_antialiased(canvas, self.x, self.y, self.radius, self.color)
            else:
                draw_circle_midpoint(canvas, int(self.x), int(self.y), int(self.radius), self.color, self.line_width)
            if self.fill_color:
                for py in range(int(self.y - self.radius), int(self.y + self.radius) + 1):
                    for px in range(int(self.x - self.radius), int(self.x + self.radius) + 1):
                        if (px - self.x) * (px - self.x) + (py - self.y) * (py - self.y) <= self.radius * self.radius:
                            canvas.draw_pixel(px, py, self.fill_color)
        else:
            # 使用Tk库函数
            outline_color = "red" if self.selected else self.color
            x1, y1 = self.x - self.radius, self.y - self.radius
            x2, y2 = self.x + self.radius, self.y + self.radius
            canvas.canvas.create_oval(
                x1, y1, x2, y2,
                outline=outline_color,
                fill=self.fill_color,
                width=self.line_width,
            )
        if self.selected:
            x1, y1 = self.x - self.radius, self.y - self.radius
            x2, y2 = self.x + self.radius, self.y + self.radius
            canvas.canvas.create_rectangle(
                x1 - 2,
                y1 - 2,
                x2 + 2,
                y2 + 2,
                outline="red",
                dash=(2, 2),
            )

    def contains_point(self, x: float, y: float) -> bool:
        return math.hypot(x - self.x, y - self.y) <= self.radius

    def move(self, dx: float, dy: float) -> None:
        self.x += dx
        self.y += dy

    def get_bounds(self) -> Tuple[float, float, float, float]:
        return (self.x - self.radius, self.y - self.radius, self.x + self.radius, self.y + self.radius)

    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        self.radius *= factor
        dx = self.x - center_x
        dy = self.y - center_y
        self.x = center_x + dx * factor
        self.y = center_y + dy * factor

    def get_center(self) -> Tuple[float, float]:
        return (self.x, self.y)

    def rotate(self, angle: float, pivot: Optional[Tuple[float, float]] = None) -> None:
        if pivot is None:
            pivot = self.get_center()
        self.x, self.y = rotate_point(self.x, self.y, pivot[0], pivot[1], angle)

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "Circle",
            "x": self.x,
            "y": self.y,
            "radius": self.radius,
            "color": self.color,
            "line_width": self.line_width,
            "fill_color": self.fill_color,
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "Circle":
        return cls(
            data["x"],
            data["y"],
            data["radius"],
            data["color"],
            data["line_width"],
            data.get("fill_color"),
        )


class Polygon(Shape):
    """多边形"""

    def __init__(
        self,
        points: List[Tuple[float, float]],
        color: str = "black",
        line_width: int = 1,
        fill_color: Optional[str] = None,
    ) -> None:
        if points:
            center_x = sum(p[0] for p in points) / len(points)
            center_y = sum(p[1] for p in points) / len(points)
        else:
            center_x = center_y = 0.0
        super().__init__(center_x, center_y, color, line_width, fill_color)
        self.points = points

    def draw(self, canvas) -> None:
        if len(self.points) < 2:
            return
        outline_color = "red" if self.selected else self.color
        for idx in range(len(self.points)):
            p1 = self.points[idx]
            p2 = self.points[(idx + 1) % len(self.points)]
            if getattr(canvas, "antialiasing", False) and self.line_width == 1:
                draw_line_wu(canvas, p1[0], p1[1], p2[0], p2[1], outline_color)
            else:
                draw_line_bresenham(canvas, int(p1[0]), int(p1[1]), int(p2[0]), int(p2[1]), outline_color, self.line_width)
        if self.fill_color:
            self.scanline_fill(canvas, self.fill_color)
        if self.selected:
            min_x, min_y, max_x, max_y = self.get_bounds()
            canvas.canvas.create_rectangle(
                min_x - 2,
                min_y - 2,
                max_x + 2,
                max_y + 2,
                outline="red",
                dash=(2, 2),
            )

    def contains_point(self, x: float, y: float) -> bool:
        if len(self.points) < 3:
            return False
        inside = False
        n = len(self.points)
        p1x, p1y = self.points[0]
        for i in range(1, n + 1):
            p2x, p2y = self.points[i % n]
            if y > min(p1y, p2y):
                if y <= max(p1y, p2y):
                    if x <= max(p1x, p2x):
                        if p1y != p2y:
                            xinters = (y - p1y) * (p2x - p1x) / (p2y - p1y) + p1x
                        else:
                            xinters = p1x
                        if p1x == p2x or x <= xinters:
                            inside = not inside
            p1x, p1y = p2x, p2y
        return inside

    def move(self, dx: float, dy: float) -> None:
        self.x += dx
        self.y += dy
        self.points = [(px + dx, py + dy) for px, py in self.points]

    def get_bounds(self) -> Tuple[float, float, float, float]:
        if not self.points:
            return (0.0, 0.0, 0.0, 0.0)
        xs = [p[0] for p in self.points]
        ys = [p[1] for p in self.points]
        return (min(xs), min(ys), max(xs), max(ys))

    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        new_points: List[Tuple[float, float]] = []
        for px, py in self.points:
            dx = px - center_x
            dy = py - center_y
            new_points.append((center_x + dx * factor, center_y + dy * factor))
        self.points = new_points
        if self.points:
            self.x = sum(p[0] for p in self.points) / len(self.points)
            self.y = sum(p[1] for p in self.points) / len(self.points)

    def get_center(self) -> Tuple[float, float]:
        return (self.x, self.y)

    def rotate(self, angle: float, pivot: Optional[Tuple[float, float]] = None) -> None:
        if not self.points:
            return
        if pivot is None:
            pivot = self.get_center()
        self.points = [rotate_point(px, py, pivot[0], pivot[1], angle) for px, py in self.points]
        if self.points:
            self.x = sum(p[0] for p in self.points) / len(self.points)
            self.y = sum(p[1] for p in self.points) / len(self.points)

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "Polygon",
            "points": self.points,
            "color": self.color,
            "line_width": self.line_width,
            "fill_color": self.fill_color,
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "Polygon":
        return cls(data["points"], data["color"], data["line_width"], data.get("fill_color"))

    def scanline_fill(self, canvas, color: str) -> None:
        if len(self.points) < 3:
            return
        min_y = int(min(p[1] for p in self.points))
        max_y = int(max(p[1] for p in self.points))
        for y in range(min_y, max_y + 1):
            intersections: List[float] = []
            for idx in range(len(self.points)):
                p1 = self.points[idx]
                p2 = self.points[(idx + 1) % len(self.points)]
                y1, y2 = p1[1], p2[1]
                if y1 > y2:
                    y1, y2 = y2, y1
                    x1, x2 = p2[0], p1[0]
                else:
                    x1, x2 = p1[0], p2[0]
                if y1 <= y < y2 and y2 - y1 > 0:
                    x = x1 + (x2 - x1) * (y - y1) / (y2 - y1)
                    intersections.append(x)
            intersections.sort()
            for j in range(0, len(intersections), 2):
                if j + 1 < len(intersections):
                    x_start = int(intersections[j])
                    x_end = int(intersections[j + 1])
                    for px in range(x_start, x_end + 1):
                        canvas.draw_pixel(px, y, color)


class Cube(Shape):
    """立方体 (3D)"""

    def __init__(
        self,
        x: float,
        y: float,
        z: float,
        size: float,
        color: str = "black",
        line_width: int = 1,
        fill_color: Optional[str] = None,
    ) -> None:
        super().__init__(x, y, color, line_width, fill_color)
        self.z = z
        self.size = size
        s = self.size / 2
        self.vertices = [
            (-s, -s, -s),
            (s, -s, -s),
            (s, s, -s),
            (-s, s, -s),
            (-s, -s, s),
            (s, -s, s),
            (s, s, s),
            (-s, s, s),
        ]
        self.edges = [
            (0, 1),
            (1, 2),
            (2, 3),
            (3, 0),
            (4, 5),
            (5, 6),
            (6, 7),
            (7, 4),
            (0, 4),
            (1, 5),
            (2, 6),
            (3, 7),
        ]
        self.rotation_x = 0.0
        self.rotation_y = 0.0
        self.rotation_z = 0.0
        self._last_bounds: Optional[Tuple[float, float, float, float]] = None

    def draw(self, canvas) -> None:
        outline_color = "red" if self.selected else self.color
        rotated_vertices = self._get_rotated_vertices()
        projected_points: List[Tuple[float, float]] = []
        for lx, ly, lz in rotated_vertices:
            px, py = self._project_local(lx, ly, lz)
            projected_points.append((self.x + px, self.y + py))
        if getattr(canvas, "use_rasterization", True):
            # 使用光栅化算法绘制立方体边
            for start, end in self.edges:
                p1 = projected_points[start]
                p2 = projected_points[end]
                if getattr(canvas, "antialiasing", False) and self.line_width == 1:
                    draw_line_wu(canvas, p1[0], p1[1], p2[0], p2[1], outline_color)
                else:
                    draw_line_bresenham(canvas, int(p1[0]), int(p1[1]), int(p2[0]), int(p2[1]), outline_color, self.line_width)
        else:
            # 使用Tk库函数
            for start, end in self.edges:
                p1 = projected_points[start]
                p2 = projected_points[end]
                canvas.canvas.create_line(
                    p1[0],
                    p1[1],
                    p2[0],
                    p2[1],
                    fill=outline_color,
                    width=self.line_width,
                )
        if projected_points:
            xs = [p[0] for p in projected_points]
            ys = [p[1] for p in projected_points]
            self._last_bounds = (min(xs), min(ys), max(xs), max(ys))

    def contains_point(self, x: float, y: float) -> bool:
        min_x, min_y, max_x, max_y = self.get_bounds()
        margin = 6.0
        return min_x - margin <= x <= max_x + margin and min_y - margin <= y <= max_y + margin

    def move(self, dx: float, dy: float) -> None:
        self.x += dx
        self.y += dy
        self._last_bounds = None

    def get_bounds(self) -> Tuple[float, float, float, float]:
        if self._last_bounds is not None:
            return self._last_bounds
        rotated_vertices = self._get_rotated_vertices()
        projected_points: List[Tuple[float, float]] = []
        for lx, ly, lz in rotated_vertices:
            px, py = self._project_local(lx, ly, lz)
            projected_points.append((self.x + px, self.y + py))
        if not projected_points:
            return (0.0, 0.0, 0.0, 0.0)
        xs = [p[0] for p in projected_points]
        ys = [p[1] for p in projected_points]
        return (min(xs), min(ys), max(xs), max(ys))

    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        self.size *= factor
        s = self.size / 2
        self.vertices = [
            (-s, -s, -s),
            (s, -s, -s),
            (s, s, -s),
            (-s, s, -s),
            (-s, -s, s),
            (s, -s, s),
            (s, s, s),
            (-s, s, s),
        ]
        self._last_bounds = None

    def rotate(self, dx: float, dy: float) -> None:
        self.rotation_y += dx * 0.01
        self.rotation_x += dy * 0.01
        self._last_bounds = None

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "Cube",
            "x": self.x,
            "y": self.y,
            "z": self.z,
            "size": self.size,
            "color": self.color,
            "line_width": self.line_width,
            "rotation_x": self.rotation_x,
            "rotation_y": self.rotation_y,
            "rotation_z": self.rotation_z,
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "Cube":
        cube = cls(
            data["x"],
            data["y"],
            data["z"],
            data["size"],
            data["color"],
            data["line_width"],
        )
        cube.rotation_x = data.get("rotation_x", 0.0)
        cube.rotation_y = data.get("rotation_y", 0.0)
        cube.rotation_z = data.get("rotation_z", 0.0)
        return cube

    def get_center(self) -> Tuple[float, float]:
        return (self.x, self.y)

    def _project_local(self, x: float, y: float, z: float) -> Tuple[float, float]:
        cos30 = 0.8660254037844386
        iso_x = (x - z) * cos30
        iso_y = y + (x + z) * 0.5
        return iso_x, -iso_y

    def _get_rotated_vertices(self) -> List[Tuple[float, float, float]]:
        rotated: List[Tuple[float, float, float]] = []
        cos_x, sin_x = math.cos(self.rotation_x), math.sin(self.rotation_x)
        cos_y, sin_y = math.cos(self.rotation_y), math.sin(self.rotation_y)
        cos_z, sin_z = math.cos(self.rotation_z), math.sin(self.rotation_z)
        for vx, vy, vz in self.vertices:
            x1 = vx * cos_y - vz * sin_y
            z1 = vx * sin_y + vz * cos_y
            vx, vz = x1, z1
            y1 = vy * cos_x - vz * sin_x
            z2 = vy * sin_x + vz * cos_x
            vy, vz = y1, z2
            x2 = vx * cos_z - vy * sin_z
            y2 = vx * sin_z + vy * cos_z
            rotated.append((x2, y2, vz))
        return rotated


class Ellipse(Shape):
    """椭圆（支持旋转）"""

    def __init__(
        self,
        x: float,
        y: float,
        rx: float,
        ry: float,
        color: str = "black",
        line_width: int = 1,
        fill_color: Optional[str] = None,
    ) -> None:
        super().__init__(x, y, color, line_width, fill_color)
        self.rx = rx
        self.ry = ry
        self.rotation = 0.0

    def draw(self, canvas) -> None:
        outline_color = "red" if self.selected else self.color
        if getattr(canvas, "use_rasterization", True):
            # 使用光栅化算法绘制椭圆轮廓
            if abs(self.rotation) < 1e-6:
                # 未旋转：使用优化的椭圆算法
                # 修复无法绘制的问题并支持线宽
                draw_ellipse_midpoint(canvas, int(round(self.x)), int(round(self.y)), 
                                      int(round(self.rx)), int(round(self.ry)), 
                                      outline_color, self.line_width)
            else:
                # 旋转椭圆：使用多边形方式绘制轮廓
                points = self._generate_points(steps=64)  # 使用较少的点以提高性能
                if len(points) >= 2:
                    for i in range(len(points)):
                        p1 = points[i]
                        p2 = points[(i + 1) % len(points)]
                        if getattr(canvas, "antialiasing", False) and self.line_width == 1:
                            draw_line_wu(canvas, p1[0], p1[1], p2[0], p2[1], outline_color)
                        else:
                            draw_line_bresenham(canvas, int(round(p1[0])), int(round(p1[1])), int(round(p2[0])), int(round(p2[1])), outline_color, self.line_width)

            # 填充处理
            if self.fill_color:
                if abs(self.rotation) < 1e-6:
                    # 未旋转椭圆的填充：使用扫描线算法
                    # 计算 y 范围
                    min_x, min_y, max_x, max_y = self.get_bounds()
                    y0 = int(math.floor(min_y))
                    y1 = int(math.ceil(max_y))
                    cx = self.x
                    cy = self.y
                    rx = self.rx
                    ry = self.ry

                    for y_world in range(y0, y1 + 1):
                        # 固定 y_world，解关于 dx = X - cx 的二次方程
                        dy = y_world - cy
                        # 方程: dx^2 / rx^2 + dy^2 / ry^2 = 1
                        # dx^2 = rx^2 * (1 - dy^2 / ry^2)
                        if abs(dy) >= ry:
                            continue
                        dx_squared = rx * rx * (1 - dy * dy / (ry * ry))
                        if dx_squared < 0:
                            continue
                        dx = math.sqrt(dx_squared)
                        x_left = cx - dx
                        x_right = cx + dx

                        x_start = int(math.ceil(x_left))
                        x_end = int(math.floor(x_right))
                        for px in range(x_start, x_end + 1):
                            canvas.draw_pixel(px, y_world, self.fill_color)
                else:
                    # 旋转椭圆的填充：使用多边形扫描线填充
                    points = self._generate_points(steps=64)
                    if len(points) >= 3:
                        try:
                            poly = Polygon(points, self.color, self.line_width, self.fill_color)
                            poly.scanline_fill(canvas, self.fill_color)
                        except Exception:
                            # 回退：如果出现问题，使用 Tk 的多边形填充
                            flat_points = [coord for point in points for coord in point]
                            canvas.canvas.create_polygon(
                                flat_points,
                                outline="",
                                fill=self.fill_color,
                            )
        else:
            # 使用Tk库函数
            points = self._generate_points()
            if points:
                flat_points = [coord for point in points for coord in point]
                canvas.canvas.create_polygon(
                    flat_points,
                    outline=outline_color,
                    fill=self.fill_color or "",
                    width=self.line_width,
                )
        if self.selected:
            min_x, min_y, max_x, max_y = self.get_bounds()
            canvas.canvas.create_rectangle(
                min_x - 2,
                min_y - 2,
                max_x + 2,
                max_y + 2,
                outline="red",
                dash=(2, 2),
            )

    def contains_point(self, x: float, y: float) -> bool:
        if self.rx == 0 or self.ry == 0:
            return False
        dx = x - self.x
        dy = y - self.y
        cos_a = math.cos(-self.rotation)
        sin_a = math.sin(-self.rotation)
        local_x = dx * cos_a - dy * sin_a
        local_y = dx * sin_a + dy * cos_a
        return (local_x / self.rx) ** 2 + (local_y / self.ry) ** 2 <= 1

    def move(self, dx: float, dy: float) -> None:
        self.x += dx
        self.y += dy

    def get_bounds(self) -> Tuple[float, float, float, float]:
        points = self._generate_points()
        if not points:
            return (self.x, self.y, self.x, self.y)
        xs = [p[0] for p in points]
        ys = [p[1] for p in points]
        return (min(xs), min(ys), max(xs), max(ys))

    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        self.rx *= factor
        self.ry *= factor
        dx = self.x - center_x
        dy = self.y - center_y
        self.x = center_x + dx * factor
        self.y = center_y + dy * factor

    def get_center(self) -> Tuple[float, float]:
        return (self.x, self.y)

    def rotate(self, angle: float, pivot: Optional[Tuple[float, float]] = None) -> None:
        if pivot is None:
            pivot = self.get_center()
        cx, cy = self.get_center()
        if abs(cx - pivot[0]) > 1e-9 or abs(cy - pivot[1]) > 1e-9:
            cx, cy = rotate_point(cx, cy, pivot[0], pivot[1], angle)
            self.x, self.y = cx, cy
        self.rotation += angle
        self.rotation = (self.rotation + math.pi) % (2 * math.pi) - math.pi

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "Ellipse",
            "x": self.x,
            "y": self.y,
            "rx": self.rx,
            "ry": self.ry,
            "color": self.color,
            "line_width": self.line_width,
            "fill_color": self.fill_color,
            "rotation": self.rotation,
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "Ellipse":
        ellipse = cls(
            data["x"],
            data["y"],
            data["rx"],
            data["ry"],
            data["color"],
            data["line_width"],
            data.get("fill_color"),
        )
        ellipse.rotation = data.get("rotation", 0.0)
        return ellipse

    def _generate_points(self, steps: int = 96) -> List[Tuple[float, float]]:
        if self.rx <= 0 or self.ry <= 0:
            return []
        cos_a = math.cos(self.rotation)
        sin_a = math.sin(self.rotation)
        points: List[Tuple[float, float]] = []
        for i in range(steps):
            theta = 2 * math.pi * i / steps
            local_x = self.rx * math.cos(theta)
            local_y = self.ry * math.sin(theta)
            rx = local_x * cos_a - local_y * sin_a
            ry = local_x * sin_a + local_y * cos_a
            points.append((self.x + rx, self.y + ry))
        return points


class BezierCurve(Shape):
    """Bézier 曲线"""

    def __init__(self, points: List[Tuple[float, float]], color: str = "black", line_width: int = 1):
        x, y = points[0] if points else (0, 0)
        super().__init__(x, y, color, line_width)
        self.points = points  # List of (x,y)

    def draw(self, canvas) -> None:
        if len(self.points) < 2: return
        
        # 绘制曲线
        draw_bezier_curve(canvas, self.points, self.color, width=self.line_width)
        
        # 选中时绘制控制多边形和控制点
        if self.selected:
            # 绘制虚线控制多边形
            for i in range(len(self.points) - 1):
                p1 = self.points[i]
                p2 = self.points[i+1]
                canvas.canvas.create_line(p1[0], p1[1], p2[0], p2[1], fill="gray", dash=(2, 2))
            
            # 绘制控制点
            for idx, p in enumerate(self.points):
                r = 4
                canvas.canvas.create_rectangle(p[0]-r, p[1]-r, p[0]+r, p[1]+r, fill="white", outline="blue")

    def contains_point(self, x: float, y: float) -> bool:
        min_x, min_y, max_x, max_y = self.get_bounds()
        return min_x <= x <= max_x and min_y <= y <= max_y
        
    def move(self, dx: float, dy: float) -> None:
        self.points = [(p[0]+dx, p[1]+dy) for p in self.points]
        self.x += dx
        self.y += dy

    def get_bounds(self) -> Tuple[float, float, float, float]:
        if not self.points: return (0,0,0,0)
        xs = [p[0] for p in self.points]
        ys = [p[1] for p in self.points]
        return (min(xs), min(ys), max(xs), max(ys))

    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        self.points = [
            (center_x + (p[0]-center_x)*factor, center_y + (p[1]-center_y)*factor) 
            for p in self.points
        ]
        
    def rotate(self, angle: float, pivot: Optional[Tuple[float, float]] = None) -> None:
        if pivot is None: pivot = self.get_center()
        self.points = [rotate_point(p[0], p[1], pivot[0], pivot[1], angle) for p in self.points]

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "BezierCurve",
            "points": self.points,
            "color": self.color,
            "line_width": self.line_width,
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "BezierCurve":
        return cls(data["points"], data["color"], data["line_width"])

    def get_handle_index(self, x: float, y: float) -> int:
        for i, p in enumerate(self.points):
            if abs(p[0]-x) < 6 and abs(p[1]-y) < 6:
                return i
        return -1

    def update_handle(self, index: int, x: float, y: float):
        if 0 <= index < len(self.points):
            self.points[index] = (x, y)


class BSplineCurve(Shape):
    """B样条曲线"""

    def __init__(self, points: List[Tuple[float, float]], degree: int = 3, color: str = "black", line_width: int = 1):
        x, y = points[0] if points else (0, 0)
        super().__init__(x, y, color, line_width)
        self.points = points
        self.degree = degree

    def draw(self, canvas) -> None:
        if len(self.points) < 2: return
        draw_bspline_curve(canvas, self.points, self.degree, self.color, width=self.line_width)
        
        if self.selected:
            for i in range(len(self.points) - 1):
                p1 = self.points[i]
                p2 = self.points[i+1]
                canvas.canvas.create_line(p1[0], p1[1], p2[0], p2[1], fill="gray", dash=(2, 2))
            for idx, p in enumerate(self.points):
                r = 4
                canvas.canvas.create_rectangle(p[0]-r, p[1]-r, p[0]+r, p[1]+r, fill="white", outline="green")

    def contains_point(self, x: float, y: float) -> bool:
        min_x, min_y, max_x, max_y = self.get_bounds()
        return min_x <= x <= max_x and min_y <= y <= max_y

    def move(self, dx: float, dy: float) -> None:
        self.points = [(p[0]+dx, p[1]+dy) for p in self.points]
        self.x += dx
        self.y += dy

    def get_bounds(self) -> Tuple[float, float, float, float]:
        if not self.points: return (0,0,0,0)
        xs = [p[0] for p in self.points]
        ys = [p[1] for p in self.points]
        return (min(xs), min(ys), max(xs), max(ys))

    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        self.points = [
            (center_x + (p[0]-center_x)*factor, center_y + (p[1]-center_y)*factor) 
            for p in self.points
        ]

    def rotate(self, angle: float, pivot: Optional[Tuple[float, float]] = None) -> None:
        if pivot is None: pivot = self.get_center()
        self.points = [rotate_point(p[0], p[1], pivot[0], pivot[1], angle) for p in self.points]

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "BSplineCurve",
            "points": self.points,
            "degree": self.degree,
            "color": self.color,
            "line_width": self.line_width,
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "BSplineCurve":
        return cls(data["points"], data["degree"], data["color"], data["line_width"])

    def get_handle_index(self, x: float, y: float) -> int:
        for i, p in enumerate(self.points):
            if abs(p[0]-x) < 6 and abs(p[1]-y) < 6:
                return i
        return -1

    def update_handle(self, index: int, x: float, y: float):
        if 0 <= index < len(self.points):
            self.points[index] = (x, y)


class BezierSurface(Shape):
    """Bézier 曲面"""

    def __init__(self, points: List[List[Tuple[float, float, float]]], color: str = "black", 
                 show_mesh: bool = True, fill_method: str = "none", fill_colors: Optional[Tuple[str, str]] = None):
        # points: 4x4 matrix
        super().__init__(0, 0, color)
        self.points = points
        self.show_mesh = show_mesh
        self.fill_method = fill_method # "none", "scanline"
        self.fill_colors = fill_colors

    def draw(self, canvas) -> None:
        fill = (self.fill_method != "none")
        # 渲染逻辑：只有在不填充的时候才显示网格，如果填充则不显示网格 (符合用户要求的"模式分离")
        # 即使 self.show_mesh 为 True，只要启用了填充，就不绘制网格
        should_show_mesh = self.show_mesh and not fill
        draw_bezier_surface(canvas, self.points, self.color, show_mesh=should_show_mesh, fill=fill, fill_colors=self.fill_colors)
        
        if self.selected:
            # Draw control grid
            # 为了区分选择，绘制简易网格
            for i in range(4):
                for j in range(3):
                    p1 = self._project(self.points[i][j])
                    p2 = self._project(self.points[i][j+1])
                    canvas.canvas.create_line(p1[0], p1[1], p2[0], p2[1], fill="purple", dash=(1,1))
            for i in range(3):
                for j in range(4):
                    p1 = self._project(self.points[i][j])
                    p2 = self._project(self.points[i+1][j])
                    canvas.canvas.create_line(p1[0], p1[1], p2[0], p2[1], fill="purple", dash=(1,1))

            for i in range(4):
                for j in range(4):
                    p = self._project(self.points[i][j])
                    r = 3
                    canvas.canvas.create_oval(p[0]-r, p[1]-r, p[0]+r, p[1]+r, fill="white", outline="purple")
    
    def _project(self, p3):
        x, y, z = p3
        return x + 0.5 * z * 0.5, y + 0.5 * z * 0.5
        
    def contains_point(self, x: float, y: float) -> bool:
        min_x, min_y, max_x, max_y = self.get_bounds()
        return min_x <= x <= max_x and min_y <= y <= max_y

    def move(self, dx: float, dy: float) -> None:
        for i in range(4):
            for j in range(4):
                px, py, pz = self.points[i][j]
                self.points[i][j] = (px + dx, py + dy, pz)

    def get_bounds(self) -> Tuple[float, float, float, float]:
        xs, ys = [], []
        for row in self.points:
            for p in row:
                px, py = self._project(p)
                xs.append(px)
                ys.append(py)
        return (min(xs), min(ys), max(xs), max(ys))

    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        for i in range(4):
            for j in range(4):
                px, py, pz = self.points[i][j]
                self.points[i][j] = (center_x + (px - center_x) * factor, center_y + (py - center_y) * factor, pz * factor)

    def rotate(self, angle: float, pivot: Optional[Tuple[float, float]] = None) -> None:
        if pivot is None: pivot = self.get_center()
        for i in range(4):
            for j in range(4):
                px, py, pz = self.points[i][j]
                nx, ny = rotate_point(px, py, pivot[0], pivot[1], angle)
                self.points[i][j] = (nx, ny, pz)

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "BezierSurface",
            "points": self.points,
            "color": self.color,
            "show_mesh": self.show_mesh,
            "fill_method": self.fill_method,
            "fill_colors": self.fill_colors
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "BezierSurface":
        return cls(data["points"], data["color"], data.get("show_mesh", True), data.get("fill_method", "none"), data.get("fill_colors"))

    def get_handle_index(self, x: float, y: float) -> Tuple[int, int]:
        for i in range(4):
            for j in range(4):
                px, py = self._project(self.points[i][j])
                if abs(px - x) < 6 and abs(py - y) < 6:
                    return (i, j)
        return (-1, -1)

    def update_handle(self, index, x: float, y: float):
        i, j = index
        if 0 <= i < 4 and 0 <= j < 4:
            old_z = self.points[i][j][2]
            k = 0.25
            self.points[i][j] = (x - k*old_z, y - k*old_z, old_z)


class BSplineSurface(Shape):
    """B-Spline 曲面"""

    def __init__(self, points: List[List[Tuple[float, float, float]]], color: str = "black", 
                 show_mesh: bool = True, fill_method: str = "none", fill_colors: Optional[Tuple[str, str]] = None):
        super().__init__(0, 0, color)
        self.points = points
        self.show_mesh = show_mesh
        self.fill_method = fill_method
        self.fill_colors = fill_colors

    def draw(self, canvas) -> None:
        fill = (self.fill_method != "none")
        # 渲染逻辑：只有在不填充的时候才显示网格
        should_show_mesh = self.show_mesh and not fill
        draw_bspline_surface(canvas, self.points, self.color, show_mesh=should_show_mesh, fill=fill, fill_colors=self.fill_colors)
        
        if self.selected:
            # Draw control grid
            n_rows = len(self.points)
            n_cols = len(self.points[0])
            
            for i in range(n_rows):
                for j in range(n_cols - 1):
                    p1 = self._project(self.points[i][j])
                    p2 = self._project(self.points[i][j+1])
                    canvas.canvas.create_line(p1[0], p1[1], p2[0], p2[1], fill="orange", dash=(1,1))
            for i in range(n_rows - 1):
                for j in range(n_cols):
                    p1 = self._project(self.points[i][j])
                    p2 = self._project(self.points[i+1][j])
                    canvas.canvas.create_line(p1[0], p1[1], p2[0], p2[1], fill="orange", dash=(1,1))

            for i in range(n_rows):
                for j in range(n_cols):
                    p = self._project(self.points[i][j])
                    r = 3
                    canvas.canvas.create_oval(p[0]-r, p[1]-r, p[0]+r, p[1]+r, fill="white", outline="orange")
    
    def _project(self, p3):
        x, y, z = p3
        return x + 0.5 * z * 0.5, y + 0.5 * z * 0.5
        
    def contains_point(self, x: float, y: float) -> bool:
        min_x, min_y, max_x, max_y = self.get_bounds()
        return min_x <= x <= max_x and min_y <= y <= max_y

    def move(self, dx: float, dy: float) -> None:
        for i in range(len(self.points)):
            for j in range(len(self.points[0])):
                px, py, pz = self.points[i][j]
                self.points[i][j] = (px + dx, py + dy, pz)

    def get_bounds(self) -> Tuple[float, float, float, float]:
        xs, ys = [], []
        for row in self.points:
            for p in row:
                px, py = self._project(p)
                xs.append(px)
                ys.append(py)
        if not xs: return (0,0,0,0)
        return (min(xs), min(ys), max(xs), max(ys))

    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        for i in range(len(self.points)):
            for j in range(len(self.points[0])):
                px, py, pz = self.points[i][j]
                self.points[i][j] = (center_x + (px - center_x) * factor, center_y + (py - center_y) * factor, pz * factor)

    def rotate(self, angle: float, pivot: Optional[Tuple[float, float]] = None) -> None:
        if pivot is None: pivot = self.get_center()
        for i in range(len(self.points)):
            for j in range(len(self.points[0])):
                px, py, pz = self.points[i][j]
                nx, ny = rotate_point(px, py, pivot[0], pivot[1], angle)
                self.points[i][j] = (nx, ny, pz)

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "BSplineSurface",
            "points": self.points,
            "color": self.color,
            "show_mesh": self.show_mesh,
            "fill_method": self.fill_method,
            "fill_colors": self.fill_colors
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "BSplineSurface":
        return cls(data["points"], data["color"], data.get("show_mesh", True), data.get("fill_method", "none"), data.get("fill_colors"))

    def get_handle_index(self, x: float, y: float) -> Tuple[int, int]:
        for i in range(len(self.points)):
            for j in range(len(self.points[0])):
                px, py = self._project(self.points[i][j])
                if abs(px - x) < 6 and abs(py - y) < 6:
                    return (i, j)
        return (-1, -1)

    def update_handle(self, index, x: float, y: float):
        i, j = index
        if 0 <= i < len(self.points) and 0 <= j < len(self.points[0]):
            old_z = self.points[i][j][2]
            k = 0.25 # Reverse projection approx
            self.points[i][j] = (x - k*old_z, y - k*old_z, old_z)


class HermiteCurve(Shape):
    """Hermite Curve"""

    def __init__(self, p1: Tuple[float, float], p2: Tuple[float, float], 
                 t1: Tuple[float, float], t2: Tuple[float, float],
                 color: str = "black", line_width: int = 1):
        super().__init__(p1[0], p1[1], color, line_width)
        self.p1 = p1
        self.p2 = p2
        self.t1 = t1 # tangent vector at p1
        self.t2 = t2 # tangent vector at p2
        
    def draw(self, canvas) -> None:
        draw_hermite_curve(canvas, self.p1, self.p2, self.t1, self.t2, self.color, width=self.line_width)
        
        if self.selected:
            # Draw endpoints
            r = 4
            canvas.canvas.create_rectangle(self.p1[0]-r, self.p1[1]-r, self.p1[0]+r, self.p1[1]+r, fill="white", outline="blue")
            canvas.canvas.create_rectangle(self.p2[0]-r, self.p2[1]-r, self.p2[0]+r, self.p2[1]+r, fill="white", outline="blue")
            
            # Draw tangent handles (as lines from points)
            # T1 is vector, so draw line from P1 to P1+T1
            h1 = (self.p1[0] + self.t1[0], self.p1[1] + self.t1[1])
            h2 = (self.p2[0] + self.t2[0], self.p2[1] + self.t2[1])
            
            canvas.canvas.create_line(self.p1[0], self.p1[1], h1[0], h1[1], fill="red", dash=(1, 1))
            canvas.canvas.create_line(self.p2[0], self.p2[1], h2[0], h2[1], fill="red", dash=(1, 1))
            
            canvas.canvas.create_oval(h1[0]-r, h1[1]-r, h1[0]+r, h1[1]+r, fill="red", outline="red")
            canvas.canvas.create_oval(h2[0]-r, h2[1]-r, h2[0]+r, h2[1]+r, fill="red", outline="red")
            
    def contains_point(self, x: float, y: float) -> bool:
         # Simplified bounding box check
        min_x, min_y, max_x, max_y = self.get_bounds()
        return min_x <= x <= max_x and min_y <= y <= max_y

    def move(self, dx: float, dy: float) -> None:
        self.p1 = (self.p1[0] + dx, self.p1[1] + dy)
        self.p2 = (self.p2[0] + dx, self.p2[1] + dy)
        # Tangents are vectors, don't change with translation unless we store them as absolute points
        # But for UI handle convenience, let's treat T1, T2 as vectors
        self.x += dx
        self.y += dy

    def get_bounds(self) -> Tuple[float, float, float, float]:
        xs = [self.p1[0], self.p2[0], self.p1[0] + self.t1[0], self.p2[0] + self.t2[0]]
        ys = [self.p1[1], self.p2[1], self.p1[1] + self.t1[1], self.p2[1] + self.t2[1]]
        return (min(xs), min(ys), max(xs), max(ys))
        
    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        self.p1 = (center_x + (self.p1[0]-center_x)*factor, center_y + (self.p1[1]-center_y)*factor)
        self.p2 = (center_x + (self.p2[0]-center_x)*factor, center_y + (self.p2[1]-center_y)*factor)
        self.t1 = (self.t1[0]*factor, self.t1[1]*factor)
        self.t2 = (self.t2[0]*factor, self.t2[1]*factor)

    def rotate(self, angle: float, pivot: Optional[Tuple[float, float]] = None) -> None:
        if pivot is None: pivot = self.get_center()
        self.p1 = rotate_point(self.p1[0], self.p1[1], pivot[0], pivot[1], angle)
        self.p2 = rotate_point(self.p2[0], self.p2[1], pivot[0], pivot[1], angle)
        
        # Rotate vectors
        cos_a = math.cos(angle)
        sin_a = math.sin(angle)
        self.t1 = (self.t1[0]*cos_a - self.t1[1]*sin_a, self.t1[0]*sin_a + self.t1[1]*cos_a)
        self.t2 = (self.t2[0]*cos_a - self.t2[1]*sin_a, self.t2[0]*sin_a + self.t2[1]*cos_a)
    
    def get_handle_index(self, x: float, y: float) -> int:
        r = 6
        # 0: p1, 1: p2, 2: t1_handle, 3: t2_handle
        h1 = (self.p1[0] + self.t1[0], self.p1[1] + self.t1[1])
        h2 = (self.p2[0] + self.t2[0], self.p2[1] + self.t2[1])
        
        if abs(self.p1[0]-x) < r and abs(self.p1[1]-y) < r: return 0
        if abs(self.p2[0]-x) < r and abs(self.p2[1]-y) < r: return 1
        if abs(h1[0]-x) < r and abs(h1[1]-y) < r: return 2
        if abs(h2[0]-x) < r and abs(h2[1]-y) < r: return 3
        return -1
        
    def update_handle(self, index: int, x: float, y: float):
        if index == 0:
            self.p1 = (x, y)
        elif index == 1:
            self.p2 = (x, y)
        elif index == 2:
            self.t1 = (x - self.p1[0], y - self.p1[1])
        elif index == 3:
            self.t2 = (x - self.p2[0], y - self.p2[1])

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "HermiteCurve",
            "p1": self.p1,
            "p2": self.p2,
            "t1": self.t1,
            "t2": self.t2,
            "color": self.color,
            "line_width": self.line_width,
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "HermiteCurve":
        return cls(data["p1"], data["p2"], data["t1"], data["t2"], data["color"], data["line_width"])


class CatmullRomCurve(Shape):
    """Catmull-Rom Spline Curve"""
    def __init__(self, points: List[Tuple[float, float]], color: str = "black", line_width: int = 1):
        x, y = points[0] if points else (0, 0)
        super().__init__(x, y, color, line_width)
        self.points = points

    def draw(self, canvas) -> None:
        draw_catmull_rom_curve(canvas, self.points, self.color, width=self.line_width)
        
        if self.selected:
             for idx, p in enumerate(self.points):
                r = 4
                canvas.canvas.create_rectangle(p[0]-r, p[1]-r, p[0]+r, p[1]+r, fill="white", outline="orange")
    
    def contains_point(self, x: float, y: float) -> bool:
        min_x, min_y, max_x, max_y = self.get_bounds()
        return min_x <= x <= max_x and min_y <= y <= max_y

    def move(self, dx: float, dy: float) -> None:
        self.points = [(p[0]+dx, p[1]+dy) for p in self.points]
        self.x += dx
        self.y += dy

    def get_bounds(self) -> Tuple[float, float, float, float]:
        if not self.points: return (0,0,0,0)
        xs = [p[0] for p in self.points]
        ys = [p[1] for p in self.points]
        return (min(xs), min(ys), max(xs), max(ys))

    def scale(self, factor: float, center_x: float, center_y: float) -> None:
        self.points = [
            (center_x + (p[0]-center_x)*factor, center_y + (p[1]-center_y)*factor) 
            for p in self.points
        ]

    def rotate(self, angle: float, pivot: Optional[Tuple[float, float]] = None) -> None:
        if pivot is None: pivot = self.get_center()
        self.points = [rotate_point(p[0], p[1], pivot[0], pivot[1], angle) for p in self.points]

    def get_handle_index(self, x: float, y: float) -> int:
        for i, p in enumerate(self.points):
            if abs(p[0]-x) < 6 and abs(p[1]-y) < 6:
                return i
        return -1

    def update_handle(self, index: int, x: float, y: float):
        if 0 <= index < len(self.points):
            self.points[index] = (x, y)

    def to_dict(self) -> Dict[str, Any]:
        return {
            "type": "CatmullRomCurve",
            "points": self.points,
            "color": self.color,
            "line_width": self.line_width,
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "CatmullRomCurve":
        return cls(data["points"], data["color"], data["line_width"])


def shape_factory(shape_type: str, *args, **kwargs) -> Shape:
    mapping = {
        "Point": Point,
        "Line": Line,
        "Rectangle": Rectangle,
        "Circle": Circle,
        "Polygon": Polygon,
        "Cube": Cube,
        "Ellipse": Ellipse,
        "BezierCurve": BezierCurve,
        "BSplineCurve": BSplineCurve,
        "BezierSurface": BezierSurface,
        "BSplineSurface": BSplineSurface,
        "HermiteCurve": HermiteCurve,
        "CatmullRomCurve": CatmullRomCurve,
    }
    if shape_type not in mapping:
        raise ValueError(f"未知的图形类型: {shape_type}")
    return mapping[shape_type](*args, **kwargs)


def create_shape_from_dict(data: Dict[str, Any]) -> Shape:
    shape_type = data.get("type")
    if shape_type == "Point":
        return Point.from_dict(data)
    if shape_type == "Line":
        return Line.from_dict(data)
    if shape_type == "Rectangle":
        return Rectangle.from_dict(data)
    if shape_type == "Circle":
        return Circle.from_dict(data)
    if shape_type == "Polygon":
        return Polygon.from_dict(data)
    if shape_type == "Cube":
        return Cube.from_dict(data)
    if shape_type == "Ellipse":
        return Ellipse.from_dict(data)
    if shape_type == "BezierCurve":
        return BezierCurve.from_dict(data)
    if shape_type == "BSplineCurve":
        return BSplineCurve.from_dict(data)
    if shape_type == "BezierSurface":
        return BezierSurface.from_dict(data)
    if shape_type == "BSplineSurface":
        return BSplineSurface.from_dict(data)
    if shape_type == "HermiteCurve":
        return HermiteCurve.from_dict(data)
    if shape_type == "CatmullRomCurve":
        return CatmullRomCurve.from_dict(data)
    raise ValueError(f"无法从数据创建图形: {shape_type}")


