LOCAL_PATH := $(call my-dir)/..
include $(CLEAR_VARS)


LOCAL_MODULE    := libvrpresent
LOCAL_CFLAGS    := -DANDROID_NDK   -DGFX_ANDROID_ANativeWindow 

LOCAL_CFLAGS    += -DDEBUG_LOG
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_ANativeWindow.cpp

LOCAL_SRC_FILES += src/LibGfxPresent/GfxPresentOvl.cpp 
LOCAL_SRC_FILES += 3rdparty/imglib/translate.cpp
 
LOCAL_SRC_FILES += src/LibVideoMixer/LibVideoMixer.cpp 
LOCAL_SRC_FILES += src/LibVideoWindow/GfxVideoWindow.cpp
LOCAL_SRC_FILES += inc/GfxPresentPrepare.cpp 
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentAlphaBlending.cpp
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentMain.cpp

LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentProcEbdSW.cpp
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentStreamEbdSW.cpp	

LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentProcSW.cpp
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentStreamSW.cpp	

LOCAL_SRC_FILES += inc/CProtect.cpp 

LOCAL_CFLAGS    += -DSTATIC_IMGPROC_LIB
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/lnx/armv4/ci_imgproc_armv4_lnx.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/lnx/neon/ci_imgproc_neon_lnx.a

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a

LOCAL_LDLIBS    := -llog  
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include \
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
					$(LOCAL_PATH)/3rdparty/linux/libwin32/inc 


LOCAL_C_INCLUDES += $(NDK)/sources/android/cpufeatures
LOCAL_STATIC_LIBRARIES += cpufeatures

LOCAL_LDLIBS    += -landroid

OUTPUT_PATH := $(LOCAL_PATH)

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)
#include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/cpufeatures)

