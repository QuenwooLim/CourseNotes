import math

def draw_line_bresenham(canvas, x1, y1, x2, y2, color):
    """使用Bresenham算法绘制直线"""
    dx = abs(x2 - x1)
    dy = abs(y2 - y1)
    sx = 1 if x1 < x2 else -1
    sy = 1 if y1 < y2 else -1
    err = dx - dy

    while True:
        canvas.draw_pixel(x1, y1, color)
        if x1 == x2 and y1 == y2:
            break
        e2 = 2 * err
        if e2 > -dy:
            err -= dy
            x1 += sx
        if e2 < dx:
            err += dx
            y1 += sy

def draw_line_dda(canvas, x1, y1, x2, y2, color):
    """使用DDA算法绘制直线"""
    dx = x2 - x1
    dy = y2 - y1
    steps = max(abs(dx), abs(dy))
    if steps == 0:
        canvas.draw_pixel(x1, y1, color)
        return
    x_increment = dx / steps
    y_increment = dy / steps
    x = float(x1)
    y = float(y1)
    for _ in range(int(steps) + 1):
        canvas.draw_pixel(round(x), round(y), color)
        x += x_increment
        y += y_increment

def draw_circle_midpoint(canvas, xc, yc, r, color):
    """使用中点圆算法绘制圆"""
    x = r
    y = 0
    p = 1 - r

    while x >= y:
        plot_circle_points(canvas, xc, yc, x, y, color)
        y += 1
        if p <= 0:
            p = p + 2 * y + 1
        else:
            x -= 1
            p = p + 2 * y - 2 * x + 1

def plot_circle_points(canvas, xc, yc, x, y, color):
    """绘制圆上的八个对称点"""
    canvas.draw_pixel(xc + x, yc + y, color)
    canvas.draw_pixel(xc - x, yc + y, color)
    canvas.draw_pixel(xc + x, yc - y, color)
    canvas.draw_pixel(xc - x, yc - y, color)
    canvas.draw_pixel(xc + y, yc + x, color)
    canvas.draw_pixel(xc - y, yc + x, color)
    canvas.draw_pixel(xc + y, yc - x, color)
    canvas.draw_pixel(xc - y, yc - x, color)

def draw_ellipse_midpoint(canvas, xc, yc, rx, ry, color):
    """使用中点椭圆算法绘制椭圆"""
    # 区域1
    x = 0
    y = ry
    p1 = ry**2 - rx**2 * ry + 0.25 * rx**2
    dx = 2 * ry**2 * x
    dy = 2 * rx**2 * y

    while dx < dy:
        plot_ellipse_points(canvas, xc, yc, x, y, color)
        x += 1
        dx += 2 * ry**2
        if p1 < 0:
            p1 += dx + ry**2
        else:
            y -= 1
            dy -= 2 * rx**2
            p1 += dx - dy + ry**2

    # 区域2
    p2 = ry**2 * (x + 0.5)**2 + rx**2 * (y - 1)**2 - rx**2 * ry**2
    while y >= 0:
        plot_ellipse_points(canvas, xc, yc, x, y, color)
        y -= 1
        dy -= 2 * rx**2
        if p2 > 0:
            p2 += rx**2 - dy
        else:
            x += 1
            dx += 2 * ry**2
            p2 += dx - dy + rx**2

def plot_ellipse_points(canvas, xc, yc, x, y, color):
    """绘制椭圆上的四个对称点"""
    canvas.draw_pixel(xc + x, yc + y, color)
    canvas.draw_pixel(xc - x, yc + y, color)
    canvas.draw_pixel(xc + x, yc - y, color)
    canvas.draw_pixel(xc - x, yc - y, color)


