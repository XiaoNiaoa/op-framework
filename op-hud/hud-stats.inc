#define _MAX_STATS_HUD_LINES 5

static 
    PlayerText:gs_HudStatsTitle[MAX_PLAYERS][_MAX_STATS_HUD_LINES] = {{INVALID_PLAYER_TEXT_DRAW, ...}, ...},
    PlayerText:gs_HudStatsVar[MAX_PLAYERS][_MAX_STATS_HUD_LINES] = {{INVALID_PLAYER_TEXT_DRAW, ...}, ...},
    PlayerText:gs_HudStatsBarBack[MAX_PLAYERS][_MAX_STATS_HUD_LINES] = {{INVALID_PLAYER_TEXT_DRAW, ...}, ...},
    PlayerText:gs_HudStatsBarFill[MAX_PLAYERS][_MAX_STATS_HUD_LINES] = {{INVALID_PLAYER_TEXT_DRAW, ...}, ...},
    PlayerText:gs_HudStatsBarMain[MAX_PLAYERS][_MAX_STATS_HUD_LINES] = {{INVALID_PLAYER_TEXT_DRAW, ...}, ...},

    bool:gs_HudStatsCreated[MAX_PLAYERS][_MAX_STATS_HUD_LINES] = {{false, ...}, ...},
    bool:gs_HudStatsUseBar[MAX_PLAYERS][_MAX_STATS_HUD_LINES] = {{false, ...}, ...};

stock OPCore_Hud_HideStats(playerid, line = -1) 
{
    // 负 1 就是隐藏全部
    if(line == -1)
    {
        for(new i; i < _MAX_STATS_HUD_LINES; i ++) 
        {
            if(gs_HudStatsTitle[playerid][i]  != INVALID_PLAYER_TEXT_DRAW)
            {
                PlayerTextDrawDestroy(playerid, gs_HudStatsTitle[playerid][i] );
                gs_HudStatsTitle[playerid][i]  = INVALID_PLAYER_TEXT_DRAW;
            }
            if(gs_HudStatsVar[playerid][i]    != INVALID_PLAYER_TEXT_DRAW)
            {
                PlayerTextDrawDestroy(playerid, gs_HudStatsVar[playerid][i]   );
                gs_HudStatsVar[playerid][i]    = INVALID_PLAYER_TEXT_DRAW;
            }
            if(gs_HudStatsBarBack[playerid][i] != INVALID_PLAYER_TEXT_DRAW)
            {
                PlayerTextDrawDestroy(playerid, gs_HudStatsBarBack[playerid][i]);
                gs_HudStatsBarBack[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
            }
            if(gs_HudStatsBarFill[playerid][i] != INVALID_PLAYER_TEXT_DRAW)
            {
                PlayerTextDrawDestroy(playerid, gs_HudStatsBarFill[playerid][i]);
                gs_HudStatsBarFill[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
            }
            if(gs_HudStatsBarMain[playerid][i] != INVALID_PLAYER_TEXT_DRAW)
            {
                PlayerTextDrawDestroy(playerid, gs_HudStatsBarMain[playerid][i]);
                gs_HudStatsBarMain[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
            }
            gs_HudStatsCreated[playerid][i] = false;
            gs_HudStatsUseBar[playerid][i] = false;
        }
        return 1;
    }
    if(gs_HudStatsTitle[playerid][line]  != INVALID_PLAYER_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, gs_HudStatsTitle[playerid][line] );
        gs_HudStatsTitle[playerid][line]  = INVALID_PLAYER_TEXT_DRAW;
    }
    if(gs_HudStatsVar[playerid][line]    != INVALID_PLAYER_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, gs_HudStatsVar[playerid][line]   );
        gs_HudStatsVar[playerid][line]    = INVALID_PLAYER_TEXT_DRAW;
    }
    if(gs_HudStatsBarBack[playerid][line] != INVALID_PLAYER_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, gs_HudStatsBarBack[playerid][line]);
        gs_HudStatsBarBack[playerid][line] = INVALID_PLAYER_TEXT_DRAW;
    }
    if(gs_HudStatsBarFill[playerid][line] != INVALID_PLAYER_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, gs_HudStatsBarFill[playerid][line]);
        gs_HudStatsBarFill[playerid][line] = INVALID_PLAYER_TEXT_DRAW;
    }
    if(gs_HudStatsBarMain[playerid][line] != INVALID_PLAYER_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, gs_HudStatsBarMain[playerid][line]);
        gs_HudStatsBarMain[playerid][line] = INVALID_PLAYER_TEXT_DRAW;
    }
    gs_HudStatsUseBar[playerid][line] = false;
	gs_HudStatsCreated[playerid][line] = false;
    return 1;
}

