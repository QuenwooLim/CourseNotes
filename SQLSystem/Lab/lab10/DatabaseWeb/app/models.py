from datetime import date
from decimal import Decimal
from typing import Optional, Dict, Any


class Book:
    """图书模型类"""
    def __init__(self, book_id: str = None, book_name: str = None, book_isbn: str = None,
                 book_author: str = None, book_publisher: str = None, book_price: Decimal = None,
                 interviews_times: int = 0):
        self.book_id = book_id
        self.book_name = book_name
        self.book_isbn = book_isbn
        self.book_author = book_author
        self.book_publisher = book_publisher
        self.book_price = book_price
        self.interviews_times = interviews_times

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Book':
        """从字典创建Book实例"""
        return cls(
            book_id=data.get('book_id'),
            book_name=data.get('book_name'),
            book_isbn=data.get('book_isbn'),
            book_author=data.get('book_author'),
            book_publisher=data.get('book_publisher'),
            book_price=Decimal(str(data.get('book_price', 0))),
            interviews_times=data.get('interviews_times', 0)
        )

    def to_dict(self) -> Dict[str, Any]:
        """转换为字典"""
        return {
            'book_id': self.book_id,
            'book_name': self.book_name,
            'book_isbn': self.book_isbn,
            'book_author': self.book_author,
            'book_publisher': self.book_publisher,
            'book_price': float(self.book_price) if self.book_price else None,
            'interviews_times': self.interviews_times
        }

    def __str__(self):
        return f"{self.book_id} - {self.book_name}"

    def __repr__(self):
        return f"Book(book_id='{self.book_id}', book_name='{self.book_name}')"


class Reader:
    """读者模型类"""
    
    def __init__(self, reader_id: str = None, reader_name: str = None,
                 reader_sex: str = None, reader_department: str = None):
        self.reader_id = reader_id
        self.reader_name = reader_name
        self.reader_sex = reader_sex
        self.reader_department = reader_department

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Reader':
        """从字典创建Reader实例"""
        return cls(
            reader_id=data.get('reader_id'),
            reader_name=data.get('reader_name'),
            reader_sex=data.get('reader_sex'),
            reader_department=data.get('reader_department')
        )

    def to_dict(self) -> Dict[str, Any]:
        """转换为字典"""
        return {
            'reader_id': self.reader_id,
            'reader_name': self.reader_name,
            'reader_sex': self.reader_sex,
            'reader_department': self.reader_department
        }

    def __str__(self):
        return f"{self.reader_id} - {self.reader_name}"

    def __repr__(self):
        return f"Reader(reader_id='{self.reader_id}', reader_name='{self.reader_name}')"


class Record:
    """借阅记录模型类"""
    
    def __init__(self, reader_id: str = None, book_id: str = None,
                 borrow_date: date = None, return_date: date = None, notes: str = None):
        self.reader_id = reader_id
        self.book_id = book_id
        self.borrow_date = borrow_date
        self.return_date = return_date
        self.notes = notes

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'Record':
        """从字典创建Record实例"""
        borrow_date = data.get('borrow_date')
        return_date = data.get('return_date')
        
        # 处理日期字段
        if isinstance(borrow_date, str):
            borrow_date = date.fromisoformat(borrow_date)
        if isinstance(return_date, str):
            return_date = date.fromisoformat(return_date)
            
        return cls(
            reader_id=data.get('reader_id'),
            book_id=data.get('book_id'),
            borrow_date=borrow_date,
            return_date=return_date,
            notes=data.get('notes')
        )

    def to_dict(self) -> Dict[str, Any]:
        """转换为字典"""
        return {
            'reader_id': self.reader_id,
            'book_id': self.book_id,
            'borrow_date': self.borrow_date.isoformat() if self.borrow_date else None,
            'return_date': self.return_date.isoformat() if self.return_date else None,
            'notes': self.notes
        }

    def __str__(self):
        return f"{self.reader_id} borrowed {self.book_id} on {self.borrow_date}"

    def __repr__(self):
        return f"Record(reader_id='{self.reader_id}', book_id='{self.book_id}', borrow_date='{self.borrow_date}')"
