#define MAX_PLAYER_QUERY_LEN 1200

#define PLAYER_FIELD_TYPE_NONE      (0)
#define PLAYER_FIELD_TYPE_INT       (1)
#define PLAYER_FIELD_TYPE_FLOAT     (2)
#define PLAYER_FIELD_TYPE_VARCHAR   (3)

#define FIELD_INT(%0,%1,%2,%3)     %0,
#define FIELD_FLOAT(%0,%1,%2,%3)   %0,
#define FIELD_STRING(%0,%1,%2,%3)  %0,
enum
{
    #include <op-framework/op-player/player-data>
    MAX_PLAYER_FIELDS
}
#undef FIELD_INT
#undef FIELD_FLOAT
#undef FIELD_STRING


#define FIELD_INT(%0,%1,%2,%3)     _%0,
#define FIELD_FLOAT(%0,%1,%2,%3)   Float:_%0,
#define FIELD_STRING(%0,%1,%2,%3)  _%0[%2],
enum E_PLAYER_DATA
{
    #include <op-framework/op-player/player-data>
}
#undef FIELD_INT
#undef FIELD_FLOAT
#undef FIELD_STRING

static gs_PlayerData[MAX_PLAYERS][E_PLAYER_DATA];

#define FIELD_INT(%0,%1,%2,%3)     %3,
#define FIELD_FLOAT(%0,%1,%2,%3)   %3,
#define FIELD_STRING(%0,%1,%2,%3)  %3,
static const gsc_PlayerDefaultData[E_PLAYER_DATA] = 
{
    #include <op-framework/op-player/player-data>
};
#undef FIELD_INT
#undef FIELD_FLOAT
#undef FIELD_STRING


enum E_PLAYER_FIELD_DATA
{
    PLAYER_FIELD_NAME[24],
    PLAYER_FIELD_STRING[64],
    PLAYER_FIELD_TYPE,
    PLAYER_FIELD_SIZE,
    PLAYER_FIELD_OFFSET
}

#define FIELD_INT(%0,%1,%2,%3)     {%1, %2, PLAYER_FIELD_TYPE_INT,     1,   _: _%0},
#define FIELD_FLOAT(%0,%1,%2,%3)   {%1, %2, PLAYER_FIELD_TYPE_FLOAT,   1,   _: _%0},
#define FIELD_STRING(%0,%1,%2,%3)  {%1, %2, PLAYER_FIELD_TYPE_VARCHAR, %2,  _: _%0},
static const gsc_PlayerFieldData[MAX_PLAYER_FIELDS + 1][E_PLAYER_FIELD_DATA] = 
{
    #include <op-framework/op-player/player-data>
    {"null", "null", PLAYER_FIELD_TYPE_NONE, 0, 0}
};
#undef FIELD_INT
#undef FIELD_FLOAT
#undef FIELD_STRING


#define _BITSET_CELL_SHIFT    (5)
#define _BITSET_CELL_SIZE     (1 << _BITSET_CELL_SHIFT)
#define _BITSET_CELL_MASK     (_BITSET_CELL_SIZE - 1)

static gs_PlayerDataDirty[MAX_PLAYERS][(MAX_PLAYER_FIELDS + _BITSET_CELL_MASK) >>> _BITSET_CELL_SHIFT];
static bool:gs_PlayerDataLoaded[MAX_PLAYERS] = {false, ...};
static bool:gs_PlayerDataSaving[MAX_PLAYERS] = {false, ...};

static stock _OPCore_Player_Default(playerid)
{
    gs_PlayerData[playerid] = gsc_PlayerDefaultData;
    return 1;
}

stock bool:OPCore_Player_IsValid(playerid)
{
    if(!IsPlayerConnected(playerid)) return false;
    if(IsPlayerNPC(playerid)) return false;
    if(!OPCore_Player_IsLoggedIn(playerid)) return false;
    return gs_PlayerDataLoaded[playerid];
}

