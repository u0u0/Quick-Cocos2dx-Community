#include "lua_cocos2dx_quick_manual.hpp"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"
#include "CCLuaEngine.h"

static int tolua_Cocos2d_Function_loadChunksFromZIP(lua_State* tolua_S)
{
    return LuaEngine::getInstance()->getLuaStack()->luaLoadChunksFromZIP(tolua_S);
}

static void extendFunctions(lua_State* tolua_S)
{
    tolua_module(tolua_S,"cc",0);
    tolua_beginmodule(tolua_S,"cc");
    tolua_function(tolua_S,"LuaLoadChunksFromZIP",tolua_Cocos2d_Function_loadChunksFromZIP);
    tolua_endmodule(tolua_S);
}

int register_all_quick_manual(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;
    
    extendFunctions(tolua_S);

    CCLOG("Quick-Cocos2dx-Community C++ support ready.");
    
    return 0;
}

