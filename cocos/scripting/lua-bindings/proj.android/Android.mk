LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2d_lua_android_static

LOCAL_MODULE_FILENAME := libluacocos2dandroid

LOCAL_SRC_FILES := ../manual/platform/android/CCLuaJavaBridge.cpp \
                   ../manual/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxLuaJavaBridge.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../.. \
                    $(LOCAL_PATH)/../manual \
                    $(LOCAL_PATH)/../../../../external/lua/tolua \
                    $(LOCAL_PATH)/../manual/platform/android \
                    $(LOCAL_PATH)/../manual/platform/android/jni

LOCAL_EXPORT_LDLIBS := -lGLESv2 \
                       -llog \
                       -lz \
                       -landroid

LOCAL_STATIC_LIBRARIES := luajit_static

include $(BUILD_STATIC_LIBRARY)

#==============================================================

include $(CLEAR_VARS)

LOCAL_MODULE    := cocos2d_lua_static

LOCAL_MODULE_FILENAME := libluacocos2d

LOCAL_SRC_FILES := ../manual/CCLuaBridge.cpp \
          ../manual/CCLuaEngine.cpp \
          ../manual/CCLuaStack.cpp \
          ../manual/CCLuaValue.cpp \
          ../manual/Cocos2dxLuaLoader.cpp \
          ../manual/LuaBasicConversions.cpp \
          ../auto/lua_cocos2dx_tmx_auto.cpp \
          ../auto/lua_cocos2dx_auto.cpp \
          ../auto/lua_cocos2dx_physics_auto.cpp \
          ../auto/lua_cocos2dx_experimental_auto.cpp \
          ../manual/cocos2d/lua_cocos2dx_experimental_manual.cpp \
          ../manual/cocos2d/lua_cocos2dx_manual.cpp \
          ../manual/cocos2d/lua_cocos2dx_physics_manual.cpp \
          ../manual/cocos2d/LuaOpengl.cpp \
          ../manual/cocos2d/LuaScriptHandlerMgr.cpp \
          ../manual/tolua_fix.cpp \
          ../../../../external/lua/tolua/tolua_event.c \
          ../../../../external/lua/tolua/tolua_is.c \
          ../../../../external/lua/tolua/tolua_map.c \
          ../../../../external/lua/tolua/tolua_push.c \
          ../../../../external/lua/tolua/tolua_to.c \
          ../../../../external/xxtea/xxtea.cpp

#3d
ifeq ($(CC_USE_3D),1)
LOCAL_SRC_FILES += ../manual/3d/lua_cocos2dx_3d_manual.cpp \
                   ../auto/lua_cocos2dx_3d_auto.cpp
endif

# audio
LOCAL_SRC_FILES += ../manual/audio/lua_audio_manual.cpp

#network
LOCAL_SRC_FILES += ../manual/network/lua_cocos2dx_network_manual.cpp \
                   ../manual/network/lua_extensions.c \
                   ../manual/network/Lua_web_socket.cpp \
                   ../../../../external/lua/luasocket/asynconnect.cpp \
                   ../../../../external/lua/luasocket/auxiliar.c \
                   ../../../../external/lua/luasocket/buffer.c \
                   ../../../../external/lua/luasocket/except.c \
                   ../../../../external/lua/luasocket/inet.c \
                   ../../../../external/lua/luasocket/io.c \
                   ../../../../external/lua/luasocket/luasocket.c \
                   ../../../../external/lua/luasocket/luasocket_scripts.c \
                   ../../../../external/lua/luasocket/mime.c \
                   ../../../../external/lua/luasocket/options.c \
                   ../../../../external/lua/luasocket/select.c \
                   ../../../../external/lua/luasocket/serial.c \
                   ../../../../external/lua/luasocket/tcp.c \
                   ../../../../external/lua/luasocket/timeout.c \
                   ../../../../external/lua/luasocket/udp.c \
                   ../../../../external/lua/luasocket/unix.c \
                   ../../../../external/lua/luasocket/usocket.c

