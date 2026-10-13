from django.shortcuts import render, HttpResponse, redirect
from django.contrib import messages
from .db_operation import DatabaseOperations

# Create your views here.

# ========== 主页 ==========
def index(request):
    """主页面 - 展示统计数据和三个表格的入口"""
    try:
        with DatabaseOperations() as db:
            stats = db.get_statistics()
        
        context = {
            'stats': stats
        }
        return render(request, 'index.html', context)
    
    except Exception as e:
        error_message = f"获取统计数据时发生错误: {str(e)}"
        context = {
            'error_message': error_message,
            'stats': {
                'reader_count': 0,
                'book_count': 0,
                'record_count': 0,
                'unreturned_count': 0
            }
        }
        return render(request, 'index.html', context)


# ========== 读者管理 ==========
def reader_list(request):
    """展示所有读者信息"""
    try:
        with DatabaseOperations() as db:
            readers = db.fetch_all_readers()
        
        context = {
            'readers': readers,
            'total_count': len(readers)
        }
        return render(request, 'reader_list.html', context)
    
    except Exception as e:
        error_message = f"获取读者信息时发生错误: {str(e)}"
        context = {
            'error_message': error_message,
            'readers': [],
            'total_count': 0
        }
        return render(request, 'reader_list.html', context)


def reader_add(request):
    """添加读者"""
    if request.method == 'POST':
        reader_id = request.POST.get('reader_id')
        reader_name = request.POST.get('reader_name')
        reader_sex = request.POST.get('reader_sex')
        reader_department = request.POST.get('reader_department')
        
        try:
            with DatabaseOperations() as db:
                success = db.add_reader(reader_id, reader_name, reader_sex, reader_department)
            
            if success:
                messages.success(request, '读者添加成功！')
                return redirect('/reader/list/')
            else:
                messages.error(request, '读者添加失败，请检查输入！')
        except Exception as e:
            messages.error(request, f'添加失败: {str(e)}')
    
    return render(request, 'reader_form.html', {'action': 'add'})


def reader_edit(request, reader_id):
    """编辑读者"""
    try:
        with DatabaseOperations() as db:
            if request.method == 'POST':
                reader_name = request.POST.get('reader_name')
                reader_sex = request.POST.get('reader_sex')
                reader_department = request.POST.get('reader_department')
                
                success = db.update_reader(reader_id, reader_name, reader_sex, reader_department)
                
                if success:
                    messages.success(request, '读者信息更新成功！')
                    return redirect('/reader/list/')
                else:
                    messages.error(request, '更新失败，请检查输入！')
            
            reader = db.get_reader(reader_id)
            if not reader:
                messages.error(request, '读者不存在！')
                return redirect('/reader/list/')
            
            context = {
                'action': 'edit',
                'reader': reader
            }
            return render(request, 'reader_form.html', context)
    
    except Exception as e:
        messages.error(request, f'操作失败: {str(e)}')
        return redirect('/reader/list/')


def reader_delete(request, reader_id):
    """删除读者"""
    try:
        with DatabaseOperations() as db:
            success = db.delete_reader(reader_id)
        
        if success:
            messages.success(request, '读者删除成功！')
        else:
            messages.error(request, '删除失败！')
    except Exception as e:
        messages.error(request, f'删除失败: {str(e)}')
    
    return redirect('/reader/list/')


# ========== 图书管理 ==========
def book_list(request):
    """展示所有图书信息"""
    try:
        with DatabaseOperations() as db:
            books = db.fetch_all_books()
        
        context = {
            'books': books,
            'total_count': len(books)
        }
        return render(request, 'book_list.html', context)
    
    except Exception as e:
        error_message = f"获取图书信息时发生错误: {str(e)}"
        context = {
            'error_message': error_message,
            'books': [],
            'total_count': 0
        }
        return render(request, 'book_list.html', context)


def book_add(request):
    """添加图书"""
    if request.method == 'POST':
        book_id = request.POST.get('book_id')
        book_name = request.POST.get('book_name')
        book_isbn = request.POST.get('book_isbn')
        book_author = request.POST.get('book_author')
        book_publisher = request.POST.get('book_publisher')
        book_price = float(request.POST.get('book_price', 0))
        interviews_times = int(request.POST.get('interviews_times', 0))
        
        try:
            with DatabaseOperations() as db:
                success = db.add_book(book_id, book_name, book_isbn, book_author, 
                                     book_publisher, book_price, interviews_times)
            
            if success:
                messages.success(request, '图书添加成功！')
                return redirect('/book/list/')
            else:
                messages.error(request, '图书添加失败，请检查输入！')
        except Exception as e:
            messages.error(request, f'添加失败: {str(e)}')
    
    return render(request, 'book_form.html', {'action': 'add'})


