LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-subdir-java-files)

LOCAL_PACKAGE_NAME := canvasRendererTest
LOCAL_CERTIFICATE := platform

LOCAL_JNI_SHARED_LIBRARIES := librenderer_jni

OUTPUT_PATH := $(LOCAL_PATH)/
LOCAL_MODULE_PATH := $(OUTPUT_PATH)/release_arm
LOCAL_UNSTRIPPED_PATH := $(OUTPUT_PATH)/unstripped_arm

include $(BUILD_PACKAGE)
