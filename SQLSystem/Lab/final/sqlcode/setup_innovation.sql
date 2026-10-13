-- ===================================================================
-- 创新模块一：智能成本测算与派单推荐系统 (安装脚本)
-- ===================================================================

USE logistics_db; -- 确保在正确的数据库上下文中操作
GO

-- ---------------------------------------------
-- 步骤 1: 创建表结构
-- ---------------------------------------------
IF OBJECT_ID('dbo.path_cost_rule', 'U') IS NOT NULL
    DROP TABLE dbo.path_cost_rule;
IF OBJECT_ID('dbo.fleet_efficiency_score', 'U') IS NOT NULL
    DROP TABLE dbo.fleet_efficiency_score;
GO

CREATE TABLE path_cost_rule (
    rule_id INT IDENTITY(1,1) PRIMARY KEY,
    center_id INT NOT NULL,
    target_province NVARCHAR(50) NOT NULL,
    base_price_per_km_ton DECIMAL(10,2) NOT NULL,
    traffic_factor DECIMAL(4,2) DEFAULT 1.0,
    CONSTRAINT fk_cost_rule_center FOREIGN KEY (center_id) REFERENCES distribution_center(center_id)
);
GO

CREATE TABLE fleet_efficiency_score (
    fleet_id INT PRIMARY KEY,
    avg_delivery_hours DECIMAL(10,1),
    safety_score INT DEFAULT 100,
    cost_efficiency_index DECIMAL(4,2),
    CONSTRAINT fk_score_fleet FOREIGN KEY (fleet_id) REFERENCES fleet(fleet_id)
);
GO


CREATE OR ALTER PROCEDURE proc_recommend_fleets
    @destination_province NVARCHAR(50), -- 输入参数：目的地省份
    @weight_ton DECIMAL(10,2)        -- 输入参数：货物重量（吨）
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @simulated_distance_km INT = 1000;

    -- 核心推荐逻辑
    SELECT TOP 3
        f.fleet_id AS [推荐车队ID],
        f.fleet_name AS [推荐车队名称],
        dc.center_name AS [所属配送中心],
        CAST(p.base_price_per_km_ton * @simulated_distance_km * @weight_ton * p.traffic_factor AS DECIMAL(10, 2)) AS [预估成本(元)],
        s.safety_score AS [安全分],
        s.cost_efficiency_index AS [成本效益指数],
        CAST((s.safety_score * 0.6 + s.cost_efficiency_index * 40) AS DECIMAL(10, 2)) AS [综合推荐指数]
    FROM 
        fleet AS f
    JOIN 
        distribution_center AS dc ON f.center_id = dc.center_id
    JOIN 
        fleet_efficiency_score AS s ON f.fleet_id = s.fleet_id
    JOIN 
        path_cost_rule AS p ON p.center_id = dc.center_id
    WHERE 
        p.target_province = @destination_province
        AND f.fleet_id <> 2 -- 业务规则：排除同城队
    ORDER BY
        [综合推荐指数] DESC; -- <<< --- 【修正点】: 使用方括号[]来引用列别名，而不是单引号''
END
GO


-- ===================================================================
-- 系统联动脚本：打通 [基础业务表] 与 [创新模块表]
-- 目的：确保原系统的操作（如录入异常、新增车队）能实时影响智能推荐算法
-- ===================================================================

USE logistics_db;
GO

PRINT N'====== 开始部署系统联动触发器 ======';
GO

-- ===================================================================
-- 联动点 1: 新车队自动初始化评分
-- 业务逻辑: 
-- 当我们在基础表 [fleet] 中创建一个新车队时，
-- 系统应该自动在 [fleet_efficiency_score] 中为它创建一份“初始画像”，
-- 否则智能推荐算法会因为找不到这个车队的评分而永远不会推荐它。
-- ===================================================================
PRINT N'正在创建触发器: trg_sync_new_fleet_score...';
GO

