-- 确保在正确的数据库上下文中
USE logistics_db;
GO

-- 4.1(a). 自动载重校验触发器
create trigger trg_check_truck_load
on [order]
after insert, update
as
begin
    -- 声明所需变量
    declare @truck_plate nvarchar(20);
    declare @new_weight decimal(10, 2);
    declare @current_total_weight decimal(10, 2);
    declare @max_load decimal(10, 2);

    -- 从 inserted 表获取被操作运单的车辆和重量
    select @truck_plate = truck_plate, @new_weight = weight 
    from inserted;

    if @truck_plate is not null
    begin
        select @max_load = max_load from truck where plate_number = @truck_plate;
        
        select @current_total_weight = isnull(sum(weight), 0) 
        from [order] 
        where truck_plate = @truck_plate and order_id not in (select order_id from inserted);

        if (@current_total_weight + @new_weight > @max_load)
        begin
            -- 1. 先声明几个 varchar 变量用于错误信息
            declare @error_msg nvarchar(500);
            declare @current_weight_str nvarchar(20);
            declare @new_weight_str nvarchar(20);
            declare @max_load_str nvarchar(20);

            -- 2. 将 decimal 类型转换为字符串
            set @current_weight_str = cast(@current_total_weight as nvarchar(20));
            set @new_weight_str = cast(@new_weight as nvarchar(20));
            set @max_load_str = cast(@max_load as nvarchar(20));
            
            -- 3. 使用 FORMATMESSAGE 或拼接字符串来构造完整的错误信息
            set @error_msg = formatmessage('分配失败：车辆 %s 将超出最大载重限制！当前载重 %s, 新增 %s, 最大载重 %s',
                                           @truck_plate, @current_weight_str, @new_weight_str, @max_load_str);
            
            -- 4. 抛出构造好的字符串错误
            raiserror (@error_msg, 16, 1);
            rollback transaction;
        end
    end
end;
go

-- 4.1(b). 车辆状态自动流转触发器 (基于运单完成)
create trigger trg_auto_update_truck_status_on_order
on [order]
after update
as
begin
    -- 确保触发器只在 'status' 列被更新时才执行，提高效率
    if not update(status) return;

    declare @truck_plate nvarchar(20);

    -- 从 inserted 表获取刚刚被更新为“已完成”的运单所属的车辆
    select @truck_plate = truck_plate 
    from inserted 
    where status = '已完成';
    
    -- 如果确实有运单被更新为“已完成”并且它有关联的车辆
    if @truck_plate is not null
    begin
        -- 检查该车辆是否还有任何其他“运输中”的运单
        if not exists (
            select 1 
            from [order] 
            where truck_plate = @truck_plate and status = '运输中'
        )
        begin
            -- 如果没有了，则将车辆状态从“运输中”更新为“空闲”
            update truck 
            set current_status = '空闲' 
            where plate_number = @truck_plate and current_status = '运输中';
        end
    end
end;
go

-- 4.1(b). 车辆状态自动流转触发器 (基于异常处理)
create trigger trg_auto_update_truck_status_on_exception
on exception_record
after update
as
begin
    -- 确保触发器只在 'handle_status' 列被更新时才执行
    if not update(handle_status) return;

    declare @truck_plate nvarchar(20);
    
    -- 从 inserted 表获取刚刚被更新为“已处理”的异常记录所属的车辆
    select @truck_plate = truck_plate 
    from inserted
    where handle_status = '已处理';

    if @truck_plate is not null
    begin
        -- 将车辆状态从'异常'更新回'空闲'
        -- （简化逻辑，实际情况可能更复杂，例如需要判断车辆是否有未完成的运单）
        update truck set current_status = '空闲' 
        where plate_number = @truck_plate and current_status = '异常';
    end
end;
go

-- 4.1(c). 审计日志触发器 (司机驾照变更)
create trigger trg_audit_driver_license
on driver
after update
as
begin
    -- 确保触发器只在 'license_level' 列被更新时才执行
    if not update(license_level) return;

    -- 将变更前后的信息插入到日志表
    insert into history_log (target_id, old_value, new_value, operation_type)
    select 
        d.driver_id,                           -- 被修改的司机ID
        '驾照等级: ' + d.license_level,         -- 旧值
        '驾照等级: ' + i.license_level,         -- 新值
        '修改司机驾照等级'                     -- 操作说明
    from 
        deleted d 
    join 
        inserted i on d.driver_id = i.driver_id
    where 
        isnull(d.license_level, '') <> isnull(i.license_level, ''); -- 仅当值确实发生改变时才记录
end;
go

-- 4.1(c). 审计日志触发器 (异常记录处理)
create trigger trg_audit_exception_handle
on exception_record
after update
as
begin
    -- 确保触发器只在 'handle_status' 列被更新时才执行
    if not update(handle_status) return;

    -- 只在状态从“未处理”变为“已处理”时记录日志
    insert into history_log (target_id, old_value, new_value, operation_type)
    select
        cast(d.record_id as nvarchar(50)), -- target_id 是 nvarchar，需要转换
        '处理状态: ' + d.handle_status,
        '处理状态: ' + i.handle_status,
        '处理异常记录'
    from 
        deleted d 
    join 
        inserted i on d.record_id = i.record_id
    where 
        d.handle_status = '未处理' and i.handle_status = '已处理';
end;
go

-- 4.2 存储过程：计算指定车队在某个月份的绩效报表（总运单数、异常事件数和累计罚款）
create procedure sp_get_fleet_monthly_performance
    @fleet_id int,
    @year int,
    @month int
as
begin
    -- 1. 声明用于存储结果的变量
    declare @total_orders int;
    declare @total_exceptions int;
    declare @total_fines decimal(10, 2);

    -- 2. 计算总运单数
    -- 查找所有与该车队车辆关联的，且在指定年月的运单
    select @total_orders = count(o.order_id)
    from [order] o
    join truck t on o.truck_plate = t.plate_number
    where t.fleet_id = @fleet_id
      and year(o.create_time) = @year
      and month(o.create_time) = @month;

    -- 3. 计算总异常数和总罚款
    -- 查找所有与该车队车辆关联的，且在指定年月的异常记录
    select 
        @total_exceptions = count(e.record_id),
        @total_fines = isnull(sum(e.fine_amount), 0)
    from exception_record e
    join truck t on e.truck_plate = t.plate_number
    where t.fleet_id = @fleet_id
      and year(e.occur_time) = @year
      and month(e.occur_time) = @month;

    -- 4. 将计算结果作为查询结果返回
    select 
        @fleet_id as '车队ID',
        @total_orders as '总运单数',
        @total_exceptions as '异常事件总数',
        @total_fines as '累计罚款金额';
end;
go

-- 4.3 视图：查询本周发生过异常的车辆和司机信息
create view v_weekly_exception_alerts as
select distinct
    t.plate_number as '车牌号',
    t.current_status as '车辆状态',
    d.name as '司机姓名',
    d.phone as '司机电话',
    e.exception_type as '异常类型',
    e.occur_time as '发生时间'
from 
    exception_record e
join 
    truck t on e.truck_plate = t.plate_number
join 
    driver d on e.driver_id = d.driver_id
where
    -- datepart(wk, ...) 获取当前日期是一年中的第几周
    datepart(wk, e.occur_time) = datepart(wk, getdate())
    and year(e.occur_time) = year(getdate());
go

-- 4.4 为 driver 表的 name 列创建非聚集索引
create index idx_driver_name on driver(name);
go