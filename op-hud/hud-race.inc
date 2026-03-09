static enum
{
    HUD_RACE_INFO_RANK,
    HUD_RACE_INFO_PLAYERS,
    HUD_RACE_INFO_TIME,
    HUD_RACE_INFO_LAP,
    MAX_HUD_RACE_INFO
}

static PlayerText:gs_HudRaceInfo[MAX_PLAYERS][MAX_HUD_RACE_INFO] = {{INVALID_PLAYER_TEXT_DRAW, ...}, ...};
static Text:gs_HudRaceBackground[6] = {INVALID_TEXT_DRAW, ...};
static bool:gs_HudRaceCreated[MAX_PLAYERS] = {false, ...};
static bool:gs_HudRaceShowing[MAX_PLAYERS] = {false, ...};

stock OPCore_Hud_ShowRaceHud(playerid, rank, totalRacers, mins, seconds, curLap = 0, totalLaps = 1)
{
    if(gs_HudRaceCreated[playerid] == false)
    {
        for(new i; i < MAX_HUD_RACE_INFO; i ++)
        {
            if(gs_HudRaceInfo[playerid][i] != INVALID_PLAYER_TEXT_DRAW) {
                PlayerTextDrawDestroy(playerid, gs_HudRaceInfo[playerid][i]);
            }
        }
        gs_HudRaceInfo[playerid][HUD_RACE_INFO_RANK] = CreatePlayerTextDraw(playerid, 573.000, 323.000, "_");
        PlayerTextDrawLetterSize(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_RANK], 0.597, 4.297);
        PlayerTextDrawAlignment(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_RANK], TEXT_DRAW_ALIGN_CENTER);
        PlayerTextDrawColour(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_RANK], 0xACCBF1FF);
        PlayerTextDrawSetShadow(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_RANK], 0);
        PlayerTextDrawBackgroundColour(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_RANK], 150);
        PlayerTextDrawFont(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_RANK], TEXT_DRAW_FONT_1);

        gs_HudRaceInfo[playerid][HUD_RACE_INFO_PLAYERS] = CreatePlayerTextDraw(playerid, 588.000, 328.000, "_");
        PlayerTextDrawLetterSize(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_PLAYERS], 0.319, 1.599);
        PlayerTextDrawAlignment(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_PLAYERS], TEXT_DRAW_ALIGN_LEFT);
        PlayerTextDrawColour(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_PLAYERS], 0xACCBF1FF);
        PlayerTextDrawSetShadow(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_PLAYERS], 0);
        PlayerTextDrawBackgroundColour(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_PLAYERS], 150);
        PlayerTextDrawFont(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_PLAYERS], TEXT_DRAW_FONT_1);

        gs_HudRaceInfo[playerid][HUD_RACE_INFO_TIME] = CreatePlayerTextDraw(playerid, 582.000, 356.000, "_");
        PlayerTextDrawLetterSize(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_TIME], 0.429, 2.699);
        PlayerTextDrawTextSize(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_TIME], 0.000, 19.000);
        PlayerTextDrawAlignment(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_TIME], TEXT_DRAW_ALIGN_CENTER);
        PlayerTextDrawColour(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_TIME], 0xACCBF1FF);
        PlayerTextDrawSetShadow(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_TIME], 0);
        PlayerTextDrawBackgroundColour(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_TIME], 150);
        PlayerTextDrawFont(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_TIME], TEXT_DRAW_FONT_1);

        if(totalLaps > 1)
        {
            gs_HudRaceInfo[playerid][HUD_RACE_INFO_LAP] = CreatePlayerTextDraw(playerid, 559.000, 382.000, "_");
            PlayerTextDrawLetterSize(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_LAP], 0.319, 1.500);
            PlayerTextDrawTextSize(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_LAP], 0.000, 17.000);
            PlayerTextDrawAlignment(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_LAP], TEXT_DRAW_ALIGN_LEFT);
            PlayerTextDrawColour(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_LAP], 0xACCBF1FF);
            PlayerTextDrawSetShadow(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_LAP], 0);
            PlayerTextDrawBackgroundColour(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_LAP], 150);
            PlayerTextDrawFont(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_LAP], TEXT_DRAW_FONT_1);
        }
        gs_HudRaceCreated[playerid] = true;
    }
    // 设置文本会自动更新 减少几十行代码开销和RPC消耗
    // 具体查阅: https://github.com/openmultiplayer/open.mp/blob/master/Server/Components/TextDraws/textdraw.hpp
    if(gs_HudRaceShowing[playerid] == false)
    {
        for(new i; i < 3; i ++) TextDrawShowForPlayer(playerid, gs_HudRaceBackground[i]);

        PlayerTextDrawShow(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_RANK]);
        PlayerTextDrawShow(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_PLAYERS]);
        PlayerTextDrawShow(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_TIME]);

        if(totalLaps > 1)
        {
            for(new i = 3; i < 6; i ++) TextDrawShowForPlayer(playerid, gs_HudRaceBackground[i]);
            PlayerTextDrawShow(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_LAP]);
        }
        gs_HudRaceShowing[playerid] = true;
    }
    PlayerTextDrawSetString(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_RANK], "%d", rank);
    switch(rank)
    {
        case 1: PlayerTextDrawSetString(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_PLAYERS], "ST~n~/%d", totalRacers);
        case 2: PlayerTextDrawSetString(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_PLAYERS], "ND~n~/%d", totalRacers);
        case 3: PlayerTextDrawSetString(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_PLAYERS], "RD~n~/%d", totalRacers);
        default: PlayerTextDrawSetString(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_PLAYERS], "TH~n~/%d", totalRacers);
    }
    PlayerTextDrawSetString(playerid, gs_HudRaceInfo[playerid][HUD_RACE_INFO_TIME], "%02d:%02d", mins, seconds);
    if(totalLaps > 1) PlayerTextDrawSetString(playerid, gs_HudRaceInfo[playerid][3], "Lap ~n~%d/%d", curLap, totalLaps);
    return 1;
}

