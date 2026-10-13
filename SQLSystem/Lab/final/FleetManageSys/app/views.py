from django.shortcuts import render, HttpResponse, redirect
from django.contrib import messages
from .db_operation import DatabaseOperations

# Create your views here.

# ========== 主页 ==========
def index(request):
    """主页面 - 展示统计数据和表格的入口"""
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

def fleet_list(request):
    """车队列表页面"""
    try:
        with DatabaseOperations() as db:
            fleets = db.fetch_all_fleets()
    except Exception as e:
        fleets = []
        messages.error(request, f"获取车队列表失败: {e}")
    
    return render(request, 'fleet_list.html', {'fleets': fleets})

def truck_list(request):
    """车辆列表页面"""
    try:
        with DatabaseOperations() as db:
            trucks = db.fetch_all_trucks()
    except Exception as e:
        trucks = []
        messages.error(request, f"获取车辆列表失败: {e}")
    
    return render(request, 'truck_list.html', {'trucks': trucks})

def driver_list(request):
    """司机列表页面"""
    try:
        with DatabaseOperations() as db:
            drivers = db.fetch_all_drivers()
    except Exception as e:
        drivers = []
        messages.error(request, f"获取司机列表失败: {e}")
    
    return render(request, 'driver_list.html', {'drivers': drivers})

def order_list(request):
    """运单列表页面"""
    try:
        with DatabaseOperations() as db:
            orders = db.fetch_all_orders()
    except Exception as e:
        orders = []
        messages.error(request, f"获取运单列表失败: {e}")
    
    return render(request, 'order_list.html', {'orders': orders})

def center_list(request):
    """配送中心列表页面"""
    try:
        with DatabaseOperations() as db:
            centers = db.fetch_all_distribution_centers()
    except Exception as e:
        centers = []
        messages.error(request, f"获取配送中心列表失败: {e}")
    
    return render(request, 'center_list.html', {'centers': centers})

def supervisor_list(request):
    """主管列表页面"""
    try:
        with DatabaseOperations() as db:
            supervisors = db.fetch_all_supervisors()
    except Exception as e:
        supervisors = []
        messages.error(request, f"获取主管列表失败: {e}")
    
    return render(request, 'supervisor_list.html', {'supervisors': supervisors})

def exception_list(request):
    """异常记录列表页面"""
    try:
        with DatabaseOperations() as db:
            exceptions = db.fetch_all_exception_records()
    except Exception as e:
        exceptions = []
        messages.error(request, f"获取异常记录列表失败: {e}")
    
    return render(request, 'exception_list.html', {'exceptions': exceptions})

def log_list(request):
    """审计日志列表页面"""
    try:
        with DatabaseOperations() as db:
            logs = db.fetch_all_logs()
    except Exception as e:
        logs = []
        messages.error(request, f"获取审计日志列表失败: {e}")
    
    return render(request, 'log_list.html', {'logs': logs})

# ========== 新增功能视图 ==========

def truck_add(request):
    """1. 录入车辆"""
    if request.method == 'POST':
        try:
            plate = request.POST.get('plate_number')
            load = request.POST.get('max_load')
            volume = request.POST.get('max_volume')
            status = request.POST.get('current_status')
            fleet_id = request.POST.get('fleet_id')
            
            with DatabaseOperations() as db:
                db.add_truck(plate, load, volume, status, fleet_id)
            messages.success(request, "车辆录入成功！")
            return redirect('truck_list')
        except Exception as e:
            messages.error(request, f"录入失败: {e}")
    
    return render(request, 'truck_form.html')

def driver_add(request):
    """1. 录入司机"""
    if request.method == 'POST':
        try:
            driver_id = request.POST.get('driver_id')
            name = request.POST.get('name')
            level = request.POST.get('license_level')
            phone = request.POST.get('phone')
            date = request.POST.get('hire_date')
            fleet_id = request.POST.get('fleet_id')
            
            with DatabaseOperations() as db:
                db.add_driver(driver_id, name, level, phone, date, fleet_id)
            messages.success(request, "司机录入成功！")
            return redirect('driver_list')
        except Exception as e:
            messages.error(request, f"录入失败: {e}")
            
    return render(request, 'driver_form.html')

