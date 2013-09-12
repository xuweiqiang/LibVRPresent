LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# 0 = the other(ISuface, Android 1.5/2.0)
# 1 = StageFright
# "make  liblog libutils libcutils libui libdl libmedia libandroid_runtime"

# 2 = T20(OMX, Android 2.0)
# 3 = DELL_MINI9(ISuface, sseCSC, Android 1.5)
# 4 = G2(Android 1.5,arm, surfaceFlinger)
# 5 = X2(Android 1.5,arm, surfaceFlinger)
# 6 = (Android 1.6,arm, Opengl)
# 7 = (Android 2.1,arm, OpenCore)

# 20 = (Android 2.2,arm,Canvas)
# 21 = (Android 2.2,arm,Surface)
# 22 = (Android 2.1,arm,Surface)
# 23 = (Android 2.1,arm,OpenCore)

# 30 = (Android 4.0,arm,Software native window)

#SURFACEMODE_VM := 1

#MAP200 :=1
ifeq ($(MAP200),1)
RENDER_TYPE := 24
ANDROID_VERSION := 22
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx_map200/CSCLib_map200.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx_map200/CSCLib_arm_map200.a
endif


ifeq ($(SURFACEMODE_VM),1)
RENDER_TYPE := 28
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentProcAndroid.cpp
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentStreamAndroid.cpp	
LOCAL_CFLAGS += -DANDROID_VM
LOCAL_CPPFLAGS += -DANDROID_VM
else
RENDER_TYPE := 24
#RENDER_TYPE := 2
#LOCAL_CFLAGS += -DRENESAS_ANDROID
#LOCAL_CFLAGS += -DMX53
endif

LOCAL_LDLIBS := -llog
#HERO_2_2 := 1
LOCAL_SHARED_LIBRARIES := libdl libutils #libui 

ifeq ($(RENDER_TYPE), 0)
ANDROID_VERSION := 21
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_CFLAGS += -DGFX_ANDROID_SF
LOCAL_CPPFLAGS += -DGFX_ANDROID_SF 
LOCAL_CFLAGS += -DOPENCORE_CC
LOCAL_CPPFLAGS += -DOPENCORE_CC
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroidSurFlinger.cpp
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/android/dummy.so
PV_TOP := external/opencore

LOCAL_C_INCLUDES += \
	$(TARGET_OUT_HEADERS)/libpv \
	external/opencore/codecs_v2/utilities/colorconvert/include/	\
	external/opencore/oscl/oscl/config/android \
	external/opencore/oscl/oscl/config/shared 
endif

ifeq ($(RENDER_TYPE), 1) #STAGEFRIGHT
ANDROID_VERSION := 22
LOCAL_CFLAGS += -DGFX_ANDROID_STAGEFRIGHT
LOCAL_CPPFLAGS += -DGFX_ANDROID_STAGEFRIGHT 

LOCAL_CFLAGS += -DSTATIC_IMGPROC_LIB
LOCAL_CPPFLAGS += -DSTATIC_IMGPROC_LIB
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib_arm.a

LOCAL_SRC_FILES += src/LibGfxContext/gfx_android_stagefright.cpp

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a

LOCAL_C_INCLUDES += \
	$(TARGET_OUT_HEADERS)/libpv \
	external/opencore/codecs_v2/utilities/colorconvert/include/	\
	external/opencore/oscl/oscl/config/android \
	frameworks/base/include/media \
	frameworks/base/include/binder \
	frameworks/base/include/ui \
	frameworks/base/include/surfaceflinger \
	frameworks/base/media/libstagefright/include	\
	frameworks/base/include/media/stagefright	\
	frameworks/base/include/android_runtime	\
	external/opencore/oscl/oscl/config/shared

LOCAL_SHARED_LIBRARIES += libstagefright libsurfaceflinger_client libstagefright_color_conversion libui libutils libdl

endif

ifeq ($(RENDER_TYPE), 2) #ISURFACE_OVERLAY
#ANDROID_VERSION := 22
ANDROID_VERSION := 23
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_CFLAGS += -DGFX_ANDROID_ISURFACE_OVERLAY
LOCAL_CPPFLAGS += -DGFX_ANDROID_ISURFACE_OVERLAY
LOCAL_CFLAGS += -DSTATIC_IMGPROC_LIB
LOCAL_CPPFLAGS += -DSTATIC_IMGPROC_LIB
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/CSCLib_android_neon.a


LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_ISurfaceOverlay.cpp

LOCAL_SHARED_LIBRARIES += libui libcutils
LOCAL_C_INCLUDES += \
	$(TARGET_OUT_HEADERS)/libpv \
	external/opencore/codecs_v2/utilities/colorconvert/include/	\
	external/opencore/oscl/oscl/config/android \
	frameworks/base/include/media \
	frameworks/base/include/binder \
	frameworks/base/include/ui \
	frameworks/base/include/surfaceflinger \
	hardware/libhardware/include/hardware	\
	external/opencore/oscl/oscl/config/shared 
endif

ifeq ($(RENDER_TYPE), 3) #NativeWindow
ANDROID_VERSION := 40
LOCAL_CPPFLAGS += -DGFX_ANDROID_NATIVEWINDOW -DSTATIC_IMGPROC_LIB -DANDROID_VR_JNI 

#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/neon/CSCLib.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/neon/CSCLib_neon.a

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/libutils.so

LOCAL_SHARED_LIBRARIES += libgui
LOCAL_SHARED_LIBRARIES += 	 \
	libstagefright \
	libstagefright_omx 


LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_NativeWindow.cpp
LOCAL_STRIP_MODULE = false
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

ifeq ($(RENDER_TYPE), 24) #surface
ANDROID_VERSION := 40
#ANDROID_VERSION := 23
#ANDROID_VERSION := 22
#ANDROID_VERSION := 21

LOCAL_CPPFLAGS += -DGFX_ANDROID_SURFACE -DSTATIC_IMGPROC_LIB -DANDROID_VR_JNI 

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/lnx/armv4/ci_imgproc_armv4_lnx.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/lnx/neon/ci_imgproc_neon_lnx.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/neon/CSCLib.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/neon/CSCLib_neon.a


LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a

#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/libutils.so

ifeq ($(ANDROID_VERSION),21)
LOCAL_SHARED_LIBRARIES += libbinder libui
endif

ifeq ($(ANDROID_VERSION),22)
LOCAL_SHARED_LIBRARIES += libbinder libui libsurfaceflinger_client
endif

ifeq ($(ANDROID_VERSION),23)
LOCAL_SHARED_LIBRARIES += libbinder libui libsurfaceflinger_client
endif

ifeq ($(ANDROID_VERSION),40)
LOCAL_SHARED_LIBRARIES += libgui
endif

ifeq ($(HERO_2_2),1) 
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/android/Hero_2.2/libui.so
endif
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/android/G2_2.2/libui.so
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/android/G2_2.2/libsurfaceflinger_client.so

LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_Surface.cpp
LOCAL_STRIP_MODULE = false
endif

ifeq ($(RENDER_TYPE), 25) #OpenCore
ANDROID_VERSION := 22
LOCAL_CFLAGS += -DGFX_ANDROID_OPENCORE -DSTATIC_IMGPROC_LIB  
LOCAL_CPPFLAGS += -DGFX_ANDROID_OPENCORE -DSTATIC_IMGPROC_LIB -fno-rtti -fno-exceptions 

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/CSCLib_android_neon.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a

#LOCAL_SHARED_LIBRARIES += libopencore_player 
#LOCAL_SHARED_LIBRARIES += libopencore_common 

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/android/Renesas_2.2/libopencore_common.so
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/android/Renesas_2.2/libopencore_player.so
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/android/Renesas_2.2/libopencorehw.so

LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_OPENCORE.cpp
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_OpenCoreMyRenderer.cpp
LOCAL_STRIP_MODULE = false
PV_TOP := external/opencore
endif


ifeq ($(RENDER_TYPE), 26) #Samsung Android
ANDROID_VERSION := 21
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_CFLAGS += -DGFX_ANDROID_SF

LOCAL_CPPFLAGS += -DGFX_ANDROID_SF 
LOCAL_CFLAGS += -DSTATIC_IMGPROC_LIB
LOCAL_CPPFLAGS += -DSTATIC_IMGPROC_LIB
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib_arm.a

LOCAL_CFLAGS += -DSAMSUNG_ANDROID
LOCAL_CPPFLAGS += -DSAMSUNG_ANDROID

LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroidSurFlinger.cpp
#PV_TOP := external/opencore

