
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := lua_extensions_static
LOCAL_MODULE_FILENAME := libluaextensions

LOCAL_SRC_FILES := $(LOCAL_PATH)/lua_extensions_more.c \
	$(LOCAL_PATH)/zlib/lua_zlib.c \
	$(LOCAL_PATH)/filesystem/lfs.c \
	$(LOCAL_PATH)/lpack/lpack.c \
	$(LOCAL_PATH)/cjson/fpconv.c \
	$(LOCAL_PATH)/cjson/lua_cjson.c \
	$(LOCAL_PATH)/cjson/strbuf.c \

ifeq ($(CC_USE_SQLITE),1)
LOCAL_SRC_FILES += $(LOCAL_PATH)/lsqlite3/sqlite3.c \
				   $(LOCAL_PATH)/lsqlite3/lsqlite3.c
endif

ifeq ($(CC_USE_UNQLITE),1)
LOCAL_SRC_FILES +=$(LOCAL_PATH)/unqlite/unqlite.c \
				  $(LOCAL_PATH)/unqlite/lunqlite.c
endif

ifeq ($(CC_USE_PROTOBUF),1)
LOCAL_SRC_FILES +=$(LOCAL_PATH)/protobuf/pb.c
endif

ifeq ($(CC_USE_SPROTO),1)
LOCAL_SRC_FILES +=$(LOCAL_PATH)/sproto/sproto.c \
				  $(LOCAL_PATH)/sproto/lsproto.c \
				  $(LOCAL_PATH)/lpeg/lpcap.c \
				  $(LOCAL_PATH)/lpeg/lpcode.c \
				  $(LOCAL_PATH)/lpeg/lpprint.c \
				  $(LOCAL_PATH)/lpeg/lptree.c \
				  $(LOCAL_PATH)/lpeg/lpvm.c
endif

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/ \
	$(LOCAL_PATH)/cjson \
	$(LOCAL_PATH)/zlib \
	$(LOCAL_PATH)/debugger \
	$(LOCAL_PATH)/filesystem \
	$(LOCAL_PATH)/lpack \
	$(LOCAL_PATH)/lsqlite3 \
	$(LOCAL_PATH)/unqlite

LOCAL_C_INCLUDES := $(LOCAL_PATH)/ \
	$(LOCAL_PATH)/cjson \
	$(LOCAL_PATH)/zlib \
	$(LOCAL_PATH)/debugger \
	$(LOCAL_PATH)/filesystem \
	$(LOCAL_PATH)/lpack \
	$(LOCAL_PATH)/lsqlite3 \
	$(LOCAL_PATH)/unqlite \
	$(COCOS2DX_ROOT)/cocos \
	$(COCOS2DX_ROOT)/external/lua/luajit/include

include $(BUILD_STATIC_LIBRARY)

