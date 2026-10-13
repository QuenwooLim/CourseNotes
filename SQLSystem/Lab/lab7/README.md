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
  <tr><td align="right"style="border-bottom:1px solid #000;">实验日期：</td><td align="left" style="border-bottom:1px solid #000;">2025年11月9日</td></tr>
  </table>
</div>
</div>

<div STYLE="page-break-after: always;"></div>

# 数据库存储过程和函数实验报告

## ✏️ 实验目的

掌握存储过程和函数

## 📋 实验内容
- 实验平台：SSMS
- 对于每一个实验问题，都会从**sql语法分析**、**sql代码**、**实验结果**这三个方面进行回答

### 1.无参数的存储过程或函数

定义一个存储过程或函数：实现打印 '本人学号+姓名'（例：张三+22111001）

- 语法分析
  - 存储过程（Stored Procedure）是一组预编译的SQL语句集合，可以通过调用来执行。存储过程可以接受参数，执行复杂的操作，并返回结果。
  - 函数（Function）也是一组预编译的SQL语句集合，但它必须返回一个值。函数通常用于计算和返回单个值，可以在SQL语句中调用。
-  SQL代码
```sql
-- 创建存储过程
CREATE PROCEDURE PrintStudentInfo
AS
BEGIN
    PRINT '23320093 林宏宇';
END;

-- 调用存储过程
EXEC PrintStudentInfo;
```
- 实验结果

![image](.\source\1.png)

### 2.有参数的存储过程或函数

#### 2.0首先将数据库中的图书表图书价格更新如下：

|book_id|book_name|book_price|
|-------|---------|----------|
|b0001|SQL Server 2012宝典|98.00|
|b0002|职称英语专用教材|36.00|
|b0003|中国通史|57.00|
|b0004|丰子恺儿童文学选集|19.80|
|b0005|英语同义词辨析词典|63.50|
|b0006|数据库基础与应用|33.00|
|b0007|微积分初步|78.50|
|b0008|ASP.NET从入门到精通|69.80|

> 此处也相应的修改record表用于后续查询

- SQL代码
```sql
-- 修改book_price
UPDATE book
SET book_price = CASE book_id
    WHEN 'b0001' THEN 98.00
    WHEN 'b0002' THEN 36.00
    WHEN 'b0003' THEN 57.00
    WHEN 'b0004' THEN 19.80
    WHEN 'b0005' THEN 63.50
    WHEN 'b0006' THEN 33.00 
    WHEN 'b0007' THEN 78.50
    WHEN 'b0008' THEN 69.80
    ELSE book_price
END;
```

- 结果展示

![image](.\source\2.png)

![image](.\source\3.png)

#### 2.1定义一个存储过程或函数，找出某个同学（输入名字，如“谢嫣然”）所读的所有图书名

- SQL语法分析
  - 存储过程可以接受输入参数，通过这些参数来执行特定的查询操作。在这个例子中，我们将定义一个存储过程，接受学生姓名作为输入参数，并查询该学生所读的所有图书名。
  - 使用@符号来定义输入参数，并在存储过程中使用JOIN语句连接相关的表，以获取所需的信息。
  - 调用时，使用EXEC命令并@传递具体的参数值来执行存储过程。

- SQL代码
```sql
-- 创建存储过程
CREATE PROCEDURE GetBooksByStudentName
    @StudentName NVARCHAR(100)
AS
BEGIN
    SELECT b.book_name
    FROM reader s
    JOIN record r ON s.reader_id = r.reader_id
    JOIN book b ON r.book_id = b.book_id
    WHERE s.reader_name = @StudentName;
END;
-- 调用存储过程
EXEC GetBooksByStudentName @StudentName = '谢嫣然';
```

- 结果展示

![image](.\source\4.png)

#### 2.2定义一个存储过程或自函数

