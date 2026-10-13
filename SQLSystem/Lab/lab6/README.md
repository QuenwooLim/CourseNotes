<div align="center">
  <img src=".\source\sysu.jpeg" alt="中山大学校徽" width="500"/>  

<br><br><br>
</div>
<div style="font-size:1.6em; font-weight:normal; line-height:1.6;">
<div style="text-align:center; font-size:2.9em; font-weight:normal; letter-spacing:0.1em;">实验作业报告</div>
<br/>
<br>
<div style="text-align:center; font-size:1.3em; line-height:1.8;">
  <table style="margin: 0 auto; font-size:1.1em;">
  <tr><td align="right">实验：</td><td align="left">数据库系统实验</td></tr>
  <tr><td align="right">学号：</td><td align="left">23320093</td></tr>
  <tr><td align="right">姓名：</td><td align="left">林宏宇</td></tr>
  <tr><td align="right">专业：</td><td align="left">计算机科学与技术</td></tr>
  <tr><td align="right">班级：</td><td align="left">计科1班</td></tr>
  <tr><td align="right">指导教师：</td><td align="left">赖韩江</td></tr>
  <tr><td align="right"style="border-bottom:1px solid #000;">实验日期：</td><td align="left" style="border-bottom:1px solid #000;">2025年11月2日</td></tr>
  </table>
</div>
</div>

<div STYLE="page-break-after: always;"></div>

# 数据库系统实验报告

## ✏️ 实验目的

通过实验加深对数据完整性的理解，学会创建和使用触发器

## 📋 实验内容
- 实验平台：SSMS
- 对于每一个实验问题，都会从**sql语法分析**、**sql代码**、**实验结果**这三个方面进行回答

### 1、创建一个触发器，当插入、更新或删除借阅记录表record的数据行时，能同时更新图书表book中借阅次数interviews_times

#### 分析
- 触发器类型：AFTER INSERT, UPDATE, DELETE
- 触发器作用：更新图书表book中借阅次数interviews_times
- 触发器逻辑：
  - INSERT：当有新的借阅记录插入时，找到对应的图书ID，借阅次数加1
  - DELETE：当有借阅记录被删除时，找到对应的图书ID，借阅次数减1
  - UPDATE：当借阅记录的图书ID被修改时，原图书ID的借阅次数减1，新图书ID的借阅次数加1

#### sql代码
```sql
CREATE TRIGGER trg_UpdateInterviewTimes
ON record
AFTER INSERT, UPDATE, DELETE
AS
BEGIN
    -- 处理插入操作（1条插入 0条删除）
    IF EXISTS(SELECT * FROM inserted) AND NOT EXISTS(SELECT * FROM deleted)
    BEGIN
        UPDATE book
        SET interviews_times = interviews_times + 1
        FROM book b
        JOIN inserted i ON b.book_id = i.book_id;
    END
    -- 处理删除操作（0条插入 1条删除）
    IF EXISTS(SELECT * FROM deleted) AND NOT EXISTS(SELECT * FROM inserted)
    BEGIN
        UPDATE book
        SET interviews_times = interviews_times - 1
        FROM book b
        JOIN deleted d ON b.book_id = d.book_id;
    END
    -- 处理更新操作（1条插入 1条删除）
    IF EXISTS(SELECT * FROM inserted) AND EXISTS(SELECT * FROM deleted)
    BEGIN
        UPDATE book
        SET interviews_times = interviews_times - 1
        FROM book b
        JOIN deleted d ON b.book_id = d.book_id;
        UPDATE book
        SET interviews_times = interviews_times + 1
        FROM book b
        JOIN inserted i ON b.book_id = i.book_id;
    END
END

-- 展示trigger创建结果
SELECT * FROM sys.triggers;
```
#### 结果展示

![trg_UpdateInterviewTimes创建结果](.\source\1.png)

### 2、创建AFTER UPDATE触发器，实现当修改读者表reader中的reader_department数据时，打印提示（例如修改李媛媛的reader_department为计算机系时，得到的提示是“修改数据读者名：李媛媛；修改前：经济管理系；修改后：计算机系”）

#### 分析
- 触发器类型：AFTER UPDATE
- 触发器作用：打印提示信息
- 触发器逻辑：
  - 检查reader_department字段是否被修改
  - 如果被修改，打印修改前后的信息
- Declared变量：@old_department, @new_department 用于存储修改前后的部门信息

#### sql代码
``` sql
CREATE TRIGGER trg_AfterUpdateReaderDepartment
ON reader
AFTER UPDATE
AS
BEGIN
    SET NOCOUNT ON;

    IF UPDATE(reader_department)
    BEGIN
        DECLARE @old_department NVARCHAR(100), @new_department NVARCHAR(100), @reader_name NVARCHAR(100);

        -- 把值赋给变量（若多行，这里会取最后一行的值；仅用于单行场景）
        SELECT @old_department = d.reader_department FROM deleted d;
        SELECT @new_department = i.reader_department, @reader_name = i.reader_name FROM inserted i;

        PRINT N'修改数据读者名：' + ISNULL(@reader_name, N'') 
              + N'；修改前：' + ISNULL(@old_department, N'') 
              + N'；修改后：' + ISNULL(@new_department, N'');
    END
END;

-- 展示trigger创建结果
SELECT * FROM sys.triggers;
```

