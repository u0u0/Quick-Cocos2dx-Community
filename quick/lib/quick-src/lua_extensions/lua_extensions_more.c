
#include "lua_extensions_more.h"

#if __cplusplus
extern "C" {
#endif

// cjson
#include "cjson/lua_cjson.h"

// zlib
#include "zlib/lua_zlib.h"

// lpack
#include "lpack/lpack.h"

// filesystem
#include "filesystem/lfs.h"

// lsqlite3
#include "lsqlite3/lsqlite3.h"

// lunqlite3
#include "unqlite/lunqlite.h"

// protoc-gen-lua
#include "protobuf/pb.h"

#if CC_USE_SPROTO
LUALIB_API int luaopen_lpeg (lua_State *L);
LUALIB_API int luaopen_sproto_core(lua_State *L);
#endif

static luaL_Reg luax_exts[] = {
    {"cjson", luaopen_cjson_safe},
    {"zlib", luaopen_zlib},
    {"pack", luaopen_pack},
    {"lfs", luaopen_lfs},
#if CC_USE_SQLITE
    {"lsqlite3", luaopen_lsqlite3},
#endif
#if CC_USE_UNQLITE
	{"unqlite", luaopen_lunqlite},
#endif
#if CC_USE_PROTOBUF
    {"pb", luaopen_pb},
#endif
#if CC_USE_SPROTO
    {"lpeg", luaopen_lpeg},
    {"sproto.core", luaopen_sproto_core},
#endif
    {NULL, NULL}
};

void luaopen_lua_extensions_more(lua_State *L)
{
    // load extensions
    luaL_Reg* lib = luax_exts;
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    for (; lib->func; lib++)
    {
        lua_pushcfunction(L, lib->func);
        lua_setfield(L, -2, lib->name);
    }
	
    lua_pop(L, 2);
}

#if __cplusplus
} // extern "C"
#endif
