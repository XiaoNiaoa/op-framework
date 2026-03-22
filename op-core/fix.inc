#define Kick(%0) SetTimerEx("_OPCore_FixKick", 40, false, "d", %0)
#define Ban(%0) SetTimerEx("_OPCore_FixBan", 50, false, "d", %0)

native _OPCore_KickTimer(playerid) = Kick;
native _OPCore_BanTimer(playerid) = Ban;

forward _OPCore_FixKick(playerid); 
public _OPCore_FixKick(playerid)
{
    _OPCore_KickTimer(playerid);
}

forward _OPCore_FixBan(playerid); 
public _OPCore_FixBan(playerid)
{
    _OPCore_BanTimer(playerid);
}

public OnGameModeInit()
{
    for(new i = 0; i <= 255; i++) 
    {
        if (!IsNickNameCharacterAllowed(i))
            AllowNickNameCharacter(i, true);
    }
    #if defined opc_fix_OnGameModeInit
        return opc_fix_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit opc_fix_OnGameModeInit
#if defined opc_fix_OnGameModeInit
    forward opc_fix_OnGameModeInit();
#endif

static const gsc_FixAnimLib[][] = 
{
    "AIRPORT",      "ATTRACTORS",   "BAR",          "BASEBALL",     "BD_FIRE",
    "BEACH",        "BENCHPRESS",   "BF_INJECTION", "BIKE_DBZ",     "BIKED",
    "BIKEH",        "BIKELEAP",     "BIKES",        "BIKEV",        "BLOWJOBZ",
    "BMX",          "BOMBER",       "BOX",          "BSKTBALL",     "BUDDY",
    "BUS",          "CAMERA",       "CAR",          "CAR_CHAT",     "CARRY",
    "CASINO",       "CHAINSAW",     "CHOPPA",       "CLOTHES",      "COACH",
    "COLT45",       "COP_AMBIENT",  "COP_DVBYZ",    "CRACK",        "CRIB",
    "DAM_JUMP",     "DANCING",      "DEALER",       "DILDO",        "DODGE",
    "DOZER",        "DRIVEBYS",     "FAT",          "FIGHT_B",      "FIGHT_C",
    "FIGHT_D",      "FIGHT_E",      "FINALE",       "FINALE2",      "FLAME",
    "FLOWERS",      "FOOD",         "FREEWEIGHTS",  "GANGS",        "GFUNK",
    "GHANDS",       "GHETTO_DB",    "GOGGLES",      "GRAFFITI",     "GRAVEYARD",
    "GRENADE",      "GYMNASIUM",    "HAIRCUTS",     "HEIST9",       "INT_HOUSE",
    "INT_OFFICE",   "INT_SHOP",     "JST_BUISNESS", "KART",         "KISSING",
    "KNIFE",        "LAPDAN1",      "LAPDAN2",      "LAPDAN3",      "LOWRIDER",
    "MD_CHASE",     "MD_END",       "MEDIC",        "MISC",         "MTB",
    "MUSCULAR",     "NEVADA",       "ON_LOOKERS",   "OTB",          "PARACHUTE",
    "PARK",         "PAULNMAC",     "PED",          "PLAYER_DVBYS", "PLAYIDLES",
    "POLICE",       "POOL",         "POOR",         "PYTHON",       "QUAD",
    "QUAD_DBZ",     "RAPPING",      "RIFLE",        "RIOT",         "ROB_BANK",
    "ROCKET",       "RUNNINGMAN",   "RUSTLER",      "RYDER",        "SCRATCHING",
    "SEX",          "SHAMAL",       "SHOP",         "SHOTGUN",      "SILENCED",
    "SKATE",        "SMOKING",      "SNIPER",       "SNM",          "SPRAYCAN",
    "STRIP",        "SUNBATHE",     "SWAT",         "SWEET",        "SWIM",
    "SWORD",        "TANK",         "TATTOOS",      "TEC",          "TRAIN",
    "TRUCK",        "UZI",          "VAN",          "VENDING",      "VORTEX",
    "WAYFARER",     "WEAPONS",      "WOP",          "WUZI",         "SnM"
};

public OnPlayerConnect(playerid) 
{
    for(new i, j = sizeof(gsc_FixAnimLib); i < j; i++) 
    {
        ApplyAnimation(playerid, gsc_FixAnimLib[i], "null", 4.0, false, false, false, false, -1, SYNC_ALL);
    }

	#if defined opc_fix_OnPlayerConnect
        return opc_fix_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect opc_fix_OnPlayerConnect
#if defined opc_fix_OnPlayerConnect
    forward opc_fix_OnPlayerConnect(playerid);
#endif


// Textdraw

#define _MAX_FIXED_GAMETEXT_STYLE 16

static Text:gs_FixGameTextTD[_MAX_FIXED_GAMETEXT_STYLE] = {INVALID_TEXT_DRAW, ...};
static gs_FixGameTextTDTimer[_MAX_FIXED_GAMETEXT_STYLE] = {-1, ...};

static PlayerText:gs_FixPlayerGameTextTD[MAX_PLAYERS][_MAX_FIXED_GAMETEXT_STYLE] = {{INVALID_PLAYER_TEXT_DRAW, ...},...};
static gs_FixPlayerGameTextTDTimer[MAX_PLAYERS][_MAX_FIXED_GAMETEXT_STYLE] = {{-1, ...},...};

static stock _OPCore_CreateFixedGameText(const playerid)
{
    if(playerid == INVALID_PLAYER_ID)
    {
        new Text:t;
        // Global style 0.
        t = gs_FixGameTextTD[0] = TextDrawCreate(320.0, 214.0, " "),
        TextDrawLetterSize(t, 1.3, 3.6),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_CENTRE),
        TextDrawColour(t, 0x906210FF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 2),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_3),
        TextDrawSetProportional(t, true),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 200.0, 620.0);

        // Global style 1.
        t = gs_FixGameTextTD[1] = TextDrawCreate(620.0, 310.0, " "),
        TextDrawLetterSize(t, 1.0, 2.6),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_RIGHT),
        TextDrawColour(t, 0x906210FF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 2),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_3),
        TextDrawSetProportional(t, true),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 10.0, 200.0);

        // Global style 2.
        t = gs_FixGameTextTD[2] = TextDrawCreate(320.0, 156.0, " "),
        TextDrawLetterSize(t, 2.1, 4.2),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_CENTRE),
        TextDrawColour(t, 0xE1E1E1FF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 3),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_0),
        TextDrawSetProportional(t, true),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 200.0, 620.0);

        // Global style 3.
        t = gs_FixGameTextTD[3] = TextDrawCreate(320.0, 154.5, " "),
        TextDrawLetterSize(t, 0.6, 2.75),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_CENTRE),
        TextDrawColour(t, 0x906210FF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 2),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_2),
        TextDrawSetProportional(t, true),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 200.0, 620.0);

        // Global style 4.
        t = gs_FixGameTextTD[4] = TextDrawCreate(320.0, 115.5, " "),
        TextDrawLetterSize(t, 0.6, 2.75),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_CENTRE),
        TextDrawColour(t, 0x906210FF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 2),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_2),
        TextDrawSetProportional(t, true),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 200.0, 620.0);

        // Global style 5.
        t = gs_FixGameTextTD[5] = TextDrawCreate(320.0, 217.0, " "),
        TextDrawLetterSize(t, 0.6, 2.75),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_CENTRE),
        TextDrawColour(t, 0xE1E1E1FF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 2),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_2),
        TextDrawSetProportional(t, true),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 200.0, 620.0);

        // Global style 6.
        t = gs_FixGameTextTD[6] = TextDrawCreate(320.0, 60.0, " "),
        TextDrawLetterSize(t, 1.0, 3.6),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_CENTRE),
        TextDrawColour(t, 0xACCBF1FF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 2),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_3),
        TextDrawSetProportional(t, true),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 200.0, 620.0);

        // Global style 7 (vehicle name).
        t = gs_FixGameTextTD[7] = TextDrawCreate(608.0, 344.0, " "),
        TextDrawLetterSize(t, 1.0, 3.0),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_RIGHT),
        TextDrawColour(t, 0x36682CFF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 2),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_2),
        TextDrawSetProportional(t, true),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 10.0, 200.0);

        // Global style 8 (location name).
        t = gs_FixGameTextTD[8] = TextDrawCreate(608.0, 385.8, " "),
        TextDrawLetterSize(t, 1.2, 3.8),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_RIGHT),
        TextDrawColour(t, 0xACCBF1FF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 2),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_0),
        TextDrawSetProportional(t, true),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 10.0, 200.0);

        // Global style 9 (radio name).
        t = gs_FixGameTextTD[9] = TextDrawCreate(320.0, 22.0, " "),
        TextDrawLetterSize(t, 0.6, 1.8),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_CENTRE),
        TextDrawColour(t, 0x906210FF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 1),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_2),
        TextDrawSetProportional(t, true),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 200.0, 620.0);

        // Global style 10 (radio switch).
        t = gs_FixGameTextTD[10] = TextDrawCreate(320.0, 22.0, " "),
        TextDrawLetterSize(t, 0.6, 1.8),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_CENTRE),
        TextDrawColour(t, 0x969696FF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 1),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_2),
        TextDrawSetProportional(t, true),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 200.0, 620.0);

        // Global style 11 (positive money).
        t = gs_FixGameTextTD[11] = TextDrawCreate(608.0, 77.0, " "),
        TextDrawLetterSize(t, 0.55, 2.2),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_RIGHT),
        TextDrawColour(t, 0x36682CFF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 2),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_3),
        TextDrawSetProportional(t, false),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 10.0, 200.0);

        // Global style 12 (negative money).
        t = gs_FixGameTextTD[12] = TextDrawCreate(608.0, 77.0, " "),
        TextDrawLetterSize(t, 0.55, 2.2),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_RIGHT),
        TextDrawColour(t, 0xB4191DFF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 2),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_3),
        TextDrawSetProportional(t, false),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 10.0, 200.0);

        // Global style 13 (stunt).
        t = gs_FixGameTextTD[13] = TextDrawCreate(380.0, 341.15, " "),
        TextDrawLetterSize(t, 0.58, 2.42),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_CENTRE),
        TextDrawColour(t, 0xDDDDDBFF),
        TextDrawSetShadow(t, 2),
        TextDrawSetOutline(t, 0),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_1),
        TextDrawSetProportional(t, true),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 40.0, 460.0);

        // Global style 14 (clock).
        t = gs_FixGameTextTD[14] = TextDrawCreate(608.0, 22.0, " "),
        TextDrawLetterSize(t, 0.55, 2.2),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_RIGHT),
        TextDrawColour(t, 0xC3C3C3FF),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 2),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_3),
        TextDrawSetProportional(t, false),
        TextDrawUseBox(t, false),
        TextDrawBoxColour(t, 0x00000000),
        TextDrawTextSize(t, 400.0, 20.0);

        // Global style 15 (popup).
        t = gs_FixGameTextTD[15] = TextDrawCreate(34.0, 28.0, " "),
        TextDrawLetterSize(t, 0.52, 2.2),
        TextDrawAlignment(t, TEXT_DRAW_ALIGN_LEFT),
        TextDrawColour(t, 0xFFFFFF96),
        TextDrawSetShadow(t, 0),
        TextDrawSetOutline(t, 0),
        TextDrawBackgroundColour(t, 0x000000AA),
        TextDrawFont(t, TEXT_DRAW_FONT_1),
        TextDrawSetProportional(t, true),
        TextDrawUseBox(t, true),
        TextDrawBoxColour(t, 0x00000080),
        TextDrawTextSize(t, 230.5, 200.0);
    }
    else
    {
        new PlayerText:t;
        // Global style 0.
        t = gs_FixPlayerGameTextTD[playerid][0] = CreatePlayerTextDraw(playerid, 320.0, 214.0, " "),
        PlayerTextDrawLetterSize(playerid, t, 1.3, 3.6),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_CENTRE),
        PlayerTextDrawColour(playerid, t, 0x906210FF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 2),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_3),
        PlayerTextDrawSetProportional(playerid, t, true),
        PlayerTextDrawUseBox(playerid, t, true),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 200.0, 620.0);

        // Global style 1.
        t = gs_FixPlayerGameTextTD[playerid][1] = CreatePlayerTextDraw(playerid, 620.0, 310.0, " "),
        PlayerTextDrawLetterSize(playerid, t, 1.0, 2.6),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_RIGHT),
        PlayerTextDrawColour(playerid, t, 0x906210FF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 2),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_3),
        PlayerTextDrawSetProportional(playerid, t, true),
        PlayerTextDrawUseBox(playerid, t, true),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 10.0, 200.0);

        // Global style 2.
        t = gs_FixPlayerGameTextTD[playerid][2] = CreatePlayerTextDraw(playerid, 320.0, 156.0, " "),
        PlayerTextDrawLetterSize(playerid, t, 2.1, 4.2),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_CENTRE),
        PlayerTextDrawColour(playerid, t, 0xE1E1E1FF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 3),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_0),
        PlayerTextDrawSetProportional(playerid, t, true),
        PlayerTextDrawUseBox(playerid, t, true),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 200.0, 620.0);

        // Global style 3.
        t = gs_FixPlayerGameTextTD[playerid][3] = CreatePlayerTextDraw(playerid, 320.0, 154.5, " "),
        PlayerTextDrawLetterSize(playerid, t, 0.6, 2.75),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_CENTRE),
        PlayerTextDrawColour(playerid, t, 0x906210FF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 2),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_2),
        PlayerTextDrawSetProportional(playerid, t, true),
        PlayerTextDrawUseBox(playerid, t, true),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 200.0, 620.0);

        // Global style 4.
        t = gs_FixPlayerGameTextTD[playerid][4] = CreatePlayerTextDraw(playerid, 320.0, 115.5, " "),
        PlayerTextDrawLetterSize(playerid, t, 0.6, 2.75),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_CENTRE),
        PlayerTextDrawColour(playerid, t, 0x906210FF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 2),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_2),
        PlayerTextDrawSetProportional(playerid, t, true),
        PlayerTextDrawUseBox(playerid, t, true),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 200.0, 620.0);

        // Global style 5.
        t = gs_FixPlayerGameTextTD[playerid][5] = CreatePlayerTextDraw(playerid, 320.0, 217.0, " "),
        PlayerTextDrawLetterSize(playerid, t, 0.6, 2.75),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_CENTRE),
        PlayerTextDrawColour(playerid, t, 0xE1E1E1FF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 2),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_2),
        PlayerTextDrawSetProportional(playerid, t, true),
        PlayerTextDrawUseBox(playerid, t, true),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 200.0, 620.0);

        // Global style 6.
        t = gs_FixPlayerGameTextTD[playerid][6] = CreatePlayerTextDraw(playerid, 320.0, 60.0, " "),
        PlayerTextDrawLetterSize(playerid, t, 1.0, 3.6),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_CENTRE),
        PlayerTextDrawColour(playerid, t, 0xACCBF1FF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 2),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_3),
        PlayerTextDrawSetProportional(playerid, t, true),
        PlayerTextDrawUseBox(playerid, t, true),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 200.0, 620.0);

        // Global style 7 (playerid, vehicle name).
        t = gs_FixPlayerGameTextTD[playerid][7] = CreatePlayerTextDraw(playerid, 608.0, 344.0, " "),
        PlayerTextDrawLetterSize(playerid, t, 1.0, 3.0),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_RIGHT),
        PlayerTextDrawColour(playerid, t, 0x36682CFF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 2),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_2),
        PlayerTextDrawSetProportional(playerid, t, true),
        PlayerTextDrawUseBox(playerid, t, true),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 10.0, 200.0);

        // Global style 8 (playerid, location name).
        t = gs_FixPlayerGameTextTD[playerid][8] = CreatePlayerTextDraw(playerid, 608.0, 385.8, " "),
        PlayerTextDrawLetterSize(playerid, t, 1.2, 3.8),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_RIGHT),
        PlayerTextDrawColour(playerid, t, 0xACCBF1FF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 2),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_0),
        PlayerTextDrawSetProportional(playerid, t, true),
        PlayerTextDrawUseBox(playerid, t, true),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 10.0, 200.0);

        // Global style 9 (playerid, radio name).
        t = gs_FixPlayerGameTextTD[playerid][9] = CreatePlayerTextDraw(playerid, 320.0, 22.0, " "),
        PlayerTextDrawLetterSize(playerid, t, 0.6, 1.8),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_CENTRE),
        PlayerTextDrawColour(playerid, t, 0x906210FF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 1),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_2),
        PlayerTextDrawSetProportional(playerid, t, true),
        PlayerTextDrawUseBox(playerid, t, true),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 200.0, 620.0);

        // Global style 10 (playerid, radio switch).
        t = gs_FixPlayerGameTextTD[playerid][10] = CreatePlayerTextDraw(playerid, 320.0, 22.0, " "),
        PlayerTextDrawLetterSize(playerid, t, 0.6, 1.8),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_CENTRE),
        PlayerTextDrawColour(playerid, t, 0x969696FF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 1),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_2),
        PlayerTextDrawSetProportional(playerid, t, true),
        PlayerTextDrawUseBox(playerid, t, true),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 200.0, 620.0);

        // Global style 11 (playerid, positive money).
        t = gs_FixPlayerGameTextTD[playerid][11] = CreatePlayerTextDraw(playerid, 608.0, 77.0, " "),
        PlayerTextDrawLetterSize(playerid, t, 0.55, 2.2),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_RIGHT),
        PlayerTextDrawColour(playerid, t, 0x36682CFF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 2),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_3),
        PlayerTextDrawSetProportional(playerid, t, false),
        PlayerTextDrawUseBox(playerid, t, true),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 10.0, 200.0);

        // Global style 12 (playerid, negative money).
        t = gs_FixPlayerGameTextTD[playerid][12] = CreatePlayerTextDraw(playerid, 608.0, 77.0, " "),
        PlayerTextDrawLetterSize(playerid, t, 0.55, 2.2),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_RIGHT),
        PlayerTextDrawColour(playerid, t, 0xB4191DFF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 2),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_3),
        PlayerTextDrawSetProportional(playerid, t, false),
        PlayerTextDrawUseBox(playerid, t, true),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 10.0, 200.0);

        // Global style 13 (Based on SA stunt bonuses.)
        t = gs_FixPlayerGameTextTD[playerid][13] = CreatePlayerTextDraw(playerid, 380.0, 341.15, " ");
        PlayerTextDrawLetterSize(playerid, t, 0.52, 2.4);
        PlayerTextDrawTextSize(playerid, t, 0.0, 513.0);
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_CENTRE);
        PlayerTextDrawColour(playerid, t, 0xC7C7C5FF);
        PlayerTextDrawUseBox(playerid, t, true);
        PlayerTextDrawBoxColour(playerid, t, 0);
        PlayerTextDrawSetOutline(playerid, t, 0);
        PlayerTextDrawSetShadow(playerid, t, 2);
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000FF);
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_1);
        PlayerTextDrawSetProportional(playerid, t, true);

        // Global style 14 (clock).
        t = gs_FixPlayerGameTextTD[playerid][14] = CreatePlayerTextDraw(playerid, 608.0, 22.0, " "),
        PlayerTextDrawLetterSize(playerid, t, 0.55, 2.2),
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_RIGHT),
        PlayerTextDrawColour(playerid, t, 0xC3C3C3FF),
        PlayerTextDrawSetShadow(playerid, t, 0),
        PlayerTextDrawSetOutline(playerid, t, 2),
        PlayerTextDrawBackgroundColour(playerid, t, 0x000000AA),
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_3),
        PlayerTextDrawSetProportional(playerid, t, false),
        PlayerTextDrawUseBox(playerid, t, false),
        PlayerTextDrawBoxColour(playerid, t, 0x00000000),
        PlayerTextDrawTextSize(playerid, t, 400.0, 20.0);

        // Global style 15 (Based on SA notification popup.)
        t = gs_FixPlayerGameTextTD[playerid][15] = CreatePlayerTextDraw(playerid, 30.0, 125.5, " ");
        PlayerTextDrawLetterSize(playerid, t, 0.5, 2.0);
        PlayerTextDrawTextSize(playerid, t, 230.0, 0.000000);
        PlayerTextDrawAlignment(playerid, t, TEXT_DRAW_ALIGN_LEFT);
        PlayerTextDrawColour(playerid, t, 0xC7C7C5FF);
        PlayerTextDrawUseBox(playerid, t, true);
        PlayerTextDrawBoxColour(playerid, t, 0x00000095);
        PlayerTextDrawSetShadow(playerid, t, 0);
        PlayerTextDrawSetOutline(playerid, t, 0);
        PlayerTextDrawBackgroundColour(playerid, t, 136);
        PlayerTextDrawFont(playerid, t, TEXT_DRAW_FONT_1);
        PlayerTextDrawSetProportional(playerid, t, true);
    }
    return 1;
}