LOCAL_C_INCLUDES += \
	$(TARGET_OUT_HEADERS)/libpv \
	external/opencore/codecs_v2/utilities/colorconvert/include/	\
	external/opencore/oscl/oscl/config/android \
	frameworks/base/include/media \
	frameworks/base/include/binder \
	frameworks/base/include/ui \
	external/opencore/oscl/oscl/config/shared 
endif

ifeq ($(RENDER_TYPE), 27) #VideoMixer SurfaceFlinger
ANDROID_VERSION := 21
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_CFLAGS += -DGFX_ANDROID_SF
LOCAL_CPPFLAGS += -DGFX_ANDROID_SF 

LOCAL_CFLAGS += -DANDROIDMIXER
LOCAL_CPPFLAGS += -DANDROIDMIXER 

LOCAL_CFLAGS += -DSTATIC_IMGPROC_LIB
LOCAL_CPPFLAGS += -DSTATIC_IMGPROC_LIB
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib_arm.a


LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroidSurFlinger.cpp

LOCAL_C_INCLUDES += \
	$(TARGET_OUT_HEADERS)/libpv \
	external/opencore/codecs_v2/utilities/colorconvert/include/	\
	external/opencore/oscl/oscl/config/android \
	frameworks/base/include/media \
	frameworks/base/include/binder \
	frameworks/base/include/ui \
	external/opencore/oscl/oscl/config/shared 
endif


ifeq ($(RENDER_TYPE), 28) #2.2 iSurfaceMode
ANDROID_VERSION := 22
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_CFLAGS += -DGFX_ANDROID_SF
LOCAL_CPPFLAGS += -DGFX_ANDROID_SF 

#LOCAL_CFLAGS += -DOPENCORE_CC
#LOCAL_CPPFLAGS += -DOPENCORE_CC
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/android/libopencore_common.so

LOCAL_CFLAGS += -DSTATIC_IMGPROC_LIB
LOCAL_CPPFLAGS += -DSTATIC_IMGPROC_LIB
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/CSCLib_android_neon.a

LOCAL_SHARED_LIBRARIES += libbinder libui
LOCAL_SHARED_LIBRARIES += libsurfaceflinger_client 
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroidSurFlinger.cpp
#PV_TOP := external/opencore

LOCAL_C_INCLUDES += \
	$(TARGET_OUT_HEADERS)/libpv \
	external/opencore/codecs_v2/utilities/colorconvert/include/	\
	external/opencore/oscl/oscl/config/android \
	frameworks/base/include/media \
	frameworks/base/include/binder \
	frameworks/base/include/ui \
	frameworks/base/include/surfaceflinger \
	external/opencore/oscl/oscl/config/shared 
endif


ifeq ($(RENDER_TYPE), 29) #NexusOne 2.2 OpenCore
ANDROID_VERSION := 22
LOCAL_CFLAGS += -DGFX_ANDROID_OPENCORE -DSTATIC_IMGPROC_LIB  
LOCAL_CPPFLAGS += -DGFX_ANDROID_OPENCORE -DSTATIC_IMGPROC_LIB -fno-rtti -fno-exceptions 
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib.a
#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/arm_lnx/CSCLib_arm.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/CSCLib_android_neon.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_OPENCORE.cpp
LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_OpenCoreMyRenderer.cpp

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/android/NexusOne_2.2/libopencore_common.so
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/android/NexusOne_2.2/libopencore_player.so
LOCAL_STRIP_MODULE = false

PV_TOP := external/opencore

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
	frameworks/base/include/media 
	

endif

ifeq ($(RENDER_TYPE), 30) #Android40 native window software
ANDROID_VERSION := 40

LOCAL_CPPFLAGS += -DGFX_ANDROID_SOFTWARE -DSTATIC_IMGPROC_LIB -DANDROID_VR_JNI 

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/imglib/lnx/arm/ci_imgproc_armv6_lnx.a

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/linux/libwin32/lib/libWin32_arm.a

#LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/libutils.so

ifeq ($(ANDROID_VERSION),40)
LOCAL_SHARED_LIBRARIES += libgui libui
endif

LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroid_Software.cpp
LOCAL_STRIP_MODULE = false
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
	LOCAL_CFLAGS += -DANDROID_VER=22
	LOCAL_CFLAGS += -DANDROID22
	LOCAL_CPPFLAGS += -DANDROID22
endif

