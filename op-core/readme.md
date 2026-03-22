# OPCore

## mysql.inc

基于 MySQL 的基础管理，统一维护数据库连接：所有功能模块共用一个句柄，避免重复连接，其他模块如果需要使用 MySQL 数据库，请先 `#include <op-framework/op-core>`

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

# OPCore Share

没什么特别的，一些包含标签的共享定义或通用数据

# OPCore Player

## 概述
管理服务器中的玩家数据，使用 MySQL 数据库存储，如分数、金钱、位置等。模块使用脏位（dirty bits）机制优化数据保存，仅更新修改过的字段，避免不必要的数据库操作，统一管理/添加/编辑/删除自定义玩家数据，模块将自动化处理。

## 依赖
- a_mysql (MySQL 插件)
- samp_bcrypt (Bcrypt 密码哈希插件)


## 机制和说明
- 所有的使用只需要在 `op-core/player/field.inc` 中添加一行玩家数据，系统将自动补全mysql表单，登陆后自动加载，离线自动保存
- 使用 `OPCore_Player_SignIn` 函数传入密码，认证成功后将自动加载玩家数据
- 完整易用的安全验证、数据操作API

## 示例

可根据需求在包含此模块前定义密码长度范围，默认是 4 - 16
```pawn
   #define MIN_PASSWORD_LENGTH  4
   #define MAX_PASSWORD_LENGTH  16

   #include <op-framework/op-core>
```
简单通过指令示例

```pawn
#include <open.mp>
#include <op-framework/op-core>

// 演示中用到的依赖
#include <Pawn.CMD>
#include <sscanf2>

CMD:register(playerid, params[])
{
    new password[MAX_PASSWORD_LENGTH + 1];
    if(sscanf(params, "s[16]", password))
    {
        SendClientMessage(playerid, 0xFFFFFFFF, "用法: /register [密码]");
        return 1;
    }
    OPCore_Player_SignIn(playerid, password, SIGNIN_MODE_REGISTER);
    return 1;
}

CMD:login(playerid, params[])
{
    new password[MAX_PASSWORD_LENGTH + 1];
    if(sscanf(params, "s[16]", password))
    {
        SendClientMessage(playerid, 0xFFFFFFFF, "用法: /login [密码]");
        return 1;
    }
    OPCore_Player_SignIn(playerid, password, SIGNIN_MODE_LOGIN);
    return 1;
}

public OnPlayerSignIn(playerid, result)
{
    switch(result)
    {
        case SIGNIN_UNREGISTERED: SendClientMessage(playerid, 0x00FF00FF, "您还没有注册，请输入/register 注册");

        case SIGNIN_ALREADY_REGISTERED: SendClientMessage(playerid, 0x00FF00FF, "您已注册，请输入/login 登录");

        case SIGNIN_LOGIN_SUCCESS: SendClientMessage(playerid, 0x00FF00FF, "登录成功！欢迎回来");

        case SIGNIN_REGISTER_SUCCESS: SendClientMessage(playerid, 0x00FF00FF, "注册成功！欢迎加入服务器");

        case SIGNIN_INCORRECT_PASSWORD: SendClientMessage(playerid, 0xFF0000FF, "密码错误，请重新输入");

        case SIGNIN_ALREADY_LOGGED_IN: SendClientMessage(playerid, 0xFFFF00FF, "你已登录，无需重复登录");
    }
    return 1;
}
```

## 回调以及状态码定义

**OnPlayerSignIn(playerid, result)**
  - **描述**：使用函数 `OPCore_Player_SignIn` 后触发登录/注册反馈

| result | 描述 |
| :--- | :--- |
| `SIGNIN_LOGIN_SUCCESS` | 登录成功 |
| `SIGNIN_REGISTER_SUCCESS` | 注册成功 |
| - | - |
| `SIGNIN_INCORRECT_PASSWORD` | 密码错误或不符合规范 |
| `SIGNIN_DATABASE_ERROR` | 数据库执行异常 |
| `SIGNIN_ALREADY_LOGGED_IN` | 已登录过 |
| `SIGNIN_LOADING` | 账号数据正在异步加载中 |
| `SIGNIN_UNREGISTERED` | 账号不存在 |
| `SIGNIN_ALREADY_REGISTERED` | 账号已存在 |

**OnPlayerSaved(playerid)**
  - **描述**：使用函数 `OPCore_Player_Save` 并保存完成后触发

## 函数列表

### 身份校验

- **OPCore_Player_IsLoggedIn(playerid)**
  - **描述**：
    - 检查玩家是否是有效的玩家 条件是已登录 并且数据已加载 可以进行数据操作
  - **参数**：playerid - 玩家 ID。
  - **返回**：true 有效 false 无效

