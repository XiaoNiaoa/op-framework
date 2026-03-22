#if !defined MIN_PASSWORD_LENGTH
    #define MIN_PASSWORD_LENGTH            (4)
#endif

#if !defined MAX_PASSWORD_LENGTH
    #define MAX_PASSWORD_LENGTH            (16)
#endif

#define _INVALID_ACCOUNT_DATABASE_ID    (0)

enum
{
    _SIGNIN_LOGIN_SUCCESS = 0,
    _SIGNIN_REGISTER_SUCCESS,
    SIGNIN_LOGIN_SUCCESS,
    SIGNIN_REGISTER_SUCCESS,
    SIGNIN_INCORRECT_PASSWORD,
    SIGNIN_DATABASE_ERROR,
    SIGNIN_ALREADY_LOGGED_IN,
    SIGNIN_LOADING,
    SIGNIN_UNREGISTERED,
    SIGNIN_ALREADY_REGISTERED
};

// 显式优于隐式
enum SIGNIN_MODE{
    SIGNIN_MODE_LOGIN,
    SIGNIN_MODE_REGISTER
};

static 
    gs_AccountDBID[MAX_PLAYERS] = {_INVALID_ACCOUNT_DATABASE_ID, ...},
    gs_AccountName[MAX_PLAYERS][MAX_PLAYER_NAME + 1],
    gs_AccountHash[MAX_PLAYERS][BCRYPT_HASH_LENGTH],
    gs_AccountRegisterDate[MAX_PLAYERS],
    gs_AccountLoginDate[MAX_PLAYERS],
    gs_AccountLeaveDate[MAX_PLAYERS];


new bool:gs_AccountLogged[MAX_PLAYERS] = {false, ...};
static bool:gs_AccountExist[MAX_PLAYERS] = {false, ...};

new bool:gs_AccountLoading[MAX_PLAYERS] = {true, ...};

stock bool:OPCore_Player_IsRegister(playerid)
{
    if(!IsPlayerConnected(playerid)) return false;
    if(IsPlayerNPC(playerid)) return false;
    return gs_AccountExist[playerid];
}

stock bool:OPCore_Player_IsLoading(playerid)
{
    if(!IsPlayerConnected(playerid)) return true;
    if(IsPlayerNPC(playerid)) return true;
    return gs_AccountLoading[playerid];
}

stock OPCore_Player_GetDBID(playerid)
{
    if(!IsPlayerConnected(playerid)) return _INVALID_ACCOUNT_DATABASE_ID;
    if(IsPlayerNPC(playerid)) return _INVALID_ACCOUNT_DATABASE_ID;
    if(gs_AccountLogged[playerid] == false || gs_AccountDBID[playerid] == _INVALID_ACCOUNT_DATABASE_ID) return _INVALID_ACCOUNT_DATABASE_ID;
    return gs_AccountDBID[playerid];
}

stock OPCore_Player_GetRegisterDate(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;
    if(IsPlayerNPC(playerid)) return 0;
    if(gs_AccountLogged[playerid] == false || gs_AccountDBID[playerid] == _INVALID_ACCOUNT_DATABASE_ID) return 0;
    return gs_AccountRegisterDate[playerid];
}

stock OPCore_Player_GetLoginDate(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;
    if(IsPlayerNPC(playerid)) return 0;
    if(gs_AccountLogged[playerid] == false || gs_AccountDBID[playerid] == _INVALID_ACCOUNT_DATABASE_ID) return 0;
    return gs_AccountLoginDate[playerid];
}

stock OPCore_Player_GetLeaveDate(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;
    if(IsPlayerNPC(playerid)) return 0;
    if(gs_AccountLogged[playerid] == false || gs_AccountDBID[playerid] == _INVALID_ACCOUNT_DATABASE_ID) return 0;
    return gs_AccountLeaveDate[playerid];
}

static stock bool:_IsValidPassword(const password[])
{
    new len = strlen(password);
    if(len < MIN_PASSWORD_LENGTH || len > MAX_PASSWORD_LENGTH)
        return false;

    for(new i = 0; password[i] != EOS; ++i) {
        switch(password[i]) {
            case '0'..'9', 'A'..'Z', 'a'..'z', '!', '@', '#', '$', '^', '&', '*', '(', ')': continue; // , '%'
            default: return false;
        }
    }
    return true;
}

