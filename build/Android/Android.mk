LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# 0 = the other(ISuface, Android 1.5/2.0)
# 1 = Acer(ISuface, sseCSC, Android 1.5)
# "make  liblog libutils libcutils libui libdl libmedia libandroid_runtime"

# 2 = T20(OMX, Android 2.0)
# 3 = DELL_MINI9(ISuface, sseCSC, Android 1.5)
# 4 = G2(Android 1.5,arm, surfaceFlinger)
# 5 = X2(Android 1.5,arm, surfaceFlinger)
# 6 = (Android 1.6,arm, Opengl)
# 7 = (Android 2.1,arm, OpenCore)

# 20 = (Android 2.2,arm,Canvas)
# 21 = (Android 2.2,arm,Surface)
# 22 Android 2.2 special. For foxconn
RENDER_TYPE := 22

ifeq ($(RENDER_TYPE), 0)
#ANDROID_VERSION := 15
ANDROID_VERSION := 21
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/Android/CSCLib_ST_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_CPPFLAGS += -DGFX_ANDROID_SF
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroidSurFlinger.cpp
endif

ifeq ($(RENDER_TYPE), 1) #ACER
ANDROID_VERSION := 15
LOCAL_CFLAGS += -DPLATOFRM_ANDROID_ACERNETBOOK
LOCAL_CPPFLAGS += -DPLATOFRM_ANDROID_ACERNETBOOK
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/Android/CSCLib_x86.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/yuvrgb/libY420RGB.a
LOCAL_CPPFLAGS += -DGFX_ANDROID_SF
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroidSurFlinger.cpp
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_x86.a
endif

ifeq ($(RENDER_TYPE), 3) #DELL-MINI9
ANDROID_VERSION := 15
LOCAL_CFLAGS += -DPLATOFRM_DELL_MINI9
LOCAL_CPPFLAGS += -DPLATOFRM_DELL_MINI9
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/Android/CSCLib_x86.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/yuvrgb/libY420RGB.a
LOCAL_CPPFLAGS += -DGFX_ANDROID_SF
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroidSurFlinger.cpp
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_x86.a
endif

ifeq ($(RENDER_TYPE), 4) #ANDROID_G2
ANDROID_VERSION := 15
LOCAL_CFLAGS += -DPLATOFRM_ANDROID_G2
LOCAL_CPPFLAGS += -DPLATOFRM_ANDROID_G2
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/Android/CSCLib_ST_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_CPPFLAGS += -DGFX_ANDROID_SF
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroidSurFlinger.cpp
LOCAL_STRIP_MODULE = false
endif

ifeq ($(RENDER_TYPE), 5) #X2
ANDROID_VERSION := 15
LOCAL_CFLAGS += -DANDROID_IPP
LOCAL_CPPFLAGS += -DANDROID_IPP
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/Android/CSCLib_ST_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/IPP/Android/libwmmxrzcc.a
LOCAL_CPPFLAGS += -DGFX_ANDROID_SF
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroidSurFlinger.cpp
LOCAL_STRIP_MODULE = false
LOCAL_C_INCLUDES += $(LOCAL_PATH)/3rdparty/IPP/Android
endif

ifeq ($(RENDER_TYPE), 6) #1.6,arm,opengl
#ANDROID_VERSION := 16
#ANDROID_VERSION := 20
ANDROID_VERSION := 22
LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES
LOCAL_CFLAGS += -DGFX_ANDROID_OPENGL_ES
LOCAL_CPPFLAGS += -DGFX_ANDROID_OPENGL_ES
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/Android/CSCLib_ST_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_OpenGlES.cpp
LOCAL_STRIP_MODULE = false
LOCAL_C_INCLUDES += \
	$(call include-path-for, corecg graphics)
#LOCAL_C_INCLUDES += $(LOCAL_PATH)/3rdparty/linux/Android/ndk1.6_GLES
endif