stock bool:OPCore_Hud_ShowStats(playerid, line, const title[], Float:value, colour = 0xACCBF1FF, bool:usebar = false, bool:usetime = false)
{
    if(!IsPlayerConnected(playerid)) return false;
    if(IsPlayerNPC(playerid)) return false;
    if(line < 0 || line >= _MAX_STATS_HUD_LINES) return false;

    if(usetime == true) 
    {
        usebar = false;
    }
    if(gs_HudStatsCreated[playerid][line] == false)
    {
        gs_HudStatsTitle[playerid][line] = CreatePlayerTextDraw(playerid, 530.0, 124.0 + (float(line) * 20.0), "_");
        PlayerTextDrawLetterSize(playerid, gs_HudStatsTitle[playerid][line], 0.4, 1.6);
        PlayerTextDrawAlignment(playerid, gs_HudStatsTitle[playerid][line], TEXT_DRAW_ALIGN_RIGHT);
        PlayerTextDrawColour(playerid, gs_HudStatsTitle[playerid][line], colour);
        PlayerTextDrawSetShadow(playerid, gs_HudStatsTitle[playerid][line], 0);
        PlayerTextDrawSetOutline(playerid, gs_HudStatsTitle[playerid][line], 2);
        PlayerTextDrawBackgroundColour(playerid, gs_HudStatsTitle[playerid][line], 255);
        PlayerTextDrawFont(playerid, gs_HudStatsTitle[playerid][line], TEXT_DRAW_FONT_2);

        if(usebar == false)
        {
            gs_HudStatsVar[playerid][line] = CreatePlayerTextDraw(playerid, 608.0, 124.0 + (float(line) * 20.0), "_");
            PlayerTextDrawLetterSize(playerid, gs_HudStatsVar[playerid][line], 0.4, 1.6);
            PlayerTextDrawAlignment(playerid, gs_HudStatsVar[playerid][line], TEXT_DRAW_ALIGN_RIGHT);
            PlayerTextDrawColour(playerid, gs_HudStatsVar[playerid][line], colour);
            PlayerTextDrawSetShadow(playerid, gs_HudStatsVar[playerid][line], 0);
            PlayerTextDrawSetOutline(playerid, gs_HudStatsVar[playerid][line], 2);
            PlayerTextDrawBackgroundColour(playerid, gs_HudStatsVar[playerid][line], 255);
            PlayerTextDrawFont(playerid, gs_HudStatsVar[playerid][line], TEXT_DRAW_FONT_2);

            gs_HudStatsUseBar[playerid][line] = false;
        }
        else
        {
            gs_HudStatsBarBack[playerid][line] = CreatePlayerTextDraw(playerid, 550.0, 130.0 + (float(line) * 20.0), "_");
            PlayerTextDrawTextSize		(playerid, gs_HudStatsBarBack[playerid][line], 550.0 + 60.0 - 4.0, 0.0);
            PlayerTextDrawLetterSize	(playerid, gs_HudStatsBarBack[playerid][line], 1.0, 0.45);
            PlayerTextDrawUseBox		(playerid, gs_HudStatsBarBack[playerid][line], true);

            gs_HudStatsBarFill[playerid][line] = CreatePlayerTextDraw(playerid, 552.0, 132.0 + (float(line) * 20.0), "_");
            PlayerTextDrawTextSize		(playerid, gs_HudStatsBarFill[playerid][line], 550.0 + 60.0 - 5.5, 0.0);
            PlayerTextDrawLetterSize	(playerid, gs_HudStatsBarFill[playerid][line], 1.0, 0.1);
            PlayerTextDrawUseBox		(playerid, gs_HudStatsBarFill[playerid][line], true);

            gs_HudStatsBarMain[playerid][line] = CreatePlayerTextDraw(playerid, 552.0, 132.0 + (float(line) * 20.0), "_");
            PlayerTextDrawLetterSize(playerid, gs_HudStatsBarMain[playerid][line], 1.0, 0.1);
            
            gs_HudStatsUseBar[playerid][line] = true;
        }
        gs_HudStatsCreated[playerid][line] = true;
    }
    if(gs_HudStatsCreated[playerid][line] == true)
    {
        // 更新标题
        PlayerTextDrawColour(playerid, gs_HudStatsTitle[playerid][line], colour);
        PlayerTextDrawSetString(playerid, gs_HudStatsTitle[playerid][line], title);
	    PlayerTextDrawShow(playerid, gs_HudStatsTitle[playerid][line]);

        // 更新进度条
        if(usebar == true)
        {
            if(gs_HudStatsUseBar[playerid][line] == false)
            {
                if(gs_HudStatsVar[playerid][line] != INVALID_PLAYER_TEXT_DRAW)
                {
                    PlayerTextDrawDestroy(playerid, gs_HudStatsVar[playerid][line]);
                    gs_HudStatsVar[playerid][line]  = INVALID_PLAYER_TEXT_DRAW;
                }
                if(gs_HudStatsBarBack[playerid][line] != INVALID_PLAYER_TEXT_DRAW)
                {
                    PlayerTextDrawDestroy(playerid, gs_HudStatsBarBack[playerid][line]);
                }
                if(gs_HudStatsBarFill[playerid][line] != INVALID_PLAYER_TEXT_DRAW)
                {
                    PlayerTextDrawDestroy(playerid, gs_HudStatsBarFill[playerid][line]);
                }
                if(gs_HudStatsBarMain[playerid][line] != INVALID_PLAYER_TEXT_DRAW)
                {
                    PlayerTextDrawDestroy(playerid, gs_HudStatsBarMain[playerid][line]);
                }
                // 124.0 + 6.0 + (float(line) * 20.0)
                gs_HudStatsBarBack[playerid][line] = CreatePlayerTextDraw(playerid, 550.0, 130.0 + (float(line) * 20.0), "_");
                PlayerTextDrawTextSize		(playerid, gs_HudStatsBarBack[playerid][line], 550.0 + 60.0 - 4.0, 0.0);
                PlayerTextDrawLetterSize	(playerid, gs_HudStatsBarBack[playerid][line], 1.0, 0.45);
                PlayerTextDrawUseBox		(playerid, gs_HudStatsBarBack[playerid][line], true);

                gs_HudStatsBarFill[playerid][line] = CreatePlayerTextDraw(playerid, 552.0, 132.0 + (float(line) * 20.0), "_");
                PlayerTextDrawTextSize		(playerid, gs_HudStatsBarFill[playerid][line], 550.0 + 60.0 - 5.5, 0.0);
                PlayerTextDrawLetterSize	(playerid, gs_HudStatsBarFill[playerid][line], 1.0, 0.1);
                PlayerTextDrawUseBox		(playerid, gs_HudStatsBarFill[playerid][line], true);

                gs_HudStatsBarMain[playerid][line] = CreatePlayerTextDraw(playerid, 552.0, 132.0 + (float(line) * 20.0), "_");
                PlayerTextDrawLetterSize(playerid, gs_HudStatsBarMain[playerid][line], 1.0, 0.1);

                gs_HudStatsUseBar[playerid][line] = true;
            }
            if(value < 0.0) value = 0.0;
            else if(value > 100.0) value = 100.0;

            PlayerTextDrawUseBox(playerid, gs_HudStatsBarMain[playerid][line], (value > 0.0)?(true):(false));
            // ((550.0 - 3.0) + (((((550.0 - 2.0) + 60.0) - 550.0) / 100.0) * value))
            PlayerTextDrawTextSize(playerid, gs_HudStatsBarMain[playerid][line], (547.0 + (0.58 * value)), 0.0);

            PlayerTextDrawBoxColour		(playerid, gs_HudStatsBarBack[playerid][line], 0x00000000 | (colour & 0x000000FF));
            PlayerTextDrawBoxColour		(playerid, gs_HudStatsBarFill[playerid][line], (colour & 0xFFFFFF00) | (0x66 & ((colour & 0x000000FF) / 2)));
            PlayerTextDrawBoxColour		(playerid, gs_HudStatsBarMain[playerid][line], colour);

            PlayerTextDrawShow(playerid, gs_HudStatsBarBack[playerid][line]);
            PlayerTextDrawShow(playerid, gs_HudStatsBarFill[playerid][line]);
            PlayerTextDrawShow(playerid, gs_HudStatsBarMain[playerid][line]);
        }
        // 或更新文本
        else
        {   
            if(gs_HudStatsUseBar[playerid][line] == true)
            {
                if(gs_HudStatsBarBack[playerid][line] != INVALID_PLAYER_TEXT_DRAW)
                {
                    PlayerTextDrawDestroy(playerid, gs_HudStatsBarBack[playerid][line]);
                    gs_HudStatsBarBack[playerid][line] = INVALID_PLAYER_TEXT_DRAW;
                }
                if(gs_HudStatsBarFill[playerid][line] != INVALID_PLAYER_TEXT_DRAW)
                {
                    PlayerTextDrawDestroy(playerid, gs_HudStatsBarFill[playerid][line]);
                    gs_HudStatsBarFill[playerid][line] = INVALID_PLAYER_TEXT_DRAW;
                }
                if(gs_HudStatsBarMain[playerid][line] != INVALID_PLAYER_TEXT_DRAW)
                {
                    PlayerTextDrawDestroy(playerid, gs_HudStatsBarMain[playerid][line]);
                    gs_HudStatsBarMain[playerid][line] = INVALID_PLAYER_TEXT_DRAW;
                }
                if(gs_HudStatsVar[playerid][line] != INVALID_PLAYER_TEXT_DRAW)
                {
                    PlayerTextDrawDestroy(playerid, gs_HudStatsVar[playerid][line]);
                }
                gs_HudStatsVar[playerid][line] = CreatePlayerTextDraw(playerid, 608.0, 124.0 + (float(line) * 20.0), "_");
                PlayerTextDrawLetterSize(playerid, gs_HudStatsVar[playerid][line], 0.4, 1.6);
                PlayerTextDrawAlignment(playerid, gs_HudStatsVar[playerid][line], TEXT_DRAW_ALIGN_RIGHT);
                PlayerTextDrawColour(playerid, gs_HudStatsVar[playerid][line], colour);
                PlayerTextDrawSetShadow(playerid, gs_HudStatsVar[playerid][line], 0);
                PlayerTextDrawSetOutline(playerid, gs_HudStatsVar[playerid][line], 2);
                PlayerTextDrawBackgroundColour(playerid, gs_HudStatsVar[playerid][line], 255);
                PlayerTextDrawFont(playerid, gs_HudStatsVar[playerid][line], TEXT_DRAW_FONT_2);
                PlayerTextDrawSetProportional(playerid, gs_HudStatsVar[playerid][line], true);

                gs_HudStatsUseBar[playerid][line] = false;
            }
            PlayerTextDrawColour(playerid, gs_HudStatsVar[playerid][line], colour);
            
            if(usetime) {
                new tvar = floatround(value);
                PlayerTextDrawSetString(playerid, gs_HudStatsVar[playerid][line], "%02d:%02d", tvar/60, tvar%60);
            }
            else
                PlayerTextDrawSetString(playerid, gs_HudStatsVar[playerid][line], "%.0f", value);

            PlayerTextDrawShow(playerid, gs_HudStatsVar[playerid][line]);
        }
    }
    return true;
}

