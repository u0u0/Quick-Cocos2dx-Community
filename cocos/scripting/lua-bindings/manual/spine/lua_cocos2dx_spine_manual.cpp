/****************************************************************************
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "scripting/lua-bindings/auto/lua_cocos2dx_spine_auto.hpp"
#include "scripting/lua-bindings/manual/spine/lua_cocos2dx_spine_manual.hpp"
#include "scripting/lua-bindings/manual/spine/lua_spSkeletonData.hpp"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "scripting/lua-bindings/manual/cocos2d/LuaScriptHandlerMgr.h"
#include "scripting/lua-bindings/manual/CCLuaValue.h"
#include "editor-support/spine/spine.h"
#include "editor-support/spine/spine-cocos2dx.h"
#include "scripting/lua-bindings/manual/spine/LuaSkeletonAnimation.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

using namespace spine;

static int lua_cocos2dx_spine_SkeletonAnimation_createWithData(lua_State* L)
{
    if (nullptr == L)
        return 0 ;
    
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(L,1,"sp.SkeletonAnimation",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(L) - 1;
    
    if (1 == argc)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isusertype(L,2,"sp.SkeletonData",0,&tolua_err)) goto tolua_lerror;
#endif
        lua_spSkeletonData *luaSpData = static_cast<lua_spSkeletonData *>(tolua_tousertype(L,2,0));
        auto tolua_ret = LuaSkeletonAnimation::createWithData(luaSpData->data, false);
        
        int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(L, nID, pLuaID, (void*)tolua_ret,"sp.SkeletonAnimation");
        return 1;
    }
    
    luaL_error(L, "'createWithData' function of SkeletonAnimation has wrong number of arguments: %d, was expecting %d\n", argc, 1);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L,"#ferror in function 'createWithData'.",&tolua_err);
#endif
    return 0;
}

static int lua_cocos2dx_spine_SkeletonAnimation_createWithJsonFile(lua_State* L)
{
    if (nullptr == L)
        return 0 ;
    
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(L,1,"sp.SkeletonAnimation",0,&tolua_err)) goto tolua_lerror;
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
        auto tolua_ret = LuaSkeletonAnimation::createWithJsonFile(skeletonDataFile, atlasFile);
        
        int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(L, nID, pLuaID, (void*)tolua_ret,"sp.SkeletonAnimation");
        return 1;
    } else if (3 == argc)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err)  ||
            !tolua_isstring(L, 3 ,0, &tolua_err)  ||
            !tolua_isnumber(L, 4 ,0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char* skeletonDataFile = tolua_tostring(L, 2, "");
        const char* atlasFile = tolua_tostring(L, 3, "");
        LUA_NUMBER scale = tolua_tonumber(L, 4, 1);
        
        auto tolua_ret = LuaSkeletonAnimation::createWithJsonFile(skeletonDataFile, atlasFile, scale);
        
        int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(L, nID, pLuaID, (void*)tolua_ret,"sp.SkeletonAnimation");
        return 1;
    }
    
    luaL_error(L, "'createWithJsonFile' function of SkeletonAnimation has wrong number of arguments: %d, was expecting %d\n", argc, 2);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L,"#ferror in function 'createWithJsonFile'.",&tolua_err);
#endif
    return 0;
}

static int lua_cocos2dx_spine_SkeletonAnimation_createWithBinaryFile(lua_State* L)
{
    if (nullptr == L)
        return 0 ;
    
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(L,1,"sp.SkeletonAnimation",0,&tolua_err)) goto tolua_lerror;
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
        auto tolua_ret = LuaSkeletonAnimation::createWithBinaryFile(skeletonDataFile, atlasFile);
        
        int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(L, nID, pLuaID, (void*)tolua_ret,"sp.SkeletonAnimation");
        return 1;
    } else if (3 == argc)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err)  ||
            !tolua_isstring(L, 3 ,0, &tolua_err)  ||
            !tolua_isnumber(L, 4 ,0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const char* skeletonDataFile = tolua_tostring(L, 2, "");
        const char* atlasFile = tolua_tostring(L, 3, "");
        LUA_NUMBER scale = tolua_tonumber(L, 4, 1);
        
        auto tolua_ret = LuaSkeletonAnimation::createWithBinaryFile(skeletonDataFile, atlasFile, scale);
        
        int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(L, nID, pLuaID, (void*)tolua_ret,"sp.SkeletonAnimation");
        return 1;
    }
    
    luaL_error(L, "'createWithBinaryFile' function of SkeletonAnimation has wrong number of arguments: %d, was expecting %d\n", argc, 2);
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L,"#ferror in function 'createWithBinaryFile'.",&tolua_err);
#endif
    return 0;
}

int executeSpineEvent(LuaSkeletonAnimation* skeletonAnimation, int handler, EventType eventType, TrackEntry* entry, spine
                      ::Event* event = nullptr )
{
    if (nullptr == skeletonAnimation || 0 == handler)
        return 0;
    
    LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
    if (nullptr == stack)
        return 0;
    
    lua_State* L = LuaEngine::getInstance()->getLuaStack()->getLuaState();
    if (nullptr == L)
        return 0;
    
    int ret = 0;
    
    std::string animationName = (entry && entry->getAnimation()) ? entry->getAnimation()->getName().buffer() : "";
    std::string eventTypeName = "";

    switch (eventType) {
        case EventType_Start:
            {
                eventTypeName = "start";
            }
            break;
        case EventType_Interrupt:
            {
                eventTypeName = "interrupt";
            }
            break;
        case EventType_End:
            {
                eventTypeName = "end";
            }
            break;
        case EventType_Dispose:
            {
                eventTypeName = "dispose";
            }
            break;
        case EventType_Complete:
            {
                eventTypeName = "complete";
            }
            break;
        case EventType_Event:
            {
                eventTypeName = "event";
            }
            break;
        default:
            break;
    }
    
    LuaValueDict spineEvent;
    spineEvent.insert(spineEvent.end(), LuaValueDict::value_type("type", LuaValue::stringValue(eventTypeName)));
    spineEvent.insert(spineEvent.end(), LuaValueDict::value_type("trackIndex", LuaValue::intValue(entry->getTrackIndex())));
    spineEvent.insert(spineEvent.end(), LuaValueDict::value_type("animation", LuaValue::stringValue(animationName)));
    spineEvent.insert(spineEvent.end(), LuaValueDict::value_type("loopCount", LuaValue::intValue(std::floor(entry->getTrackTime() / entry->getAnimationEnd()))));
    
    if (nullptr != event)
    {
        LuaValueDict eventData;
        spine::EventData &data = (spine::EventData &)event->getData();
        eventData.insert(eventData.end(), LuaValueDict::value_type("name", LuaValue::stringValue(data.getName().buffer())));
        eventData.insert(eventData.end(), LuaValueDict::value_type("intValue", LuaValue::intValue(data.getIntValue())));
        eventData.insert(eventData.end(), LuaValueDict::value_type("floatValue", LuaValue::floatValue(data.getFloatValue())));
        eventData.insert(eventData.end(), LuaValueDict::value_type("stringValue", LuaValue::stringValue(data.getStringValue().buffer())));
        eventData.insert(eventData.end(), LuaValueDict::value_type("audioPath", LuaValue::stringValue(data.getAudioPath().buffer())));
        eventData.insert(eventData.end(), LuaValueDict::value_type("volume", LuaValue::floatValue(data.getVolume())));
        eventData.insert(eventData.end(), LuaValueDict::value_type("balance", LuaValue::floatValue(data.getBalance())));
        spineEvent.insert(spineEvent.end(), LuaValueDict::value_type("eventData", LuaValue::dictValue(eventData)));
    }
    
    stack->pushLuaValueDict(spineEvent);
    ret = stack->executeFunctionByHandler(handler, 1);
    return ret;
}

int lua_cocos2dx_spine_SkeletonAnimation_registerSpineEventHandler(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"sp.SkeletonAnimation",0,&tolua_err) ||
        !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
        !tolua_isnumber(tolua_S, 3, 0, &tolua_err)                  ||
        !tolua_isnoobj(tolua_S,4,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
    	LuaSkeletonAnimation* self    = (LuaSkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
        if (NULL != self ) {
            int handler = (  toluafix_ref_function(tolua_S,2,0));
            EventType eventType = static_cast<EventType>((int)tolua_tonumber(tolua_S, 3, 0));
            
            switch (eventType) {
                case EventType_Start:
                    {
                        self->setStartListener([=](TrackEntry* entry){
                            executeSpineEvent(self, handler, eventType, entry);
                        });
                        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_START);
                    }
                    break;
                case EventType_Interrupt:
                    {
                        self->setInterruptListener([=](TrackEntry* entry){
                            executeSpineEvent(self, handler, eventType, entry);
                        });
                        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_INTERRUPT);
                    }
                    break;
                case EventType_End:
                    {
                        self->setEndListener([=](TrackEntry* entry){
                            executeSpineEvent(self, handler, eventType, entry);
                        });
                        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_END);
                    }
                    break;
                case EventType_Dispose:
                {
                    self->setDisposeListener([=](TrackEntry* entry){
                        executeSpineEvent(self, handler, eventType, entry);
                    });
                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_DISPOSE);
                }
                    break;
                case EventType_Complete:
                    {
                        self->setCompleteListener([=](TrackEntry* entry){
                            executeSpineEvent(self, handler, eventType, entry);
                        });
                        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_COMPLETE);
                    }
                    break;
                case EventType_Event:
                    {
                        self->setEventListener([=](TrackEntry* entry, spine::Event* event){
                            executeSpineEvent(self, handler, eventType, entry, event);
                        });
                        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_EVENT);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'registerSpineEventHandler'.",&tolua_err);
    return 0;
#endif
}

int lua_cocos2dx_spine_SkeletonAnimation_unregisterSpineEventHandler(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"sp.SkeletonAnimation",0,&tolua_err) ||
        !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
    	LuaSkeletonAnimation *self = (LuaSkeletonAnimation*)tolua_tousertype(tolua_S,1,0);
        if (NULL != self ) {
            EventType eventType = static_cast<EventType>((int)tolua_tonumber(tolua_S, 2, 0));
            ScriptHandlerMgr::HandlerType handlerType = ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_START;
            switch (eventType) {
                case EventType_Start:
                    handlerType = ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_START;
                    self->setStartListener(nullptr);
                    break;
                case EventType_Interrupt:
                    handlerType = ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_INTERRUPT;
                    break;
                case EventType_End:
                    handlerType = ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_END;
                    self->setEndListener(nullptr);
                    break;
                case EventType_Dispose:
                    handlerType = ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_DISPOSE;
                    break;
                case EventType_Complete:
                    handlerType = ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_COMPLETE;
                    self->setCompleteListener(nullptr);
                    break;
                case EventType_Event:
                    handlerType = ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_EVENT;
                    self->setEventListener(nullptr);
                    break;
                    
                default:
                    break;
            }
            ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)self, handlerType);
        }
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'unregisterScriptHandler'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_spine_SkeletonAnimation_setPreUpdateWorldTransformsListener(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S, 1, "sp.SkeletonAnimation", 0, &tolua_err) ||
        !toluafix_isfunction(tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err)
        ) {
        tolua_error(tolua_S, "#ferror in function 'setPreUpdateWorldTransformsListener'.", &tolua_err);
        return 0;
    }
#endif
    LuaSkeletonAnimation *self = (LuaSkeletonAnimation*)tolua_tousertype(tolua_S, 1, 0);
    int handler = (toluafix_ref_function(tolua_S, 2, 0));
    if (handler != 0) {
        self->setPreUpdateWorldTransformsListener([=](SkeletonAnimation *) {
            LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 0);
        });
        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_PREUPDATE);
    } else {
        self->setPreUpdateWorldTransformsListener(nullptr);
        ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)self, ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_PREUPDATE);
    }
    return 0;
}

static int lua_cocos2dx_spine_SkeletonAnimation_setPostUpdateWorldTransformsListener(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S, 1, "sp.SkeletonAnimation", 0, &tolua_err) ||
        !toluafix_isfunction(tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err)
        ) {
        tolua_error(tolua_S, "#ferror in function 'setPostUpdateWorldTransformsListener'.", &tolua_err);
        return 0;
    }
#endif
    LuaSkeletonAnimation *self = (LuaSkeletonAnimation*)tolua_tousertype(tolua_S, 1, 0);
    int handler = (toluafix_ref_function(tolua_S, 2, 0));
    if (handler != 0) {
        self->setPostUpdateWorldTransformsListener([=](SkeletonAnimation *) {
            LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 0);
        });
        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_POSTUPDATE);
    } else {
        self->setPostUpdateWorldTransformsListener(nullptr);
        ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)self, ScriptHandlerMgr::HandlerType::EVENT_SPINE_ANIMATION_POSTUPDATE);
    }
    return 0;
}

static int lua_cocos2dx_spine_SkeletonAnimation_addAnimation(lua_State* tolua_S)
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonAnimation_addAnimation'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 3)
    {
        int arg0;
        const char* arg1;
        bool arg2;
        
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "sp.SkeletonAnimation:addAnimation");
        
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "sp.SkeletonAnimation:addAnimation"); arg1 = arg1_tmp.c_str();
        
        ok &= luaval_to_boolean(tolua_S, 4,&arg2, "sp.SkeletonAnimation:addAnimation");
        if(!ok)
            return 0;
        cobj->addAnimation(arg0, arg1, arg2);
        
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 4)
    {
        int arg0;
        const char* arg1;
        bool arg2;
        double arg3;
        
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "sp.SkeletonAnimation:addAnimation");
        
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "sp.SkeletonAnimation:addAnimation"); arg1 = arg1_tmp.c_str();
        
        ok &= luaval_to_boolean(tolua_S, 4,&arg2, "sp.SkeletonAnimation:addAnimation");
        
        ok &= luaval_to_number(tolua_S, 5,&arg3, "sp.SkeletonAnimation:addAnimation");
        if(!ok)
            return 0;
        
        cobj->addAnimation(arg0, arg1, arg2, arg3);

        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "addAnimation",argc, 3);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonAnimation_addAnimation'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_cocos2dx_spine_SkeletonAnimation_setAnimation(lua_State* tolua_S)
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonAnimation_setAnimation'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 3)
    {
        int arg0;
        const char* arg1;
        bool arg2;
        
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "sp.SkeletonAnimation:setAnimation");
        
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "sp.SkeletonAnimation:setAnimation"); arg1 = arg1_tmp.c_str();
        
        ok &= luaval_to_boolean(tolua_S, 4,&arg2, "sp.SkeletonAnimation:setAnimation");
        if(!ok)
            return 0;
        
        cobj->setAnimation(arg0, arg1, arg2);
        
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "setAnimation",argc, 3);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonAnimation_setAnimation'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_cocos2dx_spine_SkeletonAnimation_setAttachment(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonAnimation* cobj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonAnimation",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (spine::SkeletonAnimation*)tolua_tousertype(tolua_S,1,0);
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonAnimation_setAttachment'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc > 0) {
        const char *arg0 = lua_tostring(tolua_S, 2);
        
        if(!arg0) {
            tolua_error(tolua_S, "sp.SkeletonAnimation:setAttachment arg 1 must string", nullptr);
            return 0;
        }
        
        const char* arg1 = lua_tostring(tolua_S, 3);
        if (arg1) {
            // set new Attachment
            cobj->setAttachment(arg0, arg1);
        } else {
            // remove Attachment
            cobj->setAttachment(arg0, NULL);
        }
        
        lua_settop(tolua_S, 1);
        return 1;
    }
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonAnimation_setAttachment'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_cocos2dx_spine_SkeletonAnimation_getAttachment(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonAnimation* cobj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "sp.SkeletonAnimation", 0, &tolua_err)) goto tolua_lerror;
#endif
    cobj = (spine::SkeletonAnimation*)tolua_tousertype(tolua_S, 1, 0);
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonAnimation_getAttachment'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    if (argc > 0) {
        const char *arg0 = lua_tostring(tolua_S, 2);
        if (!arg0) {
            tolua_error(tolua_S, "sp.SkeletonAnimation:getAttachment arg 1 must string", nullptr);
            return 0;
        }
        const char* arg1 = lua_tostring(tolua_S, 3);
        if (!arg1) {
            tolua_error(tolua_S, "sp.SkeletonAnimation:getAttachment arg 2 must string", nullptr);
            return 0;
        }
        
        // getAttachment auto find skin attackment
        Attachment *attachment = cobj->getAttachment(arg0, arg1);
        if (!attachment) {
            tolua_error(tolua_S, "sp.SkeletonAnimation:getAttachment can not find attachment", nullptr);
            return 0;
        }
        Slot *slot = cobj->findSlot(arg0); // no need check null for slot now
        
        // return a table
        lua_newtable(tolua_S);
        VertexAttachment *vertexAtt = NULL;
        const char *typeStr = "Unkonw";
        
        if (attachment->getRTTI().isExactly(RegionAttachment::rtti)) {
            typeStr = "Region";
            float point[8]; // 4 point
            RegionAttachment *regionAttachment = static_cast<RegionAttachment*>(attachment);
            regionAttachment->computeWorldVertices(slot->getBone(), point, 0, 2);
            // br
            lua_pushstring(tolua_S, "br");
            lua_newtable(tolua_S);
            lua_pushstring(tolua_S, "x");
            lua_pushnumber(tolua_S, point[0]);
            lua_rawset(tolua_S, -3);// end x
            lua_pushstring(tolua_S, "y");
            lua_pushnumber(tolua_S, point[1]);
            lua_rawset(tolua_S, -3);// end y
            lua_rawset(tolua_S, -3);// end br
            // bl
            lua_pushstring(tolua_S, "bl");
            lua_newtable(tolua_S);
            lua_pushstring(tolua_S, "x");
            lua_pushnumber(tolua_S, point[2]);
            lua_rawset(tolua_S, -3);// end x
            lua_pushstring(tolua_S, "y");
            lua_pushnumber(tolua_S, point[3]);
            lua_rawset(tolua_S, -3);// end y
            lua_rawset(tolua_S, -3);// end bl
            // ul
            lua_pushstring(tolua_S, "ul");
            lua_newtable(tolua_S);
            lua_pushstring(tolua_S, "x");
            lua_pushnumber(tolua_S, point[4]);
            lua_rawset(tolua_S, -3);// end x
            lua_pushstring(tolua_S, "y");
            lua_pushnumber(tolua_S, point[5]);
            lua_rawset(tolua_S, -3);// end y
            lua_rawset(tolua_S, -3);// end ul
            // ur
            lua_pushstring(tolua_S, "ur");
            lua_newtable(tolua_S);
            lua_pushstring(tolua_S, "x");
            lua_pushnumber(tolua_S, point[6]);
            lua_rawset(tolua_S, -3);// end x
            lua_pushstring(tolua_S, "y");
            lua_pushnumber(tolua_S, point[7]);
            lua_rawset(tolua_S, -3);// end y
            lua_rawset(tolua_S, -3);// end ur
        } else if (attachment->getRTTI().isExactly(BoundingBoxAttachment::rtti)) {
            typeStr = "BoundingBox";
            vertexAtt = static_cast<VertexAttachment*>(attachment);
        } else if (attachment->getRTTI().isExactly(MeshAttachment::rtti)) {
            typeStr = "Mesh";
            vertexAtt = static_cast<VertexAttachment*>(attachment);
        } else if (attachment->getRTTI().isExactly(PathAttachment::rtti)) {
            typeStr = "Path";
            vertexAtt = static_cast<VertexAttachment*>(attachment);
        } else if (attachment->getRTTI().isExactly(PointAttachment::rtti)) {
            float x, y;
            typeStr = "Point";
            PointAttachment *pointAtt = static_cast<PointAttachment*>(attachment);
            pointAtt->computeWorldPosition(slot->getBone(), x, y);
            // point
            lua_pushstring(tolua_S, "x");
            lua_pushnumber(tolua_S, x);
            lua_rawset(tolua_S, -3);// end x
            lua_pushstring(tolua_S, "y");
            lua_pushnumber(tolua_S, y);
            lua_rawset(tolua_S, -3);// end y
        } else if (attachment->getRTTI().isExactly(ClippingAttachment::rtti)) {
            typeStr = "Clipping";
        }
        
        if (vertexAtt) {
            const int length = vertexAtt->getWorldVerticesLength();
            float *points = (float *)malloc(sizeof(float) * length);
            vertexAtt->computeWorldVertices(*slot, 0, length, points, 0, 2);
            // vertices
            lua_pushstring(tolua_S, "points");
            lua_newtable(tolua_S);
            for (int index = 0; index < length / 2; index++) {
                lua_newtable(tolua_S);
                lua_pushstring(tolua_S, "x");
                lua_pushnumber(tolua_S, points[index * 2]);
                lua_rawset(tolua_S, -3);// end x
                lua_pushstring(tolua_S, "y");
                lua_pushnumber(tolua_S, points[index * 2 + 1]);
                lua_rawset(tolua_S, -3);// end x
                lua_rawseti(tolua_S, -2, index + 1);// point add to vertices
            }
            lua_rawset(tolua_S, -3);// end vertices
            free(points);
        }
        
        // type
        lua_pushstring(tolua_S, "type");
        lua_pushstring(tolua_S, typeStr);
        lua_rawset(tolua_S, -3);
        return 1;
    }
    
    luaL_error(tolua_S, "getAttachment has wrong number of arguments: %d, was expecting %d \n", argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_spine_SkeletonAnimation_getAttachment'.", &tolua_err);
#endif
    
    return 0;
}

static int lua_cocos2dx_spine_SkeletonAnimation_findBone(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonAnimation* cobj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonAnimation",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (spine::SkeletonAnimation*)tolua_tousertype(tolua_S,1,0);
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonAnimation_findBone'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        const char *arg0 = lua_tostring(tolua_S, 2);
        if(!arg0) {
            tolua_error(tolua_S, "sp.SkeletonAnimation:findBone arg 1 must string", nullptr);
            return 0;
        }
        
        // call spine func
        Bone *bone = cobj->findBone(arg0);
#if COCOS2D_DEBUG >= 1
        if (nullptr == bone) {
            tolua_error(tolua_S,"sp.SkeletonAnimation:findBone can not find bone", nullptr);
            return 0;
        }
#endif
        // === return a table ===
        lua_newtable(tolua_S);
        lua_pushstring(tolua_S, "x");
        lua_pushnumber(tolua_S, bone->getX());
        lua_rawset(tolua_S, -3);    /* bone.x */
        lua_pushstring(tolua_S, "y");
        lua_pushnumber(tolua_S, bone->getY());
        lua_rawset(tolua_S, -3);    /* bone.y */
        lua_pushstring(tolua_S, "rotation");
        lua_pushnumber(tolua_S, bone->getRotation());
        lua_rawset(tolua_S, -3);    /* bone.rotation */
        lua_pushstring(tolua_S, "scaleX");
        lua_pushnumber(tolua_S, bone->getScaleX());
        lua_rawset(tolua_S, -3);    /* bone.scaleX */
        lua_pushstring(tolua_S, "scaleY");
        lua_pushnumber(tolua_S, bone->getScaleY());
        lua_rawset(tolua_S, -3);    /* bone.scaleY */
        lua_pushstring(tolua_S, "shearX");
        lua_pushnumber(tolua_S, bone->getShearX());
        lua_rawset(tolua_S, -3);    /* bone.shearX */
        lua_pushstring(tolua_S, "shearY");
        lua_pushnumber(tolua_S, bone->getShearY());
        lua_rawset(tolua_S, -3);    /* bone.shearY */
        lua_pushstring(tolua_S, "worldX");
        lua_pushnumber(tolua_S, bone->getWorldX());
        lua_rawset(tolua_S, -3);    /* bone.worldX */
        lua_pushstring(tolua_S, "worldY");
        lua_pushnumber(tolua_S, bone->getWorldY());
        lua_rawset(tolua_S, -3);    /* bone.worldY */
        lua_pushstring(tolua_S, "worldRotationX");
        lua_pushnumber(tolua_S, bone->getWorldRotationX());
        lua_rawset(tolua_S, -3);    /* bone.worldRotationX */
        lua_pushstring(tolua_S, "worldRotationY");
        lua_pushnumber(tolua_S, bone->getWorldRotationY());
        lua_rawset(tolua_S, -3);    /* bone.worldRotationY */
        lua_pushstring(tolua_S, "worldScaleX");
        lua_pushnumber(tolua_S, bone->getWorldScaleX());
        lua_rawset(tolua_S, -3);    /* bone.worldScaleX */
        lua_pushstring(tolua_S, "worldScaleY");
        lua_pushnumber(tolua_S, bone->getWorldScaleY());
        lua_rawset(tolua_S, -3);    /* bone.worldScaleY */
        // === end of table ===
        return 1;
    }
    luaL_error(tolua_S, "findBone has wrong number of arguments: %d, was expecting %d \n", argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonAnimation_findBone'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_cocos2dx_spine_SkeletonAnimation_updateBone(lua_State* tolua_S)
{
    int argc = 0;
    spine::SkeletonAnimation* cobj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"sp.SkeletonAnimation",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (spine::SkeletonAnimation*)tolua_tousertype(tolua_S,1,0);
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonAnimation_updateBone'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 2)
    {
        const char *arg0 = lua_tostring(tolua_S, 2);
        if(!arg0) {
            tolua_error(tolua_S, "sp.SkeletonAnimation:updateBone arg 1 must string", nullptr);
            return 0;
        }
        
        Bone *bone = cobj->findBone(arg0);
#if COCOS2D_DEBUG >= 1
        if (nullptr == bone) {
            tolua_error(tolua_S,"sp.SkeletonAnimation:updateBone can not find bone", nullptr);
            return 0;
        }
#endif
        
        if(!lua_istable(tolua_S, 3)) {
            tolua_error(tolua_S, "sp.SkeletonAnimation:updateBone arg 1 must table", nullptr);
            return 0;
        }
        // try update x
        lua_pushstring(tolua_S, "x");
        lua_rawget(tolua_S, 3);
        if (!lua_isnil(tolua_S, -1)) {
            bone->setX(lua_tonumber(tolua_S, -1));
        }
        lua_pop(tolua_S, 1);
        
        // try update y
        lua_pushstring(tolua_S, "y");
        lua_rawget(tolua_S, 3);
        if (!lua_isnil(tolua_S, -1)) {
            bone->setY(lua_tonumber(tolua_S, -1));
        }
        lua_pop(tolua_S, 1);
        
        // try update rotation
        lua_pushstring(tolua_S, "rotation");
        lua_rawget(tolua_S, 3);
        if (!lua_isnil(tolua_S, -1)) {
            bone->setRotation(lua_tonumber(tolua_S, -1));
        }
        lua_pop(tolua_S, 1);
        
        // try update scaleX
        lua_pushstring(tolua_S, "scaleX");
        lua_rawget(tolua_S, 3);
        if (!lua_isnil(tolua_S, -1)) {
            bone->setScaleX(lua_tonumber(tolua_S, -1));
        }
        lua_pop(tolua_S, 1);
        
        // try update scaleY
        lua_pushstring(tolua_S, "scaleY");
        lua_rawget(tolua_S, 3);
        if (!lua_isnil(tolua_S, -1)) {
            bone->setScaleY(lua_tonumber(tolua_S, -1));
        }
        lua_pop(tolua_S, 1);
        
        // try update shearX
        lua_pushstring(tolua_S, "shearX");
        lua_rawget(tolua_S, 3);
        if (!lua_isnil(tolua_S, -1)) {
            bone->setShearX(lua_tonumber(tolua_S, -1));
        }
        lua_pop(tolua_S, 1);
        
        // try update shearY
        lua_pushstring(tolua_S, "shearY");
        lua_rawget(tolua_S, 3);
        if (!lua_isnil(tolua_S, -1)) {
            bone->setShearY(lua_tonumber(tolua_S, -1));
        }
        lua_pop(tolua_S, 1);
        
        return 0;
    }
    luaL_error(tolua_S, "updateBone has wrong number of arguments: %d, was expecting %d \n", argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_spine_SkeletonAnimation_updateBone'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_cocos2dx_spine_SkeletonAnimation_getBoundingBox(lua_State* tolua_S)
{
    spine::SkeletonAnimation* cobj = (spine::SkeletonAnimation*)tolua_tousertype(tolua_S,1,0);
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_spine_SkeletonAnimation_getBoundingBox'", nullptr);
        return 0;
    }
#endif
    
    Rect rect = cobj->getBoundingBox();
    // return a table
    lua_newtable(tolua_S);
    lua_pushstring(tolua_S, "x");
    lua_pushnumber(tolua_S, rect.origin.x);
    lua_rawset(tolua_S, -3);
    lua_pushstring(tolua_S, "y");
    lua_pushnumber(tolua_S, rect.origin.y);
    lua_rawset(tolua_S, -3);
    lua_pushstring(tolua_S, "width");
    lua_pushnumber(tolua_S, rect.size.width);
    lua_rawset(tolua_S, -3);
    lua_pushstring(tolua_S, "height");
    lua_pushnumber(tolua_S, rect.size.height);
    lua_rawset(tolua_S, -3);
    return 1;
}

static void register_spine_manual(lua_State* L)
{
    lua_pushstring(L, "sp.SkeletonAnimation");
    lua_rawget(L, LUA_REGISTRYINDEX);
    if (lua_istable(L,-1))
    {
        tolua_function(L, "createWithData", lua_cocos2dx_spine_SkeletonAnimation_createWithData);
        tolua_function(L, "createWithJsonFile", lua_cocos2dx_spine_SkeletonAnimation_createWithJsonFile);
        tolua_function(L, "createWithBinaryFile", lua_cocos2dx_spine_SkeletonAnimation_createWithBinaryFile);
        tolua_function(L, "registerSpineEventHandler", lua_cocos2dx_spine_SkeletonAnimation_registerSpineEventHandler);
        tolua_function(L, "unregisterSpineEventHandler", lua_cocos2dx_spine_SkeletonAnimation_unregisterSpineEventHandler);
        tolua_function(L, "setPreUpdateWorldTransformsListener", lua_cocos2dx_spine_SkeletonAnimation_setPreUpdateWorldTransformsListener);
        tolua_function(L, "setPostUpdateWorldTransformsListener", lua_cocos2dx_spine_SkeletonAnimation_setPostUpdateWorldTransformsListener);
        tolua_function(L, "addAnimation", lua_cocos2dx_spine_SkeletonAnimation_addAnimation);
        tolua_function(L, "setAnimation", lua_cocos2dx_spine_SkeletonAnimation_setAnimation);
        tolua_function(L, "setAttachment", lua_cocos2dx_spine_SkeletonAnimation_setAttachment);
        tolua_function(L, "getAttachment", lua_cocos2dx_spine_SkeletonAnimation_getAttachment);
        tolua_function(L, "findBone", lua_cocos2dx_spine_SkeletonAnimation_findBone);
        tolua_function(L, "updateBone", lua_cocos2dx_spine_SkeletonAnimation_updateBone);
        tolua_function(L, "getBoundingBox", lua_cocos2dx_spine_SkeletonAnimation_getBoundingBox);
    }
    lua_pop(L, 1);
    
    /*Because sp.SkeletonAnimation:create creat a LuaSkeletonAnimation object,so we need use LuaSkeletonAnimation typename for g_luaType*/
    std::string typeName = typeid(LuaSkeletonAnimation).name();
    g_luaType[typeName] = "sp.SkeletonAnimation";
    g_typeCast["SkeletonAnimation"] = "sp.SkeletonAnimation";
}

int register_spine_module(lua_State* L)
{
    lua_getglobal(L, "_G");
    if (lua_istable(L,-1))//stack:...,_G,
    {
        register_all_cocos2dx_spine(L);
        register_spine_manual(L);
        register_spSkeletonData_manual(L);
    }
    lua_pop(L, 1);

    return 1;
}
