
CREATE TABLE user_table (
	user_id			INTEGER PRIMARY KEY AUTOINCREMENT,
	token			TEXT NOT NULL,
	state			INTEGER NOT NULL,
	freeze_duetime	INTEGER NOT NULL,
);
CREATE TABLE login_history_table (
	user_id			INTEGER NOT NULL,
	ip				TEXT NOT NULL,
	dv_type			TEXT NOT NULL,
	os_type			TEXT NOT NULL,
	dv_id			TEXT NOT NULL,
	create_ts		INTEGER NOT NULL
);
CREATE UNIQUE INDEX user_table_token_index ON user_table(token);
CREATE INDEX login_history_table_user_id_index ON login_history_table(user_id);

CREATE TABLE avatar_table (
	avatar_id		INTEGER PRIMARY KEY AUTOINCREMENT,
	user_id			INTEGER,
	server_id		INTEGER,
	state			INTEGER DEFAULT 0,
	freeze_duetime	INTEGER DEFAULT 0,
	avatar_name		TEXT,
	avatar_desc		TEXT
);
CREATE TABLE avatar_object_table (
	avatar_id		INTEGER,
	object_uuid		TEXT,
	object_type		TEXT,
	object_data		TEXT
);
CREATE UNIQUE INDEX avatar_table_name_index ON avatar_table(avatar_name);
CREATE INDEX avatar_table_user_index ON avatar_table(user_id, server_id);
CREATE UNIQUE INDEX avatar_object_table_index ON avatar_object_table(avatar_id, object_uuid);
