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

## server.inc

提供服务器基础信息的动态配置功能，并60秒自动刷新一次

如果不需要此功能可以在包含 `op-core` 之前定义关闭动态信息
```pawn
#defined OPCORE_DISABLE_DYNAMIC_SERVER_INFO
#include <op-framework/op-core>
```
