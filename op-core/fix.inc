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