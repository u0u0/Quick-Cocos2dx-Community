LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := dragonBones_static
LOCAL_MODULE_FILENAME := libdragonBones

LOCAL_SRC_FILES :=
ifeq ($(CC_USE_DRAGONBONES),1)
LOCAL_SRC_FILES += animation/Animation.cpp \
				   animation/AnimationState.cpp \
				   animation/BaseTimelineState.cpp \
				   animation/TimelineState.cpp \
				   animation/WorldClock.cpp \
				   armature/Armature.cpp \
				   armature/Bone.cpp \
				   armature/Constraint.cpp \
				   armature/DeformVertices.cpp \
				   armature/Slot.cpp \
				   armature/TransformObject.cpp \
				   cocos2dx/CCArmatureDisplay.cpp \
				   cocos2dx/CCFactory.cpp \
				   cocos2dx/CCSlot.cpp \
				   cocos2dx/CCTextureAtlasData.cpp \
				   core/BaseObject.cpp \
				   core/DragonBones.cpp \
				   event/EventObject.cpp \
				   factory/BaseFactory.cpp \
				   geom/Point.cpp \
				   geom/Transform.cpp \
				   model/AnimationConfig.cpp \
				   model/AnimationData.cpp \
				   model/ArmatureData.cpp \
				   model/BoundingBoxData.cpp \
				   model/CanvasData.cpp \
				   model/ConstraintData.cpp \
				   model/DisplayData.cpp \
				   model/DragonBonesData.cpp \
				   model/SkinData.cpp \
				   model/TextureAtlasData.cpp \
				   model/UserData.cpp \
				   parser/BinaryDataParser.cpp \
				   parser/DataParser.cpp \
				   parser/JSONDataParser.cpp
endif

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_STATIC_LIBRARIES := cocos2dx_internal_static

include $(BUILD_STATIC_LIBRARY)