ifeq ($(RENDER_TYPE), 7) 
#ANDROID_VERSION := 15
ANDROID_VERSION := 22
#LOCAL_CFLAGS += -DGFX_ANDROID_OCE 
LOCAL_CFLAGS += -DGFX_ANDROID_OCE  -D__USE_FILE_OFFSET64
LOCAL_CPPFLAGS += -DGFX_ANDROID_OCE -D__USE_FILE_OFFSET64
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/Android/CSCLib_ST_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_OpenCore.cpp
LOCAL_STRIP_MODULE = false
PV_TOP := external/opencore
endif

ifeq ($(RENDER_TYPE), 8) #native bitmap
ANDROID_VERSION := 20
LOCAL_CFLAGS += -DGFX_ANDROID_NATIVE_BITMAP
LOCAL_CPPFLAGS += -DGFX_ANDROID_NATIVE_BITMAP
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/Android/CSCLib_ST_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_NativeBitmap.cpp
LOCAL_STRIP_MODULE = false
LOCAL_C_INCLUDES += \
	$(call include-path-for, corecg graphics)
endif


ifeq ($(RENDER_TYPE), 9) 
ANDROID_VERSION := 22
LOCAL_CFLAGS += -DGFX_ANDROID_OPENGL_ES_SHADER
LOCAL_CPPFLAGS += -DGFX_ANDROID_OPENGL_ES_SHADER
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/Android/CSCLib_ST_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_OpenGlES_shader.cpp
LOCAL_STRIP_MODULE = false
LOCAL_C_INCLUDES += \
	$(call include-path-for, corecg graphics)
endif

ifeq ($(RENDER_TYPE), 20) #canvas
ANDROID_VERSION := 22
LOCAL_CFLAGS += -DGFX_ANDROID_CANVAS  -DSTATIC_IMGPROC_LIB
LOCAL_CPPFLAGS += -DGFX_ANDROID_CANVAS -DSTATIC_IMGPROC_LIB
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/Android/CSCLib.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/Android/CSCLib_neon.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_Canvas.cpp
LOCAL_STRIP_MODULE = false
endif

ifeq ($(RENDER_TYPE), 21) #surface
#ANDROID_VERSION := 22
ANDROID_VERSION := 21

LOCAL_CFLAGS += -DGFX_ANDROID_SURFACE -DSTATIC_IMGPROC_LIB
LOCAL_CPPFLAGS += -DGFX_ANDROID_SURFACE -DSTATIC_IMGPROC_LIB
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/CSCLib_android.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/CSCLib_android_neon.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_Surface.cpp
LOCAL_STRIP_MODULE = false
endif

ifeq ($(RENDER_TYPE), 22) #foxconn
ANDROID_VERSION := 22_1

LOCAL_CFLAGS += -DGFX_ANDROID_SURFACE -DSTATIC_IMGPROC_LIB
LOCAL_CPPFLAGS += -DGFX_ANDROID_SURFACE -DSTATIC_IMGPROC_LIB
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/CSCLib.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/CSCLib_x86.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_Surface.cpp
LOCAL_STRIP_MODULE = false
endif

