-- use databse JY
use JY;
-- insert data into table book
INSERT INTO book (
	book_id,
	book_name,
	book_isbn,
	book_author,
	book_publisher,
	interview_times,
	book_price
)
VALUES ('b0001', 'SQL Server 2012宝典', '978-7-121-22013-5', '廖梦怡', '电子工业出版社', 18, 89.00),
('b0002', '职称英语专用教材', '978-7-121-14800-2', '孙若红', '电子工业出版社', 35, 45.00),
('b0003', '中国通史', '978-7-5388-53155', '于海娣', '黑龙江科学技术出版社', 25, 68.00),
('b0004', '丰子恺儿童文学选集', '978-7-5007-8972-7', '丰子恺', '中国少年儿童出版社', 40, 22.50),
('b0005', '英语同义词辨析词典', '978-7-5135-2294-6', '赵同水', '外语教学与研究出版社', 6, 55.00),
('b0006', '数据库基础与应用', '978-7-304-06430-3', '徐孝凯', '中央广播电视大学出版社', 5, 35.00),
('b0007', '微积分初步', '978-7-304-03742-0', '赵坚', '中央广播电视大学出版社', 4, 17.00),
('b0008', 'ASP.NET从入门到精通', '978-7-302-28753-7', '明日科技', '清华大学出版社', 27, 89.80);

-- insert data into table reader
INSERT INTO reader(
    reader_id,
    reader_name,
    reader_sex,
    reader_department
)
VALUES('r0001', '李德海', '男', '信息工程系'),
('r0002', '柳承运', '男', '信息工程系'),
('r0003', '安歌', '女', '涉外教育系'),
('r0004', '谢嫣然', '女', '涉外教育系'),
('r0005', '陈静玉', '女', '涉外教育系'),
('r0006', '李媛媛', '女', '经济管理系'),
('r0007', '胡锦波', '男', '经济管理系'),
('r0008', '蔡明伟', '男', '行政管理系');

-- insert data into table record
INSERT INTO record(
    reader_id,
    book_id,
    borrow_date,
    return_date,
    notes
)
VALUES('r0001', 'b0003', '2014-01-12', '2014-01-12', NULL),
('r0001', 'b0005', '2014-01-26', '2014-06-21', NULL),
('r0004', 'b0001', '2014-03-02', '2014-04-20', NULL),
('r0004', 'b0008', '2014-03-26', '2014-05-28', NULL),
('r0006', 'b0001', '2014-04-16', '2014-07-11', NULL),
('r0007', 'b0006', '2014-05-08', '2014-09-17', NULL),
('r0008', 'b0008', '2014-06-29', '2014-08-29', NULL),
('r0008', 'b0007', '2014-08-15', '2014-10-21', NULL);


-- delete the limitation of note about not null
-- can be null
ALTER TABLE record ALTER COLUMN notes VARCHAR(50);
-- the same as above
ALTER TABLE record ALTER COLUMN notes VARCHAR(50) NULL;

-- can not be null
-- ALTER TABLE record ALTER COLUMN notes VARCHAR(50) NOT NULL;


-- create table reader2 as same as reader
CREATE TABLE reader2(
	reader_id CHAR(8) NOT NULL,
	reader_name NVARCHAR(50) NOT NULL,
	reader_sex CHAR(2) NOT NULL,
	reader_department NVARCHAR(60) NOT NULL
);
-- insert data in batch
INSERT INTO reader2(reader_id,reader_name,reader_sex,reader_department)
SELECT r1.reader_id,r1.reader_name,r1.reader_sex,r1.reader_department
FROM reader r1
WHERE r1.reader_department='涉外教育系'


-- create table record_count to record the count of borrow times of each reader
CREATE TABLE record_count(
    reader_id CHAR(8) NOT NULL,
    borrow_count INT
);
-- insert data
INSERT INTO record_count(reader_id,borrow_count)
SELECT r2.reader_id,COUNT(*) AS borrow_count
FROM record r2
GROUP BY r2.reader_id;


-- set the interview_times of the table book to 0
UPDATE book
SET interview_times=0;

UPDATE book
SET interview_times = 10
WHERE book_publisher = '电子工业出版社';

-- set the notes of the table record to the book_name of the table book according to the book_id
SET r.notes = b.book_name
FROM record r
JOIN book b ON r.book_id = b.book_id;

-- set the notes of table record null
UPDATE record
SET notes = NULL;


-- delete data of table reader where reader_id = 'r0003'
DELETE FROM reader
WHERE reader_id = 'r0003';


-- delete data of table reader where reader_department = '行政管理系'
DELETE FROM reader
WHERE reader_department = '行政管理系';