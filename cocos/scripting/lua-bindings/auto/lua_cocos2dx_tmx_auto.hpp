#include "base/ccConfig.h"

#ifndef __cocos2dx_tmx_h__
#define __cocos2dx_tmx_h__

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int lua_register_cocos2dx_TMXObjectGroup(lua_State* tolua_S);
int lua_register_cocos2dx_TMXLayerInfo(lua_State* tolua_S);
int lua_register_cocos2dx_TMXTilesetInfo(lua_State* tolua_S);
int lua_register_cocos2dx_TMXMapInfo(lua_State* tolua_S);
int lua_register_cocos2dx_TMXLayer(lua_State* tolua_S);
int lua_register_cocos2dx_TMXTiledMap(lua_State* tolua_S);

#endif
