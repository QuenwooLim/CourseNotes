"""
主GUI界面 - 图形绘制系统
"""
import math
import tkinter as tk
from tkinter import ttk, colorchooser, messagebox
import os
import sys

# 添加当前目录到Python路径
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from canvas import DrawingCanvas
from file_manager import FileManager, CodeGenerator


# 统一浅蓝色配色方案
PALETTE = {
    "bg_root": "#eaf4ff",       # 主窗口背景 Very light blue
    "bg_toolbar": "#e6f2ff",    # 工具栏背景 Light blue
    "bg_panel": "#edf6ff",      # 左侧面板/右侧容器背景
    "bg_labelframe": "#dbeafe",  # 分组框背景
    "bg_button": "#cfe8ff",     # 普通按钮
    "bg_button_accent": "#b6dbff",  # 强调按钮/悬停
    "bg_status": "#d9ecff"      # 状态栏
}


class DrawingApplication:

    def __init__(self):
        self.root = tk.Tk()
        self.root.title("图形绘制系统 - 计算机图形学作业")
        self.root.geometry("1200x800")

        # 设置窗口样式
        self.root.configure(bg=PALETTE["bg_root"])

        # 初始化核心组件
        self.canvas: DrawingCanvas | None = None
        self.file_manager = FileManager()
        self.code_generator = CodeGenerator()

        # 当前画笔属性
        self.current_color = "black"
        self.current_fill_color = None
        self.current_line_width = 1

        # 连续旋转状态
        self.rotation_job = None
        self.rotation_direction: str | None = None
        self.rotation_step = math.radians(5)
        self.rotation_repeat_interval = 80

        # 构建界面
        self.create_menu()
        self.create_toolbar()
        self.create_status_bar()
        self.create_main_layout()

        # 绑定关闭事件
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
    
    def create_menu(self):
        """创建菜单栏"""
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)
        
        # 文件菜单
        file_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="文件", menu=file_menu)
        file_menu.add_command(label="新建", command=self.new_file, accelerator="Ctrl+N")
        file_menu.add_command(label="打开", command=self.open_file, accelerator="Ctrl+O")
        file_menu.add_separator()
        file_menu.add_command(label="保存", command=self.save_file, accelerator="Ctrl+S")
        file_menu.add_command(label="另存为", command=self.save_as_file, accelerator="Ctrl+Shift+S")
        file_menu.add_separator()
        file_menu.add_command(label="导出图像", command=self.export_image)
        file_menu.add_separator()
        file_menu.add_command(label="退出", command=self.on_closing, accelerator="Ctrl+Q")
        
        # 编辑菜单
        edit_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="编辑", menu=edit_menu)
        edit_menu.add_command(label="删除选中", command=self.delete_selected, accelerator="Delete")
        edit_menu.add_command(label="清空画布", command=self.clear_canvas, accelerator="Ctrl+Del")
        edit_menu.add_separator()
        edit_menu.add_command(label="放大", command=lambda: self.scale_selected(1.2), accelerator="Ctrl+=")
        edit_menu.add_command(label="缩小", command=lambda: self.scale_selected(0.8), accelerator="Ctrl+-")
        
        # 工具菜单
        tools_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="工具", menu=tools_menu)
        tools_menu.add_command(label="选择工具", command=lambda: self.set_tool("select"))
        tools_menu.add_command(label="点工具", command=lambda: self.set_tool("point"))
        tools_menu.add_command(label="直线工具", command=lambda: self.set_tool("line"))
        tools_menu.add_command(label="矩形工具", command=lambda: self.set_tool("rectangle"))
        tools_menu.add_command(label="圆形工具", command=lambda: self.set_tool("circle"))
        tools_menu.add_command(label="多边形工具", command=lambda: self.set_tool("polygon"))
        tools_menu.add_command(label="立方体工具", command=lambda: self.set_tool("cube"))
        tools_menu.add_command(label="椭圆工具", command=lambda: self.set_tool("ellipse"))
        
        # 代码菜单（特色功能）
        code_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="代码生成", menu=code_menu)
        code_menu.add_command(label="生成HTML Canvas", command=self.generate_html)
        code_menu.add_command(label="生成SVG", command=self.generate_svg)
        
        # 帮助菜单
        help_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="帮助", menu=help_menu)
        help_menu.add_command(label="使用说明", command=self.show_help)
        help_menu.add_command(label="关于", command=self.show_about)
        
        # 绑定快捷键
        self.root.bind('<Control-n>', lambda e: self.new_file())
        self.root.bind('<Control-o>', lambda e: self.open_file())
        self.root.bind('<Control-s>', lambda e: self.save_file())
        self.root.bind('<Control-Shift-S>', lambda e: self.save_as_file())
        self.root.bind('<Control-q>', lambda e: self.on_closing())
        self.root.bind('<Delete>', lambda e: self.delete_selected())
        self.root.bind('<Control-Delete>', lambda e: self.clear_canvas())
        self.root.bind('<Control-equal>', lambda e: self.scale_selected(1.2))
        self.root.bind('<Control-minus>', lambda e: self.scale_selected(0.8))
    
    def create_toolbar(self):
        """创建工具栏"""
        toolbar_frame = tk.Frame(self.root, bg=PALETTE["bg_toolbar"], relief=tk.RAISED, bd=1)
        toolbar_frame.pack(side=tk.TOP, fill=tk.X, padx=2, pady=2)

        # 工具按钮
        tools = [
            ("选择", "select", "🖱️"),
            ("点", "point", "⚫"),
            ("直线", "line", "📏"),
            ("矩形", "rectangle", "⬜"),
            ("圆形", "circle", "⭕"),
            ("多边形", "polygon", "🔷"),
            ("立方体", "cube", "🧊"),
            ("椭圆", "ellipse", "🏈")
        ]

        self.tool_var = tk.StringVar(value="select")
        tool_frame = tk.LabelFrame(toolbar_frame, text="绘图工具", bg=PALETTE["bg_toolbar"])
        tool_frame.pack(side=tk.LEFT, padx=5, pady=2)

        for name, tool, icon in tools:
            btn = tk.Radiobutton(
                tool_frame,
                text=f"{icon} {name}",
                variable=self.tool_var,
                value=tool,
                command=lambda t=tool: self.set_tool(t),
                bg=PALETTE["bg_toolbar"],
                relief=tk.FLAT,
                activebackground=PALETTE["bg_button_accent"],
            )
            btn.pack(side=tk.LEFT, padx=2)

        # 分隔符
        separator1 = ttk.Separator(toolbar_frame, orient=tk.VERTICAL)
        separator1.pack(side=tk.LEFT, fill=tk.Y, padx=5)

        # 颜色选择
        color_frame = tk.LabelFrame(toolbar_frame, text="颜色", bg=PALETTE["bg_toolbar"])
        color_frame.pack(side=tk.LEFT, padx=5, pady=2)

        tk.Label(color_frame, text="线条:", bg=PALETTE["bg_toolbar"]).pack(side=tk.LEFT)
        self.color_button = tk.Button(
            color_frame,
            text="  ",
            width=3,
            height=1,
            bg=self.current_color,
            command=self.choose_color,
        )
        self.color_button.pack(side=tk.LEFT, padx=2)

        tk.Label(color_frame, text="填充:", bg=PALETTE["bg_toolbar"]).pack(side=tk.LEFT, padx=(10, 0))
        self.fill_button = tk.Button(
            color_frame,
            text="无",
            width=3,
            height=1,
            bg="white",
            command=self.choose_fill_color,
            activebackground=PALETTE["bg_button_accent"],
        )
        self.fill_button.pack(side=tk.LEFT, padx=2)

        # 分隔符
        separator2 = ttk.Separator(toolbar_frame, orient=tk.VERTICAL)
        separator2.pack(side=tk.LEFT, fill=tk.Y, padx=5)

        # 线宽设置
        width_frame = tk.LabelFrame(toolbar_frame, text="线宽", bg=PALETTE["bg_toolbar"])
        width_frame.pack(side=tk.LEFT, padx=5, pady=2)

        tk.Label(width_frame, text="宽度:", bg=PALETTE["bg_toolbar"]).pack(side=tk.LEFT)
        self.width_var = tk.IntVar(value=1)
        width_spin = tk.Spinbox(
            width_frame,
            from_=1,
            to=10,
            width=5,
            textvariable=self.width_var,
            command=self.on_width_change,
        )
        width_spin.pack(side=tk.LEFT, padx=2)

        # 分隔符
        separator3 = ttk.Separator(toolbar_frame, orient=tk.VERTICAL)
        separator3.pack(side=tk.LEFT, fill=tk.Y, padx=5)

        # 绘制模式切换
        mode_frame = tk.LabelFrame(toolbar_frame, text="绘制模式", bg=PALETTE["bg_toolbar"])
        mode_frame.pack(side=tk.LEFT, padx=5, pady=2)

        self.rasterization_var = tk.BooleanVar(value=True)
        tk.Checkbutton(
            mode_frame,
            text="使用光栅算法",
            variable=self.rasterization_var,
            command=self.toggle_rasterization,
            bg=PALETTE["bg_toolbar"],
            activebackground=PALETTE["bg_button_accent"],
        ).pack(side=tk.LEFT, padx=2)
        action_frame = tk.LabelFrame(toolbar_frame, text="操作", bg=PALETTE["bg_toolbar"])
        action_frame.pack(side=tk.LEFT, padx=5, pady=2)

        tk.Button(
            action_frame,
            text="🗑️ 清空",
            command=self.clear_canvas,
            bg=PALETTE["bg_button"],
            activebackground=PALETTE["bg_button_accent"],
        ).pack(side=tk.LEFT, padx=2)

        tk.Button(
            action_frame,
            text="🔍+ 放大",
            command=lambda: self.scale_selected(1.2),
            bg=PALETTE["bg_button"],
            activebackground=PALETTE["bg_button_accent"],
        ).pack(side=tk.LEFT, padx=2)

        tk.Button(
            action_frame,
            text="🔍- 缩小",
            command=lambda: self.scale_selected(0.8),
            bg=PALETTE["bg_button"],
            activebackground=PALETTE["bg_button_accent"],
        ).pack(side=tk.LEFT, padx=2)

        rotate_ccw_btn = tk.Button(
            action_frame,
            text="顺时针 ↻",
            command=lambda: self.rotate_once("ccw"),
            bg=PALETTE["bg_button"],
            activebackground=PALETTE["bg_button_accent"],
        )
        rotate_ccw_btn.pack(side=tk.LEFT, padx=2)
        rotate_ccw_btn.bind("<ButtonPress-1>", lambda e: self.start_rotation("ccw"))
        rotate_ccw_btn.bind("<ButtonRelease-1>", self.stop_rotation)
        rotate_ccw_btn.bind("<Leave>", self.stop_rotation)

        rotate_cw_btn = tk.Button(
            action_frame,
            text="逆时针 ↺",
            command=lambda: self.rotate_once("cw"),
            bg=PALETTE["bg_button"],
            activebackground=PALETTE["bg_button_accent"],
        )
        rotate_cw_btn.pack(side=tk.LEFT, padx=2)
        rotate_cw_btn.bind("<ButtonPress-1>", lambda e: self.start_rotation("cw"))
        rotate_cw_btn.bind("<ButtonRelease-1>", self.stop_rotation)
        rotate_cw_btn.bind("<Leave>", self.stop_rotation)

    def create_main_layout(self):
        """创建主要布局"""
        # 主容器
        main_frame = tk.Frame(self.root, bg=PALETTE["bg_panel"])
        main_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # 左侧属性面板
        left_panel = tk.LabelFrame(main_frame, text="属性面板", width=200, bg=PALETTE["bg_labelframe"])
        left_panel.pack(side=tk.LEFT, fill=tk.Y, padx=(0, 5))
        left_panel.pack_propagate(False)
        
        # 图形信息
        info_frame = tk.LabelFrame(left_panel, text="图形信息", bg=PALETTE["bg_labelframe"]) 
        info_frame.pack(fill=tk.X, padx=5, pady=5)
        
        self.info_text = tk.Text(info_frame, height=8, width=25, wrap=tk.WORD, bg="#f8fbff")
        info_scroll = tk.Scrollbar(info_frame, orient=tk.VERTICAL, command=self.info_text.yview)
        self.info_text.configure(yscrollcommand=info_scroll.set)
        self.info_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        info_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        
        # 快速操作
        quick_frame = tk.LabelFrame(left_panel, text="快速操作", bg=PALETTE["bg_labelframe"]) 
        quick_frame.pack(fill=tk.X, padx=5, pady=5)
        
        tk.Button(quick_frame, text="新建文件", command=self.new_file,
                 width=15, bg=PALETTE["bg_button"], activebackground=PALETTE["bg_button_accent"]).pack(pady=2)
        tk.Button(quick_frame, text="打开文件", command=self.open_file,
                 width=15, bg=PALETTE["bg_button"], activebackground=PALETTE["bg_button_accent"]).pack(pady=2)
        tk.Button(quick_frame, text="保存文件", command=self.save_file,
                 width=15, bg=PALETTE["bg_button"], activebackground=PALETTE["bg_button_accent"]).pack(pady=2)
        tk.Button(quick_frame, text="导出图像", command=self.export_image,
                 width=15, bg=PALETTE["bg_button"], activebackground=PALETTE["bg_button_accent"]).pack(pady=2)
        
        # 代码生成（特色功能）
        code_frame = tk.LabelFrame(left_panel, text="代码生成", bg=PALETTE["bg_labelframe"]) 
        code_frame.pack(fill=tk.X, padx=5, pady=5)
        
        tk.Button(code_frame, text="生成HTML", command=self.generate_html,
                 width=15, bg=PALETTE["bg_button"], activebackground=PALETTE["bg_button_accent"]).pack(pady=2)
        tk.Button(code_frame, text="生成SVG", command=self.generate_svg,
                 width=15, bg=PALETTE["bg_button"], activebackground=PALETTE["bg_button_accent"]).pack(pady=2)
        
        # 右侧画布区域
        canvas_frame = tk.LabelFrame(main_frame, text="绘图区域", bg=PALETTE["bg_labelframe"]) 
        canvas_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        
        # 创建画布
        self.canvas = DrawingCanvas(canvas_frame, 800, 600)
        self.canvas.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # 更新信息显示
        self.update_info()
    
    def create_status_bar(self):
        """创建状态栏"""
        status_frame = tk.Frame(self.root, relief=tk.SUNKEN, bd=1, bg=PALETTE["bg_status"]) 
        status_frame.pack(side=tk.BOTTOM, fill=tk.X)
        
        self.status_var = tk.StringVar()
        self.status_var.set("就绪 - 选择工具开始绘制")
        
        status_label = tk.Label(status_frame, textvariable=self.status_var, anchor=tk.W, bg=PALETTE["bg_status"]) 
        status_label.pack(side=tk.LEFT, padx=5)
        
        # 图形计数
        self.count_var = tk.StringVar()
        self.count_var.set("图形数量: 0")
        
        count_label = tk.Label(status_frame, textvariable=self.count_var, anchor=tk.E, bg=PALETTE["bg_status"]) 
        count_label.pack(side=tk.RIGHT, padx=5)
    
    def handle_canvas_update(self):
        """处理画布更新事件，并更新UI"""
        self.update_info()
    
    def set_tool(self, tool: str):
        """设置工具"""
        self.canvas.set_tool(tool)
        self.tool_var.set(tool)
        tool_names = {
            "select": "选择工具",
            "point": "点工具",
            "line": "直线工具", 
            "rectangle": "矩形工具",
            "circle": "圆形工具",
            "polygon": "多边形工具",
            "cube": "立方体工具",
            "ellipse": "椭圆工具"
        }
        self.status_var.set(f"当前工具: {tool_names.get(tool, tool)}")
        self.update_info()
    
    def choose_color(self):
        """选择颜色"""
        color = colorchooser.askcolor(title="选择线条颜色")[1]
        if color:
            self.current_color = color
            self.color_button.config(bg=color)
            self.canvas.set_color(color)
            self.update_info()
    
    def choose_fill_color(self):
        """选择填充颜色"""
        color = colorchooser.askcolor(title="选择填充颜色")[1]
        if color:
            self.current_fill_color = color
            self.fill_button.config(bg=color, text="")
            self.canvas.set_fill_color(color)
        else:
            # 用户可能想要取消填充
            result = messagebox.askyesno("确认", "是否取消填充？")
            if result:
                self.current_fill_color = None
                self.fill_button.config(bg="white", text="无")
                self.canvas.set_fill_color("none")
        self.update_info()
    
    def on_width_change(self):
        """线宽改变事件"""
        width = self.width_var.get()
        self.canvas.set_line_width(width)
        self.update_info()
    
    def delete_selected(self):
        """删除选中的图形"""
        self.canvas.delete_selected()
        self.update_info()
    
    def clear_canvas(self):
        """清空画布"""
        if messagebox.askyesno("确认", "确定要清空画布吗？"):
            self.canvas.clear_canvas()
            self.update_info()
    
    def scale_selected(self, factor: float):
        """缩放选中的图形"""
        self.canvas.scale_selected(factor)
        self.update_info()

    def start_rotation(self, direction: str):
        """开始连续旋转，按下按钮即时生效并在长按时重复"""
        self.stop_rotation()
        self.rotation_direction = direction
        if not self.rotate_once(direction):
            self.rotation_direction = None
            return
        self.rotation_job = self.root.after(self.rotation_repeat_interval, self._perform_rotation)

    def _perform_rotation(self):
        if not self.rotation_direction:
            return
        if not self.rotate_once(self.rotation_direction):
            self.stop_rotation()
            return
        self.rotation_job = self.root.after(self.rotation_repeat_interval, self._perform_rotation)

    def stop_rotation(self, event=None):
        if self.rotation_job is not None:
            self.root.after_cancel(self.rotation_job)
            self.rotation_job = None
        self.rotation_direction = None

    def _rotation_angle(self, direction: str) -> float:
        """根据方向返回旋转角度"""
        if direction == "cw":  # 顺时针
            return -self.rotation_step
        elif direction == "ccw":  # 逆时针
            return self.rotation_step
        else:
            return 0.0

    def rotate_once(self, direction: str) -> bool:
        angle = self._rotation_angle(direction)
        success = self.canvas.rotate_selected(angle)
        if success:
            self.update_info()
        return success

    def toggle_rasterization(self):
        """切换绘制模式"""
        self.canvas.use_rasterization = self.rasterization_var.get()
        self.canvas.redraw()
        mode = "光栅算法" if self.canvas.use_rasterization else "Tk库函数"
        self.status_var.set(f"当前绘制模式: {mode}")
    
    def new_file(self):
        """新建文件"""
        if len(self.canvas.get_shapes()) > 0:
            if not messagebox.askyesno("确认", "当前有未保存的内容，确定新建吗？"):
                return
        
        self.canvas.clear_canvas()
        self.file_manager.new_file()
        self.root.title("图形绘制系统 - 新文件")
        self.update_info()
    
    def open_file(self):
        """打开文件"""
        shapes = self.file_manager.load_shapes()
        if shapes:
            self.canvas.set_shapes(shapes)
            filename = self.file_manager.get_current_file()
            if filename:
                self.root.title(f"图形绘制系统 - {os.path.basename(filename)}")
            self.update_info()
    
    def save_file(self):
        """保存文件"""
        shapes = self.canvas.get_shapes()
        if self.file_manager.quick_save(shapes):
            filename = self.file_manager.get_current_file()
            if filename:
                self.root.title(f"图形绘制系统 - {os.path.basename(filename)}")
    
    def save_as_file(self):
        """另存为文件"""
        shapes = self.canvas.get_shapes()
        if self.file_manager.save_shapes(shapes):
            filename = self.file_manager.get_current_file()
            if filename:
                self.root.title(f"图形绘制系统 - {os.path.basename(filename)}")
    
    def export_image(self):
        """导出图像"""
        self.file_manager.save_as_image(self.canvas)
    
    def generate_html(self):
        """生成HTML代码"""
        shapes = self.canvas.get_shapes()
        if not shapes:
            messagebox.showwarning("警告", "画布为空，无法生成代码")
            return
        
        self.code_generator.save_generated_code(shapes, "html")
    
    def generate_svg(self):
        """生成SVG代码"""
        shapes = self.canvas.get_shapes()
        if not shapes:
            messagebox.showwarning("警告", "画布为空，无法生成代码")
            return
        
        self.code_generator.save_generated_code(shapes, "svg")
    
    def update_info(self):
        """更新信息显示"""
        shapes = self.canvas.get_shapes()
        self.count_var.set(f"图形数量: {len(shapes)}")
        
        # 更新详细信息
        self.info_text.config(state=tk.NORMAL)
        self.info_text.delete(1.0, tk.END)
        
        info = f"当前工具: {self.canvas.current_tool}\n"
        info += f"线条颜色: {self.current_color}\n"
        info += f"填充颜色: {self.current_fill_color or '无'}\n"
        info += f"线宽: {self.current_line_width}\n\n"
        
        info += f"图形列表 ({len(shapes)} 个):\n"
        info += "-" * 25 + "\n"
        
        for i, shape in enumerate(shapes):
            shape_type = shape.__class__.__name__
            selected = " [选中]" if shape.selected else ""
            info += f"{i+1}. {shape_type}{selected}\n"
            
            if hasattr(shape, 'x') and hasattr(shape, 'y'):
                info += f"   位置: ({shape.x:.1f}, {shape.y:.1f})\n"
            
            if hasattr(shape, 'color'):
                info += f"   颜色: {shape.color}\n"
        
        self.info_text.insert(1.0, info)
        self.info_text.config(state=tk.DISABLED)
    
    def show_help(self):
        """显示帮助信息"""
        help_text = """图形绘制系统使用说明

基本操作:
• 选择工具：点击图形可选中，拖拽可移动
• 点工具：单击创建点
• 直线工具：拖拽创建直线
• 矩形工具：拖拽创建矩形
• 圆形工具：拖拽创建圆形
• 多边形工具：点击添加顶点，双击完成
• 立方体工具：单击创建，拖拽旋转

快捷键:
• Ctrl+N: 新建文件
• Ctrl+O: 打开文件
• Ctrl+S: 保存文件
• Delete: 删除选中图形
• Ctrl+Del: 清空画布
• Ctrl+=: 放大选中图形
• Ctrl+-: 缩小选中图形

特色功能:
• 自动生成HTML Canvas代码
• 自动生成SVG代码
• 支持图形属性实时调整
• 支持多种文件格式导入导出"""
        
        help_window = tk.Toplevel(self.root)
        help_window.title("使用说明")
        help_window.geometry("500x400")
        help_window.resizable(False, False)
        
        text_widget = tk.Text(help_window, wrap=tk.WORD, padx=10, pady=10)
        scrollbar = tk.Scrollbar(help_window, orient=tk.VERTICAL, command=text_widget.yview)
        text_widget.configure(yscrollcommand=scrollbar.set)
        
        text_widget.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        text_widget.insert(1.0, help_text)
        text_widget.config(state=tk.DISABLED)
    
    def show_about(self):
        """显示关于信息"""
        about_text = """图形绘制系统 v1.0

计算机图形学第一次作业
支持基本二维图形绘制与交互操作

主要功能:
✓ 绘制点、直线、矩形、圆形、多边形、立方体
✓ 图形选择、移动、缩放、删除
✓ 颜色和线宽设置
✓ 文件保存和加载
✓ 图像导出
✓ 前端代码生成

技术特点:
• 基于Python + Tkinter开发
• 面向对象的图形类设计
• 支持多种文件格式
• 自动生成前端代码

开发时间: 2025年9月
"""
        messagebox.showinfo("关于", about_text)
    
    def on_closing(self):
        """关闭程序"""
        self.stop_rotation()
        if len(self.canvas.get_shapes()) > 0:
            result = messagebox.askyesnocancel("确认退出", "是否保存当前工作？")
            if result is True:  # 保存后退出
                if self.save_file():
                    self.root.destroy()
            elif result is False:  # 不保存直接退出
                self.root.destroy()
            # result is None表示取消，不做任何操作
        else:
            self.root.destroy()
    
    def run(self):
        """运行应用程序"""
        # 启动时更新一次信息
        self.root.after(100, self.update_info)
        self.root.mainloop()


def main():
    """主函数"""
    try:
        app = DrawingApplication()
        app.run()
    except Exception as e:
        print(f"程序启动失败: {e}")
        messagebox.showerror("错误", f"程序启动失败: {e}")


if __name__ == "__main__":
    main()