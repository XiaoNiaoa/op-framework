
#if !defined CHAIN_ORDER
	#define CHAIN_ORDER() 0
#endif

#define CHAIN_HOOK(%0) forward @CO_%0();public @CO_%0(){return CHAIN_ORDER()+1;}
#define CHAIN_NEXT(%0) @CO_%0

static stock _OPC_IncludeStates() <_ALS : _ALS_x0, _ALS : _ALS_x1, _ALS : _ALS_x2, _ALS : _ALS_x3> {}
static stock _OPC_IncludeStates() <_ALS : _ALS_go> {}

#define CHAIN_FORWARD:%0_%2(%1)=%3; \
	forward %0_%2(%1); \
	public %0_%2(%1) <_ALS : _ALS_x0, _ALS : _ALS_x1> { return (%3); } \
	public %0_%2(%1) <> { return (%3); }

#define CHAIN_PUBLIC:%0(%1) %0(%1) <_ALS : _ALS_go>

CHAIN_HOOK(OPC)
#undef CHAIN_ORDER
#define CHAIN_ORDER CHAIN_NEXT(OPC)

static MySQL:gs_MySQLHandle;

static gs_MySQLPlayerRace[MAX_PLAYERS];

stock MySQL:OPCore_GetMySQLHandle() 
{
    return gs_MySQLHandle;
}

stock OPCore_GetPlayerMySQLRace(playerid) 
{
    return gs_MySQLPlayerRace[playerid];
}

stock bool:OPCore_MySQLRaceCheck(playerid, race)
{
    if(race != gs_MySQLPlayerRace[playerid]) 
    {
        SendClientMessage(playerid, 0xE74C3CFF, "错误: 出现了罕见的错误, 请重新进入游戏.");
        Kick(playerid);
        return false;
    }
    return true;
}

public OnQueryError(errorid, const error[], const callback[], const query[], MySQL:handle)
{
    switch(errorid) 
    {
        case ER_DBACCESS_DENIED_ERROR:  OPCore_Error(MySQL, "数据库访问被拒绝 => \"%s\"", query);
        case ER_UNKNOWN_TABLE:          OPCore_Error(MySQL, "未知表 => \"%s\"", query);
        case CR_SERVER_GONE_ERROR:      OPCore_Error(MySQL, "MySQL服务器已关闭 => \"%s\"", query);
        case CR_SERVER_LOST:            OPCore_Error(MySQL, "MySQL服务器连接丢失 => \"%s\"", query);
        case CR_COMMAND_OUT_OF_SYNC:    OPCore_Error(MySQL, "命令不同步 => \"%s\"", query);
        case CR_SERVER_LOST_EXTENDED:   OPCore_Error(MySQL, "MySQL服务器连接丢失扩展 => \"%s\"", query);
        case 1136:                      OPCore_Error(MySQL, "列数与值数不符 => \"%s\"", query);
        case 1054:                      OPCore_Error(MySQL, "无效字段名 => \"%s\"", query);
        case 1065:                      OPCore_Error(MySQL, "查询为空 (变量尺寸太小?) => \"%s\" from \"%s\"", query, callback);
        case 1058:                      OPCore_Error(MySQL, "列数与值数不匹配 => \"%s\" from \"%s\"", query, callback);
        case 1203:                      OPCore_Error(MySQL, "用户已拥有超过 'max_user_connections' 的活动连接数 => \"%s\" from \"%s\"", query, callback);
        case 1045:                      OPCore_Error(MySQL, "拒绝访问");
        case 1149:                      OPCore_Error(MySQL, "SQL语句语法错误 \"%s\"",query);
        default :                       OPCore_Error(MySQL, "%d | \"%s\" (回调: \"%s\" 查询: \"%s\")", errorid, error, callback, query);
    }
    return 1;
}

public OnGameModeInit()
{
	state _ALS : _ALS_go;

	gs_MySQLHandle = mysql_connect_file("mysql.ini");

    mysql_set_charset("gbk", gs_MySQLHandle);

    if(gs_MySQLHandle == MYSQL_INVALID_HANDLE || mysql_errno(gs_MySQLHandle) != 0) 
    {
        OPCore_Error(MySQL, "数据库连接失败, 服务器关闭...");
        SendRconCommand("exit");
        return 0;
    }
    else OPCore_Debug(MySQL, "数据库连接成功.");

    return OPC_OnGameModeInit();
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit(%0) CHAIN_PUBLIC:OPC_OnGameModeInit(%0)
CHAIN_FORWARD:OPC_OnGameModeInit() = 1;


public OnPlayerConnect(playerid)
{
    gs_MySQLPlayerRace[playerid]++;

	return OPC_OnPlayerConnect(playerid);
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect(%0) CHAIN_PUBLIC:OPC_OnPlayerConnect(%0)
CHAIN_FORWARD:OPC_OnPlayerConnect(playerid) = 1;

public OnPlayerDisconnect(playerid, reason)
{
    OPC_OnPlayerDisconnect(playerid, reason);

    gs_MySQLPlayerRace[playerid]++;
    
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect(%0) CHAIN_PUBLIC:OPC_OnPlayerDisconnect(%0)
CHAIN_FORWARD:OPC_OnPlayerDisconnect(playerid, reason) = 1;

public OnGameModeExit()
{
	OnMySQLClose();

    // mysql_close 函数将始终等待队列中所有查询执行完毕，从而导致服务器停止运行
    mysql_close(gs_MySQLHandle);

	return OPC_OnGameModeExit();
}
#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit(%0) CHAIN_PUBLIC:OPC_OnGameModeExit(%0)
CHAIN_FORWARD:OPC_OnGameModeExit() = 1;

forward OnMySQLClose();
public OnMySQLClose()
{
    #if defined opc_mysql_OnMySQLClose
        return opc_mysql_OnMySQLClose();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnMySQLClose
    #undef OnMySQLClose
#else
    #define _ALS_OnMySQLClose
#endif
#define OnMySQLClose opc_mysql_OnMySQLClose
#if defined opc_mysql_OnMySQLClose
    forward opc_mysql_OnMySQLClose();
#endif