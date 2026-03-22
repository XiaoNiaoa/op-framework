# OPCore Race

## 概述
提供完整的赛事创建、加入、退出等功能，支持多场赛事同时进行

## 依赖

## 安装
   ```pawn
   #include <op-framework/op-race>
   ```

## 常量

```pawn
#define MAX_RACE_CHECKPOINTS           (100)   // 一个赛道最多拥有的检查点数量
#define MAX_RACES                      (50)    // 最多同时存在的比赛
#define RACE_LEAVE_VEHICLE_TIME_LIMIT  (15)    // 离开载具的最长时间限制（秒）
#define INVALID_RACE_ID                (-1)    // 无效的比赛会话ID
```

## 函数列表

- **OPCore_Race_GetPlayerRace(playerid)**
  - **描述**：获取玩家当前所在赛事的ID。
  - **参数**：`playerid` - 玩家 ID（0 ~ MAX_PLAYERS-1）。
  - **返回**：赛事ID 或 `INVALID_RACE_ID(-1)`（不在任何赛事中）。
  - **示例**：`if(OPCore_Race_GetPlayerRace(playerid) != INVALID_RACE_ID) ...`

- **OPCore_Race_Quit(playerid)**
  - **描述**：让玩家退出当前赛事（可随时调用，系统会自动处理人数、清理、空赛事重置）。
  - **参数**：`playerid` - 玩家 ID。
  - **返回**：无返回值。
  - **示例**：创建一个 `/quitrace` 指令直接调用即可。

- **bool:OPCore_Race_IsValid(raceid)**
  - **描述**：检查赛事是否有效（根据状态是否为 `RACE_STATE_NONE` 判断）。
  - **参数**：`raceid` - 赛事 ID。
  - **返回**：`true` = 有效赛事，`false` = 无效或不存在。
  - **示例**：`if(OPCore_Race_IsValid(raceid)) ...`

- **OPCore_Race_Create()**
  - **描述**：创建一个新赛事（自动寻找空闲槽位并重置数据）。
  - **参数**：无。
  - **返回**：新赛事ID 或 `INVALID_RACE_ID`（已达到 `MAX_RACES` 上限时失败）。
  - **示例**：`new raceid = OPCore_Race_Create();`

- **bool:OPCore_Race_Join(playerid, raceid)**
  - **描述**：让玩家加入指定赛事（若玩家已在其他有效赛事中，会自动先退出旧赛事）。
  - **参数**：
    - `playerid` - 玩家 ID
    - `raceid` - 要加入的赛事 ID
  - **返回**：`true` = 加入成功，`false` = 失败（赛事无效、玩家未连接、已在该赛事中）。
  - **示例**：`if(OPCore_Race_Join(playerid, raceid)) SendClientMessage(...);`

---
