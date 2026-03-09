#if defined _INC_OPCORE_HUD
    #endinput
#endif
#define _INC_OPCORE_HUD

// 依赖环境
#if !defined _INC_open_mp
    #error "请先包含 open.mp"
#endif

// 结构
#include <op-framework/op-hud/hud-gametext>
#include <op-framework/op-hud/hud-race>