static stock bool:_OPCore_Player_Save(playerid)
{
    if(!OPCore_Player_IsLoggedIn(playerid) || gs_PlayerDataLoaded[playerid] == false || gs_PlayerDataSaving[playerid] == true)
        return false;

    gs_PlayerDataSaving[playerid] = true;

    new 
        query[1600],
        sub_query[1024],
        tmp_field[256],
        bool:first = true,
        bool:hasDirty = false,
        cell, 
        bit, 
        offset;

    sub_query[0] = '\0';

    for(new i = 0; i < MAX_PLAYER_FIELDS; i++)
    {
        cell = i >>> _BITSET_CELL_SHIFT;
        bit  = (1 << (i & _BITSET_CELL_MASK));

        if(!(gs_PlayerDataDirty[playerid][cell] & bit))
            continue;

        offset = gsc_PlayerFieldData[i][PLAYER_FIELD_OFFSET];

        switch(gsc_PlayerFieldData[i][PLAYER_FIELD_TYPE])
        {
            case PLAYER_FIELD_TYPE_INT:
                format(tmp_field, sizeof(tmp_field), "`%s`='%d'", gsc_PlayerFieldData[i][PLAYER_FIELD_NAME], gs_PlayerData[playerid][E_PLAYER_DATA:offset]);

            case PLAYER_FIELD_TYPE_FLOAT:
                format(tmp_field, sizeof(tmp_field), "`%s`='%f'", gsc_PlayerFieldData[i][PLAYER_FIELD_NAME], gs_PlayerData[playerid][E_PLAYER_DATA:offset]);

            case PLAYER_FIELD_TYPE_VARCHAR:
                mysql_format(OPCore_GetMySQLHandle(), tmp_field, sizeof(tmp_field), "`%s`='%e'", gsc_PlayerFieldData[i][PLAYER_FIELD_NAME], gs_PlayerData[playerid][E_PLAYER_DATA:offset]);
            
            default: continue;
        }
        if(!first && (strlen(sub_query) + strlen(tmp_field) + 100 > MAX_PLAYER_QUERY_LEN))
        {
            mysql_format(OPCore_GetMySQLHandle(), query, sizeof(query), "UPDATE `players` SET %s WHERE `player_uid` = '%d' LIMIT 1", sub_query, OPCore_Player_GetDBID(playerid));
            mysql_tquery(OPCore_GetMySQLHandle(), query, "_OPCore_Player_OnSave", "iii", playerid, OPCore_GetPlayerMySQLRace(playerid), true);

            sub_query[0] = '\0';
            first = true;
        }
        if(!first) strcat(sub_query, ",");
        strcat(sub_query, tmp_field);
        first = false;
        hasDirty = true;

        gs_PlayerDataDirty[playerid][cell] &= ~bit;
    }
    if(hasDirty && sub_query[0] != '\0')
    {
        mysql_format(OPCore_GetMySQLHandle(), query, sizeof(query), "UPDATE `players` SET %s WHERE `player_uid` = '%d' LIMIT 1", sub_query, OPCore_Player_GetDBID(playerid));
        mysql_tquery(OPCore_GetMySQLHandle(), query, "_OPCore_Player_OnSave", "iii", playerid, OPCore_GetPlayerMySQLRace(playerid), false);
    }
    else
    {
        gs_PlayerDataSaving[playerid]= false;
    }
    return true;
}

forward _OPCore_Player_OnSave(playerid, race_check, bool:saving);
public _OPCore_Player_OnSave(playerid, race_check, bool:saving)
{
    if(OPCore_MySQLRaceCheck(playerid, race_check))
    {
        if(cache_affected_rows() <= 0) 
        {
            new name[MAX_PLAYER_NAME + 1];
            GetPlayerName(playerid, name, MAX_PLAYER_NAME + 1);
            OPCore_Error(Player, "玩家数据保存出现异常 [playerid: %d | name: %s].", playerid, name);
        }
        gs_PlayerDataSaving[playerid] = saving;
    }
    return 1;
}

forward _OPCore_Player_Create(playerid, race_check);
public _OPCore_Player_Create(playerid, race_check)
{
    if(OPCore_MySQLRaceCheck(playerid, race_check))
    {
        new insertID = cache_insert_id();
        if(insertID > 0) {
            _OPCore_Player_Default(playerid);
            gs_PlayerDataLoaded[playerid] = true;
            CallLocalFunction("OnPlayerSignIn", "ii", playerid, SIGNIN_DATA_LOADED_FINISH);
        }
        else {
            SendClientMessage(playerid, 0xE74C3CFF, "错误: 出现了罕见的错误, 游戏数据加载失败, 请重新进入游戏.");
            Kick(playerid);
        }
    }
    return 1;
}

