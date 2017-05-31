

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#if __cplusplus
extern "C" {
#endif

#define LUA_LIB
#include "lua.h"
#include "lauxlib.h"
#include "lunqlite.h"
#if LUA_VERSION_NUM > 501
//
// Lua 5.2
//
#define lua_strlen lua_rawlen
// luaL_typerror always used with arg at ndx == NULL
#define luaL_typerror(L,ndx,str) luaL_error(L,"bad argument %d (%s expected, got nil)",ndx,str)
// luaL_register used once, so below expansion is OK for this case
#define luaL_register(L,name,reg) lua_newtable(L);luaL_setfuncs(L,reg,0)
// luaL_openlib always used with name == NULL
#define luaL_openlib(L,name,reg,nup) luaL_setfuncs(L,reg,nup)
#endif
#include "unqlite.h"

static int lunqlite_open(lua_State* L){
	int rc;
	unqlite *pDb;
	const char *filename = luaL_checkstring(L, 1);
	int mod = luaL_checkint(L,2);
	rc = unqlite_open(&pDb,filename,mod);
	if( rc != UNQLITE_OK )
	{
		lua_pushinteger(L,rc);
		return 1; 
	}
	lua_pushinteger(L,rc);
	lua_pushlightuserdata(L,pDb);
	return 2;
}

static int lunqlite_close(lua_State* L){
	int rc;
	unqlite *pDb=(unqlite*)lua_touserdata(L,1);
	rc = unqlite_close(pDb);
	lua_pushinteger(L,rc);
	return 1;
}

static int lunqlite_store(lua_State* L){
	int rc;
	unqlite *pDb=(unqlite*)lua_touserdata(L,1);
	const char *key = luaL_checkstring(L, 2);
	const char *value = luaL_checkstring(L, 3);
	//Store some records
	rc = unqlite_kv_store(pDb,key,-1,value,strlen(value));
	if( rc != UNQLITE_OK )
	{
		lua_pushinteger(L,rc);
	}
	lua_pushinteger(L,rc);
	return 1;
}

static int lunqlite_delete(lua_State* L){
	int rc;
	unqlite *pDb=(unqlite*)lua_touserdata(L,1);
	const char *key = luaL_checkstring(L, 2);
	//Store some records
	rc = unqlite_kv_delete(pDb,key,-1);
	if( rc != UNQLITE_OK )
	{
		lua_pushinteger(L,rc);
	}
	lua_pushinteger(L,rc);
	return 1;
}

static int lunqlite_fetch(lua_State* L){
	int rc;
	sxi64 nBytes;  //Data length
	char *zBuf;     //Dynamically allocated buffer
	unqlite *pDb=(unqlite*)lua_touserdata(L,1);
	const char *key = luaL_checkstring(L, 2);
	//Store some records
	rc = unqlite_kv_fetch(pDb,key,-1,NULL,&nBytes);
	if( rc != UNQLITE_OK )
	{
		lua_pushinteger(L,rc);
		return 1;
	}
	//Allocate a buffer big enough to hold the record content
	zBuf = (char *)malloc(nBytes);
	if( zBuf == NULL ){ 
		lua_pushinteger(L,rc);
		lua_pushnil(L);
		return 2;
	}
	//Copy record content in our buffer
	rc = unqlite_kv_fetch(pDb,key,-1,zBuf,&nBytes);
	lua_pushinteger(L,rc);
	lua_pushlstring(L, zBuf, nBytes);
	free(zBuf);
	return 2;
}

static int lunqlite_commit(lua_State* L){
	int rc;
	unqlite *pDb=(unqlite*)lua_touserdata(L,1);
	rc = unqlite_commit(pDb);
	lua_pushinteger(L,rc);
	return 1;
}

static int lunqlite_begin(lua_State* L){
	int rc;
	unqlite *pDb=(unqlite*)lua_touserdata(L,1);
	rc = unqlite_begin(pDb);
	lua_pushinteger(L,rc);
	return 1;
}

static int lunqlite_rollback(lua_State* L){
	int rc;
	unqlite *pDb=(unqlite*)lua_touserdata(L,1);
	rc = unqlite_rollback(pDb);
	lua_pushinteger(L,rc);
	return 1;
}

static const luaL_Reg unqlitelib[] = {
	{"open",   lunqlite_open},
	{"close",   lunqlite_close},
	{"store",   lunqlite_store},
	{"fetch",   lunqlite_fetch},
	{"delete",   lunqlite_delete},
	{"begin",   lunqlite_begin},
	{"rollback",   lunqlite_rollback},
	{"commit",   lunqlite_commit},
	{NULL, NULL}
};

int luaopen_lunqlite(lua_State *L)
{
	 luaL_register(L, "unqlite", unqlitelib);
	 return 1;
}

#if __cplusplus
}
#endif
