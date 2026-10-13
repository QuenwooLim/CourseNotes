from PIL import Image, ImageDraw, ImageFont

# Book Allocation Problem - Main Flow (English pseudocode)
steps = [
    "Start",
    "Input: pages[] (N books), students M",
    "left = max(pages[]),  right = sum(pages[]),  answer = right",
    """
    Binary Search: 
        while left <= right:
            mid = (left + right) // 2
            if is_possible(pages, M, mid):
                answer = mid
                right = mid - 1  # search smaller value
            else:
                left = mid + 1   # search larger value
    """,
    """
    is_possible(pages, M, mid):
        if is_possible:
            answer = mid
            right = mid - 1  # left half search
        else:
            left = mid + 1   # right half search
    """
    "Return answer (minimum possible maximum pages)",
    "End"
]

# 图像参数

# 美观参数
width = 500
height_per_step = 100
margin = 50
box_width = 360
box_height = 60
arrow_length = 40
radius = 20  # 圆角

box_fill = "#e3f2fd"
cond_fill = "#fff9c4"
text_color = "#222"
arrow_color = "#1976d2"


# 动态计算每个方框高度，保证箭头和方框依次排列不遮挡
try:
    font = ImageFont.truetype("/System/Library/Fonts/PingFang.ttc", 28)
except:
    font = ImageFont.load_default()

box_heights = []
for step in steps:
    is_multiline = isinstance(step, str) and (step.strip().startswith('while') or step.strip().startswith('is_possible') or ('\n' in step))
    cur_box_height = box_height
    if is_multiline:
        cur_box_height = int(box_height * 2.2)
    box_heights.append(cur_box_height)

# 计算总高度
total_height = sum(box_heights) + (len(steps) - 1) * arrow_length + margin * 2

# 创建画布
img = Image.new("RGB", (width, total_height), "white")
draw = ImageDraw.Draw(img)

# 绘制流程
y = margin
for i, step in enumerate(steps):
    x = (width - box_width) // 2
    is_cond = "if" in step or "?" in step
    is_multiline = isinstance(step, str) and (step.strip().startswith('while') or step.strip().startswith('is_possible') or ('\n' in step))
    fill_color = cond_fill if is_cond else box_fill
    cur_box_height = box_heights[i]
    draw.rounded_rectangle([x, y, x + box_width, y + cur_box_height], radius=radius, outline=arrow_color, width=3, fill=fill_color)
    try:
        bbox = draw.textbbox((0, 0), step, font=font)
        w, h = bbox[2] - bbox[0], bbox[3] - bbox[1]
    except AttributeError:
        w, h = font.getsize(step)
    text_x = x + (box_width - w) // 2
    text_y = y + (cur_box_height - h) // 2
    draw.text((text_x, text_y), step, fill=text_color, font=font)
    # 画箭头
    if i < len(steps) - 1:
        arrow_start = (width // 2, y + cur_box_height)
        arrow_end = (width // 2, y + cur_box_height + arrow_length)
        draw.line([arrow_start, arrow_end], fill=arrow_color, width=4)
        arrow_size = 10
        draw.polygon([
            (arrow_end[0] - arrow_size, arrow_end[1] - arrow_size),
            (arrow_end[0] + arrow_size, arrow_end[1] - arrow_size),
            (arrow_end[0], arrow_end[1])
        ], fill=arrow_color)
    y += cur_box_height + arrow_length

# 展示图片
img.show()
# 保存图片
img.save("flowchart.png")
