#define MAX_RACE_CHECKPOINTS           (100) // 一个赛道最多拥有的检查点数量
#define MAX_RACES                       (50)  // 最多同时存在的比赛
#define RACE_LEAVE_VEHICLE_TIME_LIMIT   (15)  // 离开载具的最长时间限制
#define INVALID_RACE_ID                 (-1)  // 无效的比赛会话ID 

static enum E_PLAYER_RACE_DATA 
{
    PLAYER_RACE_ID,                     // -1 表示没有参与任何赛事
    PLAYER_RACE_FINISH_CP,              // 当前已完成的检查点数量
    PLAYER_RACE_FINISH_LAP             // 当前已完成的圈数
}
static gs_PlayerRaceData[MAX_PLAYERS][E_PLAYER_RACE_DATA];

static enum E_RACE_RACE_TRACK_CP_DATA 
{
    CP_TYPE:RACE_TRACK_CP_TYPE,
    Float:RACE_TRACK_CP_SIZE,
    Float:RACE_TRACK_CP_POS_X,
    Float:RACE_TRACK_CP_POS_Y,
    Float:RACE_TRACK_CP_POS_Z
}
static gs_RaceCheckPointPData[MAX_RACES][MAX_RACE_CHECKPOINTS][E_RACE_RACE_TRACK_CP_DATA];

static enum RACE_STATE
{
    RACE_STATE_NONE,        // 空闲/未使用
    RACE_STATE_CREATING,    // 玩家创建中 正在赛事设置
    RACE_STATE_LOBBY,       // 准备阶段
    RACE_STATE_COUNTDOWN,   // 进入倒计时阶段
    RACE_STATE_START        // 开始阶段
}

static enum E_RACE_DATA 
{
    RACE_TRACK_NAME[64],                    // 比赛赛道名称
    RACE_HOST,                              // 主机玩家ID
    RACE_STATE:RACE_CUR_STATE,              // 赛事当前阶段
    RACE_TIMER_ID,                          // 赛事全局计时器
    RACE_START_TICK,                        // 用于记录开始的tick
    RACE_VEHICLE_TYPE,                      // 0: 不限制  400以下则限制具体类型的载具(如摩托车 船只 飞机 直升机 跑车 低底盘车等等) 400 以上则限制具体载具Model
    RACE_PLAYER_CUR,                        // 比赛实时玩家数量
    RACE_PLAYER_FINISH,                     // 当前比赛已完成的玩家数量
    RACE_PLAYER_MAX,                        // 赛事最多支持多少名玩家
    RACE_LAPS,                              // 赛道一共多少圈
    bool:RACE_GHOST_MODE,                   // 是否开启幽灵模式 (载具无碰撞)
    RACE_BGM_URL[128],                      // 可设置比赛的背景音乐
    RACE_TOTAL_CP,
    RACE_COUNTDOWN
}
static gs_RaceData[MAX_RACES][E_RACE_DATA];
static gs_RaceHideHud[MAX_RACES] = {true, ...}; // 是否隐藏HUD 可用于非竞赛环境下的点到点游戏 如任务 巡逻 去除竞赛性质

static stock _OPCore_Race_ResetPlayerData(playerid)
{
    gs_PlayerRaceData[playerid][PLAYER_RACE_ID] = INVALID_RACE_ID;
    gs_PlayerRaceData[playerid][PLAYER_RACE_FINISH_CP] = 
    gs_PlayerRaceData[playerid][PLAYER_RACE_FINISH_LAP] = 0;
    return;
}

