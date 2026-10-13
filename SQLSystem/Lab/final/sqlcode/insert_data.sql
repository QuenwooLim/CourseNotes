-- ===================================================================
-- 重置脚本：remake.sql
-- 功能：清空所有业务数据，并重新初始化大量测试数据
-- 注意：执行前请确保已连接到 logistics_db 数据库
-- ===================================================================

USE logistics_db;
GO

PRINT N'====== 开始重置数据库数据 ======';
GO

-- 1. 清空数据 (按外键依赖的逆序删除)
-- -----------------------------------------------------------
PRINT N'正在清空旧数据...';

-- 创新模块相关表
DELETE FROM Monitor_Board;
DELETE FROM order_circuit_breaker;
DELETE FROM fleet_capacity_forecast;
DELETE FROM path_cost_rule;

-- 只有当存在时才删除 (避免报错)
IF OBJECT_ID('dbo.fleet_efficiency_score', 'U') IS NOT NULL
    DELETE FROM fleet_efficiency_score;

-- 核心业务表
DELETE FROM history_log;
DELETE FROM exception_record;
DELETE FROM [order]; 
DELETE FROM driver;
DELETE FROM truck;
DELETE FROM supervisor;

-- 基础档案表
DELETE FROM fleet;
DELETE FROM distribution_center;

PRINT N'数据清空完成. 正在重置自增ID...';
GO

-- 2. 重置自增 ID (让你看着更舒服，从 ID=1 开始)
-- -----------------------------------------------------------
-- 检查表是否存在再重置，防止报错
IF OBJECT_ID('dbo.path_cost_rule', 'U') IS NOT NULL DBCC CHECKIDENT ('path_cost_rule', RESEED, 0);
IF OBJECT_ID('dbo.order_circuit_breaker', 'U') IS NOT NULL DBCC CHECKIDENT ('order_circuit_breaker', RESEED, 0);
-- Truck, Driver, Order 主键通常是字符型(车牌、ID号等)，不需要 RESEED
GO


-- 3. 插入数据
-- -----------------------------------------------------------
PRINT N'正在插入 [Distribution Center]...';
INSERT INTO distribution_center (center_id, center_name, address) VALUES
(1, N'北京顺义分拨中心', N'北京市顺义区南法信镇'),
(2, N'上海青浦分拨中心', N'上海市青浦区华新镇'),
(3, N'广州白云分拨中心', N'广州市白云区太和镇'),
(4, N'成都双流分拨中心', N'成都市双流区航空港'),
(5, N'武汉东西湖分拨中心', N'武汉市东西湖区走马岭');
GO

PRINT N'正在插入 [Fleet] (会自动触发 trg_sync_new_fleet_score 生成默认评分)...';
-- center_id 1=北京, 2=上海, 3=广州, 4=成都, 5=武汉
INSERT INTO fleet (fleet_id, fleet_name, center_id) VALUES
(1, N'京津冀快运一队', 1), -- ID: 1
(2, N'北京同城配送队', 1), -- ID: 2
(3, N'华东干线车队A', 2),  -- ID: 3
(4, N'沪杭甬特快专线', 2), -- ID: 4
(5, N'珠三角城际纵队', 3), -- ID: 5
(6, N'广深港直通车队', 3), -- ID: 6
(7, N'西南山地突击队', 4), -- ID: 7
(8, N'成渝双城经济队', 4), -- ID: 8
(9, N'华中枢纽先锋队', 5), -- ID: 9
(10, N'冷链生鲜专项队', 2); -- ID: 10
GO

PRINT N'正在插入 [Supervisor]...';
INSERT INTO supervisor (supervisor_id, name, password, phone, fleet_id) VALUES
('SUP001', N'张志强', '123456', '13800138000', 1),
('SUP002', N'王建国', '123456', '13800138001', 2),
('SUP003', N'李爱民', '123456', '13800138002', 3),
('SUP004', N'赵铁柱', '123456', '13800138003', 5),
('SUP005', N'刘华强', '123456', '13800138004', 7);
GO