forward _OPCore_Player_Load(playerid, race_check);
public _OPCore_Player_Load(playerid, race_check)
{
    if(OPCore_MySQLRaceCheck(playerid, race_check))
    {
        if(cache_num_rows())
        {
            new offset;
            for(new i = 0; i < MAX_PLAYER_FIELDS; i++)
            {
                offset = gsc_PlayerFieldData[i][PLAYER_FIELD_OFFSET];
                switch(gsc_PlayerFieldData[i][PLAYER_FIELD_TYPE])
                {
                    case PLAYER_FIELD_TYPE_INT: 
                    {
                        cache_get_value_name_int(0, gsc_PlayerFieldData[i][PLAYER_FIELD_NAME], gs_PlayerData[playerid][E_PLAYER_DATA:offset]);
                    }
                    case PLAYER_FIELD_TYPE_FLOAT: 
                    {
                        cache_get_value_name_float(0, gsc_PlayerFieldData[i][PLAYER_FIELD_NAME], Float:gs_PlayerData[playerid][E_PLAYER_DATA:offset]);
                    }
                    case PLAYER_FIELD_TYPE_VARCHAR: 
                    {
                        cache_get_value_name(0, gsc_PlayerFieldData[i][PLAYER_FIELD_NAME], gs_PlayerData[playerid][E_PLAYER_DATA:offset], gsc_PlayerFieldData[i][PLAYER_FIELD_SIZE]);
                    }
                }
            }
            gs_PlayerDataLoaded[playerid] = true;
            CallLocalFunction("OnPlayerSignIn", "ii", playerid, SIGNIN_DATA_LOADED_FINISH);
        }
        else {
            new query[128];
            mysql_format(OPCore_GetMySQLHandle(), query, sizeof(query), "INSERT INTO `players` (`player_uid`) VALUES ('%d')", OPCore_Player_GetDBID(playerid));
            mysql_tquery(OPCore_GetMySQLHandle(), query, "_OPCore_Player_Create", "ii", playerid, OPCore_GetPlayerMySQLRace(playerid));
        }
    }
    return 1;
}

public OnPlayerSignIn(playerid, result)
{
    if(OPCore_Player_IsLoggedIn(playerid) && (result == SIGNIN_LOGIN_SUCCESS || result == SIGNIN_REGISTER_SUCCESS))
    {
        new query[64];
        mysql_format(OPCore_GetMySQLHandle(), query, sizeof(query), "SELECT * FROM `players` WHERE `player_uid` = '%d' LIMIT 1", OPCore_Player_GetDBID(playerid));
        mysql_tquery(OPCore_GetMySQLHandle(), query, "_OPCore_Player_Load", "ii", playerid, OPCore_GetPlayerMySQLRace(playerid));
    }
    #if defined opc_player_OnPlayerSignIn
        return opc_player_OnPlayerSignIn(playerid, result);
    #else
        return 1;
    #endif
}

public OnGameModeInit()
{
    mysql_query(OPCore_GetMySQLHandle(), "CREATE TABLE IF NOT EXISTS `players` (`player_uid` mediumint(8) NOT NULL AUTO_INCREMENT, PRIMARY KEY (`player_uid`)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4", false);
    new Cache:result = mysql_query(OPCore_GetMySQLHandle(), "SHOW COLUMNS FROM `players`", true);
    new rows = cache_num_rows();
    new bool:exists[MAX_PLAYER_FIELDS] = {false, ...};
    new field[32], query[256];

    for(new i = 0; i < rows; i++) {
        cache_get_value_name(i, "Field", field);
        for(new j = 0; j < MAX_PLAYER_FIELDS; j++) {
            if(!strcmp(field, gsc_PlayerFieldData[j][PLAYER_FIELD_NAME])) {
                exists[j] = true;
                break;
            }
        }
    }
    for(new j = 0; j < MAX_PLAYER_FIELDS; j++) {
        if(!exists[j]) {
            mysql_format(OPCore_GetMySQLHandle(), query, sizeof(query), "ALTER TABLE `players` ADD `%s` %s", gsc_PlayerFieldData[j][PLAYER_FIELD_NAME], gsc_PlayerFieldData[j][PLAYER_FIELD_STRING]);
            mysql_query(OPCore_GetMySQLHandle(), query, false);
        }
    }
    cache_delete(result);

    #if defined opc_player_OnGameModeInit
        return opc_player_OnGameModeInit();
    #else
        return 1;
    #endif
}