public OnGameModeInit()
{
    for(new i; i < MAX_PLAYERS; i ++) {
        OPCore_Hud_HideStats(i);
    }
    #if defined opc_statistic_OnGameModeInit
        return opc_statistic_OnGameModeInit();
    #else
        return 1;
    #endif
}

public OnGameModeExit() 
{
	for(new i; i < MAX_PLAYERS; i ++) {
		OPCore_Hud_HideStats(i);
	}
    #if defined opc_statistic_OnGameModeExit
        return opc_statistic_OnGameModeExit();
    #else
        return 1;
    #endif
}

public OnPlayerConnect(playerid)
{
    OPCore_Hud_HideStats(playerid);
    
    #if defined opc_statistic_OnPlayerConnect
        return opc_statistic_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

public OnPlayerDisconnect(playerid, reason)
{
    OPCore_Hud_HideStats(playerid);

    #if defined opc_statistic_OnPlayerDisconnect
        return opc_statistic_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit opc_statistic_OnGameModeInit
#if defined opc_statistic_OnGameModeInit
    forward opc_statistic_OnGameModeInit();
#endif

#if defined _ALS_OnGameModeExit
    #undef OnGameModeExit
#else
    #define _ALS_OnGameModeExit
#endif
#define OnGameModeExit opc_statistic_OnGameModeExit
#if defined opc_statistic_OnGameModeExit
    forward opc_statistic_OnGameModeExit();
#endif

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect opc_statistic_OnPlayerConnect
#if defined opc_statistic_OnPlayerConnect
    forward opc_statistic_OnPlayerConnect(playerid);
#endif

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect opc_statistic_OnPlayerDisconnect
#if defined opc_statistic_OnPlayerDisconnect
    forward opc_statistic_OnPlayerDisconnect(playerid, reason);
#endif