def order_assign(request):
    """2. 创建运单（不分配车辆）"""
    if request.method == 'POST':
        try:
            weight = request.POST.get('weight')
            volume = request.POST.get('volume')
            dest = request.POST.get('destination')
            
            with DatabaseOperations() as db:
                order_id = db.assign_order(weight, volume, dest)
            messages.success(request, f"运单创建成功！运单号: {order_id}，请在运单列表中分配车辆。")
            return redirect('order_list')
        except Exception as e:
            messages.error(request, f"创建失败: {e}")
    
    return render(request, 'order_form.html')

def exception_add(request):
    """3. 异常记录录入"""
    if request.method == 'POST':
        try:
            etype = request.POST.get('exception_type')
            time = request.POST.get('occur_time')
            fine = request.POST.get('fine_amount')
            status = request.POST.get('handle_status')
            plate = request.POST.get('truck_plate')
            did = request.POST.get('driver_id')
            
            # 处理时间格式：将 datetime-local 格式转换为 SQL Server 识别的格式
            if time:
                # datetime-local 返回的格式是: 2026-01-15T14:30
                # 需要转换为: 2026-01-15 14:30:00
                time = time.replace('T', ' ') + ':00'
            
            with DatabaseOperations() as db:
                db.add_exception_record(etype, time, fine, status, plate, did)
            messages.success(request, "异常记录添加成功！")
            return redirect('exception_list')
        except Exception as e:
            messages.error(request, f"添加失败: {e}")
    
    # GET 请求 - 获取车辆和司机列表
    try:
        with DatabaseOperations() as db:
            trucks = db.fetch_all_trucks()
            drivers = db.fetch_all_drivers()
        return render(request, 'exception_form.html', {
            'trucks': trucks,
            'drivers': drivers
        })
    except Exception as e:
        messages.error(request, f"加载失败: {e}")
        return render(request, 'exception_form.html', {
            'trucks': [],
            'drivers': []
        })

def center_query(request):
    """4. 车队资源查询"""
    resources = []
    centers = []
    center_id = request.GET.get('center_id')
    
    try:
        with DatabaseOperations() as db:
            centers = db.fetch_all_distribution_centers()
            if center_id:
                resources = db.query_center_resources(center_id)
    except Exception as e:
        messages.error(request, f"查询失败: {e}")
            
    # 如果是int类型比较好，这里把center_id转为int以便在模板中选中
    try:
        if center_id: center_id = int(center_id)
    except:
        pass
        
    return render(request, 'center_query.html', {'resources': resources, 'center_id': center_id, 'centers': centers})

def driver_performance(request):
    """5. 司机绩效追踪"""
    stats = None
    drivers = []
    did = request.GET.get('driver_id')
    start = request.GET.get('start_date')
    end = request.GET.get('end_date')

    try:
        with DatabaseOperations() as db:
            drivers = db.fetch_all_drivers()
            if did:
                # 处理空日期串为None
                if not start: start = None
                if not end: end = None
                stats = db.query_driver_performance(did, start, end)
    except Exception as e:
        messages.error(request, f"查询失败: {e}")
            
    return render(request, 'driver_performance.html', {
        'stats': stats, 
        'drivers': drivers, 
        'driver_id': did,
        'start_date': start,
        'end_date': end
    })

def fleet_report(request):
    """6. 统计报表"""
    report = None
    fleets = []
    fid = request.GET.get('fleet_id')
    month_str = request.GET.get('month') # 格式 2025-01
    
    try:
        with DatabaseOperations() as db:
            fleets = db.fetch_all_fleets()
            if fid:
                year = None
                month = None
                if month_str:
                    try:
                        year, month = map(int, month_str.split('-'))
                    except:
                        pass # 格式不正确则忽略日期，查询总表
                
                report = db.query_fleet_report(fid, year, month)
    except Exception as e:
        messages.error(request, f"生成报表失败: {e}")
            
    # 转 fid 为 int 用于模板选中
    try:
        if fid: fid = int(fid)
    except:
        pass

    return render(request, 'fleet_report.html', {
        'report': report, 
        'fleets': fleets, 
        'fleet_id': fid,
        'month': month_str
    })