#### 结果展示

![trg_AfterUpdateReaderDepartment创建结果](.\source\2.png)

### 3、创建AFTER INSERT触发器，实现禁止向图书表book插入数据的功能

#### 分析
- 触发器类型：AFTER INSERT
- 触发器作用：禁止插入数据
- 触发器逻辑：
  - 使用ROLLBACK TRANSACTION回滚插入操作, 实现无法插入的效果

#### sql代码
``` sql
CREATE TRIGGER trg_AfterInsertBook
ON book
AFTER INSERT
AS
BEGIN
    ROLLBACK TRANSACTION;
    PRINT '禁止向图书表插入数据';
END;
-- 展示trigger创建结果
SELECT * FROM sys.triggers;
```

#### 结果展示

![trg_AfterInsertBook创建结果](.\source\3.png)

### 4、创建触发器，用于实现如果修改了读者表reader中的数据时，显示“已修改reader表的数据”的消息，否则返回“不存在要修改的数据或未修改数据”

#### 分析
- 触发器类型：AFTER UPDATE
- 触发器作用：显示修改消息
- 触发器逻辑：
  - 检查是否有数据被修改
  - 如果有，显示“已修改reader表的数据”，否则显示“不存在要修改的数据或未修改数据”

#### sql代码
``` sql
CREATE TRIGGER trg_AfterUpdateReader
ON reader
AFTER UPDATE
AS
BEGIN
    SET NOCOUNT ON
    IF EXISTS(SELECT * FROM inserted) 
    BEGIN
        PRINT '已修改reader表的数据';
    END
    ELSE
    BEGIN
        PRINT '不存在要修改的数据或未修改数据';
    END
END;
-- 展示trigger创建结果
SELECT * FROM sys.triggers;
```

#### 结果展示

![trg_AfterUpdateReader创建结果](.\source\4.png)

### 5、分别创建INSTEAD OF和AFTER 触发器，当修改图书表中book_price的价格为原价的90%~120%之间时，才允许修改，并显示修改后的价格变化（例如涨价了7.5%），否则显示“价格变动太大”。 （了解两类触发器的区别）

#### 分析
- 触发器类型：INSTEAD OF UPDATE 和 AFTER UPDATE
- 触发器作用：控制价格修改范围并显示变化
- 触发器逻辑：
  - INSTEAD OF UPDATE：检查修改后的价格是否在90%~120%范围内
    - 如果在范围内，执行更新操作
    - 如果不在范围内，打印“价格变动太大”
  - AFTER UPDATE：计算并显示价格变化百分比

#### sql代码
```sql
-- INSTEAD OF UPDATE触发器
CREATE TRIGGER trg_InsteadOfUpdateBookPrice
ON book
INSTEAD OF UPDATE
AS
BEGIN
    SET NOCOUNT ON
    DECLARE @old_price DECIMAL(10, 2), @new_price DECIMAL(10, 2);
    SELECT @old_price = d.book_price FROM deleted d;
    SELECT @new_price = i.book_price FROM inserted i;
    IF @new_price BETWEEN @old_price * 0.9 AND @old_price * 1.2
    BEGIN
        UPDATE book
        SET book_price = @new_price
        WHERE book_id = (SELECT book_id FROM inserted);
    END
    ELSE
    BEGIN
        PRINT '价格变动太大';
    END
END;

-- AFTER UPDATE触发器
CREATE OR ALTER TRIGGER trg_AfterUpdateBookPrice
ON book
AFTER UPDATE
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @old_price DECIMAL(18,6), @new_price DECIMAL(18,6), @change DECIMAL(18,6), @pct DECIMAL(10,2);

    -- 单行场景或测试时可用 TOP(1)，但请注意多行行为
    SELECT TOP (1) @old_price = d.book_price FROM deleted d;
    SELECT TOP (1) @new_price = i.book_price  FROM inserted i;

    IF @old_price IS NOT NULL AND @new_price IS NOT NULL AND @old_price <> 0
    BEGIN
        SET @change = @new_price - @old_price;
        SET @pct = CAST((@change / @old_price * 100.0) AS DECIMAL(10,2));  -- 固定两位小数

        IF @change > 0
            PRINT N'涨价了' + CONVERT(NVARCHAR(50), @pct) + N'%';
        ELSE IF @change < 0
            PRINT N'降价了' + CONVERT(NVARCHAR(50), ABS(@pct)) + N'%';
        ELSE
            PRINT N'价格未变化';
    END
END;

-- 展示trigger
SELECT * FROM sys.triggers;
```

#### 结果展示

![trg_InsteadOfUpdateBookPrice创建结果](.\source\5.png)


