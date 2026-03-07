    // 注意: 字段至少保留两项
    
    // INT 类型
    FIELD_INT(PLAYER_DATA_KILL,     "player_kills",    "int(11) NOT NULL DEFAULT '0'",      0)
    FIELD_INT(PLAYER_DATA_DEATH,    "player_deaths",   "int(11) NOT NULL DEFAULT '0'",      0)

    // FLOAT 类型
    // 比如: FIELD_FLOAT(PLAYER_DATA_POS_X,   "player_pos_x",   "float NOT NULL DEFAULT '0'",   0.0)

    // STRING 类型
    // 比如: FIELD_STRING(PLAYER_DATA_VAR,  "player_var",  32, " ")