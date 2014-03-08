
-- 角色表
CREATE TABLE avatar_table (
  avatar_id integer AUTOINCERMENT,	-- COMMENT '角色ID',
  user_id integer,					-- COMMENT '用户ID',
  server_id integer,				-- COMMENT '角色所在服务器ID',
  avatar_name text,					-- COMMENT '角色名称',
  avatar_desc text					-- COMMENT '角色基本信息'
);
CREATE UNIQUE INDEX avatar_table_id_index ON avatar_table(avatar_id);
CREATE UNIQUE INDEX avatar_table_name_index ON avatar_table(avatar_name);
CREATE INDEX avatar_table_user_index ON avatar_table(user_id);
CREATE INDEX avatar_table_server_index ON avatar_table(server_id);

-- 角色对象表
CREATE TABLE avatar_object_table (
  avatar_id integer,				-- COMMENT '角色ID',
  object_uuid text,					-- COMMENT '对象UUID;,
  object_type text,					-- COMMENT '对象类型', 
  object_data text					-- COMMENT '对象数据'
);
CREATE UNIQUE INDEX avatar_object_table_index ON avatar_object_table(avatar_id, object_uuid);
