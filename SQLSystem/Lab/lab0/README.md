# 安装与实验说明

## 下载与安装
- **SQL Server:** Microsoft® SQL Server 2022 - Express Edition
  官方下载：
  https://www.microsoft.com/zh-cn/sql-server/sql-server-downloads
- **SSMS (SQL Server Management Studio):** 建议安装 SSMS（建议使用 `19.3` 版本，安装后若提示更新请不要立即更新）
  SSMS 版本历史：
  https://learn.microsoft.com/zh-cn/ssms/release-history

### Windows 安装教程
- 图文教程： https://blog.csdn.net/m0_62975468/article/details/129696245
- 官方教程： https://learn.microsoft.com/zh-cn/sql/database-engine/install-windows/install-sql-server?view=sql-server-ver16

### Linux 安装教程
- 官方教程： https://learn.microsoft.com/zh-cn/sql/linux/sql-server-linux-setup?view=sql-server-ver17

> 注：实验室电脑已安装好 SQL Server 和 SSMS，可直接使用。

## 实验步骤
1. 熟悉 SQL Server 环境。
2. 在 SSMS 中执行 SQL 语句：
   - 打开 SSMS，选择数据库后点击工具栏的“新建查询”或使用快捷键 `Ctrl+N`。
   - 在查询编辑区输入 SQL，例如：

```sql
CREATE DATABASE StudentDB;
```

   - 按 `F5` 或点击“执行”运行语句。
3. 按实验教程第一章进行实验。