def draw_line_wu(canvas, x1, y1, x2, y2, color):
    """使用Xiaolin Wu算法绘制带反走样效果的直线"""

    def ipart(x):
        return math.floor(x)

    def fpart(x):
        return x - math.floor(x)

    def rfpart(x):
        return 1 - fpart(x)

    def IntensityClamp(a):
        return max(0.0, min(1.0, a))

    def plot(px, py, intensity):
        if intensity <= 0:
            return
        canvas.draw_pixel(int(px), int(py), color, alpha=IntensityClamp(intensity))

    steep = abs(y2 - y1) > abs(x2 - x1)
    if steep:
        x1, y1 = y1, x1
        x2, y2 = y2, x2

    if x1 > x2:
        x1, x2 = x2, x1
        y1, y2 = y2, y1

    dx = x2 - x1
    dy = y2 - y1
    if dx == 0:
        for y in range(int(min(y1, y2)), int(max(y1, y2)) + 1):
            if steep:
                canvas.draw_pixel(y, int(x1), color)
            else:
                canvas.draw_pixel(int(x1), y, color)
        return

    gradient = dy / dx

    # first endpoint
    x_end = round(x1)
    y_end = y1 + gradient * (x_end - x1)
    x_gap = rfpart(x1 + 0.5)
    x_pixel1 = int(x_end)
    y_pixel1 = ipart(y_end)

    if steep:
        plot(y_pixel1, x_pixel1, rfpart(y_end) * x_gap)
        plot(y_pixel1 + 1, x_pixel1, fpart(y_end) * x_gap)
    else:
        plot(x_pixel1, y_pixel1, rfpart(y_end) * x_gap)
        plot(x_pixel1, y_pixel1 + 1, fpart(y_end) * x_gap)

    intery = y_end + gradient

    # second endpoint
    x_end = round(x2)
    y_end = y2 + gradient * (x_end - x2)
    x_gap = fpart(x2 + 0.5)
    x_pixel2 = int(x_end)
    y_pixel2 = ipart(y_end)

    if steep:
        plot(y_pixel2, x_pixel2, rfpart(y_end) * x_gap)
        plot(y_pixel2 + 1, x_pixel2, fpart(y_end) * x_gap)
    else:
        plot(x_pixel2, y_pixel2, rfpart(y_end) * x_gap)
        plot(x_pixel2, y_pixel2 + 1, fpart(y_end) * x_gap)

    # main loop
    if steep:
        for x in range(x_pixel1 + 1, x_pixel2):
            y_floor = ipart(intery)
            plot(y_floor, x, rfpart(intery))
            plot(y_floor + 1, x, fpart(intery))
            intery += gradient
    else:
        for x in range(x_pixel1 + 1, x_pixel2):
            y_floor = ipart(intery)
            plot(x, y_floor, rfpart(intery))
            plot(x, y_floor + 1, fpart(intery))
            intery += gradient


def draw_circle_antialiased(canvas, xc, yc, r, color):
    """使用简单的超采样方法绘制带反走样效果的圆"""
    if r <= 0:
        return

    steps = max(int(2 * math.pi * r * 4), 8)
    for i in range(steps):
        angle = (2 * math.pi * i) / steps
        x = xc + r * math.cos(angle)
        y = yc + r * math.sin(angle)
        _plot_subpixel(canvas, x, y, color)


def draw_ellipse_antialiased(canvas, xc, yc, rx, ry, color):
    """使用超采样方法绘制带反走样效果的椭圆"""
    if rx <= 0 or ry <= 0:
        return

    radius = max(rx, ry)
    steps = max(int(2 * math.pi * radius * 4), 12)
    for i in range(steps):
        angle = (2 * math.pi * i) / steps
        x = xc + rx * math.cos(angle)
        y = yc + ry * math.sin(angle)
        _plot_subpixel(canvas, x, y, color)


def _plot_subpixel(canvas, fx, fy, color):
    x_floor = math.floor(fx)
    y_floor = math.floor(fy)
    wx = fx - x_floor
    wy = fy - y_floor

    weights = [
        ((1 - wx) * (1 - wy), (0, 0)),
        ((wx) * (1 - wy), (1, 0)),
        ((1 - wx) * wy, (0, 1)),
        ((wx) * wy, (1, 1)),
    ]

    for weight, (dx, dy) in weights:
        if weight <= 0:
            continue
        canvas.draw_pixel(x_floor + dx, y_floor + dy, color, alpha=weight)
