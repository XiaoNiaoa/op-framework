static bool:gs_HudStatBarCreated[MAX_PLAYERS] = {false, ...};
static PlayerText:gs_HudStatBar[MAX_PLAYERS][6] = {{INVALID_PLAYER_TEXT_DRAW, ...}, ...};
static gs_HudStatBarTimer[MAX_PLAYERS] = {-1, ...};

#define _HUD_STAT_BAR_POS_X 26.0
#define _HUD_STAT_BAR_POS_Y 111.0
#define _HUD_STAT_BAR_BAR_WIDTH 60.5

static const gs_TDCharacterWidth[176] =
{
    0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 
    12, 13, 13, 28, 28, 28, 28, 8, 17, 17, 30, 28, 28, 12, 9, 21, 28, 14, 28, 28, 28, 28, 28, 28, 28, 28, 13, 13, 30, 30, 30, 30, 
    10, 25, 23, 21, 24, 22, 20, 24, 24, 17, 20, 22, 20, 30, 27, 27, 26, 26, 24, 23, 24, 31, 23, 31, 24, 23, 21, 28, 33, 33, 14, 28, 
    10, 11, 12, 9, 11, 10, 10, 12, 12, 7, 7, 13, 5, 18, 12, 10, 12, 11, 10, 12, 8, 13, 13, 18, 17, 13, 12, 30, 30, 37, 35, 
    37, 25, 25, 25, 25, 33, 21, 24, 24, 24, 24, 17, 17, 17, 17, 27, 27, 27, 27, 31, 31, 31, 31, 11, 11, 11, 11, 11, 20, 9, 10, 
    10, 10, 10, 7, 7, 7, 7, 10, 10, 10, 10, 13, 13, 13, 13, 27, 12, 30	
};