# ========== 运单管理功能 ==========

def order_complete(request, order_id):
    """完成订单"""
    try:
        with DatabaseOperations() as db:
            db.complete_order(order_id)
        messages.success(request, f"订单 {order_id} 已完成！")
    except Exception as e:
        messages.error(request, f"完成订单失败: {e}")
    return redirect('order_list')

def order_cancel(request, order_id):
    """取消订单"""
    try:
        with DatabaseOperations() as db:
            db.cancel_order(order_id)
        messages.success(request, f"订单 {order_id} 已取消！")
    except Exception as e:
        messages.error(request, f"取消订单失败: {e}")
    return redirect('order_list')

def order_start_transport(request, order_id):
    """开始运输"""
    try:
        with DatabaseOperations() as db:
            db.start_transport(order_id)
        messages.success(request, f"订单 {order_id} 已开始运输！")
    except Exception as e:
        messages.error(request, f"开始运输失败: {e}")
    return redirect('order_list')

def order_update_status(request, order_id):
    """更新订单状态（通用）"""
    if request.method == 'POST':
        try:
            new_status = request.POST.get('status')
            with DatabaseOperations() as db:
                db.update_order_status(order_id, new_status)
            messages.success(request, f"订单 {order_id} 状态已更新为: {new_status}")
        except Exception as e:
            messages.error(request, f"更新状态失败: {e}")
    return redirect('order_list')

def order_assign_truck(request, order_id):
    """为运单分配车辆"""
    if request.method == 'POST':
        try:
            truck_plate = request.POST.get('truck_plate')
            with DatabaseOperations() as db:
                db.assign_truck_to_order(order_id, truck_plate)
            messages.success(request, f"订单 {order_id} 已分配车辆: {truck_plate}")
        except Exception as e:
            error_msg = str(e)
            # 处理 pymssql 的错误消息 (code, bytes)
            if hasattr(e, 'args') and isinstance(e.args, tuple) and len(e.args) >= 2:
                try:
                    raw_msg = e.args[1]
                    if isinstance(raw_msg, bytes):
                        decoded_msg = raw_msg.decode('utf-8')
                        # 去除 DB-Lib error message 附加信息
                        if "DB-Lib error message" in decoded_msg:
                            decoded_msg = decoded_msg.split("DB-Lib error message")[0]
                        error_msg = decoded_msg
                except Exception:
                    pass # 如果解析失败，保留原错误信息
            
            messages.error(request, f"分配车辆失败: {error_msg}")
        return redirect('order_list')
    
    # GET 请求 - 显示分配车辆表单
    try:
        with DatabaseOperations() as db:
            order = db.get_order_by_id(order_id)
            idle_trucks = db.get_idle_trucks()
        
        if not order:
            messages.error(request, "运单不存在")
            return redirect('order_list')
        
        if order.status != '待分配':
            messages.warning(request, f"运单状态为 {order.status}，只有待分配状态的运单才能分配车辆")
            return redirect('order_list')
        
        return render(request, 'order_assign_truck.html', {
            'order': order,
            'idle_trucks': idle_trucks
        })
    except Exception as e:
        messages.error(request, f"加载失败: {e}")
        return redirect('order_list')
# ========== 存储过程和视图功能 ==========

def fleet_monthly_performance(request):
    """车队月度绩效报表（使用存储过程）"""
    report = None
    fleet_id = request.GET.get('fleet_id')
    year = request.GET.get('year')
    month = request.GET.get('month')
    
    if fleet_id and year and month:
        try:
            with DatabaseOperations() as db:
                report = db.call_fleet_monthly_performance(int(fleet_id), int(year), int(month))
        except Exception as e:
            messages.error(request, f"查询失败: {e}")
    
    # 获取车队列表供选择
    try:
        with DatabaseOperations() as db:
            fleets = db.fetch_all_fleets()
    except:
        fleets = []
    
    return render(request, 'fleet_monthly_performance.html', {
        'report': report,
        'fleets': fleets,
        'selected_fleet': fleet_id,
        'selected_year': year,
        'selected_month': month
    })

