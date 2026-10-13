-- 0. modify the mistake in homework 2
INSERT INTO reader(reader_id,reader_name,reader_sex,reader_department) VALUES
('r0008', '蔡明伟', '男', '行政管理系');

-- the right answer
DELETE record
FROM record
JOIN reader ON record.reader_id = reader.reader_id
WHERE reader.reader_department = '行政管理系';


-- 1. query all the information of the table book
SELECT * FROM book;


-- 2. query the names and departments of the table reader
SELECT reader_name, reader_department FROM reader;

-- 3. the same as 2, but the result should show the comment "姓名" and "院系"
SELECT reader_name AS '姓名', reader_department AS '院系' FROM reader;

-- 4. query all the information of the table book, but only limit 5 records
-- For SQL Server, use TOP. For MySQL/PostgreSQL, use LIMIT.
SELECT TOP 10 * FROM book;

-- 5. query the "出版社名称" of the table book without duplicate records
SELECT DISTINCT book_publisher FROM book;

-- 6. query the count of the records of the table book, and the result should show the comment "总借阅次数"
SELECT COUNT(*) AS "总借阅次数" FROM record;

-- 7. count the total number of reader in table reader
SELECT COUNT(*) AS "总读者人数" FROM reader;

-- 8. query the reader whose last name is '王'
SELECT * FROM reader WHERE reader_name LIKE '王%';

-- 9. query all the information of table book ,and order by borrow times descendingly
SELECT bk.*,COUNT(rc.book_id) AS "record_times"
FROM book bk
INNER JOIN record rc ON bk.book_id = rc.book_id 
GROUP BY bk.book_id, bk.book_name, bk.book_author, bk.book_publisher, bk.book_price, bk.book_isbn, bk.interview_times
ORDER BY COUNT(rc.book_id) DESC;

-- 10. count the number of books for each publisher
SELECT book_publisher,COUNT(*) AS "book_count" 
FROM book
GROUP BY book_publisher
ORDER BY COUNT(*) DESC;

-- 11. select the publishers who have published more than 1 book and the total borrow count of their books is more than 20
SELECT
    b.book_publisher,
    COUNT(DISTINCT b.book_id) AS "total_books_published",
    COUNT(DISTINCT r.book_id) AS "total_borrow_count"
FROM
    book b
JOIN
    record r ON b.book_id = r.book_id
GROUP BY
    b.book_publisher
HAVING
    COUNT(DISTINCT b.book_id) > 1 AND COUNT(DISTINCT r.book_id) > 20;

-- 12. query the reader who does not borrow book "b005"
SELECT *
FROM reader
WHERE reader_id NOT IN (
    SELECT reader_id
    FROM record
    WHERE book_id = 'b0005'
);

-- 13. query reader name who borrow more than 2 books
SELECT re.reader_name AS "读者姓名"
FROM reader re
JOIN record rc ON re.reader_id = rc.reader_id
GROUP BY re.reader_id, re.reader_name
HAVING COUNT(rc.book_id) >= 2;

-- 14. query book_id that boorow most and fewest times
-- WITH can be used to create a temporary result set that can be referenced within a SELECT, INSERT, UPDATE, or DELETE statement.
WITH BookBorrowCounts AS (
    SELECT
        b.book_id,
        COUNT(r.book_id) as borrow_count
    FROM
        book b
    LEFT JOIN record r ON b.book_id = r.book_id
    GROUP BY
        b.book_id
)
SELECT
    book_id,
    borrow_count
FROM
    BookBorrowCounts
WHERE
    borrow_count IN (
        SELECT MAX(borrow_count) FROM BookBorrowCounts
        UNION
        SELECT MIN(borrow_count) FROM BookBorrowCounts
    )
ORDER BY
    borrow_count DESC;

-- 15. query the reader_id reader_name who borrowed the book_id 'b0002'
SELECT reader_id, reader_name
FROM reader 
WHERE reader_id IN(
    SELECT reader_id
    FROM record
    WHERE book_id = 'b0002'
)

-- 16. query the reader's reader_name,reader_department,borrow_book_name whose reader_id = 'r0007'
SELECT re.reader_name, re.reader_department, bk.book_name AS borrow_book_name
FROM reader re
JOIN record rc ON re.reader_id = rc.reader_id 
JOIN book bk ON rc.book_id = bk.book_id
WHERE re.reader_id = 'r0007';


-- 17.query the reader_name,reader_sex,reader_department who borrow most and fewest books
-- attention: 0
WITH ReaderBorrowCounts AS (
    SELECT
        re.reader_id,
        re.reader_name,
        re.reader_sex,
        re.reader_department,
        COUNT(rc.record_id) AS borrow_count
    FROM
        reader re
    LEFT JOIN record rc ON re.reader_id = rc.reader_id
    GROUP BY
        re.reader_id, re.reader_name, re.reader_sex, re.reader_department
)
SELECT
    reader_name AS '姓名',
    reader_sex AS '性别',
    reader_department AS '院系',
    borrow_count AS '借阅次数'
FROM
    ReaderBorrowCounts
WHERE
    borrow_count = ANY (
        SELECT MAX(borrow_count) FROM ReaderBorrowCounts
        UNION
        SELECT MIN(borrow_count) FROM ReaderBorrowCounts
    )
ORDER BY
    borrow_count DESC;