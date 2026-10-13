CREATE DATABASE IF NOT EXISTS mcp;
USE mcp;

DROP TABLE IF EXISTS tbl_user_sessions;
DROP TABLE IF EXISTS tbl_user_conversations;
DROP TABLE IF EXISTS tbl_course_schedules;
DROP TABLE IF EXISTS tbl_appointments;
DROP TABLE IF EXISTS tbl_notes;
DROP TABLE IF EXISTS tbl_users;
CREATE TABLE tbl_users (
    username VARCHAR(50) PRIMARY KEY COMMENT '登录名称',
    password VARCHAR(255) NOT NULL COMMENT '登录密码（哈希保存）',
    salt VARCHAR(50) NOT NULL COMMENT '加盐',
    email VARCHAR(100) COMMENT '邮箱',
    phone VARCHAR(100) COMMENT '手机号',
    full_name VARCHAR(100) COMMENT '全名称',
    last_login_time TIMESTAMP COMMENT '最后一次登录时间',
    is_active BOOLEAN DEFAULT TRUE COMMENT '是否激活',
    created_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL COMMENT '创建时间',
    updated_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP NOT NULL COMMENT '更新时间'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户表';

CREATE TABLE tbl_user_sessions (
    session_id VARCHAR(255) NOT NULL PRIMARY KEY COMMENT '会话ID',
    username VARCHAR(50) NOT NULL COMMENT '登录名称',
    ip_address VARCHAR(45) COMMENT '登录IP地址',
    user_agent TEXT,
    created_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL COMMENT '创建时间',
    expires_time TIMESTAMP COMMENT '失效时间',
    is_expired BOOLEAN DEFAULT false COMMENT '是否失效',
    KEY `idx_username` (`username`),
    FOREIGN KEY (username) REFERENCES tbl_users(username) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户会话表';

CREATE TABLE tbl_user_conversations (
    id INT AUTO_INCREMENT PRIMARY KEY COMMENT '主键',
    username VARCHAR(50) NOT NULL COMMENT '登录名称',
    title varchar(255) NOT NULL COMMENT '对话标题',
    content longtext COMMENT '对话内容',
    created_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL COMMENT '创建时间',
    updated_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP NOT NULL COMMENT '更新时间',
    KEY `idx_username` (`username`),
    FOREIGN KEY (username) REFERENCES tbl_users(username) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户对话表';

CREATE TABLE tbl_course_schedules (
    id int AUTO_INCREMENT PRIMARY KEY COMMENT '主键',
    username VARCHAR(50) NOT NULL COMMENT '登录名称',
    course varchar(100) NOT NULL COMMENT '课程',
    teacher varchar(100) NULL COMMENT '教师',
    classroom varchar(100) NULL COMMENT '教室',
    day_of_week ENUM('星期一', '星期二', '星期三', '星期四', '星期五', '星期六', '星期日') NOT NULL COMMENT '星期',
    time_slot ENUM('上午', '下午', '晚上') NOT NULL COMMENT '时间段',
    start_time varchar(5) NOT NULL COMMENT '开始时间',
    end_time varchar(5) NOT NULL COMMENT '结束时间',
    remark varchar(200) NULL COMMENT '备注',
    created_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL COMMENT '创建时间',
    updated_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP NOT NULL COMMENT '更新时间',
    KEY `idx_username` (`username`),
    FOREIGN KEY (username) REFERENCES tbl_users(username) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='预约安排表';

CREATE TABLE tbl_appointments (
    id int AUTO_INCREMENT PRIMARY KEY COMMENT '主键',
    username VARCHAR(50) NOT NULL COMMENT '登录名称',
    title varchar(255) NOT NULL COMMENT '预约标题',
    content text COMMENT '预约详情',
    start_time datetime NOT NULL COMMENT '开始时间',
    end_time datetime NULL COMMENT '结束时间',
    location varchar(255) DEFAULT NULL COMMENT '预约地点',
    attendees varchar(255) DEFAULT NULL COMMENT '参与者，多个用逗号分隔',
    created_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL COMMENT '创建时间',
    updated_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP NOT NULL COMMENT '更新时间',
    FULLTEXT KEY `ft_title_content` (`username`,`title`,`content`) COMMENT '全文搜索索引',
    FOREIGN KEY (username) REFERENCES tbl_users(username) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='预约安排表';

CREATE TABLE tbl_notes (
    id int AUTO_INCREMENT PRIMARY KEY COMMENT '主键',
    username VARCHAR(50) NOT NULL COMMENT '登录名称',
    course varchar(255) NOT NULL COMMENT '课程',
    title varchar(255) NOT NULL COMMENT '笔记标题',
    content longtext COMMENT '笔记内容(支持富文本/Markdown)',
    created_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL COMMENT '创建时间',
    updated_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP NOT NULL COMMENT '更新时间',
    FULLTEXT KEY `ft_title_content` (`username`,`title`,`content`) COMMENT '全文搜索索引',
    FOREIGN KEY (username) REFERENCES tbl_users(username) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='笔记表';

