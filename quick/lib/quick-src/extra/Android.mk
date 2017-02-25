
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := extra_static
LOCAL_MODULE_FILENAME := libextra

LOCAL_SRC_FILES := \
    $(LOCAL_PATH)/luabinding/cocos2dx_extra_luabinding.cpp \
    $(LOCAL_PATH)/luabinding/HelperFunc_luabinding.cpp \
    $(LOCAL_PATH)/apptools/HelperFunc.cpp \
    $(LOCAL_PATH)/crypto/CCCrypto.cpp \
    $(LOCAL_PATH)/crypto/base64/libbase64.c \
    $(LOCAL_PATH)/network/CCNetwork.cpp \
    $(LOCAL_PATH)/platform/android/CCCryptoAndroid.cpp \
    $(LOCAL_PATH)/platform/android/CCNativeAndroid.cpp \
    $(LOCAL_PATH)/platform/android/CCNetworkAndroid.cpp

ifeq ($(CC_USE_CURL),1)
LOCAL_SRC_FILES += $(LOCAL_PATH)/network/CCHTTPRequest.cpp
else
LOCAL_SRC_FILES += $(LOCAL_PATH)/platform/android/CCHTTPRequestAndroid.cpp
endif

# bugfix for md5_init not found on Android 5.x
ifeq ($(CC_USE_CURL),0)
ifeq ($(CC_USE_WEBSOCKET),0)
LOCAL_SRC_FILES += $(LOCAL_PATH)/crypto/md5/md5.c
endif
endif

#filters
LOCAL_SRC_FILES += \
    $(LOCAL_PATH)/luabinding/lua_cocos2dx_extension_filter_auto.cpp \
    $(LOCAL_PATH)/filters/filters/CCFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCBlurFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCBrightnessFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCContrastFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCDropShadowFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCExposureFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCGammaFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCGrayFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCHazeFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCHueFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCMaskFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCRGBFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCSaturationFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCSepiaFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCSharpenFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCTestFilter.cpp \
    $(LOCAL_PATH)/filters/filters/CCCustomFilter.cpp \
    $(LOCAL_PATH)/filters/nodes/CCFilteredSprite.cpp \
    $(LOCAL_PATH)/filters/shaders/ccFilterShaders.cpp 

#nanovg
LOCAL_SRC_FILES += \
    $(LOCAL_PATH)/luabinding/lua_cocos2dx_extension_nanovg_auto.cpp \
    $(LOCAL_PATH)/luabinding/lua_cocos2dx_extension_nanovg_manual.cpp \
    $(LOCAL_PATH)/nanovg/nanovg/nanovg.c \
    $(LOCAL_PATH)/nanovg/nanonode/NVGNode.cpp \
    $(LOCAL_PATH)/nanovg/nanonode/NVGDrawNode.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/luabinding

LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES) \
                    $(COCOS2DX_ROOT)/cocos \
                    $(COCOS2DX_ROOT)/external/lua/luajit/include \
                    $(COCOS2DX_ROOT)/external/lua/tolua \
                    $(COCOS2DX_ROOT)/external \
                    $(COCOS2DX_ROOT)/cocos/scripting/lua-bindings/manual \
					$(COCOS2DX_ROOT)/extensions \
					$(COCOS2DX_ROOT)/external/curl/include/android


include $(BUILD_STATIC_LIBRARY)