def book_edit(request, book_id):
    """编辑图书"""
    try:
        with DatabaseOperations() as db:
            if request.method == 'POST':
                book_name = request.POST.get('book_name')
                book_isbn = request.POST.get('book_isbn')
                book_author = request.POST.get('book_author')
                book_publisher = request.POST.get('book_publisher')
                book_price = float(request.POST.get('book_price', 0))
                interviews_times = int(request.POST.get('interviews_times', 0))
                
                success = db.update_book(book_id, book_name, book_isbn, book_author,
                                        book_publisher, book_price, interviews_times)
                
                if success:
                    messages.success(request, '图书信息更新成功！')
                    return redirect('/book/list/')
                else:
                    messages.error(request, '更新失败，请检查输入！')
            
            book = db.get_book(book_id)
            if not book:
                messages.error(request, '图书不存在！')
                return redirect('/book/list/')
            
            context = {
                'action': 'edit',
                'book': book
            }
            return render(request, 'book_form.html', context)
    
    except Exception as e:
        messages.error(request, f'操作失败: {str(e)}')
        return redirect('/book/list/')


def book_delete(request, book_id):
    """删除图书"""
    try:
        with DatabaseOperations() as db:
            success = db.delete_book(book_id)
        
        if success:
            messages.success(request, '图书删除成功！')
        else:
            messages.error(request, '删除失败！')
    except Exception as e:
        messages.error(request, f'删除失败: {str(e)}')
    
    return redirect('/book/list/')


# ========== 借阅记录管理 ==========
def record_list(request):
    """展示所有借阅记录"""
    try:
        with DatabaseOperations() as db:
            records = db.fetch_all_records()
        
        context = {
            'records': records,
            'total_count': len(records)
        }
        return render(request, 'record_list.html', context)
    
    except Exception as e:
        error_message = f"获取借阅记录时发生错误: {str(e)}"
        context = {
            'error_message': error_message,
            'records': [],
            'total_count': 0
        }
        return render(request, 'record_list.html', context)


def record_add(request):
    """添加借阅记录"""
    if request.method == 'POST':
        reader_id = request.POST.get('reader_id')
        book_id = request.POST.get('book_id')
        borrow_date = request.POST.get('borrow_date')
        return_date = request.POST.get('return_date') or None
        notes = request.POST.get('notes') or None
        
        try:
            with DatabaseOperations() as db:
                success = db.add_record(reader_id, book_id, borrow_date, return_date, notes)
            
            if success:
                messages.success(request, '借阅记录添加成功！')
                return redirect('/record/list/')
            else:
                messages.error(request, '借阅记录添加失败，请检查输入！')
        except Exception as e:
            messages.error(request, f'添加失败: {str(e)}')
    
    return render(request, 'record_form.html', {'action': 'add'})


def record_edit(request):
    """编辑借阅记录"""
    reader_id = request.GET.get('reader_id') or request.POST.get('reader_id')
    book_id = request.GET.get('book_id') or request.POST.get('book_id')
    borrow_date = request.GET.get('borrow_date') or request.POST.get('borrow_date')
    
    try:
        with DatabaseOperations() as db:
            if request.method == 'POST':
                return_date = request.POST.get('return_date') or None
                notes = request.POST.get('notes') or None
                
                success = db.update_record(reader_id, book_id, borrow_date, return_date, notes)
                
                if success:
                    messages.success(request, '借阅记录更新成功！')
                    return redirect('/record/list/')
                else:
                    messages.error(request, '更新失败，请检查输入！')
            
            # 查找记录
            records = db.fetch_all_records()
            record = None
            for r in records:
                if (r.reader_id == reader_id and r.book_id == book_id and 
                    str(r.borrow_date) == borrow_date):
                    record = r
                    break
            
            if not record:
                messages.error(request, '借阅记录不存在！')
                return redirect('/record/list/')
            
            context = {
                'action': 'edit',
                'record': record
            }
            return render(request, 'record_form.html', context)
    
    except Exception as e:
        messages.error(request, f'操作失败: {str(e)}')
        return redirect('/record/list/')


def record_delete(request):
    """删除借阅记录"""
    reader_id = request.GET.get('reader_id')
    book_id = request.GET.get('book_id')
    borrow_date = request.GET.get('borrow_date')
    
    try:
        with DatabaseOperations() as db:
            success = db.delete_record(reader_id, book_id, borrow_date)
        
        if success:
            messages.success(request, '借阅记录删除成功！')
        else:
            messages.error(request, '删除失败！')
    except Exception as e:
        messages.error(request, f'删除失败: {str(e)}')
    
    return redirect('/record/list/')