static stock _OPCore_DestroyFixedGameText(const playerid)
{
    if(playerid == INVALID_PLAYER_ID)
    {
        for(new i = 0; i < _MAX_FIXED_GAMETEXT_STYLE; i++)
        {
            if(gs_FixGameTextTDTimer[i] != -1) {
                KillTimer(gs_FixGameTextTDTimer[i]);
                gs_FixGameTextTDTimer[i] = -1;
            }
            if(gs_FixGameTextTD[i] != INVALID_TEXT_DRAW)
            {
                TextDrawDestroy(gs_FixGameTextTD[i]);
                gs_FixGameTextTD[i] = INVALID_TEXT_DRAW;
            }
        }
    }
    else
    {
        for(new i = 0; i < _MAX_FIXED_GAMETEXT_STYLE; i++) 
        {
            if(gs_FixPlayerGameTextTDTimer[playerid][i] != -1) {
                KillTimer(gs_FixPlayerGameTextTDTimer[playerid][i]);
                gs_FixPlayerGameTextTDTimer[playerid][i] = -1;
            }
            if(gs_FixPlayerGameTextTD[playerid][i] != INVALID_PLAYER_TEXT_DRAW) {
                PlayerTextDrawDestroy(playerid, gs_FixPlayerGameTextTD[playerid][i]);
                gs_FixPlayerGameTextTD[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
            }
        }
    }
    return 1;
}

forward bool:_OPCore_HideFixedGameTextForPlayer(playerid, style);
public bool:_OPCore_HideFixedGameTextForPlayer(playerid, style)
{
    if(!IsPlayerConnected(playerid)) return false;
    if(IsPlayerNPC(playerid)) return false;
    if(style < 0 || style >= _MAX_FIXED_GAMETEXT_STYLE) return false;

    if(gs_FixPlayerGameTextTDTimer[playerid][style] != -1)
    {
        KillTimer(gs_FixPlayerGameTextTDTimer[playerid][style]);
        gs_FixPlayerGameTextTDTimer[playerid][style] = -1;
    }
    return PlayerTextDrawHide(playerid, gs_FixPlayerGameTextTD[playerid][style]);
}

stock bool:OPCore_HideGameTextForPlayer(playerid, style)
{
    return _OPCore_HideFixedGameTextForPlayer(playerid, style);
}
#if defined _ALS_HideGameTextForPlayer
    #undef HideGameTextForPlayer
#else
    #define _ALS_HideGameTextForPlayer
#endif
#define HideGameTextForPlayer OPCore_HideGameTextForPlayer

stock bool:OPCore_GameTextForPlayer(playerid, const text[], time, style, {Float, _}:...)
{
    if(!IsPlayerConnected(playerid)) return false;
    if(IsPlayerNPC(playerid)) return false;
    if(style < 0 || style >= _MAX_FIXED_GAMETEXT_STYLE) return false;
    if (time <= 0) return false;

    if(gs_FixPlayerGameTextTDTimer[playerid][style] != -1)
    {
        KillTimer(gs_FixPlayerGameTextTDTimer[playerid][style]);
        gs_FixPlayerGameTextTDTimer[playerid][style] = -1;
    }
    gs_FixPlayerGameTextTDTimer[playerid][style] = SetTimerEx("_OPCore_HideFixedGameTextForPlayer", time, false , "ii", playerid, style);

    const static_args = 4;
    const args_diff = (3 - static_args) * 4;
    static args;
    args = numargs();
    if (args > static_args)
    {
        static string[256];
        const string_size = sizeof(string);
        while (--args >= 4)
        {
            #emit LCTRL 5
            #emit LOAD.alt args
            #emit SHL.C.alt 2
            #emit ADD.C 12
            #emit ADD
            #emit LOAD.I
            #emit PUSH.pri
        }
        #emit PUSH.S text
        #emit PUSH.C string_size
        #emit PUSH.C string
        #emit LOAD.S.pri 8
        #emit ADD.C args_diff
        #emit PUSH.pri
        #emit SYSREQ.C format
        #emit LCTRL 5
        #emit SCTRL 4
        
        PlayerTextDrawSetString(playerid, gs_FixPlayerGameTextTD[playerid][style], string);
    }
    else
    {
        PlayerTextDrawSetString(playerid, gs_FixPlayerGameTextTD[playerid][style], text);
    }
    return PlayerTextDrawShow(playerid, gs_FixPlayerGameTextTD[playerid][style]);
}
#if defined _ALS_GameTextForPlayer
    #undef GameTextForPlayer
#else
    #define _ALS_GameTextForPlayer
#endif
#define GameTextForPlayer OPCore_GameTextForPlayer

forward bool:_OPCore_FixedGameTextHideForAll(style);
public bool:_OPCore_FixedGameTextHideForAll(style)
{
    if(style < 0 || style >= _MAX_FIXED_GAMETEXT_STYLE) return false;
    if(gs_FixGameTextTDTimer[style] != -1)
    {
        KillTimer(gs_FixGameTextTDTimer[style]);
        gs_FixGameTextTDTimer[style] = -1;
    }
    return TextDrawHideForAll(gs_FixGameTextTD[style]);
}

stock bool:OPCore_HideGameTextForAll(style)
{
    return _OPCore_FixedGameTextHideForAll(style);
}
#if defined _ALS_HideGameTextForAll
    #undef HideGameTextForAll
#else
    #define _ALS_HideGameTextForAll
#endif
#define HideGameTextForAll OPCore_HideGameTextForAll

stock bool:OPCore_GameTextForAll(const text[], time, style, {Float, _}:...)
{
    if(style < 0 || style >= _MAX_FIXED_GAMETEXT_STYLE) return false;
    if (time <= 0) return false;

    if(gs_FixGameTextTDTimer[style] != -1)
    {
        KillTimer(gs_FixGameTextTDTimer[style]);
        gs_FixGameTextTDTimer[style] = -1;
    }
    gs_FixGameTextTDTimer[style] = SetTimerEx("_OPCore_FixedGameTextHideForAll", time, false , "i", style);

    const static_args = 3;
    const args_diff = (3 - static_args) * 4;
    static args;
    args = numargs();
    if (args > static_args)
    {
        static string[256];
        const string_size = sizeof(string);

        while (--args >= 3)
        {
            #emit LCTRL 5
            #emit LOAD.alt args
            #emit SHL.C.alt 2
            #emit ADD.C 12
            #emit ADD
            #emit LOAD.I
            #emit PUSH.pri
        }

        #emit PUSH.S text
        #emit PUSH.C string_size
        #emit PUSH.C string
        #emit LOAD.S.pri 8
        #emit ADD.C args_diff
        #emit PUSH.pri
        #emit SYSREQ.C format
        #emit LCTRL 5
        #emit SCTRL 4
        
        TextDrawSetString(gs_FixGameTextTD[style], string);
    }
    else
    {
        TextDrawSetString(gs_FixGameTextTD[style], text);
    }
    return TextDrawShowForAll(gs_FixGameTextTD[style]);
}
#if defined _ALS_GameTextForAll
    #undef GameTextForAll
#else
    #define _ALS_GameTextForAll
#endif
#define GameTextForAll OPCore_GameTextForAll

public OnGameModeInit()
{
    _OPCore_CreateFixedGameText(INVALID_PLAYER_ID);

    #if defined opc_fixgt_OnGameModeInit
        return opc_fixgt_OnGameModeInit();
    #else
        return 1;
    #endif
}

public OnGameModeExit()
{
    _OPCore_DestroyFixedGameText(INVALID_PLAYER_ID);

	#if defined opc_fixgt_OnGameModeExit
        return opc_fixgt_OnGameModeExit();
    #else
        return 1;
    #endif
}

public OnPlayerConnect(playerid)
{
    if(!IsPlayerNPC(playerid))
    {
        _OPCore_CreateFixedGameText(playerid);
    }
    #if defined opc_fixgt_OnPlayerConnect
        return opc_fixgt_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

public OnPlayerDisconnect(playerid, reason)
{
    if(!IsPlayerNPC(playerid))
    {
        _OPCore_DestroyFixedGameText(playerid);
    }
    #if defined opc_fixgt_OnPlayerDisconnect
        return opc_fixgt_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit opc_fixgt_OnGameModeInit
#if defined opc_fixgt_OnGameModeInit
    forward opc_fixgt_OnGameModeInit();
#endif

#if defined _ALS_OnGameModeExit
    #undef OnGameModeExit
#else
    #define _ALS_OnGameModeExit
#endif
#define OnGameModeExit opc_fixgt_OnGameModeExit
#if defined opc_fixgt_OnGameModeExit
    forward opc_fixgt_OnGameModeExit();
#endif

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect opc_fixgt_OnPlayerConnect
#if defined opc_fixgt_OnPlayerConnect
    forward opc_fixgt_OnPlayerConnect(playerid);
#endif

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect opc_fixgt_OnPlayerDisconnect
#if defined opc_fixgt_OnPlayerDisconnect
    forward opc_fixgt_OnPlayerDisconnect(playerid, reason);
#endif