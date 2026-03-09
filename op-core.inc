#if defined _INC_OPCORE
    #endinput
#endif
#define _INC_OPCORE

// 依赖环境
#if !defined _INC_open_mp
    #error "请先包含 open.mp"
#endif

#if !defined mysql_included
    #error "请先包含 a_mysql"
#endif

#if !defined _FOREACH_LOCAL_VERSION
    #error "请先包含 foreach"
#endif

// 结构
#define OPCORE_TEST_DEBUG false

#include <op-framework/op-core/debug>
#include <op-framework/op-core/mysql>

#if !defined OPCORE_DISABLE_DYNAMIC_SERVER_INFO
    #include <op-framework/op-core/server>
#endif

#include <op-framework/op-core/fix>