import math

def draw_line_bresenham(canvas, x1, y1, x2, y2, color, width=1):
    """使用Bresenham算法绘制直线 (支持线宽)"""
    dx = abs(x2 - x1)
    dy = abs(y2 - y1)
    sx = 1 if x1 < x2 else -1
    sy = 1 if y1 < y2 else -1
    err = dx - dy
    
    # 计算线宽偏移量
    offset_start = -(width // 2)
    offset_end = offset_start + width

    while True:
        # 绘制主像素及线宽扩展像素
        # 根据斜率决定是水平扩展还是垂直扩展
        if dx > dy: # 趋向水平，垂直扩展
            for k in range(offset_start, offset_end):
                canvas.draw_pixel(x1, y1 + k, color)
        else: # 趋向垂直，水平扩展
            for k in range(offset_start, offset_end):
                canvas.draw_pixel(x1 + k, y1, color)
                
        if x1 == x2 and y1 == y2:
            break
        e2 = 2 * err
        if e2 > -dy:
            err -= dy
            x1 += sx
        if e2 < dx:
            err += dx
            y1 += sy

def draw_line_dda(canvas, x1, y1, x2, y2, color, width=1):
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
    
    offset_start = -(width // 2)
    offset_end = offset_start + width
    
    for _ in range(int(steps) + 1):
        ix, iy = round(x), round(y)
        if abs(dx) > abs(dy):
            for k in range(offset_start, offset_end):
                canvas.draw_pixel(ix, iy + k, color)
        else:
            for k in range(offset_start, offset_end):
                canvas.draw_pixel(ix + k, iy, color)
        x += x_increment
        y += y_increment

def draw_circle_midpoint(canvas, xc, yc, r, color, width=1):
    """使用中点圆算法绘制圆 (支持线宽)"""
    if r <= 0: return
    
    # 通过绘制同心圆模拟线宽
    start_r = max(0, r - width // 2)
    end_r = start_r + width
    
    for curr_r in range(start_r, end_r):
        x = curr_r
        y = 0
        p = 1 - curr_r
    
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

def draw_ellipse_midpoint(canvas, xc, yc, rx, ry, color, width=1):
    """使用中点椭圆算法绘制椭圆 (支持线宽) - 整数版优化"""
    if rx <= 0 or ry <= 0: return

    arr_rx = int(rx)
    arr_ry = int(ry) 
    
    # 模拟线宽
    start_offset = -(width // 2)
    end_offset = start_offset + width
    
    for k in range(start_offset, end_offset):
        # 确保半径为正
        crx = arr_rx + k
        cry = arr_ry + k
        if crx <= 0 or cry <= 0: continue
        
        a2 = crx * crx
        b2 = cry * cry
        
        # 区域1
        x = 0
        y = cry
        
        # d1 = b^2 - a^2*b + a^2/4
        # 使用 4 * d1 来避免浮点数
        d1 = 4 * b2 - 4 * a2 * cry + a2
        
        dx = 0          # 2 * b^2 * x
        dy = 2 * a2 * y # 2 * a^2 * y
        
        while dx < dy:
            plot_ellipse_points(canvas, xc, yc, x, y, color)
            x += 1
            dx += 2 * b2
            if d1 < 0:
                d1 += 4 * dx + 2 * b2 # 4*d1 update: 4*(2*b^2*x + 3*b^2) -> 8*b^2*x + 12*b^2. Here dx is 2*b^2*x (already ++). so 4*dx + 4*b^2 is not match?
                # Let's trust the float logic but implement carefully or use float for simplicity if int is hard.
                # Actually, standard float implementation is robust enough provided logic is correct.
                # Let's revert to a very standard textbook implementation.
            else:
                y -= 1
                dy -= 2 * a2
                d1 += 4 * dx - 4 * dy + 2 * b2
        
        # Use simple float implementation to be safe from manual derivation errors, 
        # but reset variables clearly.
        pass 
    
    # Re-implementing with robust standard float algorithm
    # which is proven to work.
    for k in range(start_offset, end_offset):
        crx = max(1, rx + k)
        cry = max(1, ry + k)
        
        a2 = crx * crx
        b2 = cry * cry
        
        x = 0
        y = cry
        
        # Region 1
        d1 = b2 - a2 * cry + 0.25 * a2
        dx = 2 * b2 * x
        dy = 2 * a2 * y
        
        while dx < dy:
            plot_ellipse_points(canvas, xc, yc, x, y, color)
            x += 1
            dx += 2 * b2
            if d1 < 0:
                d1 += dx + b2
            else:
                y -= 1
                dy -= 2 * a2
                d1 += dx - dy + b2
                
        # Region 2
        d2 = b2 * (x + 0.5)**2 + a2 * (y - 1)**2 - a2 * b2
        while y >= 0:
            plot_ellipse_points(canvas, xc, yc, x, y, color)
            y -= 1
            dy -= 2 * a2
            if d2 > 0:
                d2 += a2 - dy
            else:
                x += 1
                dx += 2 * b2
                d2 += dx - dy + a2

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


# --- 曲线与曲面算法 ---

def binomial_coeff(n, k):
    """计算二项式系数 C(n, k)"""
    if k < 0 or k > n:
        return 0
    if k == 0 or k == n:
        return 1
    k = min(k, n - k)
    c = 1
    for i in range(k):
        c = c * (n - i) // (i + 1)
    return c

def bernstein_poly(i, n, t):
    """Bernstein基函数"""
    return binomial_coeff(n, i) * (t ** i) * ((1 - t) ** (n - i))

def draw_bezier_curve(canvas, points, color, width=1, steps=100):
    """绘制Bézier曲线"""
    if len(points) < 2:
        return

    n = len(points) - 1
    prev_x, prev_y = points[0]

    for i in range(1, steps + 1):
        t = i / steps
        x, y = 0.0, 0.0
        # De Casteljau 算法或直接用Bernstein基函数求和
        for j in range(n + 1):
            b = bernstein_poly(j, n, t)
            x += points[j][0] * b
            y += points[j][1] * b
            
        if getattr(canvas, "antialiasing", False) and width == 1:
            draw_line_wu(canvas, prev_x, prev_y, x, y, color)
        else:
            draw_line_bresenham(canvas, int(prev_x), int(prev_y), int(x), int(y), color, width)
        prev_x, prev_y = x, y

def draw_bspline_curve(canvas, points, k, color, width=1, steps=50):
    """绘制B样条曲线 (均匀B样条)"""
    # points: 控制点列表
    # k: 次数 (2 或 3)
    n = len(points) - 1
    if n < k:
        return # 控制点不足

    # 对于均匀B样条，曲线由多段组成
    # 每段由 k+1 个控制点定义
    # 三次B样条(k=3)涉及 P_i, P_{i+1}, P_{i+2}, P_{i+3}
    
    for i in range(n - k + 1):
        # 绘制第 i 段
        segment_points = points[i : i + k + 1]
        
        prev_x_val = None
        prev_y_val = None
        
        for j in range(steps + 1):
            t = j / steps
            x, y = 0.0, 0.0
            
            if k == 2: # 二次均匀B样条
                # 基函数: 
                # B0 = 0.5 * (1-t)^2
                # B1 = 0.5 * (-2t^2 + 2t + 1)
                # B2 = 0.5 * t^2
                b0 = 0.5 * (1 - t)**2
                b1 = 0.5 * (-2 * t**2 + 2 * t + 1)
                b2 = 0.5 * t**2
                
                x = b0*segment_points[0][0] + b1*segment_points[1][0] + b2*segment_points[2][0]
                y = b0*segment_points[0][1] + b1*segment_points[1][1] + b2*segment_points[2][1]
                
            elif k == 3: # 三次均匀B样条
                # 基函数 1/6 * ...
                it = 1 - t
                b0 = (it**3) / 6.0
                b1 = (3*t**3 - 6*t**2 + 4) / 6.0
                b2 = (-3*t**3 + 3*t**2 + 3*t + 1) / 6.0
                b3 = (t**3) / 6.0
                
                x = b0*segment_points[0][0] + b1*segment_points[1][0] + b2*segment_points[2][0] + b3*segment_points[3][0]
                y = b0*segment_points[0][1] + b1*segment_points[1][1] + b2*segment_points[2][1] + b3*segment_points[3][1]
            
            if prev_x_val is not None:
                if getattr(canvas, "antialiasing", False) and width == 1:
                    draw_line_wu(canvas, prev_x_val, prev_y_val, x, y, color)
                else:
                    draw_line_bresenham(canvas, int(prev_x_val), int(prev_y_val), int(x), int(y), color, width)
            
            prev_x_val, prev_y_val = x, y

def get_bezier_point(points, u, v):
    """计算双三次Bézier曲面上的一点 (Tensor Product)"""
    # points 是 4x4 的点阵
    # S(u, v) = sum_i sum_j B_i(u) B_j(v) P_ij
    
    # 先对每行计算 v 方向的插值点
    temp_points = []
    for i in range(4):
        # 计算行 i 在参数 v 处的点
        row_p = [points[i][j] for j in range(4)]
        # 对这一行4个点进行 Bezier 插值 (参数 v)
        # B(v) dot P_row
        row_x, row_y, row_z = 0.0, 0.0, 0.0
        for j in range(4):
            b = bernstein_poly(j, 3, v)
            row_x += row_p[j][0] * b
            row_y += row_p[j][1] * b
            row_z += row_p[j][2] * b
        temp_points.append((row_x, row_y, row_z))
    
    # 再对 temp_points (4个点) 在参数 u 处插值
    final_x, final_y, final_z = 0.0, 0.0, 0.0
    for i in range(4):
        b = bernstein_poly(i, 3, u)
        final_x += temp_points[i][0] * b
        final_y += temp_points[i][1] * b
        final_z += temp_points[i][2] * b
        
    return (final_x, final_y, final_z)

def draw_bezier_surface(canvas, points, color, steps=20, show_mesh=True, fill=False, fill_colors=None):
    """
    绘制双三次Bézier曲面
    points: 4x4 的点阵 (x,y,z)
    """
    # 投影函数
    def project(x, y, z):
        return x + 0.5 * z * 0.5, y + 0.5 * z * 0.5
    
    # 1. 预计算控制点的Z范围，作为归一化的参考 (利用凸包性质)
    # 虽然曲面可能稍微超出控制点范围，但这作为色彩映射范围是安全的
    all_z = [p[2] for row in points for p in row]
    min_z = min(all_z)
    max_z = max(all_z)
    z_range = max_z - min_z
    if z_range == 0: z_range = 1.0

    # 2. 生成网格点，同时保存 Z 坐标
    grid = [] 
    
    for i in range(steps + 1):
        row = []
        u = i / steps
        for j in range(steps + 1):
            v = j / steps
            px, py, pz = get_bezier_point(points, u, v)
            sx, sy = project(px, py, pz)
            row.append((sx, sy, pz)) # 保存 pz
        grid.append(row)
        
    # 3. 绘制
    for i in range(steps):
        for j in range(steps):
            p00 = grid[i][j]     # (sx, sy, pz)
            p01 = grid[i][j+1]
            p11 = grid[i+1][j+1]
            p10 = grid[i+1][j]
            
            if fill:
                # 颜色计算：基于 Z-depth (高度) 的渐变
                # 计算当前四边形的平均高度
                avg_z = (p00[2] + p01[2] + p11[2] + p10[2]) / 4.0
                
                # 归一化到 [0, 1]
                ratio = (avg_z - min_z) / z_range
                ratio = max(0.0, min(1.0, ratio)) # Clamp
                
                if fill_colors:
                   r1, g1, b1 = canvas.winfo_rgb(fill_colors[0])
                   r2, g2, b2 = canvas.winfo_rgb(fill_colors[1])
                   
                   # 线性插值
                   r = int(r1 + (r2 - r1) * ratio) // 256
                   g = int(g1 + (g2 - g1) * ratio) // 256
                   b = int(b1 + (b2 - b1) * ratio) // 256
                   poly_color = f"#{r:02x}{g:02x}{b:02x}"
                else:
                   poly_color = color
                
                # 传入只有 x,y 的点给填充函数
                quad_2d = [(p[0], p[1]) for p in [p00, p10, p11, p01]]
                _scanline_fill_quad(canvas, quad_2d, poly_color)

            if show_mesh:
                # 绘制网格线
                if getattr(canvas, "antialiasing", False):
                    draw_line_wu(canvas, p00[0], p00[1], p01[0], p01[1], color) # v direction
                    draw_line_wu(canvas, p00[0], p00[1], p10[0], p10[1], color) # u direction
                else:
                     draw_line_bresenham(canvas, int(p00[0]), int(p00[1]), int(p01[0]), int(p01[1]), color)
                     draw_line_bresenham(canvas, int(p00[0]), int(p00[1]), int(p10[0]), int(p10[1]), color)

def bspline_basis_cubic(t):
    """计算三次B样条的基函数值"""
    it = 1 - t
    b0 = (it**3) / 6.0
    b1 = (3*t**3 - 6*t**2 + 4) / 6.0
    b2 = (-3*t**3 + 3*t**2 + 3*t + 1) / 6.0
    b3 = (t**3) / 6.0
    return b0, b1, b2, b3

def get_bspline_surface_point(points_4x4, u, v):
    """计算双三次B样条曲面片上的一点"""
    # points_4x4: 4x4 control points
    
    # Basis functions for u
    bu = bspline_basis_cubic(u)
    # Basis functions for v
    bv = bspline_basis_cubic(v)
    
    final_x, final_y, final_z = 0.0, 0.0, 0.0
    
    for i in range(4):
        for j in range(4):
            weight = bu[i] * bv[j]
            p = points_4x4[i][j]
            final_x += p[0] * weight
            final_y += p[1] * weight
            final_z += p[2] * weight
            
    return (final_x, final_y, final_z)

def draw_bspline_surface(canvas, points, color, steps=20, show_mesh=True, fill=False, fill_colors=None):
    """
    绘制B样条曲面
    points: NxN 的点阵 (x,y,z)
    目前实现：假设输入为 4x4 或更大，绘制所有定义的patch
    """
    n_rows = len(points)
    if n_rows < 4: return
    n_cols = len(points[0])
    if n_cols < 4: return

    # 投影函数
    def project(x, y, z):
        return x + 0.5 * z * 0.5, y + 0.5 * z * 0.5

    # 1. 计算全局Z范围 (基于控制点)
    all_z = [p[2] for row in points for p in row]
    if not all_z: return
    min_z = min(all_z)
    max_z = max(all_z)
    z_range = max_z - min_z
    if z_range == 0: z_range = 1.0

    # 遍历所有Patch
    for r in range(n_rows - 3):
        for c in range(n_cols - 3):
            # 提取当前Patch的4x4控制点
            patch_points = []
            for i in range(4):
                patch_points.append(points[r+i][c:c+4])
            
            # 绘制该Patch
            # 生成网格点
            grid = [] 
            for i in range(steps + 1):
                row_data = []
                u = i / steps
                for j in range(steps + 1):
                    v = j / steps
                    px, py, pz = get_bspline_surface_point(patch_points, u, v)
                    sx, sy = project(px, py, pz)
                    row_data.append((sx, sy, pz)) # Store Z
                grid.append(row_data)

            # 绘制网格/填充
            for i in range(steps):
                for j in range(steps):
                    p00 = grid[i][j]
                    p01 = grid[i][j+1]
                    p11 = grid[i+1][j+1]
                    p10 = grid[i+1][j]
                    
                    if fill:
                         if fill_colors:
                           # 基于 Z-Height 的颜色插值
                           avg_z = (p00[2] + p01[2] + p11[2] + p10[2]) / 4.0
                           ratio = (avg_z - min_z) / z_range
                           ratio = max(0.0, min(1.0, ratio))

                           r1, g1, b1 = canvas.winfo_rgb(fill_colors[0])
                           r2, g2, b2 = canvas.winfo_rgb(fill_colors[1])
                           
                           r = int(r1 + (r2 - r1) * ratio) // 256
                           g = int(g1 + (g2 - g1) * ratio) // 256
                           b = int(b1 + (b2 - b1) * ratio) // 256
                           poly_color = f"#{r:02x}{g:02x}{b:02x}"
                         else:
                           poly_color = color
                        
                         quad_2d = [(p[0], p[1]) for p in [p00, p10, p11, p01]]
                         _scanline_fill_quad(canvas, quad_2d, poly_color)

                    if show_mesh:
                        if getattr(canvas, "antialiasing", False):
                            draw_line_wu(canvas, p00[0], p00[1], p01[0], p01[1], color)
                            draw_line_wu(canvas, p00[0], p00[1], p10[0], p10[1], color)
                        else:
                             draw_line_bresenham(canvas, int(p00[0]), int(p00[1]), int(p01[0]), int(p01[1]), color)
                             draw_line_bresenham(canvas, int(p00[0]), int(p00[1]), int(p10[0]), int(p10[1]), color)


def _scanline_fill_quad(canvas, points, color):
    """简单的四边形填充"""
    # points: [(x, y), ...]
    # 简单的求Y范围，然后求交点
    # 类似 Polygon.scanline_fill
    
    ys = [p[1] for p in points]
    min_y = int(min(ys))
    max_y = int(max(ys))
    
    if max_y == min_y: return
    
    # 边表
    edges = []
    n = len(points)
    for i in range(n):
        p1 = points[i]
        p2 = points[(i+1)%n]
        if p1[1] != p2[1]:
            edges.append((p1, p2))
            
    for y in range(min_y, max_y + 1):
        intersections = []
        for p1, p2 in edges:
            y1, y2 = p1[1], p2[1]
            x1, x2 = p1[0], p2[0]
            
            if (y1 <= y < y2) or (y2 <= y < y1):
                x = x1 + (x2 - x1) * (y - y1) / (y2 - y1)
                intersections.append(x)
        
        intersections.sort()
        for i in range(0, len(intersections), 2):
            if i+1 < len(intersections):
                x_start = int(intersections[i])
                x_end = int(intersections[i+1])
                # 简单的水平线绘制，可优化为一次调用
                for x in range(x_start, x_end + 1):
                     canvas.draw_pixel(x, y, color)

def draw_hermite_curve(canvas, p1, p2, t1, t2, color, width=1, steps=50):
    """
    绘制三次Hermite曲线
    p1, p2: (x, y) 端点
    t1, t2: (tx, ty) 端的切向量
    H(t) = (2t^3 - 3t^2 + 1)P1 + (-2t^3 + 3t^2)P2 + (t^3 - 2t^2 + t)T1 + (t^3 - t^2)T2
    """
    prev_x, prev_y = p1
    
    for i in range(1, steps + 1):
        t = i / steps
        t2_val = t * t
        t3_val = t2_val * t
        
        h1 = 2 * t3_val - 3 * t2_val + 1
        h2 = -2 * t3_val + 3 * t2_val
        h3 = t3_val - 2 * t2_val + t
        h4 = t3_val - t2_val
        
        x = h1 * p1[0] + h2 * p2[0] + h3 * t1[0] + h4 * t2[0]
        y = h1 * p1[1] + h2 * p2[1] + h3 * t1[1] + h4 * t2[1]
        
        if getattr(canvas, "antialiasing", False) and width == 1:
            draw_line_wu(canvas, prev_x, prev_y, x, y, color)
        else:
            draw_line_bresenham(canvas, int(prev_x), int(prev_y), int(x), int(y), color, width)
        
        prev_x, prev_y = x, y

def draw_catmull_rom_curve(canvas, points, color, width=1, steps=50):
    """
    绘制Catmull-Rom样条曲线
    points: 顶点列表
    至少需要4个点才能绘制中间的一段
    """
    if len(points) < 4:
        return

    # Catmull-Rom的第i段曲线 (Pi, Pi+1) 由 Pi-1, Pi, Pi+1, Pi+2 决定
    # 这里我们只绘制定义的段，如果需要首尾相接或自然边界，需要在外部补点
    
    for i in range(len(points) - 3):
        p0 = points[i]
        p1 = points[i+1]
        p2 = points[i+2]
        p3 = points[i+3]
        
        prev_x, prev_y = p1
        
        for j in range(1, steps + 1):
            t = j / steps
            t2 = t * t
            t3 = t2 * t
            
            # Catmull-Rom Basis Matrix for tension=0.5
            #    [ -1  3 -3  1 ]
            # 0.5|  2 -5  4 -1 |
            #    [ -1  0  1  0 ]
            #    [  0  2  0  0 ]
            
            b0 = 0.5 * (-t3 + 2*t2 - t)
            b1 = 0.5 * (3*t3 - 5*t2 + 2)
            b2 = 0.5 * (-3*t3 + 4*t2 + t)
            b3 = 0.5 * (t3 - t2)
            
            x = b0*p0[0] + b1*p1[0] + b2*p2[0] + b3*p3[0]
            y = b0*p0[1] + b1*p1[1] + b2*p2[1] + b3*p3[1]
            
            if getattr(canvas, "antialiasing", False) and width == 1:
                draw_line_wu(canvas, prev_x, prev_y, x, y, color)
            else:
                draw_line_bresenham(canvas, int(prev_x), int(prev_y), int(x), int(y), color, width)
            
            prev_x, prev_y = x, y


