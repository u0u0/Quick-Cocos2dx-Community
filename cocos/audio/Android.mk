LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_STATIC_LIBRARIES := static_OpenAL

LOCAL_MODULE := static_Rapid2DAudio
LOCAL_MODULE_FILENAME := libRapid2DAudio

# ogg soft decode performance optimization
LOCAL_CFLAGS := -mfpu=vfp -mfloat-abi=softfp -O2

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
	$(LOCAL_PATH)/../ \
	$(LOCAL_PATH)/../../ \
	$(LOCAL_PATH)/libogg \
	$(LOCAL_PATH)/libvorbis/lib \
	$(LOCAL_PATH)/libvorbis \
	$(LOCAL_PATH)/../../external/OpenAL/inc

# audio
LOCAL_SRC_FILES := \
	RDAudio.cpp \
	RDAudioOgg.cpp
# libogg
LOCAL_SRC_FILES += \
	libogg/src/bitwise.c \
	libogg/src/framing.c
# libvorbis
LOCAL_SRC_FILES += \
	libvorbis/lib/analysis.c \
	libvorbis/lib/barkmel.c \
	libvorbis/lib/bitrate.c \
	libvorbis/lib/block.c \
	libvorbis/lib/codebook.c \
	libvorbis/lib/envelope.c \
	libvorbis/lib/floor0.c \
	libvorbis/lib/floor1.c \
	libvorbis/lib/info.c \
	libvorbis/lib/lookup.c \
	libvorbis/lib/lpc.c \
	libvorbis/lib/lsp.c \
	libvorbis/lib/mapping0.c \
	libvorbis/lib/mdct.c \
	libvorbis/lib/psy.c \
	libvorbis/lib/registry.c \
	libvorbis/lib/res0.c \
	libvorbis/lib/sharedbook.c \
	libvorbis/lib/smallft.c \
	libvorbis/lib/synthesis.c \
	libvorbis/lib/tone.c \
	libvorbis/lib/vorbisenc.c \
	libvorbis/lib/vorbisfile.c \
	libvorbis/lib/window.c

include $(BUILD_STATIC_LIBRARY)

$(call import-module, OpenAL/android)
