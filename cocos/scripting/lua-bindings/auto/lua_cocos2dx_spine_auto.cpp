#include "lua_cocos2dx_spine_auto.hpp"
#include "spine-cocos2dx.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"



int lua_cocos2dx_spine_SkeletonRenderer_setTimeScale(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonRenderer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonRenderer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonRenderer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonRenderer_setTimeScale'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "sp.SkeletonRenderer:setTimeScale");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonRenderer_setTimeScale'", nullptr);
            return 0;
        }
        cobj->setTimeScale(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonRenderer:setTimeScale",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonRenderer_setTimeScale'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_spine_SkeletonRenderer_isOpacityModifyRGB(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonRenderer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonRenderer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonRenderer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonRenderer_isOpacityModifyRGB'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonRenderer_isOpacityModifyRGB'", nullptr);
            return 0;
        }
        bool ret = cobj->isOpacityModifyRGB();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonRenderer:isOpacityModifyRGB",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonRenderer_isOpacityModifyRGB'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_spine_SkeletonRenderer_setDebugSlotsEnabled(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonRenderer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonRenderer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonRenderer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonRenderer_setDebugSlotsEnabled'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "sp.SkeletonRenderer:setDebugSlotsEnabled");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonRenderer_setDebugSlotsEnabled'", nullptr);
            return 0;
        }
        cobj->setDebugSlotsEnabled(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonRenderer:setDebugSlotsEnabled",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonRenderer_setDebugSlotsEnabled'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_spine_SkeletonRenderer_getDebugSlotsEnabled(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonRenderer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonRenderer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonRenderer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonRenderer_getDebugSlotsEnabled'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonRenderer_getDebugSlotsEnabled'", nullptr);
            return 0;
        }
        bool ret = cobj->getDebugSlotsEnabled();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonRenderer:getDebugSlotsEnabled",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonRenderer_getDebugSlotsEnabled'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_spine_SkeletonRenderer_setBonesToSetupPose(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonRenderer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonRenderer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonRenderer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonRenderer_setBonesToSetupPose'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonRenderer_setBonesToSetupPose'", nullptr);
            return 0;
        }
        cobj->setBonesToSetupPose();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonRenderer:setBonesToSetupPose",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonRenderer_setBonesToSetupPose'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_spine_SkeletonRenderer_setSlotsToSetupPose(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonRenderer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonRenderer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonRenderer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonRenderer_setSlotsToSetupPose'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonRenderer_setSlotsToSetupPose'", nullptr);
            return 0;
        }
        cobj->setSlotsToSetupPose();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonRenderer:setSlotsToSetupPose",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonRenderer_setSlotsToSetupPose'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_spine_SkeletonRenderer_setSkin(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonRenderer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonRenderer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonRenderer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonRenderer_setSkin'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "sp.SkeletonRenderer:setSkin");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonRenderer_setSkin'", nullptr);
            return 0;
        }
        bool ret = cobj->setSkin(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonRenderer:setSkin",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonRenderer_setSkin'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_spine_SkeletonRenderer_setToSetupPose(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonRenderer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonRenderer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonRenderer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonRenderer_setToSetupPose'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonRenderer_setToSetupPose'", nullptr);
            return 0;
        }
        cobj->setToSetupPose();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonRenderer:setToSetupPose",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonRenderer_setToSetupPose'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_spine_SkeletonRenderer_setOpacityModifyRGB(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonRenderer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonRenderer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonRenderer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonRenderer_setOpacityModifyRGB'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "sp.SkeletonRenderer:setOpacityModifyRGB");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonRenderer_setOpacityModifyRGB'", nullptr);
            return 0;
        }
        cobj->setOpacityModifyRGB(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonRenderer:setOpacityModifyRGB",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonRenderer_setOpacityModifyRGB'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_spine_SkeletonRenderer_setDebugBonesEnabled(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonRenderer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonRenderer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonRenderer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonRenderer_setDebugBonesEnabled'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "sp.SkeletonRenderer:setDebugBonesEnabled");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonRenderer_setDebugBonesEnabled'", nullptr);
            return 0;
        }
        cobj->setDebugBonesEnabled(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonRenderer:setDebugBonesEnabled",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonRenderer_setDebugBonesEnabled'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_spine_SkeletonRenderer_getDebugBonesEnabled(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonRenderer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonRenderer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonRenderer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonRenderer_getDebugBonesEnabled'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonRenderer_getDebugBonesEnabled'", nullptr);
            return 0;
        }
        bool ret = cobj->getDebugBonesEnabled();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonRenderer:getDebugBonesEnabled",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonRenderer_getDebugBonesEnabled'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_spine_SkeletonRenderer_getTimeScale(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonRenderer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonRenderer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonRenderer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonRenderer_getTimeScale'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonRenderer_getTimeScale'", nullptr);
            return 0;
        }
        double ret = cobj->getTimeScale();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonRenderer:getTimeScale",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonRenderer_getTimeScale'.",&tolua_err);
#endif

    return 0;
}

static int lua_cocos2dx_spine_SkeletonRenderer_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (SkeletonRenderer)");
    return 0;
}

