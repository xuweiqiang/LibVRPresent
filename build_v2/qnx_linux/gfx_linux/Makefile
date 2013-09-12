###############################################################################
## TAL library template Makefile
## Copyright (c) 2007 Cidana, Inc.
###############################################################################

include $(TOP)/config

CFLAGS = -Wall 

#directories for the project
OUTPUT = LibVRPresent.dll
OUTPUT_PATH = ../../../lib

SRC_DIRS = ../../../src/LibGfxContext ../../../src/LibGfxPresent ../../../src/LibVideoWindow ../../../src/LibVideoMixer  ../../../src_v2/LibGfxPresent ../../../inc
INC_DIRS := ../../../inc/  ../../../3rdparty/linux   ../../../src_v2/LibGfxPresent
OBJ_DIR = ./obj

ifeq ($(BUILD_PLATFORM),QNX)
INC_DIRS +=  ../../../3rdparty/linux/libwin32/qnx_include
INC_DIRS +=  ../../../3rdparty/QNX/
INC_DIRS +=  ../../../3rdparty/imglib/qnx
EXTRA_DEPENDENCIES += ../../../3rdparty/imglib/qnx/ci_imgproc_neon_qnx.a
EXTRA_DEPENDENCIES += ../../../3rdparty/QNX/libscreen.so
EXTRA_DEPENDENCIES += ../../../3rdparty/linux/libwin32/qnx_lib/libwin32.a
CFLAGS += -DGFX_QNX
endif

ifeq ($(BUILD_PLATFORM),)
INC_DIRS +=  ../../../3rdparty/imglib
INC_DIRS +=  ../../../3rdparty/linux/libwin32/inc

LIBRARIES = SDL X11
EXTRA_DEPENDENCIES = ../../../3rdparty/linux/libwin32/lib/libwin32.a
endif

ifeq ($(BUILD_PLATFORM),ST)
LIBRARIES = pthread dl
LIBRARIES += Win32_st
endif

ifeq ($(BUILD_PLATFORM),PXA_LINUX)
LIBRARIES = pthread dl
LIBRARIES += Win32_PXA
EXTRA_DEPENDENCIES = ../../../3rdparty/linux/PXA/CSCLib.a
endif


#output format
#should be one of APPLICATION, STATIC_LIB, DYNAMIC_LIB
#BUILD_FORMAT=APPLICATION
BUILD_FORMAT=DYNAMIC_LIB
#merge the OUTPUT and the following static librarys into one static library
MERGE_LIBS = 

#flags
CXXFLAGS = $(CFLAGS) 

LDFLAGS = 

#objects
OBJS = $(OBJ_DIR)/GfxQNX.o \
    $(OBJ_DIR)/LibVideoMixer.o \
    $(OBJ_DIR)/GfxVideoWindow.o \
    $(OBJ_DIR)/GfxPresentPrepare.o \
    $(OBJ_DIR)/GfxPresentOvl.o	\
    $(OBJ_DIR)/GfxPresentAlphaBlending.o	\
    $(OBJ_DIR)/GfxPresentMain.o	\
    $(OBJ_DIR)/GfxPresentPrepare.o	\
    $(OBJ_DIR)/GfxPresentProcSW.o	\
    $(OBJ_DIR)/GfxPresentStreamSW.o	

#include external configuration
include $(TOP)/Makefile.rules
