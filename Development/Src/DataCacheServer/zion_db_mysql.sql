
-- 用户表
CREATE TABLE user_table (
  user_id int(11) NOT NULL AUTO_INCREMENT COMMENT '用户ID',
  device_name varchar(1000) DEFAULT '' COMMENT '设备标识名称',
  user_name varchar(1000) DEFAULT '' COMMENT '用户标识名称',
  state int(11) DEFAULT 0 COMMENT '用户状态 0:激活 1:冻结',
  freeze_duetime TIMESTAMP DEFAULT 0 COMMENT '封停到期时间',
  session_key varchar(1000) DEFAULT '' COMMENT 'session key',
  session_data varchar(1000) DEFAULT '' COMMENT 'session_data',
  PRIMARY KEY (user_id)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- 用户登录历史表
CREATE TABLE login_history_table (
  user_id int(11) NOT NULL COMMENT '用户ID',
  ip varchar(11) NOT NULL COMMENT '登录IP',
  dv_type varchar(100) NOT NULL COMMENT '设备名称',
  os_type varchar(100) NOT NULL COMMENT '系统名称',
  dv_id varchar(100) NOT NULL COMMENT '设备标识',
  session_key varchar(100) NOT NULL COMMENT 'session key',
  create_ts TIMESTAMP NOT NULL COMMENT '创建时间'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- 角色表
CREATE TABLE avatar_table (
  avatar_id int(11) NOT NULL AUTO_INCREMENT COMMENT '角色ID',
  user_id int(11) NOT NULL COMMENT '用户ID',
  server_id int(11) NOT NULL COMMENT '角色所在服务器ID',
  avatar_name varchar(100) NOT NULL COMMENT '角色名称',
  avatar_desc varchar(100000) NOT NULL COMMENT '角色基本信息',
  PRIMARY KEY (avatar_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE UNIQUE INDEX avatar_table_name_index ON avatar_table(server_id, avatar_name);

-- 角色对象表
CREATE TABLE avatar_object_table (
  avatar_id int(11) NOT NULL COMMENT '角色ID',
  object_uuid varchar(100) NOT NULL COMMENT '对象UUID;,
  object_type varchar(100) NOT NULL COMMENT '对象类型', 
  object_data varchar(100000) NOT NULL COMMENT '对象数据',
  PRIMARY KEY (avatar_id, object_uuid)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
