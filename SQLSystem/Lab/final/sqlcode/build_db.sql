use master;
go

-- 1. 环境清理 (若数据库已存在，则强制删除重建)
-- 这一步是为了方便测试，保证每次运行都是干净的环境
if exists (select * from sys.databases where name = 'logistics_db')
begin
    -- 强制断开所有连接并回滚未提交事务
    alter database logistics_db set single_user with rollback immediate;
    drop database logistics_db;
end
go

-- 2. 创建数据库
create database logistics_db;
go

use logistics_db;
go

-- =============================================
-- 3. 表结构定义 (ddl)
-- =============================================

-- 3.1 配送中心表 (distribution_center)
-- 处于依赖层级最顶端，需最先创建
create table distribution_center (
    center_id int primary key,              -- 中心编号 (pk)
    center_name nvarchar(100) not null,     -- 中心名称
    address nvarchar(200)                   -- 中心地址
);
go

-- 3.2 车队表 (fleet)
-- 依赖于 distribution_center
create table fleet (
    fleet_id int primary key,               -- 车队编号 (pk)
    fleet_name nvarchar(100) not null,      -- 车队名称
    center_id int,                          -- 所属中心编号 (fk)
    
    -- 外键约束
    constraint fk_fleet_center foreign key (center_id) 
        references distribution_center(center_id)
);
go

-- 3.3 调度主管表 (supervisor)
-- 依赖于 fleet，且与 fleet 为 1:1 关系
create table supervisor (
    supervisor_id nvarchar(20) primary key, -- 主管工号 (pk)
    name nvarchar(50) not null,             -- 姓名
    password nvarchar(100) not null,        -- 登录密码
    phone nvarchar(20),                     -- 联系电话
    fleet_id int not null,                  -- 所属车队编号 (fk)
    
    constraint fk_supervisor_fleet foreign key (fleet_id) 
        references fleet(fleet_id),
    
    -- 唯一性约束: 保证一个车队只有一个主管 (实现 1:1 关系)
    constraint uq_supervisor_fleet unique (fleet_id)
);
go

-- 3.4 司机表 (driver)
-- 依赖于 fleet
create table driver (
    driver_id nvarchar(20) primary key,     -- 司机工号 (pk)
    name nvarchar(50) not null,             -- 姓名
    license_level nvarchar(10),             -- 驾照等级 (如 'a1', 'b2')
    phone nvarchar(20),                     -- 联系电话
    hire_date date,                         -- 入职时间
    fleet_id int,                           -- 所属车队编号 (fk)
    
    constraint fk_driver_fleet foreign key (fleet_id) 
        references fleet(fleet_id)
);
go

-- 3.5 车辆表 (truck)
-- 依赖于 fleet
create table truck (
    plate_number nvarchar(20) primary key,  -- 车牌号 (pk)
    max_load decimal(10, 2) not null,       -- 最大载重 (单位: 吨)
    max_volume decimal(10, 2) not null,     -- 最大容积 (单位: 立方米)
    current_status nvarchar(20) default '空闲', -- 当前状态
    fleet_id int,                           -- 所属车队编号 (fk)
    
    constraint fk_truck_fleet foreign key (fleet_id) 
        references fleet(fleet_id),
        
    -- 用户自定义完整性: 限制状态值枚举
    constraint ck_truck_status check (current_status in ('空闲', '运输中', '维修中', '异常'))
);
go

-- 3.6 运单表 ([order])
-- 依赖于 truck
-- 注意: order 是 sql 关键字，需用 [] 包裹
create table [order] (
    order_id nvarchar(50) primary key,      -- 运单号 (pk)
    weight decimal(10, 2) not null,         -- 货物重量
    volume decimal(10, 2) not null,         -- 货物体积
    destination nvarchar(200) not null,     -- 目的地
    status nvarchar(20) default '待分配',   -- 运单状态
    create_time datetime default getdate(), -- 创建时间
    truck_plate nvarchar(20),               -- 承运车辆车牌 (fk, 可为空)
    
    constraint fk_order_truck foreign key (truck_plate) 
        references truck(plate_number),

    -- 状态检查约束
    constraint ck_order_status check (status in ('待分配', '运输中', '已完成', '已取消'))
);
go

-- 3.7 异常记录表 (exception_record)
-- 依赖于 truck 和 driver
create table exception_record (
    record_id int identity(1,1) primary key, -- 记录编号 (自增 pk)
    exception_type nvarchar(50) not null,    -- 异常类型
    occur_time datetime default getdate(),   -- 发生时间
    fine_amount decimal(10, 2) default 0,    -- 罚款金额
    handle_status nvarchar(20) default '未处理', -- 处理状态
    truck_plate nvarchar(20),                -- 关联车辆 (fk)
    driver_id nvarchar(20),                  -- 关联司机 (fk)
    
    constraint fk_ex_truck foreign key (truck_plate) 
        references truck(plate_number),
    constraint fk_ex_driver foreign key (driver_id) 
        references driver(driver_id),

    constraint ck_handle_status check (handle_status in ('未处理', '处理中', '已处理'))
);
go

-- 3.8 审计日志表 (history_log)
-- 独立表，无强外键约束
create table history_log (
    log_id int identity(1,1) primary key,    -- 日志编号 (自增 pk)
    target_id nvarchar(50),                  -- 被修改对象的 id
    old_value nvarchar(max),                 -- 修改前的值
    new_value nvarchar(max),                 -- 修改后的值
    change_time datetime default getdate(),  -- 修改时间
    operation_type nvarchar(50)              -- 操作类型说明
);
go
