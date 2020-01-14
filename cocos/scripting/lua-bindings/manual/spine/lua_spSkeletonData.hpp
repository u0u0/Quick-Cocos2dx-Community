/****************************************************************************
	add by u0u0
 ****************************************************************************/

#ifndef LUA_SPSKELETONDATA_H
#define LUA_SPSKELETONDATA_H

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

#include "editor-support/spine/spine-cocos2dx.h"

typedef struct{
    spine::Atlas *atlas;
    spine::SkeletonData *data;
    spine::Cocos2dTextureLoader textureLoader;
}lua_spSkeletonData;

TOLUA_API int register_spSkeletonData_manual(lua_State* L);

#endif // #ifndef LUA_SPSKELETONDATA_H