#cocostudio
ifeq ($(CC_USE_CCSTUDIO),1)
LOCAL_SRC_FILES += ../manual/cocostudio/lua_cocos2dx_coco_studio_manual.cpp \
                   ../manual/cocostudio/CustomGUIReader.cpp \
                   ../manual/cocostudio/lua_cocos2dx_csloader_manual.cpp \
                   ../auto/lua_cocos2dx_csloader_auto.cpp \
                   ../auto/lua_cocos2dx_studio_auto.cpp
endif

#spine
ifeq ($(CC_USE_SPINE),1)
LOCAL_SRC_FILES += ../manual/spine/lua_cocos2dx_spine_manual.cpp \
                   ../manual/spine/LuaSkeletonAnimation.cpp \
                   ../manual/spine/lua_spSkeletonData.cpp \
                   ../auto/lua_cocos2dx_spine_auto.cpp
endif

#ui
LOCAL_SRC_FILES += ../manual/ui/lua_cocos2dx_experimental_webview_manual.cpp \
                   ../manual/ui/lua_cocos2dx_experimental_video_manual.cpp \
                   ../manual/ui/lua_cocos2dx_ui_manual.cpp \
                   ../auto/lua_cocos2dx_experimental_video_auto.cpp \
                   ../auto/lua_cocos2dx_ui_auto.cpp \
                   ../auto/lua_cocos2dx_experimental_webview_auto.cpp

#quick
LOCAL_SRC_FILES += ../../../../external/lua/quick/lua_cocos2dx_quick_manual.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../external/lua/tolua \
                    $(LOCAL_PATH)/../../../../external/lua/luajit/include \
                    $(LOCAL_PATH)/../../../2d \
                    $(LOCAL_PATH)/../../../3d \
                    $(LOCAL_PATH)/../../../audio \
                    $(LOCAL_PATH)/../../../../external/OpenAL/inc \
                    $(LOCAL_PATH)/../../../2d \
                    $(LOCAL_PATH)/../../../editor-support/cocostudio \
                    $(LOCAL_PATH)/../../../editor-support/cocostudio/ActionTimeline \
                    $(LOCAL_PATH)/../../../editor-support/spine \
                    $(LOCAL_PATH)/../../../ui \
                    $(LOCAL_PATH)/../../../../extensions \
                    $(LOCAL_PATH)/../auto \
                    $(LOCAL_PATH)/../manual \
                    $(LOCAL_PATH)/../manual/cocos2d \
                    $(LOCAL_PATH)/../manual/3d \
                    $(LOCAL_PATH)/../manual/audio \
                    $(LOCAL_PATH)/../manual/cocostudio \
                    $(LOCAL_PATH)/../manual/spine \
                    $(LOCAL_PATH)/../manual/ui \
                    $(LOCAL_PATH)/../../../../external/xxtea \
                    $(LOCAL_PATH)/../../../.. \
                    $(LOCAL_PATH)/../../../../external/lua \
                    $(LOCAL_PATH)/../../../../external/lua/quick

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../external/lua/tolua \
                           $(LOCAL_PATH)/../../../../external/lua/luajit/include \
                           $(LOCAL_PATH)/../auto \
                           $(LOCAL_PATH)/../manual \
                           $(LOCAL_PATH)/../manual/cocos2d \
                           $(LOCAL_PATH)/../manual/3d \
                           $(LOCAL_PATH)/../manual/audio \
                           $(LOCAL_PATH)/../manual/cocostudio \
                           $(LOCAL_PATH)/../manual/spine \
                           $(LOCAL_PATH)/../manual/ui \
                           $(LOCAL_PATH)/../../../../external/lua/quick \
                           $(LOCAL_PATH)/../../../..

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2d_lua_android_static

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,lua/luajit/prebuilt/android)
$(call import-module,.)