PRINT N'正在插入 [Truck]...';
-- 插入约 20 辆车
INSERT INTO truck (plate_number, max_load, max_volume, current_status, fleet_id) VALUES
(N'京A-88888', 30.0, 120.0, N'空闲', 1),
(N'京C-12345', 10.0, 45.0, N'运输中', 2),
(N'京E-56789', 5.0, 20.0, N'空闲', 2),
(N'沪B-99999', 35.0, 130.0, N'运输中', 3),
(N'沪D-11111', 30.0, 110.0, N'维修中', 3),
(N'浙A-54321', 20.0, 80.0, N'空闲', 4),
(N'粤A-66666', 25.0, 90.0, N'空闲', 5),
(N'粤B-77777', 30.0, 120.0, N'运输中', 5),
(N'粤Z-00001', 15.0, 60.0, N'运输中', 6),
(N'川A-10086', 30.0, 100.0, N'空闲', 7),
(N'川B-10010', 25.0, 95.0, N'异常', 7), -- 异常车辆
(N'鄂A-WH001', 40.0, 150.0, N'运输中', 9),
(N'鄂A-WH002', 40.0, 150.0, N'运输中', 9),
(N'沪C-COLD1', 10.0, 30.0, N'空闲', 10), -- 冷链
(N'沪C-COLD2', 10.0, 30.0, N'运输中', 10);
GO

PRINT N'正在插入 [Driver]...';
INSERT INTO driver (driver_id, name, license_level, phone, hire_date, fleet_id) VALUES
('D001', N'刘跑跑', 'A1', '13911112222', '2020-01-01', 1),
('D002', N'王快手', 'A2', '13922223333', '2021-03-15', 2),
('D003', N'陈稳重', 'A1', '13933334444', '2019-06-20', 3),
('D004', N'赵老四', 'B2', '13944445555', '2022-11-11', 5),
('D005', N'孙悟空', 'A1', '13955556666', '2018-05-05', 7),
('D006', N'猪八戒', 'B2', '13966667777', '2023-01-01', 10),
('D007', N'沙悟净', 'A2', '13977778888', '2021-09-09', 9);
GO

PRINT N'正在插入 [Path Cost Rule] (创新模块一)...';
-- 北京出发
INSERT INTO path_cost_rule (center_id, target_province, base_price_per_km_ton, traffic_factor) VALUES
(1, N'上海市', 0.80, 1.2),
(1, N'广东省', 0.75, 1.1),
(1, N'四川省', 1.20, 1.5), -- 山路贵
(1, N'浙江省', 0.85, 1.1);

-- 上海出发
INSERT INTO path_cost_rule (center_id, target_province, base_price_per_km_ton, traffic_factor) VALUES
(2, N'北京市', 0.80, 1.3), -- 进京堵
(2, N'江苏省', 0.60, 1.0),
(2, N'浙江省', 0.65, 1.0);

-- 广州出发
INSERT INTO path_cost_rule (center_id, target_province, base_price_per_km_ton, traffic_factor) VALUES
(3, N'北京市', 0.75, 1.1),
(3, N'湖南省', 0.70, 1.2);

-- 成都出发
INSERT INTO path_cost_rule (center_id, target_province, base_price_per_km_ton, traffic_factor) VALUES
(4, N'重庆市', 0.90, 1.3),
(4, N'陕西省', 1.10, 1.4);
GO

PRINT N'正在模拟更新 [Fleet Efficiency Score] (制造数据差异)...';
-- 手动调整一些车队的分数，以便体现推荐算法的差异
-- 车队1: 优秀
UPDATE fleet_efficiency_score SET avg_delivery_hours=24.0, safety_score=98, cost_efficiency_index=1.2 WHERE fleet_id=1;
-- 车队3: 便宜但慢
UPDATE fleet_efficiency_score SET avg_delivery_hours=72.0, safety_score=95, cost_efficiency_index=1.5 WHERE fleet_id=3;
-- 车队7: 危险 (山路车队)
UPDATE fleet_efficiency_score SET avg_delivery_hours=50.0, safety_score=75, cost_efficiency_index=0.9 WHERE fleet_id=7;
GO

PRINT N'正在插入 [Order] (大量历史与当前订单)...';
-- 1. 已完成的单子 (用于统计报表)
INSERT INTO [order] (order_id, weight, volume, destination, status, create_time, truck_plate) VALUES
('ORD20251001001', 5.0, 20, N'上海市', N'已完成', '2025-10-01 08:00:00', N'京A-88888'),
('ORD20251001002', 2.0, 40, N'北京市', N'已完成', '2025-10-02 09:30:00', N'京C-12345'),
('ORD20251105001', 5.0, 80, N'杭州市', N'已完成', '2025-11-05 14:20:00', N'沪B-99999'),
('ORD20251201001', 8.0, 30, N'深圳市', N'已完成', '2025-12-01 10:00:00', N'粤A-66666');

