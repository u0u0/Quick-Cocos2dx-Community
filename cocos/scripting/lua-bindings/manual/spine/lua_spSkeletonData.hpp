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

#include "spine-cocos2dx.h"

typedef struct{
    spAtlas *atlas;
    spSkeletonData *data;
    spAttachmentLoader *attachmentLoader;
}lua_spSkeletonData;

TOLUA_API int register_spSkeletonData_manual(lua_State* L);

#endif // #ifndef LUA_SPSKELETONDATA_H
