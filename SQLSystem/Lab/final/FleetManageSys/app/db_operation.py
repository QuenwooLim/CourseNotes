import pymssql
from typing import List, Optional
from app.models import Fleet, Truck, Driver, Order, DistributionCenter, ExceptionRecord, Supervisor, HistoryLog, PathCostRule, FleetEfficiencyScore

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
                database='logistics_db'
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
    
    def fetch_all_fleets(self) -> List[Fleet]:
        """获取所有车队信息"""
        try:
            self.cursor.execute("SELECT fleet_id, fleet_name, center_id FROM fleet")
            rows = self.cursor.fetchall()
            items = [Fleet.from_dict(row) for row in rows]
            return items
        except Exception as e:
            print(f"查询车队信息失败: {e}")
            return []

    def fetch_all_trucks(self) -> List[Truck]:
        try:
            self.cursor.execute("SELECT * FROM truck")
            rows = self.cursor.fetchall()
            items = [Truck.from_dict(row) for row in rows]
            return items
        except Exception as e:
            print(f"查询车辆信息失败: {e}")
            return []

    def fetch_all_drivers(self) -> List[Driver]:
        try:
            self.cursor.execute("SELECT * FROM driver")
            rows = self.cursor.fetchall()
            items = [Driver.from_dict(row) for row in rows]
            return items
        except Exception as e:
            print(f"查询司机信息失败: {e}")
            return []
            
    def fetch_all_orders(self) -> List[Order]:
        try:
            self.cursor.execute("SELECT * FROM [order]")
            rows = self.cursor.fetchall()
            items = [Order.from_dict(row) for row in rows]
            return items
        except Exception as e:
            print(f"查询运单信息失败: {e}")
            return []

    def fetch_all_distribution_centers(self) -> List[DistributionCenter]:
        try:
            self.cursor.execute("SELECT * FROM distribution_center")
            rows = self.cursor.fetchall()
            items = [DistributionCenter.from_dict(row) for row in rows]
            return items
        except Exception as e:
            print(f"查询配送中心信息失败: {e}")
            return []

    def fetch_all_supervisors(self) -> List[Supervisor]:
        try:
            self.cursor.execute("SELECT * FROM supervisor")
            rows = self.cursor.fetchall()
            items = [Supervisor.from_dict(row) for row in rows]
            return items
        except Exception as e:
            print(f"查询主管信息失败: {e}")
            return []
    
    def fetch_all_exception_records(self) -> List[ExceptionRecord]:
        try:
            self.cursor.execute("SELECT * FROM exception_record")
            rows = self.cursor.fetchall()
            items = [ExceptionRecord.from_dict(row) for row in rows]
            return items
        except Exception as e:
            print(f"查询异常记录失败: {e}")
            return []

    def fetch_all_logs(self) -> List[HistoryLog]:
        try:
            self.cursor.execute("SELECT * FROM history_log ORDER BY change_time DESC")
            rows = self.cursor.fetchall()
            items = [HistoryLog.from_dict(row) for row in rows]
            return items
        except Exception as e:
            print(f"查询审计日志失败: {e}")
            return []

    def get_statistics(self):
        """获取统计数据"""
        try:
            stats = {}
            # 统计车队数量
            self.cursor.execute("SELECT COUNT(*) as count FROM fleet")
            row = self.cursor.fetchone()
            stats['fleet_count'] = row['count'] if row else 0
            return stats
        except Exception as e:
            print(f"统计数据获取失败: {e}")
            return {'fleet_count': 0}

    # ================= 新增功能方法 =================

    def add_driver(self, driver_id, name, license_level, phone, hire_date, fleet_id):
        """1. 录入司机"""
        sql = """
            INSERT INTO driver (driver_id, name, license_level, phone, hire_date, fleet_id)
            VALUES (%s, %s, %s, %s, %s, %s)
        """
        self.cursor.execute(sql, (driver_id, name, license_level, phone, hire_date, fleet_id))
        self.conn.commit()

    def add_truck(self, plate_number, max_load, max_volume, current_status, fleet_id):
        """1. 录入车辆"""
        sql = """
            INSERT INTO truck (plate_number, max_load, max_volume, current_status, fleet_id)
            VALUES (%s, %s, %s, %s, %s)
        """
        self.cursor.execute(sql, (plate_number, max_load, max_volume, current_status, fleet_id))
        self.conn.commit()

    def get_idle_trucks(self):
        """获取空闲车辆（用于运单分配下拉列表）"""
        self.cursor.execute("SELECT plate_number, max_load FROM truck WHERE current_status = '空闲'")
        return self.cursor.fetchall()
    
    def get_order_by_id(self, order_id):
        """根据运单号获取运单详情"""
        self.cursor.execute("SELECT * FROM [order] WHERE order_id = %s", (order_id,))
        row = self.cursor.fetchone()
        if row:
            return Order.from_dict(row)
        return None
    
    def get_driver_by_id(self, driver_id):
        """根据司机工号获取司机详情"""
        self.cursor.execute("SELECT * FROM driver WHERE driver_id = %s", (driver_id,))
        row = self.cursor.fetchone()
        if row:
            return Driver.from_dict(row)
        return None
    
    def update_driver(self, driver_id, license_level, phone, fleet_id):
        """更新司机信息（驾照、电话、车队）"""
        sql = """
            UPDATE driver
            SET license_level = %s, phone = %s, fleet_id = %s
            WHERE driver_id = %s
        """
        self.cursor.execute(sql, (license_level, phone, fleet_id, driver_id))
        self.conn.commit()
    
    def call_fleet_monthly_performance(self, fleet_id, year, month):
        """调用存储过程获取车队月度绩效"""
        sql = "EXEC sp_get_fleet_monthly_performance %s, %s, %s"
        self.cursor.execute(sql, (fleet_id, year, month))
        result = self.cursor.fetchone()
        return result
    
    def get_weekly_exception_alerts(self):
        """使用视图获取本周异常警报"""
        self.cursor.execute("SELECT * FROM v_weekly_exception_alerts ORDER BY 发生时间 DESC")
        return self.cursor.fetchall()

    def assign_order(self, weight, volume, destination):
        """2. 创建运单 (自动生成运单号，默认状态为待分配，不分配车辆)"""
        # 生成运单号: ORD + 日期(YYYYMMDD) + 当日序号(4位)
        sql_get_order_id = """
            DECLARE @today_str NVARCHAR(8) = CONVERT(NVARCHAR(8), GETDATE(), 112);
            DECLARE @seq INT;
            
            -- 获取今天已有的最大序号
            SELECT @seq = ISNULL(MAX(CAST(RIGHT(order_id, 4) AS INT)), 0) + 1
            FROM [order]
            WHERE order_id LIKE 'ORD' + @today_str + '%';
            
            -- 生成新运单号
            SELECT 'ORD' + @today_str + RIGHT('0000' + CAST(@seq AS NVARCHAR), 4) AS new_order_id;
        """
        self.cursor.execute(sql_get_order_id)
        result = self.cursor.fetchone()
        order_id = result['new_order_id']
        
        # 插入运单（不分配车辆，truck_plate 为 NULL）
        sql = """
            INSERT INTO [order] (order_id, weight, volume, destination, status, create_time, truck_plate)
            VALUES (%s, %s, %s, %s, '待分配', GETDATE(), NULL)
        """
        self.cursor.execute(sql, (order_id, weight, volume, destination))
        self.conn.commit()
        return order_id
    
    def assign_truck_to_order(self, order_id, truck_plate):
        """为运单分配车辆 (触发器会检查载重)"""
        sql = """
            UPDATE [order]
            SET truck_plate = %s
            WHERE order_id = %s AND status = '待分配'
        """
        self.cursor.execute(sql, (truck_plate, order_id))
        if self.cursor.rowcount == 0:
            raise Exception("运单不存在或状态不是待分配")
        self.conn.commit()
    
    def add_exception_record(self, exception_type, occur_time, fine_amount, handle_status, truck_plate, driver_id):
        """3. 异常记录录入"""
        sql = """
            INSERT INTO exception_record (exception_type, occur_time, fine_amount, handle_status, truck_plate, driver_id)
            VALUES (%s, %s, %s, %s, %s, %s)
        """
        self.cursor.execute(sql, (exception_type, occur_time, fine_amount, handle_status, truck_plate, driver_id))
        self.conn.commit()
    
    def complete_order(self, order_id):
        """完成订单 - 将订单状态更新为已完成"""
        sql = """
            UPDATE [order] 
            SET status = '已完成'
            WHERE order_id = %s
        """
        self.cursor.execute(sql, (order_id,))
        self.conn.commit()
    
    def cancel_order(self, order_id):
        """取消订单 - 将订单状态更新为已取消"""
        sql = """
            UPDATE [order] 
            SET status = '已取消'
            WHERE order_id = %s
        """
        self.cursor.execute(sql, (order_id,))
        self.conn.commit()
    
    def update_order_status(self, order_id, new_status):
        """更新订单状态 - 通用方法"""
        sql = """
            UPDATE [order] 
            SET status = %s
            WHERE order_id = %s
        """
        self.cursor.execute(sql, (new_status, order_id))
        self.conn.commit()
    
    def start_transport(self, order_id):
        """开始运输 - 将订单状态更新为运输中，同时更新车辆状态"""
        sql = """
            UPDATE [order] 
            SET status = '运输中'
            WHERE order_id = %s;
            
            UPDATE truck
            SET current_status = '运输中'
            WHERE plate_number = (SELECT truck_plate FROM [order] WHERE order_id = %s);
        """
        self.cursor.execute(sql, (order_id, order_id))
        self.conn.commit()

    def query_center_resources(self, center_id):
        """4. 车队资源查询"""
        sql = """
            SELECT F.fleet_name, T.plate_number, T.current_status, T.max_load, T.max_volume
            FROM truck T
            JOIN fleet F ON T.fleet_id = F.fleet_id
            WHERE F.center_id = %s
            ORDER BY F.fleet_name, T.current_status
        """
        self.cursor.execute(sql, (center_id,))
        return self.cursor.fetchall()

    def query_driver_performance(self, driver_id, start_date=None, end_date=None):
        """5. 司机绩效追踪"""
        stats = {}
        
        sql_exceptions = "SELECT * FROM exception_record WHERE driver_id = %s"
        params = [driver_id]
        
        if start_date and end_date:
            sql_exceptions += " AND occur_time BETWEEN %s AND %s"
            params.extend([start_date, end_date])
            
        self.cursor.execute(sql_exceptions, tuple(params))
        exceptions = self.cursor.fetchall()
        
        stats['exception_count'] = len(exceptions)
        stats['exceptions'] = exceptions
        return stats

    def query_fleet_report(self, fleet_id, year=None, month=None):
        """6. 统计报表 (安全与效率)"""
        report = {}
        
        # 基础 SQL
        sql_orders = """
            SELECT COUNT(*) as OrderCount 
            FROM [order] O
            JOIN truck T ON O.truck_plate = T.plate_number
            WHERE T.fleet_id = %s
        """
        
        sql_exceptions = """
            SELECT COUNT(*) as ExceptionCount, SUM(fine_amount) as TotalFines
            FROM exception_record E
            JOIN truck T ON E.truck_plate = T.plate_number
            WHERE T.fleet_id = %s
        """
        
        params = [fleet_id]
        
        # 如果有日期限制，添加条件
        if year and month:
             # 构建日期范围
            start_date = f"{year}-{month}-01"
            if month == 12:
                end_date = f"{year+1}-01-01"
            else:
                end_date = f"{year}-{int(month)+1}-01"
            
            date_condition = " AND O.create_time >= %s AND O.create_time < %s"
            sql_orders += date_condition
            
            ex_date_condition = " AND E.occur_time >= %s AND E.occur_time < %s"
            sql_exceptions += ex_date_condition
            
            params.extend([start_date, end_date])

        # 执行查询
        self.cursor.execute(sql_orders, tuple(params))
        report['total_orders'] = self.cursor.fetchone()['OrderCount']
        
        self.cursor.execute(sql_exceptions, tuple(params))
        res = self.cursor.fetchone()
        report['total_exceptions'] = res['ExceptionCount']
        report['total_fines'] = res['TotalFines'] or 0
        
        return report

    def get_fleet_recommendations(self, destination_province: str, weight_ton: float) -> List[dict]:
        """
        调用存储过程 proc_recommend_fleets 获取推荐车队
        """
        try:
            # 调用存储过程
            # pymssql 的 callproc 用法: cursor.callproc('proc_name', (args,))
            # 但是为了获取结果集，有时候直接 execute EXEC 更方便
            self.cursor.execute("EXEC proc_recommend_fleets %s, %s", (destination_province, weight_ton))
            rows = self.cursor.fetchall()
            return rows
        except Exception as e:
            print(f"获取车队推荐失败: {e}")
            return []

    # ==========================
    # 创新模块一：成本规则管理 (CRUD API)
    # ==========================
    def get_cost_rules(self, target_province=None, center_id=None):
        """调用 usp_GetCostRules"""
        try:
            sql = "EXEC usp_GetCostRules %s, %s"
            params = (target_province if target_province else None, center_id if center_id else None)
            self.cursor.execute(sql, params)
            return self.cursor.fetchall()
        except Exception as e:
            print(f"查询成本规则失败: {e}")
            return []

    def add_cost_rule(self, center_id, target_province, base_price, traffic_factor=1.0):
        """调用 usp_AddCostRule"""
        try:
            sql = """
                DECLARE @out_id INT;
                EXEC usp_AddCostRule %s, %s, %s, %s, @out_id OUTPUT;
                SELECT @out_id AS new_id;
            """
            self.cursor.execute(sql, (center_id, target_province, base_price, traffic_factor))
            row = self.cursor.fetchone()
            self.conn.commit()
            return row['new_id'] if row else None
        except Exception as e:
            print(f"新增成本规则失败: {e}")
            self.conn.rollback()
            raise e

    def update_cost_rule(self, rule_id, center_id, target_province, base_price, traffic_factor):
        """调用 usp_UpdateCostRule"""
        try:
            self.cursor.execute(
                "EXEC usp_UpdateCostRule %s, %s, %s, %s, %s",
                (rule_id, center_id, target_province, base_price, traffic_factor)
            )
            self.conn.commit()
            return True
        except Exception as e:
            print(f"更新成本规则失败: {e}")
            self.conn.rollback()
            return False

    def delete_cost_rule(self, rule_id):
        """调用 usp_DeleteCostRule"""
        try:
            self.cursor.execute("EXEC usp_DeleteCostRule %s", (rule_id,))
            self.conn.commit()
            return True
        except Exception as e:
            print(f"删除成本规则失败: {e}")
            self.conn.rollback()
            return False

    def get_cost_rules_advanced(self, center_id=None, max_price=None, province_keyword=None):
        """调用 usp_SearchCostRules_Advanced"""
        try:
            sql = "EXEC usp_SearchCostRules_Advanced %s, %s, %s"
            # 处理空字符串为 None
            c_id = center_id if center_id else None
            m_p = max_price if max_price else None
            p_k = province_keyword if province_keyword else None
            
            self.cursor.execute(sql, (c_id, m_p, p_k))
            return self.cursor.fetchall()
        except Exception as e:
            print(f"高级查询成本规则失败: {e}")
            return []

    def get_route_statistics(self):
        """调用 usp_GetRouteStatistics"""
        try:
            self.cursor.execute("EXEC usp_GetRouteStatistics")
            return self.cursor.fetchall()
        except Exception as e:
            print(f"获取线路统计失败: {e}")
            return []

    # ==========================
    # 创新模块二：运力预警
    # ==========================
    def get_capacity_dashboard(self):
        """调用 usp_GetCapacityDashboard"""
        try:
            # 确保数据是最新的(可选)
            # self.cursor.execute("EXEC usp_InitCapacityCalendar 7") 
            self.cursor.execute("EXEC usp_GetCapacityDashboard")
            return self.cursor.fetchall()
        except Exception as e:
            print(f"获取运力日历失败: {e}")
            return []

    # ==========================
    # 创新模块三：实时监控
    # ==========================
    def get_monitor_board(self):
        """刷新并获取监控大屏数据"""
        try:
            # 先刷新
            self.cursor.execute("EXEC usp_RefreshMonitorBoard")
            self.conn.commit()
            
            # 再查询
            self.cursor.execute("SELECT * FROM Monitor_Board")
            rows = self.cursor.fetchall()
            # 转换为字典格式 {key: value}
            data = {row['monitor_key']: row['monitor_value'] for row in rows}
            return data
        except Exception as e:
            print(f"获取监控数据失败: {e}")
            return {}

    def __enter__(self):
        """上下文管理器进入"""
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        """上下文管理器退出，自动关闭连接"""
        self.close()