def weekly_exception_alerts(request):
    """本周异常警报（使用视图）"""
    alerts = []
    try:
        with DatabaseOperations() as db:
            alerts = db.get_weekly_exception_alerts()
    except Exception as e:
        messages.error(request, f"查询失败: {e}")
    
    return render(request, 'weekly_exception_alerts.html', {'alerts': alerts})

# ========== 司机管理功能 ==========

def driver_edit(request, driver_id):
    """编辑司机信息"""
    if request.method == 'POST':
        try:
            license_level = request.POST.get('license_level')
            phone = request.POST.get('phone')
            fleet_id = request.POST.get('fleet_id')
            
            with DatabaseOperations() as db:
                db.update_driver(driver_id, license_level, phone, fleet_id)
            messages.success(request, f"司机 {driver_id} 信息已更新！")
        except Exception as e:
            messages.error(request, f"更新失败: {e}")
        return redirect('driver_list')
    
    # GET 请求 - 显示编辑表单
    try:
        with DatabaseOperations() as db:
            driver = db.get_driver_by_id(driver_id)
            fleets = db.fetch_all_fleets()
        
        if not driver:
            messages.error(request, "司机不存在")
            return redirect('driver_list')
        
        return render(request, 'driver_edit.html', {
            'driver': driver,
            'fleets': fleets
        })
    except Exception as e:
        messages.error(request, f"加载失败: {e}")
        return redirect('driver_list')
def innovation_recommendation(request):
    """创新模块：智能成本测算与派单推荐系统"""
    recommendations = []
    search_params = {
        'destination': '',
        'weight': ''
    }
    
    if request.method == 'POST':
        destination = request.POST.get('destination', '').strip()
        weight_str = request.POST.get('weight', '').strip()
        
        search_params['destination'] = destination
        search_params['weight'] = weight_str
        
        if destination and weight_str:
            try:
                weight = float(weight_str)
                with DatabaseOperations() as db:
                    raw_rows = db.get_fleet_recommendations(destination, weight)
                    # 转换字典键名为英文，方便模板调用
                    for row in raw_rows:
                        recommendations.append({
                            'id': row.get('推荐车队ID'),
                            'name': row.get('推荐车队名称'),
                            'center': row.get('所属配送中心'),
                            'cost': row.get('预估成本(元)'),
                            'safety': row.get('安全分'),
                            'efficiency': row.get('成本效益指数'),
                            'score': row.get('综合推荐指数')
                        })
            except ValueError:
                messages.error(request, "请输入有效的重量数值")
            except Exception as e:
                messages.error(request, f"推荐系统运行错误: {e}")
    
    return render(request, 'innovation_recommendation.html', {
        'recommendations': recommendations,
        'search_params': search_params
    })

def innovation_cost_rules(request):
    """创新模块：成本规则管理 (CRUD + 统计 + 高级搜索)"""
    search_params = {
        'center_id': '',
        'max_price': '',
        'province_keyword': '',
        'use_advanced': False
    }
    
    rules = []
    stats = []
    centers = []

    try:
        with DatabaseOperations() as db:
            centers = db.fetch_all_distribution_centers()
            stats = db.get_route_statistics()

            # 处理搜索
            if request.GET.get('use_advanced') == 'true':
                search_params['use_advanced'] = True
                search_params['center_id'] = request.GET.get('center_id', '')
                search_params['max_price'] = request.GET.get('max_price', '')
                search_params['province_keyword'] = request.GET.get('province_keyword', '')
                
                rules = db.get_cost_rules_advanced(
                    search_params['center_id'], 
                    search_params['max_price'], 
                    search_params['province_keyword']
                )
            else:
                # 默认显示所有或简单搜索
                rules = db.get_cost_rules()
                
    except Exception as e:
        messages.error(request, f"加载数据失败: {e}")

    return render(request, 'innovation_cost_rules.html', {
        'rules': rules,
        'stats': stats,
        'centers': centers,
        'search_params': search_params
    })

