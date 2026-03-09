# OPCore Core



## mysql.inc

本质是一个基于 MySQL 的基础管理核心，统一维护数据库连接：所有功能模块共用一个句柄，避免重复连接，其他模块如果需要使用 MySQL 数据库，请先 `#include <op-framework/op-core>`

**OPCore_GetMySQLHandle()**

- 获取 MySQL 连接句柄
- 返回 MySQL:handle

**OPCore_GetPlayerMySQLRace(playerid)**

- 用于验证异步查询返回时，玩家是否仍是发起查询时的同一个人，有效防止数据写错
- 返回 当前玩家的 Race 序列号

```pawn
// 发起查询 并传递序列号
new query[128];
mysql_format(OPCore_GetMySQLHandle(), query, sizeof(query), "INSERT INTO `players` (`player_uid`) VALUES ('%d')", OPCore_Player_GetDBID(playerid));
mysql_tquery(OPCore_GetMySQLHandle(), query, "_OPCore_Player_Create", "ii", playerid, OPCore_GetPlayerMySQLRace(playerid));

// 完成查询 并检查是否还是同一个玩家
forward _OPCore_Player_Create(playerid, race_check);
public _OPCore_Player_Create(playerid, race_check)
{
    if(OPCore_MySQLRaceCheck(playerid, race_check) == true)
    {
        
    }
    return 1;
}
```

**OPCore_MySQLRaceCheck(playerid, race)**

- 异步查询结束使用此函数检查发起查询是否是同一个人
- 若校验失败将强制踢出玩家以防止数据污染

## interior.inc

包含GTASA所有内饰+隐藏内饰数据，数据来源于单机文件，如体育场、四龙赌场办公室、我记不清了，很久之前人工整理的

**OPCore_GetInteriorDoorPos(OPCORE_INTERIOR:index, &Float:x, &Float:y, &Float:z, &int)**

- 获取内饰的内部门口坐标和内饰id，用于创建内饰出口的 pickup 拾取物

**OPCore_GetInteriorPos(OPCORE_INTERIOR:index, &Float:x, &Float:y, &Float:z, &Float:angle)**

- 获取实际玩家进入之后的坐标和角度，

**OPCore_GotoInterior(playerid, OPCORE_INTERIOR:index, worldid = 0)**

- 传送玩家到某个内饰中，并可以设置虚拟世界ID

## server.inc

提供服务器基础信息的动态配置功能，并60秒自动刷新一次

如果不需要此功能可以在包含 `op-core` 之前定义关闭动态信息
```pawn
#defined OPCORE_DISABLE_DYNAMIC_SERVER_INFO
#include <op-framework/op-core>
```
