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
    
    # 车队管理
    path('fleet/list/', views.fleet_list, name='fleet_list'),

    # 车辆管理
    path('truck/list/', views.truck_list, name='truck_list'),

    # 司机管理
    path('driver/list/', views.driver_list, name='driver_list'),

    # 运单管理
    path('order/list/', views.order_list, name='order_list'),

    # 配送中心
    path('center/list/', views.center_list, name='center_list'),

    # 主管管理
    path('supervisor/list/', views.supervisor_list, name='supervisor_list'),

    # 异常记录
    path('exception/list/', views.exception_list, name='exception_list'),

    # 审计日志
    path('log/list/', views.log_list, name='log_list'),

    # 新增功能路由
    path('truck/add/', views.truck_add, name='truck_add'),
    path('driver/add/', views.driver_add, name='driver_add'),
    path('order/assign/', views.order_assign, name='order_assign'),
    path('exception/add/', views.exception_add, name='exception_add'),
    path('center/query/', views.center_query, name='center_query'),
    path('driver/performance/', views.driver_performance, name='driver_performance'),
    path('fleet/report/', views.fleet_report, name='fleet_report'),
    
    # 运单管理功能路由
    path('order/complete/<str:order_id>/', views.order_complete, name='order_complete'),
    path('order/cancel/<str:order_id>/', views.order_cancel, name='order_cancel'),
    path('order/start/<str:order_id>/', views.order_start_transport, name='order_start_transport'),
    path('order/update/<str:order_id>/', views.order_update_status, name='order_update_status'),
    path('order/assign-truck/<str:order_id>/', views.order_assign_truck, name='order_assign_truck'),
    
    # 司机管理功能路由
    path('driver/edit/<str:driver_id>/', views.driver_edit, name='driver_edit'),
    
    # 存储过程和视图功能路由
    path('fleet/monthly-performance/', views.fleet_monthly_performance, name='fleet_monthly_performance'),
    path('exception/weekly-alerts/', views.weekly_exception_alerts, name='weekly_exception_alerts'),
    
    # 创新模块一：智能推荐
    path('innovation/recommendation/', views.innovation_recommendation, name='innovation_recommendation'),
    # 创新模块一：成本规则管理 (CRUD API)
    path('innovation/cost-rules/', views.innovation_cost_rules, name='innovation_cost_rules'),
    path('innovation/cost-rules/add/', views.innovation_cost_rule_add, name='innovation_cost_rule_add'),
    path('innovation/cost-rules/delete/<int:rule_id>/', views.innovation_cost_rule_delete, name='innovation_cost_rule_delete'),
    
    # 创新模块二：运力预警
    path('innovation/capacity/', views.innovation_capacity, name='innovation_capacity'),
    
    # 创新模块三：实时监控
    path('innovation/monitor/', views.innovation_monitor, name='innovation_monitor'),
]