stock void:OPCore_Player_SignIn(playerid, const password[], SIGNIN_MODE:mode)
{
    if(!IsPlayerNPC(playerid) && IsPlayerConnected(playerid))
    {
        if(gs_AccountLoading[playerid] == true)
        {
            OnPlayerSignIn(playerid, SIGNIN_LOADING);
            return;
        }
        if(gs_AccountLogged[playerid] == false)
        {
            if(!_IsValidPassword(password))
            {
                OnPlayerSignIn(playerid, SIGNIN_INCORRECT_PASSWORD);
                return;
            }
            if(mode == SIGNIN_MODE_LOGIN)
            {
                if(!gs_AccountExist[playerid]) {
                    OnPlayerSignIn(playerid, SIGNIN_UNREGISTERED);
                    return;
                }
                gs_AccountLoading[playerid] = true;
                bcrypt_verify(playerid, "_OPCore_Player_HashCheck", password, gs_AccountHash[playerid], "i", OPCore_GetPlayerMySQLRace(playerid));
            }
            else if(mode == SIGNIN_MODE_REGISTER)
            {
                if(gs_AccountExist[playerid]) {
                    OnPlayerSignIn(playerid, SIGNIN_ALREADY_REGISTERED);
                    return;
                }
                gs_AccountLoading[playerid] = true;
                bcrypt_hash(playerid, "_OPCore_Player_HashPassword", password, BCRYPT_COST, "ii", playerid, OPCore_GetPlayerMySQLRace(playerid));
            }
            return;
        }
        OnPlayerSignIn(playerid, SIGNIN_ALREADY_LOGGED_IN);
    }
    return;
}

forward _OPCore_Player_CheckExists(playerid, raceCheck);
public _OPCore_Player_CheckExists(playerid, raceCheck)
{
    if(OPCore_MySQLRaceCheck(playerid, raceCheck)) 
    {
        gs_AccountLoading[playerid] = false;
        if(cache_num_rows()) 
        {
            cache_get_value_name_int(0, "account_uid", gs_AccountDBID[playerid]);
            cache_get_value_name(0, "account_hash", gs_AccountHash[playerid], BCRYPT_HASH_LENGTH);
            
            gs_AccountExist[playerid] = true;
            return;
        }
        else
        {
            gs_AccountExist[playerid] = false;
            gs_AccountDBID[playerid] = _INVALID_ACCOUNT_DATABASE_ID;
        }
    }
    return;
}

forward _OPCore_Player_HashPassword(playerid, hashid, raceCheck);
public _OPCore_Player_HashPassword(playerid, hashid, raceCheck) 
{
    if(OPCore_MySQLRaceCheck(playerid, raceCheck))
    {
        bcrypt_get_hash(gs_AccountHash[playerid]);

        new curTime = gs_AccountRegisterDate[playerid] = gs_AccountLoginDate[playerid] = gettime();

        new query[512];
        mysql_format(OPCore_GetMySQLHandle(), query, sizeof(query), 
            "INSERT INTO `account` (`account_name`, `account_hash`, `account_registeron`, `account_logindate`, `account_gameid`) VALUES ('%e', '%e', '%d', '%d', '%d')", 
            gs_AccountName[playerid], 
            gs_AccountHash[playerid],
            curTime,
            curTime,
            playerid
        );
        mysql_tquery(OPCore_GetMySQLHandle(), query, "_OPCore_Player_OnRegisterSuccess", "ii", playerid, OPCore_GetPlayerMySQLRace(playerid));  
    }
    return 1;
}

forward _OPCore_Player_HashCheck(playerid, bool:success, raceCheck);
public _OPCore_Player_HashCheck(playerid, bool:success, raceCheck)
{
    if(OPCore_MySQLRaceCheck(playerid, raceCheck))
    {
        if(success == true)
        {
            new query[128];
            mysql_format(OPCore_GetMySQLHandle(), query, sizeof(query), "SELECT * FROM `account` WHERE `account_uid` = '%d' LIMIT 1", gs_AccountDBID[playerid]);
            mysql_tquery(OPCore_GetMySQLHandle(), query, "_OPCore_Player_OnLoginSuccess", "ii", playerid, OPCore_GetPlayerMySQLRace(playerid));
        }
        else
        {
            gs_AccountLoading[playerid] = false;
            OnPlayerSignIn(playerid, SIGNIN_INCORRECT_PASSWORD);
        }
    }
    return 1;
}

forward _OPCore_Player_OnLoginSuccess(playerid, raceCheck);
public _OPCore_Player_OnLoginSuccess(playerid, raceCheck) 
{
    if(OPCore_MySQLRaceCheck(playerid, raceCheck))
    {
        if(cache_num_rows())
        {
            gs_AccountLogged[playerid] = true;

            cache_get_value_name_int(0, "account_registeron", gs_AccountRegisterDate[playerid]);
            cache_get_value_name_int(0, "account_leavedate", gs_AccountLeaveDate[playerid]);

            gs_AccountLoginDate[playerid] = gettime();

            new query[128];
            mysql_format(OPCore_GetMySQLHandle(), query, sizeof(query), 
                "UPDATE `account` SET `account_gameid` = '%d', `account_logindate` = '%d' WHERE `account_uid` = '%d' LIMIT 1", 
                playerid, 
                gs_AccountLoginDate[playerid], 
                gs_AccountDBID[playerid]
            );
            mysql_tquery(OPCore_GetMySQLHandle(), query);

            OnPlayerSignIn(playerid, _SIGNIN_LOGIN_SUCCESS);
        }
        else
        {
            OnPlayerSignIn(playerid, SIGNIN_DATABASE_ERROR);
        }
        gs_AccountLoading[playerid] = false;
    }
    return 1;
}

