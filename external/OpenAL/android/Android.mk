LOCAL_PATH := $(call my-dir)

# lib al common
include $(CLEAR_VARS)
LOCAL_MODULE := static_alcommon
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libcommon.a
include $(PREBUILT_STATIC_LIBRARY)

# lib OpenAL
include $(CLEAR_VARS)
LOCAL_MODULE := static_OpenAL
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libopenal.a
LOCAL_STATIC_LIBRARIES += static_alcommon
include $(PREBUILT_STATIC_LIBRARY)
