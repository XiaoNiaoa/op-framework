# 一些实用封装好的函数

没什么特别的

## sound.inc

* OPCore_SoundPlay(soundid, Float:range, Float:x, Float:y, Float:z, worldid = 0, ints = 0)
    - 根据范围和位置播放声音 worldid/ints 为 -1的话则忽略虚拟世界或内饰

* OPCore_SoundStop(playerid, interval = 0)
    - 可以定时停止声音播放


## interior.inc

包含GTASA所有内饰+隐藏内饰数据，数据来源于单机文件，如体育场、四龙赌场办公室、我记不清了，很久之前人工整理的

**OPCore_GetInteriorDoorPos(OPCORE_INTERIOR:index, &Float:x, &Float:y, &Float:z, &int)**

- 获取内饰的内部门口坐标和内饰id，用于创建内饰出口的 pickup 拾取物

**OPCore_GetInteriorPos(OPCORE_INTERIOR:index, &Float:x, &Float:y, &Float:z, &Float:angle)**

- 获取实际玩家进入之后的坐标和角度，

**OPCore_GotoInterior(playerid, OPCORE_INTERIOR:index, worldid = 0)**

- 传送玩家到某个内饰中，并可以设置虚拟世界ID