ifeq ($(ANDROID_VERSION), 23)
	LOCAL_CFLAGS += -DANDROID_VER=23
	LOCAL_CFLAGS += -DANDROID23
	LOCAL_CPPFLAGS += -DANDROID23
	LOCAL_MODULE_TAGS := optional
endif

ifeq ($(ANDROID_VERSION), 40)
	LOCAL_MODULE_TAGS := optional
	LOCAL_CFLAGS += -DANDROID40
	LOCAL_CPPFLAGS += -DANDROID40
endif

# LOCAL_CFLAGS +=  -Wfatal-errors
# LOCAL_CPPFLAGS +=   -Wfatal-errors

LOCAL_CFLAGS += -Wno-non-virtual-dtor
# LOCAL_CFLAGS -=  -Werror=non-virtual-dtor
# LOCAL_CPPFLAGS -= -Werror=non-virtual-dtor

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
					frameworks/base/include/utils 
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

ifeq ($(ANDROID_VERSION), 40)
			LOCAL_C_INCLUDES += \
					./frameworks/native/include	\
					./frameworks/native/include/gui/	\
					./frameworks/base/include/surfaceflinger	\
					./hardware/libhardware/include/hardware
endif

ifeq ($(RENDER_TYPE), 23)
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
	frameworks/base/include/media 
	
endif


ifeq ($(RENDER_TYPE), 25)
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
	frameworks/base/include/media 
	
endif

LOCAL_C_INCLUDES += $(PV_TOP)/oscl/oscl/config/linux
#                LOCAL_C_INCLUDES += /usr/include
# LOCAL_C_INCLUDES += $(LOCAL_PATH)/yuv
# LOCAL_LDFLAGS += $(LOCAL_PATH)/yuv/libY420RGB.a

#LOCAL_SRC_FILES += src/common/dp.cpp 
#LOCAL_SRC_FILES += src/common/tsemaphore.cpp

#LOCAL_SRC_FILES += src/LibGfxContext/GfxAndroidSurface.cpp 

LOCAL_SRC_FILES += src/LibGfxPresent/GfxPresentOvl.cpp 
LOCAL_SRC_FILES += src/LibVideoMixer/LibVideoMixer.cpp 
LOCAL_SRC_FILES += src/LibVideoWindow/GfxVideoWindow.cpp
LOCAL_SRC_FILES += inc/GfxPresentPrepare.cpp 
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentAlphaBlending.cpp
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentMain.cpp

LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentProcEbdSW.cpp
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentStreamEbdSW.cpp	

ifneq ($(SURFACEMODE_VM),1)
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentProcSW.cpp
LOCAL_SRC_FILES += src_v2/LibGfxPresent/GfxPresentStreamSW.cpp	
endif

LOCAL_MODULE:= libvrpresent

ifeq ($(RENDER_TYPE), 6) #1.6,arm,opengl
LOCAL_SHARED_LIBRARIES += liblog libGLESv1_CM
endif

# LOCAL_SHARED_LIBRARIES += liblog


ifeq ($(ANDROID_VERSION), 21)
LOCAL_SHARED_LIBRARIES += libbinder
endif

ifeq ($(ANDROID_VERSION), 22_1)
LOCAL_SHARED_LIBRARIES := \
	libutils libcutils libhardware libandroid_runtime \
	liblog libicuuc libdl libui
endif

ifeq ($(ANDROID_VERSION), 22)
#LOCAL_SHARED_LIBRARIES += libbinder
#LOCAL_SHARED_LIBRARIES += libGLESv2
#LOCAL_SHARED_LIBRARIES += libsurfaceflinger_client
endif	


ifeq ($(RENDER_TYPE), 0)
LOCAL_SHARED_LIBRARIES += libopencore_common 
endif	

ifeq ($(RENDER_TYPE), 23)
LOCAL_SHARED_LIBRARIES += libopencore_player 
LOCAL_SHARED_LIBRARIES += libopencore_common 
endif	

ifeq ($(RENDER_TYPE), 25)
#LOCAL_SHARED_LIBRARIES += libopencore_player 
#LOCAL_SHARED_LIBRARIES += libopencore_common 
endif	

ifeq ($(RENDER_TYPE), 28)
#LOCAL_SHARED_LIBRARIES += libopencore_player 
#LOCAL_SHARED_LIBRARIES += libopencore_common 
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