CREATE OR ALTER TRIGGER trg_sync_new_fleet_score
ON fleet
AFTER INSERT
AS
BEGIN
    SET NOCOUNT ON;

    -- 自动为新车队插入默认评分记录
    -- 默认值: 平均送达48小时(假设值), 安全分100(满分), 成本指数1.0(平均水平)
    INSERT INTO fleet_efficiency_score (fleet_id, avg_delivery_hours, safety_score, cost_efficiency_index)
    SELECT 
        fleet_id, 
        48.0,   -- 默认平均送达时间
        100,    -- 默认安全分满分
        1.0     -- 默认成本指数
    FROM inserted;
    
    PRINT N'  -> [联动] 检测到新车队，已自动初始化其效率评分画像。';
END
GO


-- ===================================================================
-- 联动点 2: 发生异常自动扣除安全分
-- 业务逻辑: 
-- 当我们在基础表 [exception_record] 中录入一条新的违规/事故记录时，
-- 系统应自动找到该车辆所属的车队，并扣除其 [fleet_efficiency_score] 中的安全分。
-- 扣分规则: 
-- 1. 严重事故/损坏: 扣 20 分
-- 2. 一般违规: 扣 5 分
-- ===================================================================
PRINT N'正在创建触发器: trg_sync_exception_penalty...';
GO

CREATE OR ALTER TRIGGER trg_sync_exception_penalty
ON exception_record
AFTER INSERT
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @truck_plate NVARCHAR(20);
    DECLARE @exception_type NVARCHAR(50);
    DECLARE @fleet_id INT;
    DECLARE @penalty_score INT;

    -- 1. 获取异常信息
    SELECT @truck_plate = truck_plate, @exception_type = exception_type 
    FROM inserted;

    -- 2. 查找该车辆所属的车队
    SELECT @fleet_id = fleet_id 
    FROM truck 
    WHERE plate_number = @truck_plate;

    -- 如果找不到关联车队（数据异常情况），则退出
    IF @fleet_id IS NULL RETURN;

    -- 3. 制定扣分规则 (简单逻辑)
    IF @exception_type LIKE N'%事故%' OR @exception_type LIKE N'%损坏%'
        SET @penalty_score = 20; -- 严重情况扣20分
    ELSE
        SET @penalty_score = 5;  -- 其他情况扣5分

    -- 4. 执行扣分操作
    -- 更新评分表，并且确保分数最低不低于 0
    UPDATE fleet_efficiency_score
    SET safety_score = CASE 
        WHEN safety_score - @penalty_score < 0 THEN 0 
        ELSE safety_score - @penalty_score 
    END
    WHERE fleet_id = @fleet_id;

    -- 5. (可选) 记录审计日志，方便追踪是谁扣的分
    INSERT INTO history_log (target_id, old_value, new_value, operation_type)
    VALUES (
        CAST(@fleet_id AS NVARCHAR(50)),
        N'关联车辆: ' + @truck_plate,
        N'扣减分数: ' + CAST(@penalty_score AS NVARCHAR(10)),
        N'系统自动扣分触发器'
    );

    PRINT N'  -> [联动] 检测到异常录入，已自动扣除车队(ID:' + CAST(@fleet_id AS NVARCHAR(10)) + N') 安全分 ' + CAST(@penalty_score AS NVARCHAR(10)) + N' 分。';
END
GO

PRINT N'====== 系统联动触发器部署完成 ======';
GO

-- ===================================================================
-- 创新模块一：后端API专用存储过程
-- 文件名: innovation_1_crud_api.sql
-- 职责: 
-- 提供一套完整的、可供后端调用的存储过程，用于管理成本规则。
-- 对应前端界面的 “增、删、改、查” 功能。
-- ===================================================================

USE logistics_db;
GO

PRINT N'====== 正在为 [创新模块一] 创建后端API专用存储过程 ======';
GO

-- ---------------------------------------------
-- 1. 查询成本规则 (Read)
--    - 功能: 获取成本规则列表，支持按省份和配送中心进行筛选。
--    - 调用者: 后端 /api/cost-rules (GET) 接口
-- ---------------------------------------------
PRINT N'--> 正在创建存储过程: usp_GetCostRules...';
GO
CREATE OR ALTER PROCEDURE usp_GetCostRules
    @target_province NVARCHAR(50) = NULL, -- (可选) 按目标省份筛选
    @center_id INT = NULL                 -- (可选) 按配送中心ID筛选
