-- 创建表
CREATE TABLE book (
    book_id CHAR(8) NOT NULL,
    book_name NVARCHAR(50) NOT NULL,
    book_isbn CHAR(17) NOT NULL,
    book_author NVARCHAR(10) NOT NULL,
    book_publisher NVARCHAR(50) NOT NULL,
    book_price MONEY NOT NULL,
    interview_times SMALLINT NOT NULL
);

-- 添加列说明（Extended Property）
EXEC sp_addextendedproperty 
    @name = N'MS_Description', @value = N'图书编号', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'book', 
    @level2type = N'COLUMN', @level2name = 'book_id';

EXEC sp_addextendedproperty 
    @name = N'MS_Description', @value = N'书名', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'book', 
    @level2type = N'COLUMN', @level2name = 'book_name';

EXEC sp_addextendedproperty 
    @name = N'MS_Description', @value = N'图书isbn号', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'book', 
    @level2type = N'COLUMN', @level2name = 'book_isbn';

EXEC sp_addextendedproperty 
    @name = N'MS_Description', @value = N'作者', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'book', 
    @level2type = N'COLUMN', @level2name = 'book_author';

EXEC sp_addextendedproperty 
    @name = N'MS_Description', @value = N'出版社', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'book', 
    @level2type = N'COLUMN', @level2name = 'book_publisher';

EXEC sp_addextendedproperty 
    @name = N'MS_Description', @value = N'价格', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'book', 
    @level2type = N'COLUMN', @level2name = 'book_price';

EXEC sp_addextendedproperty 
    @name = N'MS_Description', @value = N'借阅次数', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'book', 
    @level2type = N'COLUMN', @level2name = 'interview_times';


-- 检验说明是否添加成功
USE jy --jy替换成你的databese名
SELECT
A.name AS table_name,
B.name AS column_name,
C.value AS column_description
FROM sys.tables A
INNER JOIN sys.columns B ON B.object_id = A.object_id
LEFT JOIN sys.extended_properties C ON C.major_id = B.object_id AND C.minor_id = B.column_id
WHERE A.name = 'book' --把table替换成你要查询的表名


-- 创建读者表
CREATE TABLE reader(
	reader_id CHAR(8) NOT NULL,
	reader_name NVARCHAR(50) NOT NULL,
	reader_sex CHAR(2) NOT NULL,
	reader_department NVARCHAR(60) NOT NULL
);

-- 创建借阅记录表
CREATE TABLE record(
	reader_id CHAR(8) NOT NULL,
	book_id CHAR(8) NOT NULL,
	borrow_date date NOT NULL,
	return_date date NOT NULL,
	notes nvarchar(50) NOT NULL
);


-- 插入新列 not null 指定默认值为6
ALTER TABLE book ADD total SMALLINT NOT NULL;

-- 修改列
ALTER TABLE book ALTER COLUMN interview_times INT NOT NULL;

-- 删除列
-- ALTER TABLE book DROP COLUMN total;
-- 显示错误：

-- 1. 删除默认约束
ALTER TABLE book DROP CONSTRAINT DF__book__total__3A81B327;

-- 2. 删除 total 列
ALTER TABLE book DROP COLUMN total;