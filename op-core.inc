#if defined _INC_OPCORE
    #endinput
#endif
#define _INC_OPCORE

// 依赖环境
#if !defined _INC_open_mp
    #error 请先包含 open.mp
#endif

#tryinclude  <a_mysql>
#tryinclude  <samp_bcrypt>

#if !defined mysql_included
    #error 请先包含 a_mysql
#endif

#if !defined _samp_bcrypt_included
    #error 请先包含 samp_bcrypt
#endif

// 结构
#define OPCORE_TEST_DEBUG false

#include <op-framework/op-core/debug>
#include <op-framework/op-core/fix>

#include <op-framework/op-core/shared/interiors>
#include <op-framework/op-core/shared/skins>
#include <op-framework/op-core/shared/vehicle-category>
#include <op-framework/op-core/shared/vehicle-model>
#include <op-framework/op-core/shared/vehicle-name>
#include <op-framework/op-core/shared/weather>
#include <op-framework/op-core/shared/zone>
#include <op-framework/op-core/shared/zone-popcycle>
#include <op-framework/op-core/shared/zone-type>

#include <op-framework/op-core/mysql>

#if !defined OPCORE_DISABLE_DYNAMIC_SERVER_INFO
    #include <op-framework/op-core/server>
#endif

#include <op-framework/op-core/player/account>
#include <op-framework/op-core/player/data>