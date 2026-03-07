# OPCore Player

## 概述
管理服务器中的玩家数据，使用 MySQL 数据库存储，如分数、金钱、位置等。模块使用脏位（dirty bits）机制优化数据保存，仅更新修改过的字段，避免不必要的数据库操作，统一管理/添加/编辑/删除自定义玩家数据，模块将自动化处理。

## 依赖
- a_mysql (MySQL 插件)
- samp_bcrypt (Bcrypt 密码哈希插件)
- op-framework/op-core (核心)

## 安装
   ```pawn
   #include <op-framework/op-player>
   ```

## 函数列表
#### 账号相关
- **bool:OPCore_Player_IsValid(playerid)**
  - **描述**：检查玩家是否是有效的用户(已登录/非NPC)。
  - **参数**：playerid - 玩家 ID。
  - **返回**：true 如果有效，否则 false。

- **OPCore_Player_GetDBID(playerid)**
  - **描述**：获取玩家账号数据库 ID。
  - **参数**：playerid - 玩家 ID。
  - **返回**：账号 ID 或 0 如果无效。

- **OPCore_Player_GetRegisterDate(playerid)**
  - **描述**：获取账号注册时间戳。
  - **参数**：playerid - 玩家 ID。
  - **返回**：时间戳或 0 如果无效。

- **OPCore_Player_GetLoginDate(playerid)**
  - **描述**：获取最近登录时间戳。
  - **参数**：playerid - 玩家 ID。
  - **返回**：时间戳或 0 如果无效。

- **OPCore_Player_GetLeaveDate(playerid)**
  - **描述**：获取上次离开时间戳。
  - **参数**：playerid - 玩家 ID。
  - **返回**：时间戳或 0 如果无效。

- **bool:OPCore_Player_SignIn(playerid)**
  - **描述**：启动玩家登录过程（检查账号存在，显示对话框）。通常在 OnPlayerRequestClass 调用
  - **参数**：playerid - 玩家 ID。
  - **返回**：true 如果启动成功，否则 false。
  - **注意**：内部处理 MySQL 查询和对话框显示, 重复调用不会导致重复登陆

#### 玩家数据相关
- **Float:OPCore_Player_GetDataFloat(playerid, field)**
  - **描述**：获取 FLOAT 类型字段值。
  - **参数**：
    - playerid - 玩家 ID。
    - field - 字段 ID (e.g., PLAYER_DATA_POS_X)。
  - **返回**：值或 0.0 如果无效。

- **OPCore_Player_GetDataInt(playerid, field)**
  - **描述**：获取 INT 类型字段值。
  - **参数**：同上。
  - **返回**：值或 0 如果无效。

- **bool:OPCore_Player_GetDataString(playerid, field, string[], size = sizeof(string))**
  - **描述**：获取 STRING 类型字段值。
  - **参数**：
    - playerid - 玩家 ID。
    - field - 字段 ID。
    - string[] - 输出字符串。
    - size - 字符串大小（默认 sizeof(string)）。
  - **返回**：true 如果成功，否则 false。

- **bool:OPCore_Player_SetDataFloat(playerid, field, Float:value)**
  - **描述**：设置 FLOAT 类型字段值，并标记脏位（自动保存）。
  - **参数**：同 Get，额外 value - 新值。
  - **返回**：true 如果成功，否则 false。

- **bool:OPCore_Player_SetDataInt(playerid, field, value)**
  - **描述**：设置 INT 类型字段值，并标记脏位。
  - **参数**：同上。
  - **返回**：true 如果成功，否则 false。

- **bool:OPCore_Player_SetDataString(playerid, field, const value[])**
  - **描述**：设置 STRING 类型字段值，并标记脏位。
  - **参数**：同 Get，额外 value[] - 新字符串。
  - **返回**：true 如果成功，否则 false。

- **bool:OPCore_Player_GiveDataFloat(playerid, field, Float:value)**
  - **描述**：为 FLOAT 字段增加值，并标记脏位。
  - **参数**：同 Set。
  - **返回**：true 如果成功，否则 false。

- **bool:OPCore_Player_GiveDataInt(playerid, field, value)**
  - **描述**：为 INT 字段增加值，并标记脏位。
  - **参数**：同 Set。
  - **返回**：true 如果成功，否则 false。


## 回调
- **OnPlayerLoggedIn(playerid)**：玩家登录成功后调用
