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

## 示例

可根据需求在包含此模块前定义密码长度范围，默认是 4 - 16
```pawn
   #define MIN_PASSWORD_LENGTH  4
   #define MAX_PASSWORD_LENGTH  16

   #include <op-framework/op-player>
```
简单通过指令示例

```pawn
#include <open.mp>
#include <a_mysql>
#include <samp_bcrypt>
#include <foreach>
#include <Pawn.CMD>
#include <sscanf2>

#include <op-framework/op-core>
#include <op-framework/op-player>

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

        case SIGNIN_DATA_LOADED_FINISH: SendClientMessage(playerid, 0xFFFF00FF, "数据加载完成");
    }
    return 1;
}
```

## 回调以及状态码定义

**OnPlayerSignIn(playerid, result)**

| result | 描述 |
| :--- | :--- |
| `SIGNIN_LOGIN_SUCCESS` | 登录成功 |
| `SIGNIN_REGISTER_SUCCESS` | 注册成功 |
| `SIGNIN_INCORRECT_PASSWORD` | 密码错误或不符合规范 |
| `SIGNIN_DATABASE_ERROR` | 数据库执行异常 |
| `SIGNIN_ALREADY_LOGGED_IN` | 已登录过 |
| `SIGNIN_LOADING` | 账号数据正在异步加载中 |
| `SIGNIN_UNREGISTERED` | 账号不存在 |
| `SIGNIN_ALREADY_REGISTERED` | 账号已存在 |
| `SIGNIN_DATA_LOADED_FINISH` | 玩家数据加载完成 |

## 函数列表

### 身份校验

- **OPCore_Player_IsValid(playerid)**
  - **描述**：
    - 检查玩家是否是有效的玩家 条件是已登录 并且数据已加载 可以进行数据操作
    - 任何关于玩家有效性的判断应该以此为检测标准 而不是 `OPCore_Player_IsLoggedIn`
    - `OPCore_Player_IsLoggedIn`只是检测是否登录 可能在登录成功的瞬间 `0.01ms`内数据还没有加载完成 (单线程/异步)
  - **参数**：playerid - 玩家 ID。
  - **返回**：true 有效 false 无效

- **bool:OPCore_Player_IsRegister(playerid)**
  - **描述**：
      - 检查玩家是否存在于数据库中 常用于判断是给玩家显示登录窗口或注册窗口
      - 即便不检测也可以直接使用 `OPCore_Player_SignIn` 系统会自动判断
  - **参数**：playerid - 玩家 ID。
  - **返回**：true 数据库存在该玩家账户 false 不存在

- **OPCore_Player_IsLoggedIn(playerid)**
  - **描述**：
    - 检查玩家是否登录 且非 NPC
    - 此函数在实际开发中用不到，但保留
  - **参数**：playerid - 玩家 ID。
  - **返回**：true 已登录 false 未登录

- **bool:OPCore_Player_IsLoading(playerid)**
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
- **OPCore_Player_GetDBID(playerid)**
  - **描述**：获取账号数据库唯一标识符 (UID)
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