forward _OPCore_Player_OnRegisterSuccess(playerid, raceCheck);
public _OPCore_Player_OnRegisterSuccess(playerid, raceCheck) 
{
    if(OPCore_MySQLRaceCheck(playerid, raceCheck))
    {
        new insertID = cache_insert_id();
        if(insertID > 0) {
            gs_AccountLogged[playerid] = true;
            gs_AccountDBID[playerid] = insertID;
            gs_AccountLeaveDate[playerid] = 0;
            OnPlayerSignIn(playerid, _SIGNIN_REGISTER_SUCCESS);
        }
        else
        {
            OnPlayerSignIn(playerid, SIGNIN_DATABASE_ERROR);
        }
        gs_AccountLoading[playerid] = false;
    }
    return 1;
}

public OnGameModeInit()
{
    mysql_query(OPCore_GetMySQLHandle(), "CREATE TABLE IF NOT EXISTS `account` (\
        `account_uid` mediumint(8) NOT NULL AUTO_INCREMENT,\
        `account_name` varchar(25) CHARACTER SET utf8 DEFAULT NULL,\
        `account_hash` varchar(65) DEFAULT NULL,\
        `account_registeron` int(8) NOT NULL DEFAULT '0',\
        `account_logindate` int(8) NOT NULL DEFAULT '0',\
        `account_leavedate` int(8) NOT NULL DEFAULT '0',\
        `account_gameid` smallint(6) unsigned DEFAULT '65535',\
        PRIMARY KEY (`account_uid`),\
        UNIQUE KEY `account_name` (`account_name`)) \
        ENGINE=InnoDB DEFAULT CHARSET=utf8;", false
    );

    new query[64];
    mysql_format(OPCore_GetMySQLHandle(), query, sizeof(query), "UPDATE `account` SET `account_gameid` = '%d'", INVALID_PLAYER_ID);
    mysql_query(OPCore_GetMySQLHandle(), query, false);

    #if defined opc_a_OnGameModeInit
        return opc_a_OnGameModeInit();
    #else
        return 1;
    #endif
}

public OnPlayerConnect(playerid)
{
    if(!IsPlayerNPC(playerid))
    {
        gs_AccountDBID[playerid] = _INVALID_ACCOUNT_DATABASE_ID;
        gs_AccountName[playerid][0] = '\0';
        gs_AccountHash[playerid][0] = '\0';
        gs_AccountRegisterDate[playerid] = 
        gs_AccountLoginDate[playerid] = 
        gs_AccountLeaveDate[playerid] = 0;

        gs_AccountLogged[playerid] = false;
        gs_AccountExist[playerid] = false;
        gs_AccountLoading[playerid] = true;

        GetPlayerName(playerid, gs_AccountName[playerid], MAX_PLAYER_NAME + 1);
        for(new i; gs_AccountName[playerid][i] != 0; i ++)
        {
            if(gs_AccountName[playerid][i] < 0)
                gs_AccountName[playerid][i] += 256;
        }
        new query[128];
        mysql_format(OPCore_GetMySQLHandle(), query, sizeof(query), "SELECT `account_uid`, `account_hash` FROM `account` WHERE `account_name` = '%e' LIMIT 1", gs_AccountName[playerid]);
        mysql_tquery(OPCore_GetMySQLHandle(), query, "_OPCore_Player_CheckExists", "ii", playerid, OPCore_GetPlayerMySQLRace(playerid));
    }

    #if defined opc_a_OnPlayerConnect
        return opc_a_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

public OnPlayerDisconnect(playerid, reason)
{
    if(!IsPlayerNPC(playerid))
    {
        if(gs_AccountLogged[playerid] == true && gs_AccountDBID[playerid] != _INVALID_ACCOUNT_DATABASE_ID)
        {
            new query[200];
            mysql_format(OPCore_GetMySQLHandle(), query, sizeof(query), 
                "UPDATE `account` SET `account_gameid` = '%d', `account_leavedate` = '%d' WHERE `account_uid` = '%d' LIMIT 1", 
                INVALID_PLAYER_ID, 
                gettime(), 
                gs_AccountDBID[playerid]
            );
            mysql_tquery(OPCore_GetMySQLHandle(), query);
        }
        gs_AccountLoading[playerid] = true;
        gs_AccountExist[playerid] = false;
        gs_AccountLogged[playerid] = false;
        gs_AccountDBID[playerid] = _INVALID_ACCOUNT_DATABASE_ID;
    }
    #if defined opc_a_OnPlayerDisconnect
        return opc_a_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit opc_a_OnGameModeInit
#if defined opc_a_OnGameModeInit
    forward opc_a_OnGameModeInit();
#endif

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect opc_a_OnPlayerConnect
#if defined opc_a_OnPlayerConnect
    forward opc_a_OnPlayerConnect(playerid);
#endif

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect opc_a_OnPlayerDisconnect
#if defined opc_a_OnPlayerDisconnect
    forward opc_a_OnPlayerDisconnect(playerid, reason);
#endif

forward OnPlayerSignIn(playerid, result);