"""
URL configuration for DatabaseWeb project.

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/5.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path

from app import views

urlpatterns = [
    # path('admin/', admin.site.urls),
    
    # 主页
    path('', views.index, name='index'),
    
    # 读者管理
    path('reader/list/', views.reader_list, name='reader_list'),
    path('reader/add/', views.reader_add, name='reader_add'),
    path('reader/edit/<str:reader_id>/', views.reader_edit, name='reader_edit'),
    path('reader/delete/<str:reader_id>/', views.reader_delete, name='reader_delete'),
    
    # 图书管理
    path('book/list/', views.book_list, name='book_list'),
    path('book/add/', views.book_add, name='book_add'),
    path('book/edit/<str:book_id>/', views.book_edit, name='book_edit'),
    path('book/delete/<str:book_id>/', views.book_delete, name='book_delete'),
    
    # 借阅记录管理
    path('record/list/', views.record_list, name='record_list'),
    path('record/add/', views.record_add, name='record_add'),
    path('record/edit/', views.record_edit, name='record_edit'),
    path('record/delete/', views.record_delete, name='record_delete'),
]