如果图书的价格小于等于35块，增加图书的价格为 book_price *（1+discount）；如果图书价格大于55块，图书的价格打个折扣，更改为 book_price *（1-discount）（输入为discount ，book_price 更新为book_price *（1+discount）或book_price *（1-discount））注意：不要互相影响。例如 discount=0.9，那么 30元的图书更新为 30*（1+0.9）=57；到这里就应该结束，不要继续做 57*（1-0.9）

- SQL语法分析
  - 这里需要分情况考虑，先更新价格小于等于35的数据，再更新价格大于55的数据，避免互相影响。
  - 使用UPDATE语句来修改book表中的book_price字段，根据输入的discount参数进行计算。
  - 为防止触发器影响更新操作，先禁用相关触发器。

- SQL代码
```sql
-- 停止触发器trg_InsteadOfUpdateBookPrice
DISABLE TRIGGER trg_InsteadOfUpdateBookPrice ON book;

-- 创建存储过程
CREATE PROCEDURE UpdateBookPrices
    @discount FLOAT
AS
BEGIN
    -- 先更新价格小于等于35的图书
    UPDATE book
    SET book_price = book_price * (1 + @discount)
    WHERE book_price <= 35;
    
    -- 再更新价格大于55的图书
    UPDATE book
    SET book_price = book_price * (1 - @discount)
    WHERE book_price > 55;
END;

-- 调用存储过程
EXEC UpdateBookPrices @discount = 0.1;

-- 删除错误的存储过程
-- DROP PROCEDURE UpdateBookPrices;

```

- 结果展示

![image](.\source\5.png)

#### 2.3给定下面的导师关系表，输入为姓名，找出该学生的师承关系（导师、导师的导师、……），并返回所有相关导师的姓名。例如，输入Mary，输出为 Susan, John

|MetorName|StudentName|
|---------|-----------|
|Alice|Bob|
|Alice|Carol|
|David|Alice|
|Mary|David|
|Susan|Mary|
|John|Susan|

- SQL语法分析
  - 先创建相应的表并插入数据
  - 这里需要使用递归查询来找出学生的师承关系。可以使用CTE（Common Table Expression）来实现递归查询。
  - 定义一个存储过程，接受学生姓名作为输入参数，并使用递归CTE来查找所有相关导师的姓名。
  - 使用UNION ALL将当前导师与其导师连接起来，直到没有更多的导师为止。
- SQL代码
```sql
-- 创建导师关系表
CREATE TABLE MentorStudent (
    MentorName NVARCHAR(100),
    StudentName NVARCHAR(100)
); 
-- 插入数据
INSERT INTO MentorStudent (MentorName, StudentName) VALUES
('Alice', 'Bob'),
('Alice', 'Carol'),
('David', 'Alice'),
('Mary', 'David'),
('Susan', 'Mary'),
('John', 'Susan');
-- 创建存储过程
CREATE PROCEDURE GetMentorChain
    @StudentName NVARCHAR(100)
AS
BEGIN
    WITH MentorCTE AS (
        SELECT MentorName
        FROM MentorStudent
        WHERE StudentName = @StudentName
        UNION ALL
        SELECT ms.MentorName
        FROM MentorStudent ms
        INNER JOIN MentorCTE mcte ON ms.StudentName = mcte.MentorName
    )
    SELECT MentorName FROM MentorCTE;
END;

-- 调用存储过程
EXEC GetMentorChain @StudentName = 'Mary';
```

- 结果展示

![image](.\source\6.png)

### 3.游标: 定义一个存储过程或函数，用游标的方式计算所有图书的总价。
- SQL语法分析
  - 游标（Cursor）是一种数据库对象，用于逐行处理查询结果集。在这个例子中，我们将使用游标来遍历book表中的所有图书价格，并计算总价。
  - 定义一个存储过程，使用DECLARE语句声明游标，OPEN语句打开游标，FETCH语句逐行获取数据，并在循环中累加价格，最后关闭游标并返回总价。