AS
BEGIN
    SET NOCOUNT ON;

    SELECT 
        r.rule_id,
        r.center_id,
        dc.center_name, -- 连表查询，方便前端直接显示中心名称
        r.target_province,
        r.base_price_per_km_ton,
        r.traffic_factor
    FROM 
        path_cost_rule r
    JOIN 
        distribution_center dc ON r.center_id = dc.center_id
    WHERE 
        (@target_province IS NULL OR r.target_province LIKE N'%' + @target_province + N'%')
        AND 
        (@center_id IS NULL OR r.center_id = @center_id)
    ORDER BY 
        r.center_id, r.target_province;
END
GO

-- ---------------------------------------------
-- 2. 新增成本规则 (Create)
--    - 功能: 插入一条新的成本规则。
--    - 调用者: 后端 /api/cost-rules (POST) 接口
-- ---------------------------------------------
PRINT N'--> 正在创建存储过程: usp_AddCostRule...';
GO
CREATE OR ALTER PROCEDURE usp_AddCostRule
    @center_id INT,
    @target_province NVARCHAR(50),
    @base_price_per_km_ton DECIMAL(10,2),
    @traffic_factor DECIMAL(4,2),
    @new_rule_id INT OUTPUT -- 返回新增记录的ID
AS
BEGIN
    SET NOCOUNT ON;

    INSERT INTO path_cost_rule (center_id, target_province, base_price_per_km_ton, traffic_factor)
    VALUES (@center_id, @target_province, @base_price_per_km_ton, @traffic_factor);

    SET @new_rule_id = SCOPE_IDENTITY(); -- 获取刚刚插入的自增ID
END
GO


-- ---------------------------------------------
-- 3. 修改成本规则 (Update)
--    - 功能: 根据ID更新一条已存在的成本规则。
--    - 调用者: 后端 /api/cost-rules/{id} (PUT) 接口
-- ---------------------------------------------
PRINT N'--> 正在创建存储过程: usp_UpdateCostRule...';
GO
CREATE OR ALTER PROCEDURE usp_UpdateCostRule
    @rule_id INT,
    @center_id INT,
    @target_province NVARCHAR(50),
    @base_price_per_km_ton DECIMAL(10,2),
    @traffic_factor DECIMAL(4,2)
AS
BEGIN
    SET NOCOUNT ON;

    UPDATE path_cost_rule
    SET 
        center_id = @center_id,
        target_province = @target_province,
        base_price_per_km_ton = @base_price_per_km_ton,
        traffic_factor = @traffic_factor
    WHERE 
        rule_id = @rule_id;
END
GO


-- ---------------------------------------------
-- 4. 删除成本规则 (Delete)
--    - 功能: 根据ID删除一条成本规则。
--    - 调用者: 后端 /api/cost-rules/{id} (DELETE) 接口
-- ---------------------------------------------
PRINT N'--> 正在创建存储过程: usp_DeleteCostRule...';
GO
CREATE OR ALTER PROCEDURE usp_DeleteCostRule
    @rule_id INT
AS
BEGIN
    SET NOCOUNT ON;

    DELETE FROM path_cost_rule
    WHERE rule_id = @rule_id;
END
GO

PRINT N'====== 后端API专用存储过程创建完成! ======';
GO


-- 创新模块一：补充查询接口 (高级搜索 & 统计报表)

PRINT N'====== 正在创建补充查询接口 ======';
GO

-- 1. 高级组合查询接口 (对应场景一)
-- 功能: 支持按出发地、价格区间等复杂条件筛选
PRINT N'--> 正在创建存储过程: usp_SearchCostRules_Advanced...';
GO
CREATE OR ALTER PROCEDURE usp_SearchCostRules_Advanced
    @center_id INT = NULL,              -- 筛选出发地
    @max_price DECIMAL(10,2) = NULL,    -- 筛选最高价格 (查低于此价格的)
    @province_keyword NVARCHAR(50) = NULL -- 筛选省份关键字
AS
BEGIN
    SET NOCOUNT ON;
    
    SELECT 
        r.rule_id, 
        dc.center_name, 
        r.target_province, 
        r.base_price_per_km_ton
    FROM 
        path_cost_rule r 
    JOIN 
        distribution_center dc ON r.center_id = dc.center_id
    WHERE 
        (@center_id IS NULL OR r.center_id = @center_id)
        AND 
        (@max_price IS NULL OR r.base_price_per_km_ton < @max_price)
        AND
        (@province_keyword IS NULL OR r.target_province LIKE N'%' + @province_keyword + N'%');