stock OPCore_Hud_HideRaceHud(playerid)
{
    if(gs_HudRaceCreated[playerid] == true)
    {
        for(new i; i < MAX_HUD_RACE_INFO; i ++)
        {
            if(gs_HudRaceInfo[playerid][i] != INVALID_PLAYER_TEXT_DRAW)
            {
                PlayerTextDrawHide(playerid, gs_HudRaceInfo[playerid][i]);
                PlayerTextDrawDestroy(playerid, gs_HudRaceInfo[playerid][i]);
                gs_HudRaceInfo[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
            }
        }
        for(new i; i < 6; i ++) 
        {
            TextDrawHideForPlayer(playerid, gs_HudRaceBackground[i]);
        }
        gs_HudRaceCreated[playerid] = false;
        gs_HudRaceShowing[playerid] = false;
    }
    return 1;
}

public OnGameModeInit()
{
    gs_HudRaceBackground[0] = TextDrawCreate(553.000, 324.000, "LD_BUM:blkdot");
    TextDrawTextSize(gs_HudRaceBackground[0], 58.000, 60.000);
    TextDrawAlignment(gs_HudRaceBackground[0], TEXT_DRAW_ALIGN_LEFT);
    TextDrawColour(gs_HudRaceBackground[0], 255);
    TextDrawSetShadow(gs_HudRaceBackground[0], 0);
    TextDrawSetOutline(gs_HudRaceBackground[0], 0);
    TextDrawBackgroundColour(gs_HudRaceBackground[0], 255);
    TextDrawFont(gs_HudRaceBackground[0], TEXT_DRAW_FONT_SPRITE_DRAW);
    TextDrawSetProportional(gs_HudRaceBackground[0], true);

    gs_HudRaceBackground[1] = TextDrawCreate(554.000, 325.000, "LD_BUM:blkdot");
    TextDrawTextSize(gs_HudRaceBackground[1], 56.000, 58.000);
    TextDrawAlignment(gs_HudRaceBackground[1], TEXT_DRAW_ALIGN_LEFT);
    TextDrawColour(gs_HudRaceBackground[1], 0xACCBF1FF);
    TextDrawSetShadow(gs_HudRaceBackground[1], 0);
    TextDrawSetOutline(gs_HudRaceBackground[1], 0);
    TextDrawBackgroundColour(gs_HudRaceBackground[1], 255);
    TextDrawFont(gs_HudRaceBackground[1], TEXT_DRAW_FONT_SPRITE_DRAW);
    TextDrawSetProportional(gs_HudRaceBackground[1], true);

    gs_HudRaceBackground[2] = TextDrawCreate(556.000, 327.000, "LD_BUM:blkdot");
    TextDrawTextSize(gs_HudRaceBackground[2], 52.000, 54.000);
    TextDrawAlignment(gs_HudRaceBackground[2], TEXT_DRAW_ALIGN_LEFT);
    TextDrawColour(gs_HudRaceBackground[2], 255);
    TextDrawSetShadow(gs_HudRaceBackground[2], 0);
    TextDrawSetOutline(gs_HudRaceBackground[2], 0);
    TextDrawBackgroundColour(gs_HudRaceBackground[2], 255);
    TextDrawFont(gs_HudRaceBackground[2], TEXT_DRAW_FONT_SPRITE_DRAW);
    TextDrawSetProportional(gs_HudRaceBackground[2], true);

    gs_HudRaceBackground[3] = TextDrawCreate(553.000, 383.90, "LD_BUM:blkdot");
    TextDrawTextSize(gs_HudRaceBackground[3], 58.000, 32.000);
    TextDrawAlignment(gs_HudRaceBackground[3], TEXT_DRAW_ALIGN_LEFT);
    TextDrawColour(gs_HudRaceBackground[3], 255);
    TextDrawSetShadow(gs_HudRaceBackground[3], 0);
    TextDrawSetOutline(gs_HudRaceBackground[3], 0);
    TextDrawBackgroundColour(gs_HudRaceBackground[3], 255);
    TextDrawFont(gs_HudRaceBackground[3], TEXT_DRAW_FONT_SPRITE_DRAW);
    TextDrawSetProportional(gs_HudRaceBackground[3], true);

    gs_HudRaceBackground[4] = TextDrawCreate(554.000, 383.90, "LD_BUM:blkdot");
    TextDrawTextSize(gs_HudRaceBackground[4], 56.000, 31.000);
    TextDrawAlignment(gs_HudRaceBackground[4], TEXT_DRAW_ALIGN_LEFT);
    TextDrawColour(gs_HudRaceBackground[4], 0xACCBF1FF);
    TextDrawSetShadow(gs_HudRaceBackground[4], 0);
    TextDrawSetOutline(gs_HudRaceBackground[4], 0);
    TextDrawBackgroundColour(gs_HudRaceBackground[4], 255);
    TextDrawFont(gs_HudRaceBackground[4], TEXT_DRAW_FONT_SPRITE_DRAW);
    TextDrawSetProportional(gs_HudRaceBackground[4], true);

    gs_HudRaceBackground[5] = TextDrawCreate(556.000, 383.90, "LD_BUM:blkdot");
    TextDrawTextSize(gs_HudRaceBackground[5], 52.000, 29.000);
    TextDrawAlignment(gs_HudRaceBackground[5], TEXT_DRAW_ALIGN_LEFT);
    TextDrawColour(gs_HudRaceBackground[5], 255);
    TextDrawSetShadow(gs_HudRaceBackground[5], 0);
    TextDrawSetOutline(gs_HudRaceBackground[5], 0);
    TextDrawBackgroundColour(gs_HudRaceBackground[5], 255);
    TextDrawFont(gs_HudRaceBackground[5], TEXT_DRAW_FONT_SPRITE_DRAW);
    TextDrawSetProportional(gs_HudRaceBackground[5], true);

    for(new i; i < MAX_PLAYERS; i ++) {
        OPCore_Hud_HideRaceHud(i);
    }

    #if defined opc_hud_race_OnGameModeInit
        return opc_hud_race_OnGameModeInit();
    #else
        return 1;
    #endif
}

public OnGameModeExit() 
{
    // 暂时不信任机制 人工清空
	for(new i; i < MAX_PLAYERS; i ++) {
		OPCore_Hud_HideRaceHud(i);
	}
    for(new i; i < 6; i ++) 
    {
        if(gs_HudRaceBackground[i] != INVALID_TEXT_DRAW)
        {
            TextDrawDestroy(gs_HudRaceBackground[i]);
            gs_HudRaceBackground[i] = INVALID_TEXT_DRAW;
        }
    }
    #if defined opc_hud_race_OnGameModeExit
        return opc_hud_race_OnGameModeExit();
    #else
        return 1;
    #endif
}

public OnPlayerConnect(playerid)
{
    OPCore_Hud_HideRaceHud(playerid);
    
    #if defined opc_hud_race_OnPlayerConnect
        return opc_hud_race_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

public OnPlayerDisconnect(playerid, reason)
{
    OPCore_Hud_HideRaceHud(playerid);

    #if defined opc_hud_race_OnPlayerDisconnect
        return opc_hud_race_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit opc_hud_race_OnGameModeInit
#if defined opc_hud_race_OnGameModeInit
    forward opc_hud_race_OnGameModeInit();
#endif

#if defined _ALS_OnGameModeExit
    #undef OnGameModeExit
#else
    #define _ALS_OnGameModeExit
#endif
#define OnGameModeExit opc_hud_race_OnGameModeExit
#if defined opc_hud_race_OnGameModeExit
    forward opc_hud_race_OnGameModeExit();
#endif

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect opc_hud_race_OnPlayerConnect
#if defined opc_hud_race_OnPlayerConnect
    forward opc_hud_race_OnPlayerConnect(playerid);
#endif

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect opc_hud_race_OnPlayerDisconnect
#if defined opc_hud_race_OnPlayerDisconnect
    forward opc_hud_race_OnPlayerDisconnect(playerid, reason);
#endif