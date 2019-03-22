#ifndef __LUA_TEMPLATE_RUNTIME_FRAMEWORKS_RUNTIME_SRC_CLASSES_LUA_MODULE_REGISTER_H__
#define __LUA_TEMPLATE_RUNTIME_FRAMEWORKS_RUNTIME_SRC_CLASSES_LUA_MODULE_REGISTER_H__

#include "audio/lua_audio_manual.h"
#include "network/lua_cocos2dx_network_manual.h"
#include "cocostudio/lua_cocos2dx_coco_studio_manual.hpp"
#include "ui/lua_cocos2dx_ui_manual.hpp"
#include "spine/lua_cocos2dx_spine_manual.hpp"
#include "3d/lua_cocos2dx_3d_manual.h"
#include "lua/quick/lua_cocos2dx_quick_manual.hpp"
#include "dragonBones/lua_dragonBones.hpp"

int lua_module_register(lua_State* L)
{
    //Dont' change the module register order unless you know what your are doing
    register_audio_module(L);
    register_network_module(L);
#if CC_USE_CCSTUDIO
    register_cocostudio_module(L);
#endif
    register_ui_moudle(L);
#if CC_USE_SPINE
    register_spine_module(L);
#endif
#if CC_USE_3D
    register_cocos3d_module(L);
#endif
#if CC_USE_DRAGONBONES
    register_dragonBones_manual(L);
#endif
    return 1;
}

#endif  // __LUA_TEMPLATE_RUNTIME_FRAMEWORKS_RUNTIME_SRC_CLASSES_LUA_MODULE_REGISTER_H__

