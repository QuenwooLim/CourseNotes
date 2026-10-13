#!/bin/bash
# 自动生成纹理清单文件 textures.json
# 使用方法: cd /root/autodl-tmp/computerGraphic/model/textures && bash generate_textures_json.sh

echo "正在扫描纹理文件..."

# 生成JSON文件
echo "[" > textures.json

# 获取所有图片文件（排除textures.json自身）
files=$(ls -1 *.png *.jpg *.jpeg 2>/dev/null | grep -v "textures.json")

# 计数
count=0
total=$(echo "$files" | wc -l)

# 遍历文件
for file in $files; do
    count=$((count + 1))
    if [ $count -lt $total ]; then
        echo "  \"$file\"," >> textures.json
    else
        echo "  \"$file\"" >> textures.json
    fi
done

echo "]" >> textures.json

echo "✓ 成功生成 textures.json，共 $count 个纹理文件"
echo "文件位置: $(pwd)/textures.json"
