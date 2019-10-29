// dragonBones Lua binding, since Quick-Cocos2dx-Community 3.7.6

#include "lua_dragonBones.hpp"
#include "cocos2dx/CCDragonBonesHeaders.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "LuaScriptHandlerMgr.h"
#include "CCLuaEngine.h"
#include "CCLuaValue.h"

static int lua_dragonBones_CCFactory_loadDragonBonesData(lua_State* L)
{
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(L, 1, "dragonBones.CCFactory", 0, &tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *filePath = tolua_tostring(L, 2, "");
        const char *name = tolua_tostring(L, 3, "");
        float scale = tolua_tonumber(L, 4, 1.0);
        dragonBones::CCFactory::getFactory()->loadDragonBonesData(filePath, name, scale);
        return 0;
    }
    luaL_error(L, "'loadDragonBonesData' function of dragonBones.CCFactory has wrong number of arguments: %d, was expecting 1~3\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'loadDragonBonesData'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_CCFactory_loadTextureAtlasData(lua_State* L)
{
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(L, 1, "dragonBones.CCFactory", 0, &tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *filePath = tolua_tostring(L, 2, "");
        const char *name = tolua_tostring(L, 3, "");
        float scale = tolua_tonumber(L, 4, 1.0);
        dragonBones::CCFactory::getFactory()->loadTextureAtlasData(filePath, name, scale);
        return 0;
    }
    luaL_error(L, "'loadTextureAtlasData' function of dragonBones.CCFactory has wrong number of arguments: %d, was expecting 1~3\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'loadTextureAtlasData'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_CCFactory_buildArmatureDisplay(lua_State* L)
{
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(L, 1, "dragonBones.CCFactory", 0, &tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *armatureName = tolua_tostring(L, 2, "");
        const char *dragonBonesName = tolua_tostring(L, 3, "");
        const char *skinName = tolua_tostring(L, 4, "");
        const char *textureAtlasName = tolua_tostring(L, 5, "");
        auto tolua_ret = dragonBones::CCFactory::getFactory()->buildArmatureDisplay(armatureName, dragonBonesName, skinName, textureAtlasName);
        
        int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(L, nID, pLuaID, (void*)tolua_ret, "dragonBones.CCArmatureDisplay");
        return 1;
    }
    luaL_error(L, "'buildArmatureDisplay' function of dragonBones.CCFactory has wrong number of arguments: %d, was expecting 1~4\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'buildArmatureDisplay'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_CCFactory_removeDragonBonesData(lua_State* L)
{
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(L, 1, "dragonBones.CCFactory", 0, &tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *name = tolua_tostring(L, 2, "");
        int disposeData = tolua_toboolean(L, 3, 1);
        dragonBones::CCFactory::getFactory()->removeDragonBonesData(name, disposeData);
        return 0;
    }
    luaL_error(L, "'removeDragonBonesData' function of dragonBones.CCFactory has wrong number of arguments: %d, was expecting 1~2\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'removeDragonBonesData'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_CCFactory_removeTextureAtlasData(lua_State* L)
{
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(L, 1, "dragonBones.CCFactory", 0, &tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *name = tolua_tostring(L, 2, "");
        int disposeData = tolua_toboolean(L, 3, 1);
        dragonBones::CCFactory::getFactory()->removeTextureAtlasData(name, disposeData);
        return 0;
    }
    luaL_error(L, "'removeTextureAtlasData' function of dragonBones.CCFactory has wrong number of arguments: %d, was expecting 1~2\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'removeTextureAtlasData'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_CCFactory_clear(lua_State* L)
{
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(L, 1, "dragonBones.CCFactory", 0, &tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        dragonBones::CCFactory::getFactory()->clear();
        return 0;
    }
    luaL_error(L, "'clear' function of dragonBones.CCFactory has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'clear'.", &tolua_err);
#endif
    return 0;
}

int lua_register_dragonBones_CCFactory(lua_State* tolua_S)
{
    tolua_usertype(tolua_S, "dragonBones.CCFactory");
    tolua_cclass(tolua_S,"CCFactory", "dragonBones.CCFactory", "", nullptr);
    
    tolua_beginmodule(tolua_S,"CCFactory");
        tolua_function(tolua_S,"loadDragonBonesData", lua_dragonBones_CCFactory_loadDragonBonesData);
        tolua_function(tolua_S,"loadTextureAtlasData", lua_dragonBones_CCFactory_loadTextureAtlasData);
        tolua_function(tolua_S,"buildArmatureDisplay", lua_dragonBones_CCFactory_buildArmatureDisplay);
        tolua_function(tolua_S,"removeDragonBonesData", lua_dragonBones_CCFactory_removeDragonBonesData);
        tolua_function(tolua_S,"removeTextureAtlasData", lua_dragonBones_CCFactory_removeTextureAtlasData);
        tolua_function(tolua_S,"clear", lua_dragonBones_CCFactory_clear);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(dragonBones::CCFactory).name();
    g_luaType[typeName] = "dragonBones.CCFactory";
    g_typeCast["CCFactory"] = "dragonBones.CCFactory";
    return 1;
}

static int lua_dragonBones_CCArmatureDisplay_getArmature(lua_State* L)
{
    int argc = 0;
    dragonBones::CCArmatureDisplay *display = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.CCArmatureDisplay", 0, &tolua_err)) goto tolua_lerror;
#endif
    display = static_cast<dragonBones::CCArmatureDisplay *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        auto tolua_ret = display->getArmature();
        tolua_pushusertype(L, (void*)tolua_ret, "dragonBones.Armature");
        return 1;
    }
    luaL_error(L, "'getArmature' function of dragonBones.CCArmatureDisplay has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getArmature'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_CCArmatureDisplay_getAnimation(lua_State* L)
{
    int argc = 0;
    dragonBones::CCArmatureDisplay *display = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.CCArmatureDisplay", 0, &tolua_err)) goto tolua_lerror;
#endif
    display = static_cast<dragonBones::CCArmatureDisplay *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        auto tolua_ret = display->getAnimation();
        tolua_pushusertype(L, (void*)tolua_ret, "dragonBones.Animation");
        return 1;
    }
    luaL_error(L, "'getAnimation' function of dragonBones.CCArmatureDisplay has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getAnimation'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_CCArmatureDisplay_getBoundingBox(lua_State* L)
{
    int argc = 0;
    dragonBones::CCArmatureDisplay *display = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.CCArmatureDisplay", 0, &tolua_err)) goto tolua_lerror;
#endif
    display = static_cast<dragonBones::CCArmatureDisplay *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        auto ret = display->getBoundingBox();
        rect_to_luaval(L, ret);
        return 1;
    }
    luaL_error(L, "'getBoundingBox' function of dragonBones.CCArmatureDisplay has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getBoundingBox'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_CCArmatureDisplay_addDBEventListener(lua_State* L)
{
    int argc = 0;
    dragonBones::CCArmatureDisplay *display = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.CCArmatureDisplay", 0, &tolua_err)) goto tolua_lerror;
#endif
    display = static_cast<dragonBones::CCArmatureDisplay *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 2)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err)
            || !toluafix_isfunction(L, 3, "LUA_FUNCTION", 0, &tolua_err)) {
            tolua_error(L, "#ferror in function 'addDBEventListener'.", &tolua_err);
            return 0;
        }
#endif
        const char *eventName = (const char*)tolua_tostring(L, 2, 0);
#if COCOS2D_DEBUG >= 1
        // check eventName valid
        if (strcmp(dragonBones::EventObject::START, eventName) != 0
            && strcmp(dragonBones::EventObject::LOOP_COMPLETE, eventName) != 0
            && strcmp(dragonBones::EventObject::COMPLETE, eventName) != 0
            && strcmp(dragonBones::EventObject::FADE_IN, eventName) != 0
            && strcmp(dragonBones::EventObject::FADE_IN_COMPLETE, eventName) != 0
            && strcmp(dragonBones::EventObject::FADE_OUT, eventName) != 0
            && strcmp(dragonBones::EventObject::FADE_OUT_COMPLETE, eventName) != 0
            && strcmp(dragonBones::EventObject::FRAME_EVENT, eventName) != 0
            && strcmp(dragonBones::EventObject::SOUND_EVENT, eventName) != 0) {
            luaL_error(L, "error in function 'addDBEventListener'. invalid eventName");
            return 0;
        }
#endif
        int handler = (toluafix_ref_function(L, 3, 0));
        display->addDBEventListener(eventName, [=](dragonBones::EventObject *object) {
            lua_newtable(L);
            
            lua_pushstring(L, "type");
            lua_pushstring(L, object->type.c_str());
            lua_rawset(L, -3);
            
            lua_pushstring(L, "animationState");
            tolua_pushusertype(L, (void*)object->animationState, "dragonBones.AnimationState");
            lua_rawset(L, -3);
            // call lua func
            LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 1);
        });
        // mark: addCustomHandler can not by cancel one by one, so skip removeDBEventListener
        ScriptHandlerMgr::getInstance()->addCustomHandler((void*)display, handler);
        return 0;
    }
    luaL_error(L, "'addDBEventListener' function of dragonBones.CCArmatureDisplay has wrong number of arguments: %d, was expecting 2\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'addDBEventListener'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_CCArmatureDisplay_dispose(lua_State* L)
{
    int argc = 0;
    dragonBones::CCArmatureDisplay *display = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.CCArmatureDisplay", 0, &tolua_err)) goto tolua_lerror;
#endif
    display = static_cast<dragonBones::CCArmatureDisplay *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        display->dispose();
        return 0;
    }
    luaL_error(L, "'dispose' function of dragonBones.CCArmatureDisplay has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'dispose'.", &tolua_err);
#endif
    return 0;
}

int lua_register_dragonBones_CCArmatureDisplay(lua_State* tolua_S)
{
    tolua_usertype(tolua_S, "dragonBones.CCArmatureDisplay");
    tolua_cclass(tolua_S, "CCArmatureDisplay", "dragonBones.CCArmatureDisplay", "cc.Node", nullptr);
    
    tolua_beginmodule(tolua_S, "CCArmatureDisplay");
        tolua_function(tolua_S, "getArmature", lua_dragonBones_CCArmatureDisplay_getArmature);
        tolua_function(tolua_S, "getAnimation", lua_dragonBones_CCArmatureDisplay_getAnimation);
        tolua_function(tolua_S, "getBoundingBox", lua_dragonBones_CCArmatureDisplay_getBoundingBox);
        tolua_function(tolua_S, "addDBEventListener", lua_dragonBones_CCArmatureDisplay_addDBEventListener);
        tolua_function(tolua_S, "dispose", lua_dragonBones_CCArmatureDisplay_dispose);
    tolua_endmodule(tolua_S);
    
    std::string typeName = typeid(dragonBones::CCArmatureDisplay).name();
    g_luaType[typeName] = "dragonBones.CCArmatureDisplay";
    g_typeCast["CCArmatureDisplay"] = "dragonBones.CCArmatureDisplay";
    return 1;
}

static int lua_dragonBones_Armature_setFlipX(lua_State* L)
{
    int argc = 0;
    dragonBones::Armature *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Armature", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Armature *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isboolean(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        int isFlip = tolua_toboolean(L, 2, 0);
        obj->setFlipX(isFlip);
        return 0;
    }
    luaL_error(L, "'setFlipX' function of dragonBones.Armature has wrong number of arguments: %d, was expecting 1\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'setFlipX'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Armature_getFlipX(lua_State* L)
{
    int argc = 0;
    dragonBones::Armature *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Armature", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Armature *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushboolean(L, obj->getFlipX());
        return 1;
    }
    luaL_error(L, "'getFlipX' function of dragonBones.Armature has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getFlipX'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Armature_setFlipY(lua_State* L)
{
    int argc = 0;
    dragonBones::Armature *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Armature", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Armature *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isboolean(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        int isFlip = tolua_toboolean(L, 2, 0);
        obj->setFlipY(isFlip);
        return 0;
    }
    luaL_error(L, "'setFlipY' function of dragonBones.Armature has wrong number of arguments: %d, was expecting 1\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'setFlipY'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Armature_getFlipY(lua_State* L)
{
    int argc = 0;
    dragonBones::Armature *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Armature", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Armature *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushboolean(L, obj->getFlipY());
        return 1;
    }
    luaL_error(L, "'getFlipY' function of dragonBones.Armature has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getFlipY'.", &tolua_err);
#endif
    return 0;
}

int lua_register_dragonBones_Armature(lua_State* tolua_S)
{
    tolua_usertype(tolua_S, "dragonBones.Armature");
    tolua_cclass(tolua_S, "Armature", "dragonBones.Armature", "", nullptr);
    
    tolua_beginmodule(tolua_S, "Armature");
        tolua_function(tolua_S, "setFlipX", lua_dragonBones_Armature_setFlipX);
        tolua_function(tolua_S, "getFlipX", lua_dragonBones_Armature_getFlipX);
        tolua_function(tolua_S, "setFlipY", lua_dragonBones_Armature_setFlipY);
        tolua_function(tolua_S, "getFlipY", lua_dragonBones_Armature_getFlipY);
    tolua_endmodule(tolua_S);
    
    std::string typeName = typeid(dragonBones::Armature).name();
    g_luaType[typeName] = "dragonBones.Armature";
    g_typeCast["Armature"] = "dragonBones.Armature";
    return 1;
}

static int lua_dragonBones_Animation_reset(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        obj->reset();
        return 0;
    }
    luaL_error(L, "'reset' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'reset'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_stop(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *name= tolua_tostring(L, 2, "");
        obj->stop(name);
        return 0;
    }
    luaL_error(L, "'stop' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 1\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'stop'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_play(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *name= tolua_tostring(L, 2, "");
        int times = (int)tolua_tonumber(L, 3, -1);
        auto tolua_ret = obj->play(name, times);
        tolua_pushusertype(L, (void*)tolua_ret, "dragonBones.AnimationState");
        return 1;
    }
    luaL_error(L, "'play' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 1~2\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'play'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_fadeIn(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *name = tolua_tostring(L, 2, "");
        float fadeInTime = (float)tolua_tonumber(L, 3, -1);
        int playTimes = (int)tolua_tonumber(L, 4, -1);
        int layer = (int)tolua_tonumber(L, 5, 0);
        const char *group = tolua_tostring(L, 6, "");
        int mode = (int)tolua_tonumber(L, 7, 0);
        dragonBones::AnimationFadeOutMode fadeOutMode = (dragonBones::AnimationFadeOutMode)mode;
        if (dragonBones::AnimationFadeOutMode::None == fadeOutMode) {
            fadeOutMode = dragonBones::AnimationFadeOutMode::SameLayerAndGroup;
        }
        
        auto tolua_ret = obj->fadeIn(name, fadeInTime, playTimes, layer, group, fadeOutMode);
        tolua_pushusertype(L, (void*)tolua_ret, "dragonBones.AnimationState");
        return 1;
    }
    luaL_error(L, "'fadeIn' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 1~6\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'fadeIn'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_gotoAndPlayByTime(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *name = tolua_tostring(L, 2, "");
        float time = (float)tolua_tonumber(L, 3, 0.f);
        int playTimes = (int)tolua_tonumber(L, 4, -1);
        auto tolua_ret = obj->gotoAndPlayByTime(name, time, playTimes);
        tolua_pushusertype(L, (void*)tolua_ret, "dragonBones.AnimationState");
        return 1;
    }
    luaL_error(L, "'gotoAndPlayByTime' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 1~3\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'gotoAndPlayByTime'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_gotoAndPlayByFrame(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *name = tolua_tostring(L, 2, "");
        unsigned frame = (unsigned)tolua_tonumber(L, 3, 0);
        int playTimes = (int)tolua_tonumber(L, 4, -1);
        auto tolua_ret = obj->gotoAndPlayByFrame(name, frame, playTimes);
        tolua_pushusertype(L, (void*)tolua_ret, "dragonBones.AnimationState");
        return 1;
    }
    luaL_error(L, "'gotoAndPlayByFrame' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 1~3\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'gotoAndPlayByFrame'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_gotoAndPlayByProgress(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *name = tolua_tostring(L, 2, "");
        float progress = (float)tolua_tonumber(L, 3, 0);
        int playTimes = (int)tolua_tonumber(L, 4, -1);
        auto tolua_ret = obj->gotoAndPlayByProgress(name, progress, playTimes);
        tolua_pushusertype(L, (void*)tolua_ret, "dragonBones.AnimationState");
        return 1;
    }
    luaL_error(L, "'gotoAndPlayByProgress' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 1~3\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'gotoAndPlayByProgress'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_gotoAndStopByTime(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *name = tolua_tostring(L, 2, "");
        float time = (float)tolua_tonumber(L, 3, 0.f);
        auto tolua_ret = obj->gotoAndStopByTime(name, time);
        tolua_pushusertype(L, (void*)tolua_ret, "dragonBones.AnimationState");
        return 1;
    }
    luaL_error(L, "'gotoAndStopByTime' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 1~2\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'gotoAndStopByTime'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_gotoAndStopByFrame(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *name = tolua_tostring(L, 2, "");
        unsigned frame = (unsigned)tolua_tonumber(L, 3, 0);
        auto tolua_ret = obj->gotoAndStopByFrame(name, frame);
        tolua_pushusertype(L, (void*)tolua_ret, "dragonBones.AnimationState");
        return 1;
    }
    luaL_error(L, "'gotoAndStopByFrame' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 1~2\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'gotoAndStopByFrame'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_gotoAndStopByProgress(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *name = tolua_tostring(L, 2, "");
        float progress = (float)tolua_tonumber(L, 3, 0.f);
        auto tolua_ret = obj->gotoAndStopByProgress(name, progress);
        tolua_pushusertype(L, (void*)tolua_ret, "dragonBones.AnimationState");
        return 1;
    }
    luaL_error(L, "'gotoAndStopByProgress' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 1~2\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'gotoAndStopByProgress'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_getState(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *animationName = tolua_tostring(L, 2, "");
        auto tolua_ret = obj->getState(animationName);
        tolua_pushusertype(L, (void*)tolua_ret, "dragonBones.AnimationState");
        return 1;
    }
    luaL_error(L, "'getState' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 1\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getState'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_hasAnimation(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *animationName = tolua_tostring(L, 2, "");
        int tolua_ret = obj->hasAnimation(animationName);
        lua_pushboolean(L, tolua_ret);
        return 1;
    }
    luaL_error(L, "'hasAnimation' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 1\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'hasAnimation'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_getStates(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_newtable(L);
        int index = 1;
        for (const auto animationState : obj->getStates())
        {
            tolua_pushusertype(L, (void*)animationState, "dragonBones.AnimationState");
            lua_rawseti(L, -2, index++);
        }
        return 1;
    }
    luaL_error(L, "'getStates' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getStates'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_isPlaying(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushboolean(L, (int)obj->isPlaying());
        return 1;
    }
    luaL_error(L, "'isPlaying' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'isPlaying'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_isCompleted(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushboolean(L, (int)obj->isCompleted());
        return 1;
    }
    luaL_error(L, "'isCompleted' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'isCompleted'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_getLastAnimationName(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushstring(L, (const char *)obj->getLastAnimationName().c_str());
        return 1;
    }
    luaL_error(L, "'getLastAnimationName' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getLastAnimationName'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_getAnimationNames(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_newtable(L);
        int index = 1;
        for (const auto name : obj->getAnimationNames())
        {
            lua_pushstring(L, name.c_str());
            lua_rawseti(L, -2, index++);
        }
        return 1;
    }
    luaL_error(L, "'getAnimationNames' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getAnimationNames'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_Animation_getLastAnimationState(lua_State* L)
{
    int argc = 0;
    dragonBones::Animation *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.Animation", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::Animation *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        tolua_pushusertype(L, (void*)obj->getLastAnimationState(), "dragonBones.AnimationState");
        return 1;
    }
    luaL_error(L, "'getLastAnimationState' function of dragonBones.Animation has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getLastAnimationState'.", &tolua_err);
#endif
    return 0;
}

int lua_register_dragonBones_Animation(lua_State* tolua_S)
{
    tolua_usertype(tolua_S, "dragonBones.Animation");
    tolua_cclass(tolua_S, "Animation", "dragonBones.Animation", "", nullptr);
    
    tolua_beginmodule(tolua_S, "Animation");
        tolua_function(tolua_S, "reset", lua_dragonBones_Animation_reset);
        tolua_function(tolua_S, "stop", lua_dragonBones_Animation_stop);
        tolua_function(tolua_S, "play", lua_dragonBones_Animation_play);
        tolua_function(tolua_S, "fadeIn", lua_dragonBones_Animation_fadeIn);
        tolua_function(tolua_S, "gotoAndPlayByTime", lua_dragonBones_Animation_gotoAndPlayByTime);
        tolua_function(tolua_S, "gotoAndPlayByFrame", lua_dragonBones_Animation_gotoAndPlayByFrame);
        tolua_function(tolua_S, "gotoAndPlayByProgress", lua_dragonBones_Animation_gotoAndPlayByProgress);
        tolua_function(tolua_S, "gotoAndStopByTime", lua_dragonBones_Animation_gotoAndStopByTime);
        tolua_function(tolua_S, "gotoAndStopByFrame", lua_dragonBones_Animation_gotoAndStopByFrame);
        tolua_function(tolua_S, "gotoAndStopByProgress", lua_dragonBones_Animation_gotoAndStopByProgress);
        tolua_function(tolua_S, "getState", lua_dragonBones_Animation_getState);
        tolua_function(tolua_S, "hasAnimation", lua_dragonBones_Animation_hasAnimation);
        tolua_function(tolua_S, "getStates", lua_dragonBones_Animation_getStates);
        tolua_function(tolua_S, "isPlaying", lua_dragonBones_Animation_isPlaying);
        tolua_function(tolua_S, "isCompleted", lua_dragonBones_Animation_isCompleted);
        tolua_function(tolua_S, "getLastAnimationName", lua_dragonBones_Animation_getLastAnimationName);
        tolua_function(tolua_S, "getAnimationNames", lua_dragonBones_Animation_getAnimationNames);
        tolua_function(tolua_S, "getLastAnimationState", lua_dragonBones_Animation_getLastAnimationState);
    tolua_endmodule(tolua_S);
    
    std::string typeName = typeid(dragonBones::Animation).name();
    g_luaType[typeName] = "dragonBones.Animation";
    g_typeCast["Animation"] = "dragonBones.Animation";
    return 1;
}

static int lua_dragonBones_AnimationState_play(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        obj->play();
        return 0;
    }
    luaL_error(L, "'play' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'play'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_stop(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        obj->stop();
        return 0;
    }
    luaL_error(L, "'stop' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'stop'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_fadeOut(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isnumber(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        float fadeOutTime = (float)tolua_tonumber(L, 2, 0);
        bool pausePlayhead = (float)tolua_toboolean(L, 3, 1);
        obj->fadeOut(fadeOutTime, pausePlayhead);
        return 0;
    }
    luaL_error(L, "'fadeOut' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 1~2\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'fadeOut'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_containsBoneMask(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *boneName = tolua_tostring(L, 2, "");
        lua_pushboolean(L, obj->containsBoneMask(boneName));
        return 1;
    }
    luaL_error(L, "'containsBoneMask' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 1\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'containsBoneMask'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_addBoneMask(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *boneName = tolua_tostring(L, 2, "");
        int recursive = (float)tolua_toboolean(L, 3, true);
        obj->addBoneMask(boneName, recursive);
        return 0;
    }
    luaL_error(L, "'addBoneMask' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 1~2\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'addBoneMask'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_removeBoneMask(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc >= 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char *boneName = tolua_tostring(L, 2, "");
        int recursive = (float)tolua_toboolean(L, 3, true);
        obj->removeBoneMask(boneName, recursive);
        return 0;
    }
    luaL_error(L, "'removeBoneMask' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 1~2\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'removeBoneMask'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_removeAllBoneMask(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        obj->removeAllBoneMask();
        return 0;
    }
    luaL_error(L, "'removeAllBoneMask' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'removeAllBoneMask'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_setResetToPose(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isboolean(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        int resetToPose = tolua_toboolean(L, 2, true);
        obj->resetToPose = resetToPose;
        return 0;
    }
    luaL_error(L, "'setResetToPose' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 1\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'setResetToPose'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_setAutoFadeOutTime(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isnumber(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        float autoFadeOutTime = (float)tolua_tonumber(L, 2, true);
        obj->autoFadeOutTime = autoFadeOutTime;
        return 0;
    }
    luaL_error(L, "'setAutoFadeOutTime' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 1\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'setAutoFadeOutTime'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_setTimeScale(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isnumber(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        float timeScale = (float)tolua_tonumber(L, 2, true);
        obj->timeScale = timeScale;
        return 0;
    }
    luaL_error(L, "'setTimeScale' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 1\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'setTimeScale'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_isFadeIn(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushboolean(L, (int)obj->isFadeIn());
        return 1;
    }
    luaL_error(L, "'isFadeIn' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'isFadeIn'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_isFadeOut(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushboolean(L, (int)obj->isFadeOut());
        return 1;
    }
    luaL_error(L, "'isFadeOut' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'isFadeOut'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_isFadeComplete(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushboolean(L, (int)obj->isFadeComplete());
        return 1;
    }
    luaL_error(L, "'isFadeComplete' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'isFadeComplete'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_isPlaying(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushboolean(L, (int)obj->isPlaying());
        return 1;
    }
    luaL_error(L, "'isPlaying' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'isPlaying'.", &tolua_err);
#endif
    return 0;
}


static int lua_dragonBones_AnimationState_isCompleted(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushboolean(L, (int)obj->isCompleted());
        return 1;
    }
    luaL_error(L, "'isCompleted' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'isCompleted'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_getCurrentPlayTimes(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushnumber(L, obj->getCurrentPlayTimes());
        return 1;
    }
    luaL_error(L, "'getCurrentPlayTimes' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getCurrentPlayTimes'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_getTotalTime(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushnumber(L, obj->getTotalTime());
        return 1;
    }
    luaL_error(L, "'getTotalTime' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getTotalTime'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_getCurrentTime(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushnumber(L, obj->getCurrentTime());
        return 1;
    }
    luaL_error(L, "'getCurrentTime' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getCurrentTime'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_setCurrentTime(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        float time = (float)tolua_tonumber(L, 2, 0);
        obj->setCurrentTime(time);
        return 0;
    }
    luaL_error(L, "'setCurrentTime' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 1\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'setCurrentTime'.", &tolua_err);
#endif
    return 0;
}

static int lua_dragonBones_AnimationState_getName(lua_State* L)
{
    int argc = 0;
    dragonBones::AnimationState *obj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "dragonBones.AnimationState", 0, &tolua_err)) goto tolua_lerror;
#endif
    obj = static_cast<dragonBones::AnimationState *>(tolua_tousertype(L, 1, 0));
    
    argc = lua_gettop(L) - 1;
    if (argc == 0)
    {
        lua_pushstring(L, obj->getName().c_str());
        return 1;
    }
    luaL_error(L, "'getName' function of dragonBones.AnimationState has wrong number of arguments: %d, was expecting 0\n", argc);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'getName'.", &tolua_err);
#endif
    return 0;
}

int lua_register_dragonBones_AnimationState(lua_State* tolua_S)
{
    tolua_usertype(tolua_S, "dragonBones.AnimationState");
    tolua_cclass(tolua_S, "AnimationState", "dragonBones.AnimationState", "", nullptr);
    
    tolua_beginmodule(tolua_S, "AnimationState");
        tolua_function(tolua_S, "play", lua_dragonBones_AnimationState_play);
        tolua_function(tolua_S, "stop", lua_dragonBones_AnimationState_stop);
        tolua_function(tolua_S, "fadeOut", lua_dragonBones_AnimationState_fadeOut);
        tolua_function(tolua_S, "containsBoneMask", lua_dragonBones_AnimationState_containsBoneMask);
        tolua_function(tolua_S, "addBoneMask", lua_dragonBones_AnimationState_addBoneMask);
        tolua_function(tolua_S, "removeBoneMask", lua_dragonBones_AnimationState_removeBoneMask);
        tolua_function(tolua_S, "removeAllBoneMask", lua_dragonBones_AnimationState_removeAllBoneMask);
        tolua_function(tolua_S, "setResetToPose", lua_dragonBones_AnimationState_setResetToPose);
        tolua_function(tolua_S, "setAutoFadeOutTime", lua_dragonBones_AnimationState_setAutoFadeOutTime);
        tolua_function(tolua_S, "setTimeScale", lua_dragonBones_AnimationState_setTimeScale);
        tolua_function(tolua_S, "isFadeIn", lua_dragonBones_AnimationState_isFadeIn);
        tolua_function(tolua_S, "isFadeOut", lua_dragonBones_AnimationState_isFadeOut);
        tolua_function(tolua_S, "isFadeComplete", lua_dragonBones_AnimationState_isFadeComplete);
        tolua_function(tolua_S, "isPlaying", lua_dragonBones_AnimationState_isPlaying);
        tolua_function(tolua_S, "isCompleted", lua_dragonBones_AnimationState_isCompleted);
        tolua_function(tolua_S, "getCurrentPlayTimes", lua_dragonBones_AnimationState_getCurrentPlayTimes);
        tolua_function(tolua_S, "getTotalTime", lua_dragonBones_AnimationState_getTotalTime);
        tolua_function(tolua_S, "getCurrentTime", lua_dragonBones_AnimationState_getCurrentTime);
        tolua_function(tolua_S, "setCurrentTime", lua_dragonBones_AnimationState_setCurrentTime);
        tolua_function(tolua_S, "getName", lua_dragonBones_AnimationState_getName);
    tolua_endmodule(tolua_S);
    
    std::string typeName = typeid(dragonBones::AnimationState).name();
    g_luaType[typeName] = "dragonBones.AnimationState";
    g_typeCast["AnimationState"] = "dragonBones.AnimationState";
    return 1;
}

TOLUA_API int register_dragonBones_manual(lua_State* tolua_S)
{
    tolua_open(tolua_S);
    
    tolua_module(tolua_S, "dragonBones", 0);
    tolua_beginmodule(tolua_S, "dragonBones");
    
    lua_register_dragonBones_AnimationState(tolua_S);
    lua_register_dragonBones_Armature(tolua_S);
    lua_register_dragonBones_Animation(tolua_S);
    lua_register_dragonBones_CCArmatureDisplay(tolua_S);
    lua_register_dragonBones_CCFactory(tolua_S);
    
    tolua_endmodule(tolua_S);
    return 1;
}
