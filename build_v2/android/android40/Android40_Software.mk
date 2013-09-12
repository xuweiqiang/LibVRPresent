LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_LDLIBS := -llog
LOCAL_SHARED_LIBRARIES := libdl libutils libgui libui

LOCAL_CPPFLAGS += -DGFX_ANDROID_SOFTWARE -DSTATIC_IMGPROC_LIB -DANDROID_VR_JNI 

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/lnx/arm/ci_imgproc_armv6_lnx.a

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a

LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_Software.cpp
LOCAL_STRIP_MODULE = false

LOCAL_CFLAGS += -DANDROID -D_WIN32
LOCAL_CPPFLAGS += -DANDROID -D_WIN32
LOCAL_CFLAGS += -D__linux__
LOCAL_CPPFLAGS += -D__linux__

LOCAL_CPPFLAGS += -DHAVE_SYS_UIO_H

LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS += -DANDROID40
LOCAL_CPPFLAGS += -DANDROID40

LOCAL_CFLAGS += -Wno-non-virtual-dtor

LOCAL_C_INCLUDES += \
					$(LOCAL_PATH)/inc \
					$(LOCAL_PATH)/3rdparty	\
					$(LOCAL_PATH)/3rdparty/linux/Android/nv/openmax/il \
					$(LOCAL_PATH)/src/common \
					$(LOCAL_PATH)/src/LibGfxContext \
					$(LOCAL_PATH)/src/LibGfxPresent \
					$(LOCAL_PATH)/src/LibVideoMixer \
					$(LOCAL_PATH)/src/LibVideoWindow \
					$(LOCAL_PATH)/3rdparty/linux \
					$(LOCAL_PATH)/3rdparty/linux/Android/win32/include \
					$(LOCAL_PATH)/3rdparty/imglib \
					$(LOCAL_PATH)/3rdparty/linux/libwin32/inc \
					$(JNI_H_INCLUDE)

LOCAL_C_INCLUDES += \
					./frameworks/base/include/surfaceflinger	\
					./hardware/libhardware/include/hardware


LOCAL_C_INCLUDES += $(PV_TOP)/oscl/oscl/config/linux

LOCAL_SRC_FILES += src/LibGfxPresent/GfxPresentOvl.cpp 
LOCAL_SRC_FILES += src/LibVideoMixer/LibVideoMixer.cpp 
LOCAL_SRC_FILES += src/LibVideoWindow/GfxVideoWindow.cpp
LOCAL_SRC_FILES += inc/GfxPresentPrepare.cpp 
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentAlphaBlending.cpp
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentMain.cpp

LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentProcEbdSW.cpp
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentStreamEbdSW.cpp	

LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentProcSW.cpp
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentStreamSW.cpp	

LOCAL_MODULE:= libvrpresent

OUTPUT_PATH := $(LOCAL_PATH)
LOCAL_MODULE_PATH := $(OUTPUT_PATH)/release_arm
LOCAL_UNSTRIPPED_PATH := $(OUTPUT_PATH)/unstripped_arm

LOCAL_PRELINK_MODULE:=false
include $(BUILD_SHARED_LIBRARY)
