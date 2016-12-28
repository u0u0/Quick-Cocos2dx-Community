#include "lua_spSkeletonData.hpp"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"

using namespace spine;

static int lua_cocos2dx_SkeletonData_create(lua_State* L)
{
    if (nullptr == L)
        return 0 ;
    
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(L,1,"sp.SkeletonData",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(L) - 1;
    
    if (2 == argc)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err)  ||
            !tolua_isstring(L, 3 ,0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char* skeletonDataFile = tolua_tostring(L, 2, "");
        const char* atlasFile = tolua_tostring(L, 3, "");;

        spAtlas *atlas = spAtlas_createFromFile(atlasFile, NULL);
        CCAssert(atlas, "Error reading atlas file.");
#define SUPER(VALUE) (&VALUE->super)
        spAttachmentLoader *attachmentLoader = SUPER(Cocos2dAttachmentLoader_create(atlas));
#undef SUPER
        spSkeletonJson *json = spSkeletonJson_createWithLoader(attachmentLoader);
        spSkeletonData *data = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile);
        CCAssert(data, json->error ? json->error : "Error reading skeleton data.");
        spSkeletonJson_dispose(json);
        
        lua_spSkeletonData *luaSpData = new lua_spSkeletonData;
        luaSpData->atlas = atlas;
        luaSpData->data = data;
        luaSpData->attachmentLoader = attachmentLoader;
        
        tolua_pushusertype(L,(void*)luaSpData,"sp.SkeletonData");
        tolua_register_gc(L,lua_gettop(L));
        return 1;
    }
    luaL_error(L, "'create' function of SkeletonData has wrong number of arguments: %d, was expecting %d\n", argc, 2);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L,"#ferror in function 'create'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_SkeletonData_finalize(lua_State* L)
{
    lua_spSkeletonData *luaSpData = static_cast<lua_spSkeletonData *>(tolua_tousertype(L,1,0));
    spSkeletonData_dispose(luaSpData->data);
    spAtlas_dispose(luaSpData->atlas);
    spAttachmentLoader_dispose(luaSpData->attachmentLoader);
    delete luaSpData;
    CCLOG("sp.SkeletonData freed");
    return 0;
}

static void lua_register_spSkeletonData(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"sp.SkeletonData");
    tolua_cclass(tolua_S,"SkeletonData","sp.SkeletonData","",lua_cocos2dx_SkeletonData_finalize);
    
    tolua_beginmodule(tolua_S,"SkeletonData");
        tolua_function(tolua_S,"create",lua_cocos2dx_SkeletonData_create);
    tolua_endmodule(tolua_S);
    
    std::string typeName = typeid(spSkeletonData).name();
    g_luaType[typeName] = "sp.SkeletonData";
    g_typeCast["SkeletonData"] = "sp.SkeletonData";
}

int register_spSkeletonData_manual(lua_State* L)
{
    if (nullptr == L)
        return 0;
    
    tolua_open(L);
    
    tolua_module(L,"sp",0);
    tolua_beginmodule(L,"sp");
    
    lua_register_spSkeletonData(L);
    
    tolua_endmodule(L);
    
    return 0;
}