-- 2. 运输中的单子 (用于监控)
INSERT INTO [order] (order_id, weight, volume, destination, status, create_time, truck_plate) VALUES
('ORD20260115001', 15.0, 60, N'南京市', N'运输中', GETDATE()-1, N'沪B-99999'),
('ORD20260115002', 5.0, 10, N'天津市', N'运输中', GETDATE()-2, N'京C-12345'),
('ORD20260116001', 30.0, 100, N'武汉市', N'运输中', GETDATE(), N'鄂A-WH001'),
('ORD20260116002', 30.0, 100, N'长沙市', N'运输中', GETDATE(), N'鄂A-WH002'),
('ORD20260116003', 10.0, 40, N'广州市', N'运输中', GETDATE(), N'粤B-77777');

-- 3. 待分配的单子 (用于积压监控)
INSERT INTO [order] (order_id, weight, volume, destination, status, create_time, truck_plate) VALUES
('ORD20260116088', 2.0, 5, N'石家庄', N'待分配', GETDATE(), NULL),
('ORD20260116089', 50.0, 200, N'乌鲁木齐', N'待分配', GETDATE(), NULL), -- 大单
('ORD20260116090', 1.0, 2, N'苏州市', N'待分配', GETDATE(), NULL);
GO

PRINT N'正在插入 [Exception Record]...';
-- 这里的插入会触发 trg_sync_exception_penalty 扣分
INSERT INTO exception_record (exception_type, occur_time, fine_amount, handle_status, truck_plate, driver_id) VALUES
(N'超速违规', DATEADD(DAY, -5, GETDATE()), 200.00, N'已处理', N'京C-12345', 'D002'),
(N'货物损坏', DATEADD(DAY, -2, GETDATE()), 5000.00, N'未处理', N'川B-10010', 'D005'), -- 此次会导致车队7扣分
(N'疲劳驾驶', DATEADD(DAY, -10, GETDATE()), 500.00, N'已处理', N'沪D-11111', 'D003');
GO

PRINT N'正在插入 [Order Circuit Breaker] (创新模块二)...';
INSERT INTO order_circuit_breaker (threshold_percentage, action_type)
VALUES (0.90, N'禁止接单');
GO

PRINT N'正在初始化 [Fleet Capacity Forecast] (创新模块二)...';
-- 手动调用初始化过程
EXEC usp_InitCapacityCalendar 30;

-- 模拟一些爆仓数据 (让车队1在明天的运力被占满)
UPDATE fleet_capacity_forecast 
SET used_capacity = total_capacity * 0.95, risk_level = N'爆仓'
WHERE fleet_id = 1 AND forecast_date = CAST(DATEADD(DAY, 1, GETDATE()) AS DATE);

-- 模拟一些紧张数据
UPDATE fleet_capacity_forecast 
SET used_capacity = total_capacity * 0.75, risk_level = N'紧张'
WHERE fleet_id = 3 AND forecast_date = CAST(DATEADD(DAY, 2, GETDATE()) AS DATE);
GO

PRINT N'正在初始化 [Monitor Board Keys]...';
INSERT INTO Monitor_Board (monitor_key, monitor_value, description) VALUES 
(N'pending_orders', N'0', N'待分配积压单量'),
(N'today_weight', N'0', N'今日新增运单总重'),
(N'active_drivers', N'0', N'活跃司机人数'),
(N'truck_usage_rate', N'0%', N'车辆利用率'),
(N'truck_fault_rate', N'0%', N'车辆故障率'),
(N'idle_heavy_trucks', N'0', N'空闲重卡数量'),
(N'unhandled_exceptions', N'0', N'当前未处理异常数'),
(N'today_fines', N'0', N'今日罚款总金额'),
(N'circuit_breaker_status', N'0', N'运力熔断预警状态'),
(N'algo_adoption_rate', N'0%', N'智能派单成功率'),
(N'future_pressure', N'0%', N'未来24小时运力紧张度');
GO

PRINT N'正在刷新 [Monitor Board] (创新模块三)...';
EXEC usp_RefreshMonitorBoard;
GO

PRINT N'====== 数据库重置与数据灌入全部完成! ======';
GO