### 6、写SQL语句来验证上面1-5的触发器是否起作用。
#### sql代码
```sql
-- 验证1：插入record表
INSERT INTO record (reader_id, book_id, borrow_date, return_date, notes) VALUES ('r0002', 'b0004', '2014-05-10', '2014-05-12', NULL);
-- 验证2：更新reader表的reader_department
UPDATE reader
SET reader_department = N'计算机系'
WHERE reader_department = N'经济管理系';
-- 验证3：插入book表
INSERT INTO book (book_id, book_name, book_isbn, book_author, book_publisher, book_price, interviews_times) VALUES ('b1111', N'测试图书', 9781234567890, N'测试作者', N'测试出版社', 50.00, 0);
-- 验证4：更新reader表
UPDATE reader
SET reader_name = N'测试读者'
WHERE reader_id = 'r0201';
UPDATE reader
SET reader_name = N'李德海2'
WHERE reader_id = 'r0001';
-- 验证5：更新book表的book_price
-- 价格在允许范围内
UPDATE book
SET book_price = book_price * 1.1
WHERE book_id = 'b0001';
-- 价格超出允许范围
UPDATE book
SET book_price = book_price * 1.5
WHERE book_id = 'b0001';
```

#### 结果展示
- 验证1结果：book表中对应book_id的interviews_times增加1, 在使用前需要**禁用**`trg_InsteadOfUpdateBookPrice`触发器

插入前:

![](.\source\6.png)

插入后:

![](.\source\7.png)

- 验证2结果：控制台打印修改前后部门信息

![](.\source\8.png)

- 验证3结果：插入操作被回滚，book表中无新增数据，禁止插入生效

![](.\source\9.png)

- 验证4结果：控制台打印“已修改reader表的数据”

![](.\source\10.png)

- 验证5结果：第一次更新成功，控制台打印涨价百分比；第二次更新失败，控制台打印“价格变动太大”

![](.\source\11.png)

### 7、删除2，3的触发器
#### 分析
- 删除触发器使用DROP TRIGGER语句
- 指定要删除的触发器名称

#### sql代码
```sql
DROP TRIGGER IF EXISTS trg_AfterUpdateReaderDepartment;
DROP TRIGGER IF EXISTS trg_AfterInsertBook;
```
#### 结果展示
![删除触发器结果](.\source\12.png)

## 💡 实验总结


### 语法总结

本次实验主要练习了 SQL Server 中触发器（trigger）的定义与使用，涉及的要点包括：

- 触发器类型：常见的有 AFTER（操作完成后触发）和 INSTEAD OF（替代原操作执行，常用于视图或需要先校验再提交的场景）。
- 伪表 inserted / deleted：在触发器内部通过这两个只读伪表获取新旧行数据，必须以集合（set-based）方式处理，多行操作时不能依赖单一标量变量来接收值。 
- 列变更检测：可以使用 IF UPDATE(column_name) 判断某列是否在 UPDATE 中被修改，但仍需结合 inserted/deleted 做更精确的比较。 
- 控制与反馈：常见用法包括 SET NOCOUNT ON（避免返回多余的计数信息）、使用事务与 ROLLBACK 控制是否回滚操作、使用 PRINT/RAISERROR/THROW 提示或抛出错误。 
- 设计与性能：触发器应尽量保持简短和幂等，避免在触发器内执行耗时的外部调用或复杂查询，以免影响 DML 性能和并发吞吐。

总结：触发器适合做数据完整性保障与级联维护（如借阅次数统计），但复杂逻辑应谨慎放入触发器，注意多行与并发场景的正确处理。

### 反思

- 多行场景的处理误区：实验中用 SELECT 把 inserted/deleted 的值赋给标量变量只适用于单行测试，实际应用必须用 JOIN / EXISTS / MERGE 或基于集合的 UPDATE 来处理多行。 
- 副作用与可观察性：用 PRINT 打印信息方便调试，但生产环境应使用日志表、审计或抛错机制（THROW/RAISERROR）来记录和通知错误，避免依赖控制台输出作为业务判断依据。 
- 事务与回滚风险：使用 ROLLBACK 可以直接拒绝不合法操作（如禁止插入），但要注意触发器内的回滚会影响整个事务边界，可能导致上层应用难以定位问题。 
- 测试覆盖不足：应补充对边界情况的验证，例如空集插入、重复更新、price 为 0、NULL 值、外键约束冲突和并发修改等。

### 心得体会

通过本次实验，我对触发器的触发时机、伪表用法以及 INSTEAD OF 与 AFTER 的差异有了更直观的理解。实践中实现了借阅次数自动维护、基于价格波动的更新控制以及禁止插入的回滚逻辑，体会到触发器在保证数据完整性方面的便捷性，也认识到滥用触发器可能造成的复杂性和调试困难，后续应该针对具体要求设计完备的触发器逻辑，并加强对多行操作和异常场景的处理能力。

## 📚 参考资料
- 实验课件
- 作业

## 附件
- 无（代码已经在报告中逐步展示）