static stock _OPCore_Race_ResetData(raceid)
{
    if(raceid < 0 || raceid >= MAX_RACES) return;

    gs_RaceData[raceid][RACE_TRACK_NAME][0] = '\0';
    gs_RaceData[raceid][RACE_HOST] = INVALID_PLAYER_ID;
    gs_RaceData[raceid][RACE_CUR_STATE] = RACE_STATE_NONE;
    if(gs_RaceData[raceid][RACE_TIMER_ID] != INVALID_TIMER)
    {
        KillTimer(gs_RaceData[raceid][RACE_TIMER_ID]);
        gs_RaceData[raceid][RACE_TIMER_ID] = INVALID_TIMER;
    }
    gs_RaceData[raceid][RACE_START_TICK] = 
    gs_RaceData[raceid][RACE_VEHICLE_TYPE] = 
    gs_RaceData[raceid][RACE_PLAYER_CUR] = 
    gs_RaceData[raceid][RACE_PLAYER_FINISH] = 0;
    gs_RaceData[raceid][RACE_PLAYER_MAX] = 10;
    gs_RaceData[raceid][RACE_LAPS] = 1;
    gs_RaceData[raceid][RACE_GHOST_MODE] = false;
    gs_RaceData[raceid][RACE_BGM_URL][0] = '\0';
    strcat(gs_RaceData[raceid][RACE_BGM_URL], "无", 128);
    gs_RaceData[raceid][RACE_TOTAL_CP] = 0;
    gs_RaceData[raceid][RACE_COUNTDOWN] = 5;

    gs_RaceHideHud[raceid] = true;

    for(new j = 0; j < MAX_RACE_CHECKPOINTS; j++)
    {
        gs_RaceCheckPointPData[raceid][j][RACE_TRACK_CP_TYPE] = UNKNOWN_CP_TYPE;
        gs_RaceCheckPointPData[raceid][j][RACE_TRACK_CP_SIZE] = 
        gs_RaceCheckPointPData[raceid][j][RACE_TRACK_CP_POS_X] = 
        gs_RaceCheckPointPData[raceid][j][RACE_TRACK_CP_POS_Y] = 
        gs_RaceCheckPointPData[raceid][j][RACE_TRACK_CP_POS_Z] = 0.0;
    }
    return;
}

stock OPCore_Race_GetPlayerRace(playerid)
{
    if(0 <= playerid < MAX_PLAYERS) return gs_PlayerRaceData[playerid][PLAYER_RACE_ID];
    return INVALID_RACE_ID;
}

stock OPCore_Race_Quit(playerid)
{
    if(0 <= playerid < MAX_PLAYERS && 0 <= gs_PlayerRaceData[playerid][PLAYER_RACE_ID] < MAX_RACES)
    {
        new raceid = gs_PlayerRaceData[playerid][PLAYER_RACE_ID];
        gs_RaceData[raceid][RACE_PLAYER_CUR] -= 1;
        if(gs_RaceData[raceid][RACE_PLAYER_CUR] <= 0)  _OPCore_Race_ResetData(raceid);

        _OPCore_Race_ResetPlayerData(playerid);
    }
    return;
}

stock bool:OPCore_Race_IsValid(raceid)
{
    if(0 <= raceid < MAX_RACES && gs_RaceData[raceid][RACE_CUR_STATE] != RACE_STATE_NONE)
        return true;

    return false;
}

#define _OPCore_Race_IsValid(%1) (0 <= %1 < MAX_RACES && gs_RaceData[%1][RACE_CUR_STATE] != RACE_STATE_NONE)

stock OPCore_Race_Create()
{
    new raceid = INVALID_RACE_ID;
    for(new i = 0; i < MAX_RACES; i++)
    {   
        if(_OPCore_Race_IsValid(i) == false) {
            raceid = i;
            break;
        }
    }
    if(raceid != INVALID_RACE_ID) {
        _OPCore_Race_ResetData(raceid);
        gs_RaceData[raceid][RACE_CUR_STATE] = RACE_STATE_CREATING;
    }
    return raceid;
}

stock bool:OPCore_Race_Join(playerid, raceid)
{
    if(_OPCore_Race_IsValid(raceid) == true && IsPlayerConnected(playerid) && gs_PlayerRaceData[playerid][PLAYER_RACE_ID] != raceid)
    {
        OPCore_Race_Quit(playerid);
        gs_RaceData[raceid][RACE_PLAYER_CUR] += 1;
        gs_PlayerRaceData[playerid][PLAYER_RACE_ID] = raceid;
        return true;
    }
    return false;
}