public OnPlayerConnect(playerid)
{
    _OPCore_Player_Default(playerid);

    gs_PlayerDataLoaded[playerid] = false;
    gs_PlayerDataSaving[playerid] = false;

    #if defined opc_player_OnPlayerConnect
        return opc_player_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

public OnPlayerDisconnect(playerid, reason)
{
    _OPCore_Player_Save(playerid);
    _OPCore_Player_Default(playerid);

    gs_PlayerDataLoaded[playerid] = false;

    #if defined opc_player_OnPlayerDisconnect
        return opc_player_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}

/**
 *                                       
 *           db         88888888ba   88  
 *          d88b        88      "8b  88  
 *         d8'`8b       88      ,8P  88  
 *        d8'  `8b      88aaaaaa8P'  88  
 *       d8YaaaaY8b     88""""""'    88  
 *      d8""""""""8b    88           88  
 *     d8'        `8b   88           88  
 *    d8'          `8b  88           88  
 *                                       
 *                                       
 */

stock Float:OPCore_Player_GetDataFloat(playerid, field)
{
    if(!OPCore_Player_IsLoggedIn(playerid) || gs_PlayerDataLoaded[playerid] == false) return 0.0;

    new offset = gsc_PlayerFieldData[field][PLAYER_FIELD_OFFSET];

    return Float:gs_PlayerData[playerid][E_PLAYER_DATA:offset];
}

stock OPCore_Player_GetDataInt(playerid, field)
{
    if(!OPCore_Player_IsLoggedIn(playerid) || gs_PlayerDataLoaded[playerid] == false) return 0;
    
    new offset = gsc_PlayerFieldData[field][PLAYER_FIELD_OFFSET];

    return gs_PlayerData[playerid][E_PLAYER_DATA:offset];
}

stock bool:OPCore_Player_GetDataString(playerid, field, string[], size = sizeof(string))
{
    if(!OPCore_Player_IsLoggedIn(playerid) || gs_PlayerDataLoaded[playerid] == false) return false;

    new offset = gsc_PlayerFieldData[field][PLAYER_FIELD_OFFSET];

    string[0] = '\0';
    strcat(string, gs_PlayerData[playerid][E_PLAYER_DATA:offset], size);
    return true;
}

stock bool:OPCore_Player_SetDataFloat(playerid, field, Float:value)
{
    if(!OPCore_Player_IsLoggedIn(playerid) || gs_PlayerDataLoaded[playerid] == false) return false;

    new offset = gsc_PlayerFieldData[field][PLAYER_FIELD_OFFSET];

    gs_PlayerData[playerid][E_PLAYER_DATA:offset] = _:value;

    gs_PlayerDataDirty[playerid][field >>> _BITSET_CELL_SHIFT] |= (1 << (field & _BITSET_CELL_MASK));
    return true;
}

stock bool:OPCore_Player_SetDataInt(playerid, field, value)
{
    if(!OPCore_Player_IsLoggedIn(playerid) || gs_PlayerDataLoaded[playerid] == false) return false;

    new offset = gsc_PlayerFieldData[field][PLAYER_FIELD_OFFSET];

    gs_PlayerData[playerid][E_PLAYER_DATA:offset] = value;

    gs_PlayerDataDirty[playerid][field >>> _BITSET_CELL_SHIFT] |= (1 << (field & _BITSET_CELL_MASK));
    return true;
}

stock bool:OPCore_Player_SetDataString(playerid, field, const value[])
{
    if(!OPCore_Player_IsLoggedIn(playerid) || gs_PlayerDataLoaded[playerid] == false) return false;
    
    new offset = gsc_PlayerFieldData[field][PLAYER_FIELD_OFFSET];

    format(gs_PlayerData[playerid][E_PLAYER_DATA:offset], gsc_PlayerFieldData[field][PLAYER_FIELD_SIZE], value);

    gs_PlayerDataDirty[playerid][field >>> _BITSET_CELL_SHIFT] |= (1 << (field & _BITSET_CELL_MASK));
    return true;
}

stock bool:OPCore_Player_GiveDataFloat(playerid, field, Float:value)
{
    if(!OPCore_Player_IsLoggedIn(playerid) || gs_PlayerDataLoaded[playerid] == false) return false;

    new offset = gsc_PlayerFieldData[field][PLAYER_FIELD_OFFSET];

    new Float:currentVal = Float:gs_PlayerData[playerid][E_PLAYER_DATA:offset];
    gs_PlayerData[playerid][E_PLAYER_DATA:offset] = _:(currentVal + value);

    gs_PlayerDataDirty[playerid][field >>> _BITSET_CELL_SHIFT] |= (1 << (field & _BITSET_CELL_MASK));
    return true;
}

stock bool:OPCore_Player_GiveDataInt(playerid, field, value)
{
    if(!OPCore_Player_IsLoggedIn(playerid) || gs_PlayerDataLoaded[playerid] == false) return false;

    new offset = gsc_PlayerFieldData[field][PLAYER_FIELD_OFFSET];

    gs_PlayerData[playerid][E_PLAYER_DATA:offset] += value;

    gs_PlayerDataDirty[playerid][field >>> _BITSET_CELL_SHIFT] |= (1 << (field & _BITSET_CELL_MASK));
    return true;
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit opc_player_OnGameModeInit
#if defined opc_player_OnGameModeInit
    forward opc_player_OnGameModeInit();
#endif

#if defined _ALS_OnPlayerSignIn
    #undef OnPlayerSignIn
#else
    #define _ALS_OnPlayerSignIn
#endif
#define OnPlayerSignIn opc_player_OnPlayerSignIn
#if defined opc_player_OnPlayerSignIn
    forward opc_player_OnPlayerSignIn(playerid, result);
#endif

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect opc_player_OnPlayerConnect
#if defined opc_player_OnPlayerConnect
    forward opc_player_OnPlayerConnect(playerid);
#endif

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect opc_player_OnPlayerDisconnect
#if defined opc_player_OnPlayerDisconnect
    forward opc_player_OnPlayerDisconnect(playerid, reason);
#endif