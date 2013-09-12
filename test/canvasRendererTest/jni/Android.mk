LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

PV_TOP := external/opencore

PV_CFLAGS := -Wno-non-virtual-dtor -DENABLE_MEMORY_PLAYBACK -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -DUSE_CML2_CONFIG

#ANDROID_VER := 1.6
#ANDROID_VER := 2.1
#ANDROID_VER := 1.5
ANDROID_VER := 2.2

ifeq ($(ANDROID_VER),1.5)
LOCAL_CFLAGS :=  -DANDROID_VER=15
else
LOCAL_CFLAGS :=  -DANDROID_VER=21
endif

ifeq ($(ANDROID_VER),2.2)
LOCAL_CFLAGS :=  -DANDROID_VER=22
endif

# time bomb options
USING_TIMEBOMB := 1

ifeq ($(USING_TIMEBOMB),1)
LOCAL_CFLAGS +=  -D_ENABLE_TIME_BOMB_ -DTIME_BOMB_YEAR=2010 -DTIME_BOMB_MONTH=6 -DTIME_BOMB_DAY=30
endif

#LOCAL_CFLAGS += $(PV_CFLAGS)

LOCAL_SRC_FILES := \
	renderer_jni.cpp

# set local parameters
ifeq ($(TARGET_ARCH),arm)
ARCH_PREFIX := arm
endif

ifeq ($(TARGET_ARCH),x86)
ARCH_PREFIX := linux_x86
endif

EXTERNAL_DEP_PATH := $(LOCAL_PATH)/../externaldep

OUTPUT_PATH := $(LOCAL_PATH)/../

# set include path
LOCAL_C_INCLUDES := \
	$(JNI_H_INCLUDE) \
	external/skia/include \
	frameworks/base/include

LOCAL_ARM_MODE := arm

LOCAL_PRELINK_MODULE := false

LOCAL_STATIC_LIBRARIES := 

# set dynamic lib
LOCAL_SHARED_LIBRARIES := \
	libutils libcutils libui libhardware libandroid_runtime \
	liblog libicuuc libdl libskia

ifeq ($(ANDROID_VER), 2.2)
#LOCAL_SHARED_LIBRARIES += libstagefright libsurfaceflinger_client libstagefright_color_conversion libstagefright_omx
endif

# set external static lib
LOCAL_LDFLAGS := 

# set binary output path
LOCAL_MODULE := librenderer_jni
LOCAL_MODULE_PATH := $(OUTPUT_PATH)/release_$(ARCH_PREFIX)
LOCAL_UNSTRIPPED_PATH := $(OUTPUT_PATH)/unstripped_$(ARCH_PREFIX)


include $(BUILD_SHARED_LIBRARY)

