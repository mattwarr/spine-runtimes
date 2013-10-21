LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libspine

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/spine-c
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include

FILE_LIST := $(wildcard $(LOCAL_PATH)/*.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/spine-c/*.c)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_CFLAGS	:= -DFIXED_POINT -Werror -ffast-math -O3 -D_ARM_ASSEM_ -DANDROID
LOCAL_CPPFLAGS	:= -DFIXED_POINT -D_ARM_ASSEM_
LOCAL_LDLIBS	:= -llog -landroid

include $(BUILD_SHARED_LIBRARY)
