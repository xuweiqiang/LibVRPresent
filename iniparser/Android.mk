LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=		\
		src/iniparser.c	\
		src/dictionary.c
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/src/

LOCAL_MODULE:= libiniparser


include $(BUILD_STATIC_LIBRARY)




include $(CLEAR_VARS)

LOCAL_SRC_FILES:=		\
		test/iniexample.c
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/src/

LOCAL_MODULE:= inidemo 

LOCAL_SHARED_LIBRARIES := libui libutils
LOCAL_STATIC_LIBRARIES := libiniparser
include $(BUILD_EXECUTABLE)
