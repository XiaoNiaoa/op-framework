static gs_SoundTimer[MAX_PLAYERS] = {-1, ...};

stock OPCore_SoundStop(playerid, interval = 0)
{
    if(gs_SoundTimer[playerid] != -1)
    {
        KillTimer(gs_SoundTimer[playerid]);
        gs_SoundTimer[playerid] = -1;
    }

    if(interval <= 0) PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
    
    else gs_SoundTimer[playerid] = SetTimerEx("_OPCore_SoundStop", interval, false, "i", playerid);

    return 1;
}

stock OPCore_SoundPlay(soundid, Float:range, Float:x, Float:y, Float:z, worldid = 0, ints = 0)
{
    // 预编译
    #if defined _Y_ITERATE_LOCAL_VERSION || defined _FOREACH_LOCAL_VERSION
    foreach (new i : Player)
    #else
    for (new i = 0; i < MAX_PLAYERS; i++)
    #endif
    {
        #if !defined _Y_ITERATE_LOCAL_VERSION && !defined _FOREACH_LOCAL_VERSION
        if(!IsPlayerConnected(i)) 
            continue;
        #endif

        if((worldid != -1 && GetPlayerVirtualWorld(i) != worldid) || (ints != -1 && GetPlayerInterior(i) != ints)) 
            continue;

        // openmp内部实现: range >= glm::distance(player.getPosition(), position);
        if(IsPlayerInRangeOfPoint(i, range, x, y, z)) 
            PlayerPlaySound(i, soundid, x, y, z);
    }
    return 1;
}

forward _OPCore_SoundStop(playerid);
public _OPCore_SoundStop(playerid)
{
    gs_SoundTimer[playerid] = -1;
    PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if(gs_SoundTimer[playerid] != -1)
    {
        KillTimer(gs_SoundTimer[playerid]);
        gs_SoundTimer[playerid] = -1;
    }

    #if defined opc_sound_OnPlayerDisconnect
        return opc_sound_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect opc_sound_OnPlayerDisconnect
#if defined opc_sound_OnPlayerDisconnect
    forward opc_sound_OnPlayerDisconnect(playerid, reason);
#endif