ifeq ($(RENDER_TYPE), 2) #OMX
ANDROID_VERSION := 21
LOCAL_CFLAGS += -DGFX_ANDROID_OMX
LOCAL_CPPFLAGS += -DGFX_ANDROID_OMX 

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/Android/CSCLib_ST_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid.cpp 
LOCAL_LDFLAGS += -L/home/cidana/ramon/main_fisher/t20_rootfs/system/lib/
#LOCAL_LDFLAGS += -L/home/cidana/ramon/main_fisher/nv/lib/ 
LOCAL_LDFLAGS += -lnvddk_2d
LOCAL_LDFLAGS += -lnvddk_2d_v2
#LOCAL_LDFLAGS += -lnvddk_audiomixer_impl
#LOCAL_LDFLAGS += -lnvddk_audiomixer
LOCAL_LDFLAGS += -lnvddk_disp
LOCAL_LDFLAGS += -lnvdispatch_helper
LOCAL_LDFLAGS += -lnvdispmgr_d
#LOCAL_LDFLAGS += -lnvdispmgr_impl_d
LOCAL_LDFLAGS += -lnvidia_graphics_jni
LOCAL_LDFLAGS += -lnvmm_audio
LOCAL_LDFLAGS += -lnvmm_contentpipe
LOCAL_LDFLAGS += -lnvmm_image
LOCAL_LDFLAGS += -lnvmm_manager_impl
LOCAL_LDFLAGS += -lnvmm_manager
LOCAL_LDFLAGS += -lnvmm_misc
LOCAL_LDFLAGS += -lnvmm_parser
LOCAL_LDFLAGS += -lnvmm_service
LOCAL_LDFLAGS += -lnvmm
LOCAL_LDFLAGS += -lnvmm_tracklist
LOCAL_LDFLAGS += -lnvmm_utils
LOCAL_LDFLAGS += -lnvmm_videorenderer
LOCAL_LDFLAGS += -lnvmm_video
LOCAL_LDFLAGS += -lnvmm_vp6_video
LOCAL_LDFLAGS += -lnvmm_writer
LOCAL_LDFLAGS += -lnvodm_disp
LOCAL_LDFLAGS += -lnvodm_dtvtuner
LOCAL_LDFLAGS += -lnvodm_hdmi
LOCAL_LDFLAGS += -lnvodm_imager
LOCAL_LDFLAGS += -lnvodm_query
LOCAL_LDFLAGS += -lnvomxilclient
LOCAL_LDFLAGS += -lnvomx
LOCAL_LDFLAGS += -lnvos
LOCAL_LDFLAGS += -lnvrm_graphics_impl
LOCAL_LDFLAGS += -lnvrm_graphics
LOCAL_LDFLAGS += -lnvrm
LOCAL_LDFLAGS += -lnvsm
LOCAL_LDFLAGS += -lnvwinsys
LOCAL_LDFLAGS += -lnvwsi
endif


LOCAL_CFLAGS += -DANDROID -D_WIN32
LOCAL_CPPFLAGS += -DANDROID -D_WIN32
LOCAL_CFLAGS += -D__linux__
LOCAL_CPPFLAGS += -D__linux__


LOCAL_CPPFLAGS += -DHAVE_SYS_UIO_H

ifeq ($(ANDROID_VERSION), 20)
	LOCAL_CFLAGS += -DANDROID20
	LOCAL_CPPFLAGS += -DANDROID20
endif


ifeq ($(ANDROID_VERSION), 21)
	LOCAL_CFLAGS += -DANDROID21
	LOCAL_CPPFLAGS += -DANDROID21
endif

ifeq ($(ANDROID_VERSION), 22_1)
	LOCAL_CFLAGS += -DANDROID21
	LOCAL_CPPFLAGS += -DANDROID21
endif

ifeq ($(ANDROID_VERSION), 22)
	LOCAL_CFLAGS += -DANDROID22
	LOCAL_CPPFLAGS += -DANDROID22
endif

ifeq ($(ANDROID_VERSION), 22)
	LOCAL_CFLAGS += -DANDROID22
	LOCAL_CPPFLAGS += -DANDROID22
endif
# LOCAL_CFLAGS +=  -Wfatal-errors
# LOCAL_CPPFLAGS +=   -Wfatal-errors

LOCAL_CFLAGS += -Wno-non-virtual-dtor
# LOCAL_CFLAGS -=  -Werror=non-virtual-dtor
# LOCAL_CPPFLAGS -= -Werror=non-virtual-dtor

LOCAL_C_INCLUDES += \
					$(LOCAL_PATH)/inc \
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
					
		ifeq ($(RENDER_TYPE), 7) 
				LOCAL_C_INCLUDES += \
					$(PV_TOP)/android \
					$(PV_TOP)/engines/common/include \
					$(PV_TOP)/engines/player/config/android \
					$(PV_TOP)/engines/player/include \
					$(PV_TOP)/nodes/pvmediaoutputnode/include \
					$(PV_TOP)/nodes/pvdownloadmanagernode/config/opencore \
					$(PV_TOP)/pvmi/pvmf/include \
					$(PV_TOP)/fileformats/mp4/parser/config/opencore \
					$(PV_TOP)/oscl/oscl/config/android \
					$(PV_TOP)/oscl/oscl/config/shared \
					$(PV_TOP)/engines/author/include \
					$(PV_TOP)/android/drm/oma1/src \
					$(PV_TOP)/build_config/opencore_dynamic \
					$(TARGET_OUT_HEADERS)/libpv \
					frameworks/base/include/ui \
					frameworks/base/include/binder \
					frameworks/base/include/media \
					frameworks/base/include/utils \
					$(JNI_H_INCLUDE)
		endif
		