forward _OPCore_Hud_HideStatBar(playerid);
public _OPCore_Hud_HideStatBar(playerid)
{
    if(gs_HudStatBarTimer[playerid] != -1) 
    {
        KillTimer(gs_HudStatBarTimer[playerid]);
        gs_HudStatBarTimer[playerid] = -1;
    }
    if(gs_HudStatBarCreated[playerid] == true)
    {
        for(new i; i < 6; i ++) 
        {
            if(gs_HudStatBar[playerid][i] != INVALID_PLAYER_TEXT_DRAW) 
            {
                PlayerTextDrawHide(playerid, gs_HudStatBar[playerid][i]);
                PlayerTextDrawDestroy(playerid, gs_HudStatBar[playerid][i]);
                gs_HudStatBar[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
            }
        }
        gs_HudStatBarCreated[playerid] = false;
    }
    return 1;
}

static stock _OPCore_Hud_CreateStatBar(playerid)
{
    gs_HudStatBar[playerid][0] = CreatePlayerTextDraw(playerid, _HUD_STAT_BAR_POS_X - 4.0, _HUD_STAT_BAR_POS_Y, "_"); // 背景黑框
    PlayerTextDrawLetterSize(playerid, gs_HudStatBar[playerid][0], 0.6, 2.45);
    PlayerTextDrawSetShadow(playerid, gs_HudStatBar[playerid][0], 0);
    PlayerTextDrawAlignment(playerid, gs_HudStatBar[playerid][0], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, gs_HudStatBar[playerid][0], -1);
    PlayerTextDrawBackgroundColour(playerid, gs_HudStatBar[playerid][0], 255);
    PlayerTextDrawBoxColour(playerid, gs_HudStatBar[playerid][0], 0x00000095);
    PlayerTextDrawUseBox(playerid, gs_HudStatBar[playerid][0], true);

    // 进度条槽 全小写26个字母(x=255) 全大写26个字母(x=298) 总长度612/295
    // 字符串"SJmUSswlbpep9p"：(x = 161) 0字符: 26  
    gs_HudStatBar[playerid][1] = CreatePlayerTextDraw(playerid, _HUD_STAT_BAR_POS_X, _HUD_STAT_BAR_POS_Y + 6.0, "_");
    PlayerTextDrawLetterSize(playerid, gs_HudStatBar[playerid][1], 0.6, 1.0);
    PlayerTextDrawSetShadow(playerid, gs_HudStatBar[playerid][1], 0);
    PlayerTextDrawAlignment(playerid, gs_HudStatBar[playerid][1], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, gs_HudStatBar[playerid][1], -1);
    PlayerTextDrawBackgroundColour(playerid, gs_HudStatBar[playerid][1], 255);
    PlayerTextDrawBoxColour(playerid, gs_HudStatBar[playerid][1], 0x5F5F5DFF);
    PlayerTextDrawUseBox(playerid, gs_HudStatBar[playerid][1], true);

    gs_HudStatBar[playerid][2] = CreatePlayerTextDraw(playerid, 128.0, _HUD_STAT_BAR_POS_Y + 5.0, "_"); // + 或 -
    PlayerTextDrawLetterSize(playerid, gs_HudStatBar[playerid][2], 0.475, 1.9);
    PlayerTextDrawTextSize(playerid, gs_HudStatBar[playerid][2], 400.0, 17.0);
    PlayerTextDrawSetShadow(playerid, gs_HudStatBar[playerid][2], 0);
    PlayerTextDrawAlignment(playerid, gs_HudStatBar[playerid][2], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, gs_HudStatBar[playerid][2], -1);
    PlayerTextDrawBackgroundColour(playerid, gs_HudStatBar[playerid][2], 255);
    PlayerTextDrawBoxColour(playerid, gs_HudStatBar[playerid][2], 50);

    gs_HudStatBar[playerid][3] = CreatePlayerTextDraw(playerid, 23.0, _HUD_STAT_BAR_POS_Y + 2.0, "_"); // 标题
    PlayerTextDrawLetterSize(playerid, gs_HudStatBar[playerid][3], 0.5, 2.0);
    PlayerTextDrawTextSize(playerid, gs_HudStatBar[playerid][3], 400.0, 17.0);
    PlayerTextDrawSetShadow(playerid, gs_HudStatBar[playerid][3], 0);
    PlayerTextDrawAlignment(playerid, gs_HudStatBar[playerid][3], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, gs_HudStatBar[playerid][3], 0xC3C3C3FF);

    gs_HudStatBar[playerid][4] = CreatePlayerTextDraw(playerid, _HUD_STAT_BAR_POS_X, _HUD_STAT_BAR_POS_Y + 6.0, "_");
    PlayerTextDrawLetterSize(playerid, gs_HudStatBar[playerid][4], 0.6, 1.0);
    PlayerTextDrawSetShadow(playerid, gs_HudStatBar[playerid][4], 0);
    PlayerTextDrawAlignment(playerid, gs_HudStatBar[playerid][4], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawUseBox(playerid, gs_HudStatBar[playerid][4], true);

    gs_HudStatBar[playerid][5] = CreatePlayerTextDraw(playerid, _HUD_STAT_BAR_POS_X, _HUD_STAT_BAR_POS_Y + 6.0, "_");
    PlayerTextDrawLetterSize(playerid, gs_HudStatBar[playerid][5], 0.6, 1.0);
    PlayerTextDrawSetShadow(playerid, gs_HudStatBar[playerid][5], 0);
    PlayerTextDrawAlignment(playerid, gs_HudStatBar[playerid][5], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawUseBox(playerid, gs_HudStatBar[playerid][5], true);
}

stock bool:OPCore_Hud_ShowStatBar(playerid, const title[], oldValue, newValue, maxValue, time = 2500)
{
    if(!IsPlayerConnected(playerid)) return false;
    if(IsPlayerNPC(playerid)) return false;
    if(time <= 0) return true;
    if(oldValue <= 0 && newValue <= 0) return true;
    if(oldValue < 0 || newValue < 0) return false;
    if(maxValue <= 0) return false;

    if(gs_HudStatBarCreated[playerid] == false)
    {
        _OPCore_Hud_CreateStatBar(playerid);
        gs_HudStatBarCreated[playerid] = true;
    }

    if(gs_HudStatBarTimer[playerid] != -1) KillTimer(gs_HudStatBarTimer[playerid]);
    gs_HudStatBarTimer[playerid] = SetTimerEx("_OPCore_Hud_HideStatBar", time, false , "i", playerid);

    PlayerTextDrawSetString(playerid, gs_HudStatBar[playerid][3], title);

    new titleLen, len = strlen(title);
    for (new i = 0; i < len; i++) {
        if(title[i] >= 0 && title[i] < 176) titleLen += gs_TDCharacterWidth[title[i]];
    }
    new Float:barMinX = _HUD_STAT_BAR_POS_X + (float(titleLen) * 0.135646) + (float(len) * 7.0307) + 6.22;
    barMinX += 6.0;
    new Float:barMaxX = barMinX + _HUD_STAT_BAR_BAR_WIDTH;
    
    PlayerTextDrawSetPos(playerid, gs_HudStatBar[playerid][1], barMinX, _HUD_STAT_BAR_POS_Y + 6.0);
    PlayerTextDrawTextSize(playerid, gs_HudStatBar[playerid][1], barMaxX, 0.0);
    PlayerTextDrawTextSize(playerid, gs_HudStatBar[playerid][0], barMaxX + 15.0, 0.0);
    PlayerTextDrawSetPos(playerid, gs_HudStatBar[playerid][2], barMaxX + 5.0, _HUD_STAT_BAR_POS_Y + 5.0);
    PlayerTextDrawSetPos(playerid, gs_HudStatBar[playerid][4], barMinX, _HUD_STAT_BAR_POS_Y + 6.0);
    PlayerTextDrawSetPos(playerid, gs_HudStatBar[playerid][5], barMinX, _HUD_STAT_BAR_POS_Y + 6.0);

    if(newValue >= oldValue) {
        PlayerTextDrawTextSize(playerid, gs_HudStatBar[playerid][4], barMinX + ((float(newValue) / float(maxValue)) * _HUD_STAT_BAR_BAR_WIDTH), 0.0);
        PlayerTextDrawTextSize(playerid, gs_HudStatBar[playerid][5], barMinX + ((float(oldValue) / float(maxValue)) * _HUD_STAT_BAR_BAR_WIDTH), 0.0);
        PlayerTextDrawBoxColour(playerid, gs_HudStatBar[playerid][4], 0x36682CFF);
        PlayerTextDrawBoxColour(playerid, gs_HudStatBar[playerid][5], 0xD8D8D8FF);
        PlayerTextDrawSetString(playerid, gs_HudStatBar[playerid][2], "+");
    } else {
        PlayerTextDrawTextSize(playerid, gs_HudStatBar[playerid][4], barMinX + ((float(oldValue) / float(maxValue)) * _HUD_STAT_BAR_BAR_WIDTH), 0.0);
        PlayerTextDrawTextSize(playerid, gs_HudStatBar[playerid][5], barMinX + ((float(newValue) / float(maxValue)) * _HUD_STAT_BAR_BAR_WIDTH), 0.0);
        PlayerTextDrawBoxColour(playerid, gs_HudStatBar[playerid][4], 0xB53B3EFF);
        PlayerTextDrawBoxColour(playerid, gs_HudStatBar[playerid][5], 0xD8D8D8FF);
        PlayerTextDrawSetString(playerid, gs_HudStatBar[playerid][2], "-");
    }
    for(new i; i < 6; i ++) PlayerTextDrawShow(playerid, gs_HudStatBar[playerid][i]);
    return true;
}

public OnGameModeInit()
{
    for(new i; i < MAX_PLAYERS; i ++) 
    {
        _OPCore_Hud_HideStatBar(i);
    }
    #if defined opc_hud_statbar_OnGameModeInit
        return opc_hud_statbar_OnGameModeInit();
    #else
        return 1;
    #endif
}

public OnGameModeExit() 
{
	for(new i; i < MAX_PLAYERS; i ++) 
    {
		_OPCore_Hud_HideStatBar(i);
	}
    #if defined opc_hud_statbar_OnGameModeExit
        return opc_hud_statbar_OnGameModeExit();
    #else
        return 1;
    #endif
}

public OnPlayerConnect(playerid)
{
    _OPCore_Hud_HideStatBar(playerid);
    
    #if defined opc_hud_statbar_OnPlayerConnect
        return opc_hud_statbar_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

public OnPlayerDisconnect(playerid, reason)
{
    _OPCore_Hud_HideStatBar(playerid);

    #if defined opc_hud_statbar_OnPlayerDisconnect
        return opc_hud_statbar_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit opc_hud_statbar_OnGameModeInit
#if defined opc_hud_statbar_OnGameModeInit
    forward opc_hud_statbar_OnGameModeInit();
#endif

#if defined _ALS_OnGameModeExit
    #undef OnGameModeExit
#else
    #define _ALS_OnGameModeExit
#endif
#define OnGameModeExit opc_hud_statbar_OnGameModeExit
#if defined opc_hud_statbar_OnGameModeExit
    forward opc_hud_statbar_OnGameModeExit();
#endif

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect opc_hud_statbar_OnPlayerConnect
#if defined opc_hud_statbar_OnPlayerConnect
    forward opc_hud_statbar_OnPlayerConnect(playerid);
#endif

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect opc_hud_statbar_OnPlayerDisconnect
#if defined opc_hud_statbar_OnPlayerDisconnect
    forward opc_hud_statbar_OnPlayerDisconnect(playerid, reason);
#endif