int lua_register_cocos2dx_spine_SkeletonRenderer(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"sp.SkeletonRenderer");
    tolua_cclass(tolua_S,"SkeletonRenderer","sp.SkeletonRenderer","cc.Node",nullptr);

    tolua_beginmodule(tolua_S,"SkeletonRenderer");
        tolua_function(tolua_S,"setTimeScale",lua_cocos2dx_spine_SkeletonRenderer_setTimeScale);
        tolua_function(tolua_S,"isOpacityModifyRGB",lua_cocos2dx_spine_SkeletonRenderer_isOpacityModifyRGB);
        tolua_function(tolua_S,"setDebugSlotsEnabled",lua_cocos2dx_spine_SkeletonRenderer_setDebugSlotsEnabled);
        tolua_function(tolua_S,"getDebugSlotsEnabled",lua_cocos2dx_spine_SkeletonRenderer_getDebugSlotsEnabled);
        tolua_function(tolua_S,"setBonesToSetupPose",lua_cocos2dx_spine_SkeletonRenderer_setBonesToSetupPose);
        tolua_function(tolua_S,"setSlotsToSetupPose",lua_cocos2dx_spine_SkeletonRenderer_setSlotsToSetupPose);
        tolua_function(tolua_S,"setSkin",lua_cocos2dx_spine_SkeletonRenderer_setSkin);
        tolua_function(tolua_S,"setToSetupPose",lua_cocos2dx_spine_SkeletonRenderer_setToSetupPose);
        tolua_function(tolua_S,"setOpacityModifyRGB",lua_cocos2dx_spine_SkeletonRenderer_setOpacityModifyRGB);
        tolua_function(tolua_S,"setDebugBonesEnabled",lua_cocos2dx_spine_SkeletonRenderer_setDebugBonesEnabled);
        tolua_function(tolua_S,"getDebugBonesEnabled",lua_cocos2dx_spine_SkeletonRenderer_getDebugBonesEnabled);
        tolua_function(tolua_S,"getTimeScale",lua_cocos2dx_spine_SkeletonRenderer_getTimeScale);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(spine::SkeletonRenderer).name();
    g_luaType[typeName] = "sp.SkeletonRenderer";
    g_typeCast["SkeletonRenderer"] = "sp.SkeletonRenderer";
    return 1;
}

int lua_cocos2dx_spine_SkeletonAnimation_setMix(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonAnimation* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonAnimation",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonAnimation*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonAnimation_setMix'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        std::string arg1;
        double arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "sp.SkeletonAnimation:setMix");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "sp.SkeletonAnimation:setMix");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "sp.SkeletonAnimation:setMix");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonAnimation_setMix'", nullptr);
            return 0;
        }
        cobj->setMix(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonAnimation:setMix",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonAnimation_setMix'.",&tolua_err);
#endif

    return 0;
}

int lua_cocos2dx_spine_SkeletonAnimation_clearTracks(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonAnimation* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonAnimation",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonAnimation*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonAnimation_clearTracks'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonAnimation_clearTracks'", nullptr);
            return 0;
        }
        cobj->clearTracks();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonAnimation:clearTracks",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonAnimation_clearTracks'.",&tolua_err);
#endif

    return 0;
}
int lua_cocos2dx_spine_SkeletonAnimation_clearTrack(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonAnimation* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonAnimation",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (spine::SkeletonAnimation*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonAnimation_clearTrack'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonAnimation_clearTrack'", nullptr);
            return 0;
        }
        cobj->clearTrack();
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "sp.SkeletonAnimation:clearTrack");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_spine_SkeletonAnimation_clearTrack'", nullptr);
            return 0;
        }
        cobj->clearTrack(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "sp.SkeletonAnimation:clearTrack",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonAnimation_clearTrack'.",&tolua_err);
#endif

    return 0;
}
static int lua_cocos2dx_spine_SkeletonAnimation_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (SkeletonAnimation)");
    return 0;
}

int lua_register_cocos2dx_spine_SkeletonAnimation(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"sp.SkeletonAnimation");
    tolua_cclass(tolua_S,"SkeletonAnimation","sp.SkeletonAnimation","sp.SkeletonRenderer",nullptr);

    tolua_beginmodule(tolua_S,"SkeletonAnimation");
        tolua_function(tolua_S,"setMix",lua_cocos2dx_spine_SkeletonAnimation_setMix);
        tolua_function(tolua_S,"clearTracks",lua_cocos2dx_spine_SkeletonAnimation_clearTracks);
        tolua_function(tolua_S,"clearTrack",lua_cocos2dx_spine_SkeletonAnimation_clearTrack);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(spine::SkeletonAnimation).name();
    g_luaType[typeName] = "sp.SkeletonAnimation";
    g_typeCast["SkeletonAnimation"] = "sp.SkeletonAnimation";
    return 1;
}
TOLUA_API int register_all_cocos2dx_spine(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"sp",0);
	tolua_beginmodule(tolua_S,"sp");

	lua_register_cocos2dx_spine_SkeletonRenderer(tolua_S);
	lua_register_cocos2dx_spine_SkeletonAnimation(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