END
GO

-- 2. 线路统计报表接口 (对应场景二)
-- 功能: 统计每个配送中心配置了多少条线路
PRINT N'--> 正在创建存储过程: usp_GetRouteStatistics...';
GO
CREATE OR ALTER PROCEDURE usp_GetRouteStatistics
AS
BEGIN
    SET NOCOUNT ON;
    
    SELECT 
        dc.center_name AS [配送中心名称], 
        COUNT(r.rule_id) AS [配置线路数量]
    FROM 
        distribution_center dc
    LEFT JOIN 
        path_cost_rule r ON dc.center_id = r.center_id
    GROUP BY 
        dc.center_name
    ORDER BY 
        [配置线路数量] DESC;
END
GO

PRINT N'====== 补充接口创建完成 ======';
GO

-- ===================================================================
-- 创新模块二：基于时间序列的“运力预警与超卖熔断”系统
-- ===================================================================
USE logistics_db;
GO

PRINT N'====== 部署创新模块二：运力预警与熔断 ======';
GO

-- 1. 创建表结构
IF OBJECT_ID('dbo.fleet_capacity_forecast', 'U') IS NOT NULL DROP TABLE dbo.fleet_capacity_forecast;
IF OBJECT_ID('dbo.order_circuit_breaker', 'U') IS NOT NULL DROP TABLE dbo.order_circuit_breaker;
GO

CREATE TABLE fleet_capacity_forecast (
    id INT IDENTITY(1,1) PRIMARY KEY,
    fleet_id INT,
    forecast_date DATE,                 -- 预测日期
    total_capacity DECIMAL(10,2),       -- 总运力
    used_capacity DECIMAL(10,2) DEFAULT 0, -- 已用运力
    risk_level NVARCHAR(20) DEFAULT N'正常', -- 正常/紧张/爆仓
    FOREIGN KEY (fleet_id) REFERENCES fleet(fleet_id)
);

CREATE TABLE order_circuit_breaker (
    rule_id INT IDENTITY(1,1) PRIMARY KEY,
    threshold_percentage DECIMAL(5,2),  -- 阈值 (如 0.90 表示 90%)
    action_type NVARCHAR(50)            -- 动作 (禁止接单/加价/管理员审批)
);
GO

-- 插入默认熔断规则
INSERT INTO order_circuit_breaker (threshold_percentage, action_type) VALUES (0.90, N'禁止接单');
GO

-- 2. 存储过程：初始化未来30天的运力日历
-- (在实际系统中应由定时任务每晚执行，这里手动调用一次初始化)
CREATE OR ALTER PROCEDURE usp_InitCapacityCalendar
    @days INT = 30
AS
BEGIN
    SET NOCOUNT ON;
    DECLARE @i INT = 0;
    DECLARE @current_date DATE;
    
    WHILE @i < @days
    BEGIN
        SET @current_date = DATEADD(DAY, @i, GETDATE());
        
        -- 为每个车队插入当天的记录（如果不存在）
        INSERT INTO fleet_capacity_forecast (fleet_id, forecast_date, total_capacity)
        SELECT 
            f.fleet_id, 
            @current_date, 
            ISNULL((SELECT SUM(max_load) FROM truck WHERE fleet_id = f.fleet_id AND current_status != N'报废'), 0)
        FROM fleet f
        WHERE NOT EXISTS (
            SELECT 1 FROM fleet_capacity_forecast 
            WHERE fleet_id = f.fleet_id AND forecast_date = @current_date
        );
        
        SET @i = @i + 1;
    END
END
GO

-- 执行一次初始化
EXEC usp_InitCapacityCalendar 7; -- 先初始化一周
GO

-- 3. 存储过程：获取运力日历视图 (供前端展示)
CREATE OR ALTER PROCEDURE usp_GetCapacityDashboard
AS
BEGIN
    SET NOCOUNT ON;
    SELECT 
        f.fleet_name,
        c.forecast_date,
        c.total_capacity,
        c.used_capacity,
        CAST(CASE WHEN c.total_capacity > 0 THEN (c.used_capacity / c.total_capacity * 100) ELSE 0 END AS DECIMAL(5,1)) AS usage_pct,
        c.risk_level
    FROM fleet_capacity_forecast c
    JOIN fleet f ON c.fleet_id = f.fleet_id
    WHERE c.forecast_date >= CAST(GETDATE() AS DATE)
    ORDER BY c.forecast_date, f.fleet_id;
