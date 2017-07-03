// Copyright 2016 KeNan Liu
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "tolua_fix.h"
#include "CCLuaEngine.h"
#include "lua_audio_manual.h"
#include "RDAudio.h"

#define RD_AUDIO_BUFFER "Rapid2D_CAudioBuffer"
#define RD_AUDIO_SOURCE "Rapid2D_CAudioPlayer"

typedef struct _RDAudioItem {
    ALuint id;
    bool deleted;
} RDAudioItem;

static void callback(int handler, ALuint bufferID)
{
    lua_State * L = cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState();
    
    if (bufferID > 0) {
        // create userdata
        RDAudioItem *bufferItem = (RDAudioItem *)lua_newuserdata(L, sizeof(RDAudioItem));
        bufferItem->id = bufferID;
        bufferItem->deleted = false;
        // set metatable
        luaL_getmetatable(L, RD_AUDIO_BUFFER);
        lua_setmetatable(L, -2);
    } else {
        lua_pushnil(L);
    }
    
    cocos2d::LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 1);
    toluafix_remove_function_by_refid(L, handler);
}

static int lnewBuffer(lua_State * L)
{
    // FilePath
    const char *path = lua_tostring(L, 1);
    int handler = (toluafix_ref_function(L,2,0));
    RDAudio::getInstance()->loadFileAsyn(path, handler, callback);
    
    return 0;// number of return values
}

static int lnewSource(lua_State * L)
{
    ALuint sourceID;
    // clear old error code.
    alGetError();
    // grab a source ID from openAL
    alGenSources(1, &sourceID);
    // check the error code.
    if (alGetError() != AL_NO_ERROR) {
        cocos2d::log("Rapid2D_CAudio.newSource() fail");
        return 0;
    }
    
    // create userdata
    RDAudioItem *sourceItem = (RDAudioItem *)lua_newuserdata(L, sizeof(RDAudioItem));
    sourceItem->id = sourceID;
    sourceItem->deleted = false;
    // set metatable
    luaL_getmetatable(L, RD_AUDIO_SOURCE);
    lua_setmetatable(L, -2);
    return 1;// number of return values
}

/******************** for buffer metatable ********************/
static int lBufferGC(lua_State *L)
{
    RDAudioItem *bufferItem = (RDAudioItem *)luaL_checkudata(L, 1, RD_AUDIO_BUFFER);
    if (!bufferItem->deleted) {
        alDeleteBuffers(1, &bufferItem->id);
        bufferItem->deleted = true;
    }
    return 0;// number of return values
}

static const struct luaL_Reg meta_buffer [] = {
    {"__gc", lBufferGC},
    {NULL, NULL}
};

/******************** for source metatable ********************/
static int lSourceGC(lua_State *L)
{
    RDAudioItem *sourceItem = (RDAudioItem *)luaL_checkudata(L, 1, RD_AUDIO_SOURCE);
    if (!sourceItem->deleted) {
        alDeleteSources(1, &sourceItem->id);
        sourceItem->deleted = true;
    }
    return 0;// number of return values
}

static int lSourcePlay2d(lua_State *L)
{
    RDAudioItem *sourceItem = (RDAudioItem *)luaL_checkudata(L, 1, RD_AUDIO_SOURCE);
    RDAudioItem *bufferItem = (RDAudioItem *)luaL_checkudata(L, 2, RD_AUDIO_BUFFER);
    int isLoop = lua_toboolean(L, 3);
    
    if (sourceItem->deleted || bufferItem->deleted) {
        cocos2d::log("Rapid2D_CAudioBuffer.play2d() fail for deleted!");
        return 0;
    }
    
    // attach the buffer to the source
    alSourcei(sourceItem->id, AL_BUFFER, bufferItem->id);
    // set loop sound
    alSourcei(sourceItem->id, AL_LOOPING, isLoop);
    alSourcePlay(sourceItem->id);
    // clean error code
    alGetError();
    return 0;
}

