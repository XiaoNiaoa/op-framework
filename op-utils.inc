#if defined _INC_OPCORE_UTILS
    #endinput
#endif
#define _INC_OPCORE_UTILS

// 依赖环境
#if !defined _INC_open_mp
    #error 请先包含 open.mp
#endif

#include <op-framework/op-core>

// 结构
#include <op-framework/op-utils/sound>
#include <op-framework/op-utils/interior>