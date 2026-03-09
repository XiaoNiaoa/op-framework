#if defined _INC_OPCORE_PLAYER
    #endinput
#endif
#define _INC_OPCORE_PLAYER

// 依赖环境
#if !defined _INC_open_mp
    #error "请先包含 open.mp"
#endif

#if !defined mysql_included
    #error "请先包含 a_mysql"
#endif

#if !defined _samp_bcrypt_included
    #error "请先包含 samp_bcrypt"
#endif

#if !defined _INC_OPCORE
    #error "请先包含 op-framework/op-core"
#endif

// 结构
#include <op-framework/op-player/player-account>
#include <op-framework/op-player/player>