static int lSourcePause(lua_State *L)
{
    RDAudioItem *sourceItem = (RDAudioItem *)luaL_checkudata(L, 1, RD_AUDIO_SOURCE);
    if (sourceItem->deleted) {
        cocos2d::log("Rapid2D_CAudioBuffer.pause() fail for deleted!");
        return 0;
    }
    alSourcePause(sourceItem->id);
    return 0;
}

static int lSourceResume(lua_State *L)
{
    RDAudioItem *sourceItem = (RDAudioItem *)luaL_checkudata(L, 1, RD_AUDIO_SOURCE);
    if (sourceItem->deleted) {
        cocos2d::log("Rapid2D_CAudioBuffer.resume() fail for deleted!");
        return 0;
    }
    alSourcePlay(sourceItem->id);
    return 0;
}

static int lSourceStop(lua_State *L)
{
    RDAudioItem *sourceItem = (RDAudioItem *)luaL_checkudata(L, 1, RD_AUDIO_SOURCE);
    if (sourceItem->deleted) {
        cocos2d::log("Rapid2D_CAudioBuffer.stop() fail for deleted!");
        return 0;
    }
    alSourceStop(sourceItem->id);
    return 0;
}

static int lSourceSetVolume(lua_State *L)
{
    RDAudioItem *sourceItem = (RDAudioItem *)luaL_checkudata(L, 1, RD_AUDIO_SOURCE);
    if (sourceItem->deleted) {
        cocos2d::log("Rapid2D_CAudioBuffer.setVolume() fail for deleted!");
        return 0;
    }
    
    lua_Number volume = lua_tonumber(L, 2);
    if (volume < 0.0f) {
        volume = 0.0f;
    }
    if (volume > 1.0f) {
        volume = 1.0f;
    }
    alSourcef(sourceItem->id, AL_GAIN, volume);
    return 0;
}

static int lSourceGetStat(lua_State *L)
{
    RDAudioItem *sourceItem = (RDAudioItem *)luaL_checkudata(L, 1, RD_AUDIO_SOURCE);
    if (sourceItem->deleted) {
        cocos2d::log("Rapid2D_CAudioBuffer.getStat() fail for deleted!");
        return 0;
    }
    
    ALint stat;
    alGetSourcei(sourceItem->id, AL_SOURCE_STATE, &stat);
    switch (stat) {
        case AL_INITIAL:
            lua_pushinteger(L, 1);
            break;
        case AL_PLAYING:
            lua_pushinteger(L, 2);
            break;
        case AL_PAUSED:
            lua_pushinteger(L, 3);
            break;
        case AL_STOPPED:
            lua_pushinteger(L, 4);
            break;
        default:
            cocos2d::log("Rapid2D_CAudioBuffer.getStat() failed!");
            return 0;
    }
    return 1;
}

static const struct luaL_Reg meta_source [] = {
    {"__gc", lSourceGC},
    {"play2d", lSourcePlay2d},
    {"pause", lSourcePause},
    {"resume", lSourceResume},
    {"stop", lSourceStop},
    {"setVolume", lSourceSetVolume},
    {"getStat", lSourceGetStat},
    {NULL, NULL}
};

static const struct luaL_Reg audio_funcs [] = {
    {"newBuffer", lnewBuffer},
    {"newSource", lnewSource},
    {NULL, NULL}
};

TOLUA_API int register_audio_module(lua_State* L)
{    
    lua_getglobal(L, "_G");
    if (lua_istable(L,-1))//stack:...,_G,
    {
        luaL_newmetatable(L, RD_AUDIO_BUFFER);
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);  /* duplicates the metatable */
        lua_setfield(L, -2, "__index");
        /* add method to metatable */
        luaL_setfuncs(L, meta_buffer, 0);
        lua_pop(L, 1);  /* pop new metatable */
        
        luaL_newmetatable(L, RD_AUDIO_SOURCE);
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);  /* duplicates the metatable */
        lua_setfield(L, -2, "__index");
        /* add method to metatable */
        luaL_setfuncs(L, meta_source, 0);
        lua_pop(L, 1);  /* pop new metatable */
        
        // binding userdata to new metatable
        luaL_register(L,"Rapid2D_CAudio", audio_funcs);
    }
    lua_pop(L, 1);
    
    return 1;
}