- **OPCore_Player_IsRegister(playerid)**
  - **描述**：
      - 检查玩家是否存在于数据库中 常用于判断是给玩家显示登录窗口或注册窗口
      - 即便不检测也可以直接使用 `OPCore_Player_SignIn` 系统会自动判断
  - **参数**：playerid - 玩家 ID。
  - **返回**：true 数据库存在该玩家账户 false 不存在

- **OPCore_Player_IsLoading(playerid)**
  - **描述**：
    - 检查玩家账号数据是否正在加载中 - MySQL异步安全
    - 但实际开发基本用不上，系统有相应的防御机制 以及对应回调的反馈 `OnPlayerSignIn` `SIGNIN_LOADING`
  - **参数**：playerid - 玩家 ID。
  - **返回**：true 异步查询进行中 false 异步安全

### 账户操作

- **OPCore_Player_SignIn(playerid, const password[], SIGNIN_MODE:mode)**
  - **描述**：启动玩家登录或注册
  - **参数**：playerid 玩家ID
  - **参数**：password 密码
  - **参数**：mode     操作模式 (登录操作 SIGNIN_MODE_LOGIN 或 注册操作 SIGNIN_MODE_REGISTER)

### 账号相关

- **OPCore_Player_Save(playerid)**
  - **描述**：
    - 保存玩家数据，保存成功后触发回调 `OnPlayerSaved`
  - **参数**：playerid - 玩家 ID。
  - **返回**：玩家未登录或保存中返回false 执行保存返回true。

```pawn
// 保存是异步
// 正确使用方法
OPCore_Player_SetDataInt(playerid, PLAYER_DATA_MONEY, 500);
OPCore_Player_SetDataInt(playerid, PLAYER_DATA_DEATH, 2);
OPCore_Player_SetDataInt(playerid, PLAYER_DATA_SCORE, 100);
OPCore_Player_Save(playerid); // 设置完后一起保存

// 错误方法 连续设置+保存
OPCore_Player_SetDataInt(playerid, PLAYER_DATA_MONEY, 500);
OPCore_Player_Save(playerid);
OPCore_Player_SetDataInt(playerid, PLAYER_DATA_DEATH, 2);
OPCore_Player_Save(playerid);
OPCore_Player_SetDataInt(playerid, PLAYER_DATA_SCORE, 100);
OPCore_Player_Save(playerid);
```

- **OPCore_Player_GetDBID(playerid)**
  - **描述**：获取账号数据库唯一标识符 (UID)
  - **参数**：playerid - 玩家 ID。
  - **返回**：账号 ID 或 0 如果无效。

- **bool:OPCore_Player_SignIn(playerid)**
  - **描述**：启动玩家登录过程（检查账号存在，显示对话框）。通常在 OnPlayerRequestClass 调用
  - **参数**：playerid - 玩家 ID。
  - **返回**：true 如果启动成功，否则 false。
  - **注意**：内部处理 MySQL 查询和对话框显示, 重复调用不会导致重复登陆

### 玩家数据相关
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

- **bool:OPCore_Player_SetDataFloat(playerid, field, Float:value, bool:save = false)**
  - **描述**：设置 FLOAT 类型字段值，并标记脏位,可选是否设置数据后即可保存。
  - **参数**：同 Get，额外 value - 新值。
  - **返回**：true 如果成功，否则 false。

- **bool:OPCore_Player_SetDataInt(playerid, field, value, bool:save = false)**
  - **描述**：设置 INT 类型字段值，并标记脏位,可选是否设置数据后即可保存。
  - **参数**：同上。
  - **返回**：true 如果成功，否则 false。

- **bool:OPCore_Player_SetDataString(playerid, field, const value[], bool:save = false)**
  - **描述**：设置 STRING 类型字段值，并标记脏位,可选是否设置数据后即可保存。
  - **参数**：同 Get，额外 value[] - 新字符串。
  - **返回**：true 如果成功，否则 false。

- **bool:OPCore_Player_GiveDataFloat(playerid, field, Float:value, bool:save = false)**
  - **描述**：为 FLOAT 字段增加值，并标记脏位,可选是否设置数据后即可保存。
  - **参数**：同 Set。
  - **返回**：true 如果成功，否则 false。

- **bool:OPCore_Player_GiveDataInt(playerid, field, value, bool:save = false)**
  - **描述**：为 INT 字段增加值，并标记脏位,可选是否设置数据后即可保存。
  - **参数**：同 Set。
  - **返回**：true 如果成功，否则 false。


### 其他辅助函数

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