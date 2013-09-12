###############################################################################
## TAL library template Makefile
## Copyright (c) 2007 Cidana, Inc.
###############################################################################

include $(TOP)/config

#directories for the project
OUTPUT = libvrpresent.so
OUTPUT_PATH = ../../../lib

SRC_DIRS = ../../../src/LibGfxContext ../../../src/LibGfxPresent ../../../src/LibVideoWindow ../../../src/LibVideoMixer  ../../../src_v2/LibGfxPresent ../../../inc ../../../3rdparty/imglib
INC_DIRS := ../../../inc/  ../../../3rdparty/linux ../../../3rdparty/linux/libwin32/inc ../../../3rdparty/imglib/arm_lnx ../../../src_v2/LibGfxPresent 
OBJ_DIR = ./obj

ifeq ($(BUILD_PLATFORM),ST)
INC_DIRS += ../../../3rdparty/ST_Linux/ 
endif

ifeq ($(BUILD_PLATFORM),ST_NODATACOPY)
INC_DIRS += ../../../3rdparty/ST_Linux/noDataCopy/ 
endif

#defines

#library path and dependent libraries
LIB_DIRS = ../../../3rdparty/linux/libwin32/lib ../../../3rdparty/ST_Linux/noDataCopy .

#libraries will be linked with "-l" option (not the prefix lib should be omitted)
ifeq ($(BUILD_PLATFORM),)
LIBRARIES = SDL X11
else
LIBRARIES = pthread dl

	ifeq ($(BUILD_PLATFORM),ST)
	LIBRARIES += Win32_st
	LIBRARIES += rt
	endif

	ifeq ($(BUILD_PLATFORM),ST_NODATACOPY)
	LIBRARIES += Win32_st
	LIBRARIES += rt
#	LIBRARIES += stg
	endif

	ifeq ($(BUILD_PLATFORM),PXA_LINUX)
	LIBRARIES += Win32_PXA
	endif
	
endif
#libraries with full name 

ifeq ($(BUILD_PLATFORM),PXA_LINUX)
EXTRA_DEPENDENCIES = ../../../3rdparty/linux/PXA/CSCLib.a
endif

ifeq ($(BUILD_PLATFORM),ST)
EXTRA_DEPENDENCIES = ../../../3rdparty/imglib/arm_lnx/CSCLib.a
EXTRA_DEPENDENCIES += ../../../3rdparty/imglib/arm_lnx/CSCLib_arm.a
EXTRA_DEPENDENCIES += ../../../3rdparty/ST_Linux/libstg.a
endif
ifeq ($(BUILD_PLATFORM),ST_NODATACOPY)
EXTRA_DEPENDENCIES = ../../../3rdparty/imglib/lnx/armv6/ci_imgproc_armv6_lnx.a
#EXTRA_DEPENDENCIES += ../../../3rdparty/imglib/arm_lnx/CSCLib_arm.a
#EXTRA_DEPENDENCIES += ../../../3rdparty/ST_Linux/noDataCopy/libstg.a
EXTRA_DEPENDENCIES += ../../../3rdparty/ST_Linux/noDataCopy/libstg.so.0
endif


#output format
#should be one of APPLICATION, STATIC_LIB, DYNAMIC_LIB
#BUILD_FORMAT=APPLICATION
BUILD_FORMAT=DYNAMIC_LIB
#merge the OUTPUT and the following static librarys into one static library
MERGE_LIBS = 

#flags
CFLAGS = -Wall 
CXXFLAGS = $(CFLAGS) 

LDFLAGS = 

#objects

OBJS = $(OBJ_DIR)/GfxFramebuffer.o \
    $(OBJ_DIR)/GfxST_Linux.o \
    $(OBJ_DIR)/LibVideoMixer.o \
    $(OBJ_DIR)/GfxVideoWindow.o \
    $(OBJ_DIR)/GfxPresentPrepare.o \
    $(OBJ_DIR)/GfxPresentOvl.o	\
    $(OBJ_DIR)/GfxPresentAlphaBlending.o	\
    $(OBJ_DIR)/GfxPresentMain.o	\
    $(OBJ_DIR)/GfxPresentPrepare.o	\
    $(OBJ_DIR)/CProtect.o	\
    $(OBJ_DIR)/translate.o	\
    $(OBJ_DIR)/GfxPresentProcSW.o	\
    $(OBJ_DIR)/GfxPresentStreamSW.o	


#include external configuration
include $(TOP)/Makefile.rules
