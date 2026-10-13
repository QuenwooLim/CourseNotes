import http.server
import socketserver
import webbrowser

PORT = 8000
url = f"http://localhost:{PORT}/goodhouse.html"

print(f"正在启动本地服务器，端口 {PORT} ...")
print(f"请在浏览器中访问：{url}")

# 自动打开浏览器（可选）
try:
    webbrowser.open(url)
except Exception:
    pass

Handler = http.server.SimpleHTTPRequestHandler
with socketserver.TCPServer(("", PORT), Handler) as httpd:
    httpd.serve_forever()