END
GO

-- ===================================================================
-- 创新模块三：全系统实时运行状态监控
-- ===================================================================
PRINT N'====== 部署创新模块三：实时监控大屏 ======';
GO

-- 1. 创建监控表
IF OBJECT_ID('dbo.Monitor_Board', 'U') IS NOT NULL DROP TABLE dbo.Monitor_Board;
GO

CREATE TABLE Monitor_Board (
    monitor_key NVARCHAR(50) PRIMARY KEY, -- 指标名称
    monitor_value NVARCHAR(50),           -- 指标值
    last_updated DATETIME DEFAULT GETDATE(),
    description NVARCHAR(100)
);
GO

-- 初始化指标
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

-- 2. 存储过程：刷新所有监控指标
-- (为了简化触发器复杂度，这里写一个统一刷新过程，由后端定期调用或在关键操作后调用)
CREATE OR ALTER PROCEDURE usp_RefreshMonitorBoard
AS
BEGIN
    SET NOCOUNT ON;
    
    DECLARE @val_str NVARCHAR(50);
    DECLARE @val_dec DECIMAL(10,2);
    DECLARE @val_int INT;
    DECLARE @today DATE = CAST(GETDATE() AS DATE);
    DECLARE @tomorrow DATE = DATEADD(DAY, 1, @today);

    -- 1. 待分配积压单量
    SELECT @val_int = COUNT(*) FROM [order] WHERE status = N'待分配';
    UPDATE Monitor_Board SET monitor_value = CAST(@val_int AS NVARCHAR), last_updated = GETDATE() WHERE monitor_key = 'pending_orders';

    -- 2. 今日新增运单总重
    SELECT @val_dec = ISNULL(SUM(weight), 0) FROM [order] WHERE CAST(create_time AS DATE) = @today;
    UPDATE Monitor_Board SET monitor_value = CAST(@val_dec AS NVARCHAR), last_updated = GETDATE() WHERE monitor_key = 'today_weight';

    -- 4. 车辆利用率
    DECLARE @total_trucks INT;
    DECLARE @busy_trucks INT;
    SELECT @total_trucks = COUNT(*) FROM truck;
    IF @total_trucks > 0
    BEGIN
        SELECT @busy_trucks = COUNT(*) FROM truck WHERE current_status = N'运输中';
        SET @val_str = CAST(CAST((@busy_trucks * 100.0 / @total_trucks) AS DECIMAL(5,1)) AS NVARCHAR) + '%';
        UPDATE Monitor_Board SET monitor_value = @val_str, last_updated = GETDATE() WHERE monitor_key = 'truck_usage_rate';
    END

    -- 7. 当前未处理异常数
    SELECT @val_int = COUNT(*) FROM exception_record WHERE handle_status = N'未处理';
    UPDATE Monitor_Board SET monitor_value = CAST(@val_int AS NVARCHAR), last_updated = GETDATE() WHERE monitor_key = 'unhandled_exceptions';
    
    -- 11. 未来24小时运力紧张度
    DECLARE @total_cap DECIMAL(10,2);
    DECLARE @used_cap DECIMAL(10,2);
    SELECT @total_cap = SUM(total_capacity), @used_cap = SUM(used_capacity) 
    FROM fleet_capacity_forecast WHERE forecast_date = @tomorrow;
    
    IF @total_cap > 0
        UPDATE Monitor_Board SET monitor_value = CAST(CAST((@used_cap * 100.0 / @total_cap) AS DECIMAL(5,1)) AS NVARCHAR) + '%', last_updated = GETDATE() WHERE monitor_key = 'future_pressure';
    ELSE
         UPDATE Monitor_Board SET monitor_value = N'0%', last_updated = GETDATE() WHERE monitor_key = 'future_pressure';

END
GO

-- 初始刷新
EXEC usp_RefreshMonitorBoard;
GO
