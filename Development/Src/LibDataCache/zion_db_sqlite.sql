
CREATE TABLE user_table (
	user_id			INTEGER PRIMARY KEY AUTOINCREMENT,
	token			TEXT NOT NULL,
	state			INTEGER NOT NULL,
	freeze_duetime	TIMESTAMP DEFAULT 0 COMMENT '封停到期时间',
	session_key varchar(1000) DEFAULT '' COMMENT 'session key',
	session_data varchar(1000) DEFAULT '' COMMENT 'session_data',
	PRIMARY KEY (user_id)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE login_history_table (
	user_id int(11) NOT NULL COMMENT '用户ID',
	ip varchar(11) NOT NULL COMMENT '登录IP',
	dv_type varchar(100) NOT NULL COMMENT '设备名称',
	os_type varchar(100) NOT NULL COMMENT '系统名称',
	dv_id varchar(100) NOT NULL COMMENT '设备标识',
	session_key varchar(100) NOT NULL COMMENT 'session key',
	create_ts TIMESTAMP NOT NULL COMMENT '创建时间'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;



CREATE TABLE avatar_table (
	avatar_id	INTEGER PRIMARY KEY AUTOINCREMENT,
	user_id		INTEGER,
	server_id	INTEGER,
	flag		INTEGER,
	avatar_name	TEXT,
	avatar_desc	TEXT
);
CREATE UNIQUE INDEX avatar_table_name_index ON avatar_table(avatar_name);
CREATE INDEX avatar_table_user_index ON avatar_table(user_id, server_id);
CREATE TABLE avatar_object_table (
	avatar_id INTEGER,
	object_uuid TEXT,
	object_type TEXT,
	object_data TEXT
);
CREATE UNIQUE INDEX avatar_object_table_index ON avatar_object_table(avatar_id, object_uuid);