def innovation_cost_rule_add(request):
    """新增成本规则"""
    if request.method == 'POST':
        try:
            center_id = request.POST.get('center_id')
            target_province = request.POST.get('target_province')
            base_price = request.POST.get('base_price')
            traffic_factor = request.POST.get('traffic_factor') or 1.0

            if not (center_id and target_province and base_price):
                messages.error(request, "请填写所有必填字段")
                return redirect('innovation_cost_rules')

            with DatabaseOperations() as db:
                db.add_cost_rule(center_id, target_province, base_price, traffic_factor)
                messages.success(request, "成功添加成本规则")
        except Exception as e:
            messages.error(request, f"添加失败: {e}")
            
    return redirect('innovation_cost_rules')

def innovation_cost_rule_delete(request, rule_id):
    """删除成本规则"""
    if request.method == 'POST':
        try:
            with DatabaseOperations() as db:
                if db.delete_cost_rule(rule_id):
                    messages.success(request, "成功删除规则")
                else:
                    messages.error(request, "删除失败")
        except Exception as e:
            messages.error(request, f"操作异常: {e}")
            
    return redirect('innovation_cost_rules')

def innovation_cost_rules(request):
    """创新模块：成本规则管理 (CRUD + 统计 + 高级搜索)"""
    search_params = {
        'center_id': '',
        'max_price': '',
        'province_keyword': '',
        'use_advanced': False
    }
    
    rules = []
    stats = []
    centers = []

    try:
        with DatabaseOperations() as db:
            centers = db.fetch_all_distribution_centers()
            stats = db.get_route_statistics()

            # 处理搜索
            if request.GET.get('use_advanced') == 'true':
                search_params['use_advanced'] = True
                search_params['center_id'] = request.GET.get('center_id', '')
                search_params['max_price'] = request.GET.get('max_price', '')
                search_params['province_keyword'] = request.GET.get('province_keyword', '')
                
                rules = db.get_cost_rules_advanced(
                    search_params['center_id'], 
                    search_params['max_price'], 
                    search_params['province_keyword']
                )
            else:
                # 默认显示所有或简单搜索
                rules = db.get_cost_rules()
                
    except Exception as e:
        messages.error(request, f"加载数据失败: {e}")

    return render(request, 'innovation_cost_rules.html', {
        'rules': rules,
        'stats': stats,
        'centers': centers,
        'search_params': search_params
    })

def innovation_cost_rule_add(request):
    """新增成本规则"""
    if request.method == 'POST':
        try:
            center_id = request.POST.get('center_id')
            target_province = request.POST.get('target_province')
            base_price = request.POST.get('base_price')
            traffic_factor = request.POST.get('traffic_factor') or 1.0

            if not (center_id and target_province and base_price):
                messages.error(request, "请填写所有必填字段")
                return redirect('innovation_cost_rules')

            with DatabaseOperations() as db:
                db.add_cost_rule(center_id, target_province, base_price, traffic_factor)
                messages.success(request, "成功添加成本规则")
        except Exception as e:
            messages.error(request, f"添加失败: {e}")
            
    return redirect('innovation_cost_rules')

def innovation_cost_rule_delete(request, rule_id):
    """删除成本规则"""
    if request.method == 'POST':
        try:
            with DatabaseOperations() as db:
                if db.delete_cost_rule(rule_id):
                    messages.success(request, "成功删除规则")
                else:
                    messages.error(request, "删除失败")
        except Exception as e:
            messages.error(request, f"操作异常: {e}")
            
    return redirect('innovation_cost_rules')

def innovation_capacity(request):
    """创新模块二：运力预警"""
    data = []
    try:
        with DatabaseOperations() as db:
            data = db.get_capacity_dashboard()
    except Exception as e:
        messages.error(request, f"加载运力数据失败: {e}")
        
    return render(request, 'innovation_capacity.html', {'capacity_data': data})

def innovation_monitor(request):
    """创新模块三：实时监控"""
    monitor_data = {}
    try:
        with DatabaseOperations() as db:
            monitor_data = db.get_monitor_board()
    except Exception as e:
        messages.error(request, f"加载监控数据失败: {e}")
        
    return render(request, 'innovation_monitor.html', {'monitor': monitor_data})