- SQL代码
```sql
-- 创建存储过程
CREATE PROCEDURE CalculateTotalBookPrice
AS
BEGIN
    DECLARE @TotalPrice FLOAT = 0;
    DECLARE @BookPrice FLOAT;

    DECLARE BookCursor CURSOR FOR
    SELECT book_price FROM book;

    OPEN BookCursor;

    FETCH NEXT FROM BookCursor INTO @BookPrice;

    WHILE @@FETCH_STATUS = 0
    BEGIN
        SET @TotalPrice = @TotalPrice + @BookPrice;
        FETCH NEXT FROM BookCursor INTO @BookPrice;
    END;

    CLOSE BookCursor;
    DEALLOCATE BookCursor;

    -- 输出总价
    SELECT @TotalPrice AS TotalBookPrice;
END;

-- 调用存储过程
EXEC CalculateTotalBookPrice;
```

- 结果展示

![image](.\source\7.png)

## 💡 实验总结

### 语法总结

本次实验主要涉及SQL Server中存储过程和函数的创建与使用,核心语法要点如下:

1. **存储过程的创建与调用**
   - 创建语法: `CREATE PROCEDURE 过程名 [@参数名 数据类型] AS BEGIN ... END`
   - 调用语法: `EXEC 过程名 [@参数名 = 参数值]`
   - 使用`PRINT`语句输出信息

2. **参数的使用**
   - 输入参数使用`@`符号定义,如`@StudentName NVARCHAR(100)`
   - 在过程体中通过`WHERE`条件使用参数进行筛选
   - 支持多个参数传入,实现灵活的查询和更新操作

3. **条件判断与更新**
   - 使用`IF`语句进行条件判断
   - `UPDATE`语句配合`WHERE`子句实现有条件的数据更新
   - 注意更新顺序,避免互相影响(如价格调整问题)

4. **递归查询(CTE)**
   - 使用`WITH ... AS ()`定义公用表表达式
   - 通过`UNION ALL`实现递归,查询层级关系
   - 适用于树形结构数据的遍历(如师承关系)

5. **游标的使用**
   - 声明游标: `DECLARE 游标名 CURSOR FOR SELECT语句`
   - 打开游标: `OPEN 游标名`
   - 提取数据: `FETCH NEXT FROM 游标名 INTO @变量`
   - 循环处理: `WHILE @@FETCH_STATUS = 0`
   - 关闭释放: `CLOSE 游标名; DEALLOCATE 游标名`

### 不足与改进

1. **上次课程中触发器的影响**: 在价格更新实验中,由于触发器的存在,导致更新结果不符合预期。今后在设计存储过程时,需要考虑触发器对数据操作的影响,必要时禁用相关触发器。
2. **嵌套操作的问题**: 在处理复杂的更新逻辑时,需要注意操作的顺序和依赖关系,避免出现数据互相影响的情况。可以通过分步更新来解决此类问题。

### 心得体会

通过本次实验,我深入理解了SQL Server中存储过程和函数的实际应用:

1. **存储过程的优势**: 存储过程可以封装复杂的业务逻辑,减少网络传输,提高执行效率。预编译的特性使得重复执行时性能更好,同时也增强了代码的安全性和可维护性。

2. **参数化的重要性**: 通过参数化查询,可以实现灵活的数据操作,避免SQL注入风险。在实际开发中,应该优先使用参数化的存储过程而不是动态拼接SQL语句，这样可以提高代码的安全性和可读性。

3. **递归查询的强大**: CTE递归查询为处理层级关系数据提供了优雅的解决方案。师承关系的查询让我认识到,递归在处理树形、图形结构数据时非常实用。

总的来说,本次实验让我对数据库编程有了更深入的认识,掌握了存储过程和函数这一重要的数据库对象,为今后开发复杂的数据库应用打下了坚实基础。

## 📚 参考资料
- 实验课件
- 作业

## 附件
- 无（代码已经在报告中逐步展示）