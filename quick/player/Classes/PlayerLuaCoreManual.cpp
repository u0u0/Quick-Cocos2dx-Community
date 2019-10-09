#include "PlayerLuaCoreManual.hpp"

#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"
#include "CCLuaEngine.h"
#include "PlayerProtocol.h"

USING_NS_CC;
using namespace player;

/* method: getFrameSize of class  ProjectConfig */
#ifndef TOLUA_DISABLE_tolua_PlayerLuaCore_ProjectConfig_getFrameSize00
static int tolua_PlayerLuaCore_ProjectConfig_getFrameSize00(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"const ProjectConfig",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        const ProjectConfig* self = (const ProjectConfig*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFrameSize'", NULL);
#endif
        {
            int argc = 0;
            argc = lua_gettop(tolua_S)-1;
            if (argc == 0)
            {
                cocos2d::Size tolua_ret = (cocos2d::Size)  self->getFrameSize();
                size_to_luaval(tolua_S, tolua_ret);
                return 1;
            }
            CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getFrameSize",argc, 0);
            return 0;
        }
    }
    return 1;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getFrameSize'.",&tolua_err);
    return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFrameSize of class  ProjectConfig */
#ifndef TOLUA_DISABLE_tolua_PlayerLuaCore_ProjectConfig_setFrameSize00
static int tolua_PlayerLuaCore_ProjectConfig_setFrameSize00(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"ProjectConfig",0,&tolua_err) ||
        (tolua_isvaluenil(tolua_S,2,&tolua_err))
        )
        goto tolua_lerror;
    else
#endif
    {
        ProjectConfig* self = (ProjectConfig*)  tolua_tousertype(tolua_S,1,0);
        int argc = 0;
        bool ok  = true;
        argc = lua_gettop(tolua_S)-1;
        
        if (1 == argc)
        {
            cocos2d::Size size;
            ok &= luaval_to_size(tolua_S, 2, &size);
            if (!ok)
                return 0;
            
            self->setFrameSize(size);
            return 0;
        }
        else if(2 == argc)
        {
            double width;
            ok &= luaval_to_number(tolua_S, 2,&width);
            
            if (!ok)
                return 0;
            
            double height;
            ok &= luaval_to_number(tolua_S, 3,&height);
            
            if (!ok)
                return 0;
            
            self->setFrameSize(Size(width, height));
            return 0;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setFrameSize'.",&tolua_err);
    return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

int luaopen_PlayerLuaCore_Manual(lua_State* tolua_S)
{
    // engine version
    lua_pushstring(tolua_S, cocos2dVersion());
    lua_setglobal(tolua_S, "__VERSION__");
    
    // ProjectConfig
    tolua_beginmodule(tolua_S,"ProjectConfig");
        tolua_function(tolua_S,"getFrameSize",tolua_PlayerLuaCore_ProjectConfig_getFrameSize00);
        tolua_function(tolua_S,"setFrameSize",tolua_PlayerLuaCore_ProjectConfig_setFrameSize00);
    tolua_endmodule(tolua_S);

    return 0;
}
