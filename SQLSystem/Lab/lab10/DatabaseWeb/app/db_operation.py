import pymssql
from .models import Book, Reader, Record
from typing import List, Optional


class DatabaseOperations:
    def __init__(self):
        self.conn = None
        self.cursor = None
        self.connect()
    
    def connect(self):
        """建立数据库连接"""
        try:
            self.conn = pymssql.connect(
                server='192.168.3.76', 
                user='lhy', 
                password='123456..', 
                database='JY'
            )
            self.cursor = self.conn.cursor(as_dict=True)  # 使用字典格式返回结果
        except Exception as e:
            print(f"数据库连接失败: {e}")
            raise
    
    def close(self):
        """关闭数据库连接"""
        if self.cursor:
            self.cursor.close()
        if self.conn:
            self.conn.close()
    
    def fetch_all_readers(self) -> List[Reader]:
        """获取所有读者信息"""
        try:
            self.cursor.execute("SELECT reader_id, reader_name, reader_sex, reader_department FROM reader")
            rows = self.cursor.fetchall()
            readers = [Reader.from_dict(row) for row in rows]
            return readers
        except Exception as e:
            print(f"查询读者信息失败: {e}")
            return []
    
    def fetch_all_books(self) -> List[Book]:
        """获取所有图书信息"""
        try:
            self.cursor.execute("SELECT book_id, book_name, book_isbn, book_author, book_publisher, book_price, interviews_times FROM book")
            rows = self.cursor.fetchall()
            books = [Book.from_dict(row) for row in rows]
            return books
        except Exception as e:
            print(f"查询图书信息失败: {e}")
            return []
    
    def fetch_all_records(self) -> List[Record]:
        """获取所有借阅记录"""
        try:
            self.cursor.execute("SELECT reader_id, book_id, borrow_date, return_date, notes FROM record")
            rows = self.cursor.fetchall()
            records = [Record.from_dict(row) for row in rows]
            return records
        except Exception as e:
            print(f"查询借阅记录失败: {e}")
            return []
    
    # 读者操作
    def add_reader(self, reader_id: str, reader_name: str, reader_sex: str, reader_department: str) -> bool:
        """添加读者"""
        try:
            self.cursor.execute(
                "INSERT INTO reader (reader_id, reader_name, reader_sex, reader_department) VALUES (%s, %s, %s, %s)",
                (reader_id, reader_name, reader_sex, reader_department)
            )
            self.conn.commit()
            return True
        except Exception as e:
            print(f"添加读者失败: {e}")
            self.conn.rollback()
            return False
    
    def update_reader(self, reader_id: str, reader_name: str, reader_sex: str, reader_department: str) -> bool:
        """更新读者信息"""
        try:
            self.cursor.execute(
                "UPDATE reader SET reader_name=%s, reader_sex=%s, reader_department=%s WHERE reader_id=%s",
                (reader_name, reader_sex, reader_department, reader_id)
            )
            self.conn.commit()
            return True
        except Exception as e:
            print(f"更新读者失败: {e}")
            self.conn.rollback()
            return False
    
    def delete_reader(self, reader_id: str) -> bool:
        """删除读者"""
        try:
            self.cursor.execute("DELETE FROM reader WHERE reader_id=%s", (reader_id,))
            self.conn.commit()
            return True
        except Exception as e:
            print(f"删除读者失败: {e}")
            self.conn.rollback()
            return False
    
    def get_reader(self, reader_id: str) -> Optional[Reader]:
        """获取单个读者信息"""
        try:
            self.cursor.execute(
                "SELECT reader_id, reader_name, reader_sex, reader_department FROM reader WHERE reader_id=%s",
                (reader_id,)
            )
            row = self.cursor.fetchone()
            return Reader.from_dict(row) if row else None
        except Exception as e:
            print(f"查询读者失败: {e}")
            return None
    
    # 图书操作
    def add_book(self, book_id: str, book_name: str, book_isbn: str, book_author: str, 
                 book_publisher: str, book_price: float, interviews_times: int) -> bool:
        """添加图书"""
        try:
            self.cursor.execute(
                "INSERT INTO book (book_id, book_name, book_isbn, book_author, book_publisher, book_price, interviews_times) VALUES (%s, %s, %s, %s, %s, %s, %s)",
                (book_id, book_name, book_isbn, book_author, book_publisher, book_price, interviews_times)
            )
            self.conn.commit()
            return True
        except Exception as e:
            print(f"添加图书失败: {e}")
            self.conn.rollback()
            return False
    
    def update_book(self, book_id: str, book_name: str, book_isbn: str, book_author: str,
                   book_publisher: str, book_price: float, interviews_times: int) -> bool:
        """更新图书信息"""
        try:
            self.cursor.execute(
                "UPDATE book SET book_name=%s, book_isbn=%s, book_author=%s, book_publisher=%s, book_price=%s, interviews_times=%s WHERE book_id=%s",
                (book_name, book_isbn, book_author, book_publisher, book_price, interviews_times, book_id)
            )
            self.conn.commit()
            return True
        except Exception as e:
            print(f"更新图书失败: {e}")
            self.conn.rollback()
            return False
    
    def delete_book(self, book_id: str) -> bool:
        """删除图书"""
        try:
            self.cursor.execute("DELETE FROM book WHERE book_id=%s", (book_id,))
            self.conn.commit()
            return True
        except Exception as e:
            print(f"删除图书失败: {e}")
            self.conn.rollback()
            return False
    
    def get_book(self, book_id: str) -> Optional[Book]:
        """获取单个图书信息"""
        try:
            self.cursor.execute(
                "SELECT book_id, book_name, book_isbn, book_author, book_publisher, book_price, interviews_times FROM book WHERE book_id=%s",
                (book_id,)
            )
            row = self.cursor.fetchone()
            return Book.from_dict(row) if row else None
        except Exception as e:
            print(f"查询图书失败: {e}")
            return None
    
    # 借阅记录操作
    def add_record(self, reader_id: str, book_id: str, borrow_date: str, return_date: str = None, notes: str = None) -> bool:
        """添加借阅记录"""
        try:
            self.cursor.execute(
                "INSERT INTO record (reader_id, book_id, borrow_date, return_date, notes) VALUES (%s, %s, %s, %s, %s)",
                (reader_id, book_id, borrow_date, return_date, notes)
            )
            self.conn.commit()
            return True
        except Exception as e:
            print(f"添加借阅记录失败: {e}")
            self.conn.rollback()
            return False
    
    def update_record(self, reader_id: str, book_id: str, borrow_date: str, return_date: str = None, notes: str = None) -> bool:
        """更新借阅记录"""
        try:
            self.cursor.execute(
                "UPDATE record SET return_date=%s, notes=%s WHERE reader_id=%s AND book_id=%s AND borrow_date=%s",
                (return_date, notes, reader_id, book_id, borrow_date)
            )
            self.conn.commit()
            return True
        except Exception as e:
            print(f"更新借阅记录失败: {e}")
            self.conn.rollback()
            return False
    
    def delete_record(self, reader_id: str, book_id: str, borrow_date: str) -> bool:
        """删除借阅记录"""
        try:
            self.cursor.execute(
                "DELETE FROM record WHERE reader_id=%s AND book_id=%s AND borrow_date=%s",
                (reader_id, book_id, borrow_date)
            )
            self.conn.commit()
            return True
        except Exception as e:
            print(f"删除借阅记录失败: {e}")
            self.conn.rollback()
            return False
    
    # 统计数据
    def get_statistics(self) -> dict:
        """获取统计数据"""
        try:
            stats = {}
            # 读者总数
            self.cursor.execute("SELECT COUNT(*) as count FROM reader")
            stats['reader_count'] = self.cursor.fetchone()['count']
            
            # 图书总数
            self.cursor.execute("SELECT COUNT(*) as count FROM book")
            stats['book_count'] = self.cursor.fetchone()['count']
            
            # 借阅记录总数
            self.cursor.execute("SELECT COUNT(*) as count FROM record")
            stats['record_count'] = self.cursor.fetchone()['count']
            
            # 未归还的借阅记录数
            self.cursor.execute("SELECT COUNT(*) as count FROM record WHERE return_date IS NULL")
            stats['unreturned_count'] = self.cursor.fetchone()['count']
            
            return stats
        except Exception as e:
            print(f"获取统计数据失败: {e}")
            return {
                'reader_count': 0,
                'book_count': 0,
                'record_count': 0,
                'unreturned_count': 0
            }
    
    def __enter__(self):
        """上下文管理器进入"""
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        """上下文管理器退出，自动关闭连接"""
        self.close()