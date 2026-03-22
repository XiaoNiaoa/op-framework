#define OPCore_Debug(%0,%1) printf("[OPCore::"#%0"] " %1)
#define OPCore_Error(%0,%1) printf("[OPCore::"#%0" Error] " %1)

#if OPCORE_TEST_DEBUG
    #define OPCore_TestDebug(%0,%1) printf("* [OPCore::"#%0"] " %1)
    #define OPCore_TestError(%0,%1) printf("* [OPCore::"#%0" Error] " %1)
#else
    #define OPCore_TestDebug(%0,%1)
    #define OPCore_TestError(%0,%1)
#endif