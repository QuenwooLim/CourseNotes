import pymssql
"""
SQL Server连接配置信息
主机: 192.168.3.76
端口: 1433
用户: lhy
密码: 123456..
数据库: JY
"""
import pymssql
# 连接到数据库
conn = pymssql.connect(server='192.168.3.76', user='lhy', password='123456..', database='JY')
cursor = conn.cursor()

# Q1:插入新读者数据
insert_query = "INSERT INTO reader (reader_id, reader_name, reader_sex, reader_department) VALUES (%s, %s, %s, %s)"
new_reader = ('r0020', '王小明', '男', '临床医学系')
cursor.execute(insert_query, new_reader)
conn.commit()

# Q2: 更新读者部门信息
update_query = "UPDATE reader SET reader_department = %s WHERE reader_id = %s"
updated_department = ('护理系', 'r0020')
cursor.execute(update_query, updated_department)
conn.commit()

# Q3: 查询价格大于50的书籍
select_query = "SELECT * FROM book WHERE book_price > %s"
cursor.execute(select_query, (50,))
# 获取并打印结果
results = cursor.fetchall()
for row in results:
    print(row)

# Q4: 查询信息工程系的读者
select_query = "SELECT * FROM reader WHERE reader_department = %s"
cursor.execute(select_query, ('信息工程系',))
# 获取并打印结果
results = cursor.fetchall()
for row in results:
    print(row)

# Q5: 删除指定读者数据
delete_query = "DELETE FROM reader WHERE reader_id = %s"
cursor.execute(delete_query, ('r0020',))
conn.commit()

# Q6: 显示所有图书和读者数据
# 查询所有图书数据
select_books_query = "SELECT * FROM book"
cursor.execute(select_books_query)
books = cursor.fetchall()
print("Book Table: ")
for book in books:
    print(book)
# 查询所有读者数据
select_readers_query = "SELECT * FROM reader"
cursor.execute(select_readers_query)
readers = cursor.fetchall()
print("\nReader Table: ")
for reader in readers:
    print(reader)

cursor.close()
conn.close()