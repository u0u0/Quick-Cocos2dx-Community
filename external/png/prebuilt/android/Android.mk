LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos_png_static
LOCAL_MODULE_FILENAME := png
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libpng.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include/android
LOCAL_STATIC_LIBRARIES += cocos_libz_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos_libz_static
LOCAL_MODULE_FILENAME := libz
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libz.a
include $(PREBUILT_STATIC_LIBRARY)