ifeq ($(RENDER_TYPE), 20) 
	LOCAL_C_INCLUDES += \
		external/skia/include \
		$(LOCAL_PATH)/3rdparty/linux/Android \
		frameworks/base/include
endif



ifeq ($(ANDROID_VERSION), 21)
			LOCAL_C_INCLUDES += \
					./frameworks/base/include/surfaceflinger	\
					$(LOCAL_PATH)/3rdparty/linux/Android \
					./hardware/libhardware/include/hardware
endif
ifeq ($(ANDROID_VERSION), 22_1)
			LOCAL_C_INCLUDES += \
					./frameworks/base/include/surfaceflinger	\
					$(LOCAL_PATH)/3rdparty/linux/Android \
					./hardware/libhardware/include/hardware
endif
ifeq ($(ANDROID_VERSION), 22)
			LOCAL_C_INCLUDES += \
					./frameworks/base/include/surfaceflinger	\
					./hardware/libhardware/include/hardware
endif

LOCAL_C_INCLUDES += $(PV_TOP)/oscl/oscl/config/linux
#                LOCAL_C_INCLUDES += /usr/include
# LOCAL_C_INCLUDES += $(LOCAL_PATH)/yuv
# LOCAL_LDFLAGS += $(LOCAL_PATH)/yuv/libY420RGB.a

#LOCAL_SRC_FILES += src/common/dp.cpp 
#LOCAL_SRC_FILES += src/common/tsemaphore.cpp

#LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroidSurface.cpp 

LOCAL_SRC_FILES += src/LibGfxPresent/AlphaBlending.cpp 
LOCAL_SRC_FILES += src/LibGfxPresent/GfxPresentOvl.cpp 
LOCAL_SRC_FILES += src/LibVideoMixer/LibVideoMixer.cpp 
LOCAL_SRC_FILES += src/LibVideoWindow/GfxVideoWindow.cpp
LOCAL_SRC_FILES += inc/GfxPresentPrepare.cpp 
	

LOCAL_MODULE:= libcidanavrp

LOCAL_SHARED_LIBRARIES := libui libutils libdl
ifeq ($(RENDER_TYPE), 6) #1.6,arm,opengl
LOCAL_SHARED_LIBRARIES += liblog libGLESv1_CM
endif

# LOCAL_SHARED_LIBRARIES += liblog

#ifeq ($(ANDROID_VERSION), 20)
#LOCAL_SHARED_LIBRARIES += libbinder
LOCAL_SHARED_LIBRARIES += libskia
#endif	

ifeq ($(ANDROID_VERSION), 21)
LOCAL_SHARED_LIBRARIES += libbinder
endif

ifeq ($(ANDROID_VERSION), 22_1)
LOCAL_SHARED_LIBRARIES := \
	libutils libcutils libhardware libandroid_runtime \
	liblog libicuuc libdl libui
endif

ifeq ($(ANDROID_VERSION), 22)
LOCAL_SHARED_LIBRARIES += libGLESv2
LOCAL_SHARED_LIBRARIES += libsurfaceflinger_client
endif	


ifeq ($(RENDER_TYPE), 7) 
LOCAL_SHARED_LIBRARIES += \
	libutils libcutils libui libhardware libandroid_runtime \
	liblog libicuuc libdl \
	libopencore_player libopencore_common
endif

# LOCAL_SHARED_LIBRARIES += libomx_sharedlibrary

OUTPUT_PATH := $(LOCAL_PATH)
LOCAL_MODULE_PATH := $(OUTPUT_PATH)/release_arm
LOCAL_UNSTRIPPED_PATH := $(OUTPUT_PATH)/unstripped_arm

LOCAL_PRELINK_MODULE:=false
include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)
#include $(BUILD_STATIC_LIBRARY)
