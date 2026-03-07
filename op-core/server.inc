new OPCORE_SERVER_NAME[64]; 
new OPCORE_SERVER_URL[64];
new OPCORE_SERVER_MODE[32];
new OPCORE_SERVER_MAP[32];
new OPCORE_SERVER_LANGUAGE[32];
new OPCORE_SERVER_RCON[24];
new OPCORE_SERVER_COLOR[12];

static bool:gs_FirstInit = true;

stock OPCore_PrintServerInfo()
{
    print(" ");
    OPCore_Debug(Server, "| OPCore Server Info");
    OPCore_Debug(Server, "|-----------------------------------------");
    OPCore_Debug(Server, "| Name.......: %s", OPCORE_SERVER_NAME);
    OPCore_Debug(Server, "| URL........: %s", OPCORE_SERVER_URL);
    OPCore_Debug(Server, "| Mode.......: %s", OPCORE_SERVER_MODE);
    OPCore_Debug(Server, "| Map........: %s", OPCORE_SERVER_MAP);
    OPCore_Debug(Server, "| Language...: %s", OPCORE_SERVER_LANGUAGE);
    OPCore_Debug(Server, "|-----------------------------------------");
    return 1;
}

forward OPCore_ServerInfoUpdate();
public OPCore_ServerInfoUpdate()
{
    new Cache:result = mysql_query(OPCore_GetMySQLHandle(), "SELECT * FROM `serverinfo` LIMIT 1", true);

    if(!cache_is_valid(result))
    {
        OPCore_Debug(Server, "服务器信息查询失败 (errno: %d)", mysql_errno(OPCore_GetMySQLHandle()));
        return 0;
    }

    if(cache_num_rows() > 0)
    {
        cache_get_value_name(0, "server_name", OPCORE_SERVER_NAME, 64);
        cache_get_value_name(0, "server_url", OPCORE_SERVER_URL, 64);
        cache_get_value_name(0, "server_mode", OPCORE_SERVER_MODE, 32);
        cache_get_value_name(0, "server_map", OPCORE_SERVER_MAP, 32);
        cache_get_value_name(0, "server_language", OPCORE_SERVER_LANGUAGE, 32);
        cache_get_value_name(0, "server_rcon", OPCORE_SERVER_RCON, 24);
        cache_get_value_name(0, "server_color", OPCORE_SERVER_COLOR, 12);
    }
    else
    {
        if(cache_is_valid(result))
            cache_delete(result);
            
        new Cache:insertResult = mysql_query(OPCore_GetMySQLHandle(), "INSERT INTO `serverinfo` (`server_name`) VALUES ('open.mp server');", true);

        new rows = cache_affected_rows();

        if(cache_is_valid(insertResult))
            cache_delete(insertResult);
        
        if(rows > 0) 
        {
            print(" ");
            strcat(OPCORE_SERVER_NAME, "open.mp server", 64);
            strcat(OPCORE_SERVER_URL, "www.open.mp", 64);
            strcat(OPCORE_SERVER_MODE, "v1.0.0", 32);
            strcat(OPCORE_SERVER_MAP, "San Andreas", 32);
            strcat(OPCORE_SERVER_LANGUAGE, "english", 32);
            strcat(OPCORE_SERVER_RCON, "rconpassword", 24);
            strcat(OPCORE_SERVER_COLOR, "{298ce8}", 12);
        }
        else
        {
            OPCore_Error(Server, "插入默认配置失败，强制关闭服务器...");
            SendRconCommand("exit");
            return 0;
        }
    }

    if(gs_FirstInit == true)
    {
        OPCore_PrintServerInfo();
        gs_FirstInit = false;
    }

    new cmd[64];
    format(cmd, sizeof(cmd), "name %s", OPCORE_SERVER_NAME);
    SendRconCommand(cmd);
    format(cmd, sizeof(cmd), "website %s", OPCORE_SERVER_URL);
    SendRconCommand(cmd);
    format(cmd, sizeof(cmd), "game.mode %s", OPCORE_SERVER_MODE);
    SendRconCommand(cmd);
    format(cmd, sizeof(cmd), "game.map %s", OPCORE_SERVER_MAP);
    SendRconCommand(cmd);
    format(cmd, sizeof(cmd), "language %s", OPCORE_SERVER_LANGUAGE);
    SendRconCommand(cmd);
    format(cmd, sizeof(cmd), "rcon.password %s", OPCORE_SERVER_RCON);
    SendRconCommand(cmd);

    if(cache_is_valid(result))
        cache_delete(result);
    return 1;
}

public OnGameModeInit() 
{
    mysql_query(OPCore_GetMySQLHandle(), "CREATE TABLE IF NOT EXISTS `serverinfo` (\
        `server_name` varchar(64) NOT NULL DEFAULT 'open.mp server',\
        `server_url` varchar(64) NOT NULL DEFAULT 'www.open.mp',\
        `server_mode` varchar(32) NOT NULL DEFAULT 'v1.0.0',\
        `server_map` varchar(32) NOT NULL DEFAULT 'San Andreas',\
        `server_language` varchar(32) NOT NULL DEFAULT 'english',\
        `server_rcon` varchar(24) NOT NULL DEFAULT 'rconpassword',\
        `server_color` varchar(12) NOT NULL DEFAULT '{298ce8}'\
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;", false
    );
    
    OPCore_ServerInfoUpdate();

    SetTimer("OPCore_ServerInfoUpdate", 60 * 1000, true);

    new password[9];
    password[0] = 'A' + random(26);
    password[1] = 'A' + random(26);
    password[2] = 'A' + random(26);
    password[3] = 'A' + random(26);
    password[4] = 'A' + random(26);
    password[5] = 'A' + random(26);
    password[6] = 'A' + random(26);
    password[7] = 'A' + random(26);
    password[8] = '\0';
    
    SendRconCommand("password %s", password);
    
    SetTimer("_OPCore_RemoveServerPassword", 2*1000, false);

    #if defined opc_server_OnGameModeInit
        return opc_server_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit opc_server_OnGameModeInit
#if defined opc_server_OnGameModeInit
    forward opc_server_OnGameModeInit();
#endif

forward _OPCore_RemoveServerPassword();
public _OPCore_RemoveServerPassword()
{
    SendRconCommand("password 0");
    return 1;
}