LOCAL_PATH := $(call my-dir)

# ----- begin of shared Module -----
include $(CLEAR_VARS)

LOCAL_MODULE    := libogg
LOCAL_SRC_FILES := android/libs/$(TARGET_ARCH_ABI)/libogg.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_SHARED_LIBRARY)

# ----- begin of static_Rapid2DAudio -----
include $(CLEAR_VARS)

# Dependency module
LOCAL_STATIC_LIBRARIES := static_OpenAL
LOCAL_SHARED_LIBRARIES := libogg

LOCAL_MODULE := static_Rapid2DAudio
LOCAL_MODULE_FILENAME := libRapid2DAudio

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
	$(LOCAL_PATH)/../ \
	$(LOCAL_PATH)/../../ \
	$(LOCAL_PATH)/../../external/OpenAL/inc

# audio
LOCAL_SRC_FILES := \
	RDAudio.cpp

include $(BUILD_STATIC_LIBRARY)

$(call import-module, OpenAL/android)
