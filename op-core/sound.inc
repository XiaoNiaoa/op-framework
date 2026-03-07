stock OPCore_SoundStop(playerid, interval = 0)
{
    if(interval <= 0) PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
    
    else SetTimerEx("_OPCore_SoundStop", interval, false, "i", playerid);

    return 1;
}

stock OPCore_SoundPlay(soundid, Float:range, Float:x, Float:y, Float:z, worldid = 0, ints = 0)
{
    foreach(new i : Player) 
    {
        if(GetPlayerVirtualWorld(i) != worldid || GetPlayerInterior(i) != ints) 
            continue;

        if(IsPlayerInRangeOfPoint(i, range, x, y, z)) 
            PlayerPlaySound(i, soundid, x, y, z);
    }
    return 1;
}

forward _OPCore_SoundStop(playerid);
public _OPCore_SoundStop(playerid)
{
    PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
    return 1;
}