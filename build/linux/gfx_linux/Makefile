###############################################################################
## TAL library template Makefile
## Copyright (c) 2007 Cidana, Inc.
###############################################################################

include $(TOP)/config

#directories for the project
OUTPUT = LibVRPresent.dll
OUTPUT_PATH = ../../../lib

SRC_DIRS = ../../../src/LibGfxContext ../../../src/LibGfxPresent ../../../src/LibVideoWindow ../../../src/LibVideoMixer
INC_DIRS := ../../../inc/  ../../../3rdparty/linux ../../../3rdparty/linux/libwin32/inc
OBJ_DIR = ./obj

#defines

#library path and dependent libraries
LIB_DIRS = ../../../3rdparty/linux/libwin32/lib

#libraries will be linked with "-l" option (not the prefix lib should be omitted)
ifeq ($(BUILD_PLATFORM),)
LIBRARIES = SDL X11
else
LIBRARIES = pthread dl

	ifeq ($(BUILD_PLATFORM),ST)
	LIBRARIES += Win32_st
	endif

	ifeq ($(BUILD_PLATFORM),PXA_LINUX)
	LIBRARIES += Win32_PXA
	endif
	
endif

#libraries with full name 

ifeq ($(BUILD_PLATFORM),ST)
EXTRA_DEPENDENCIES = ../../../3rdparty/linux/ST/CSCLib.a
else
	ifeq ($(BUILD_PLATFORM),PXA_LINUX)
	EXTRA_DEPENDENCIES = ../../../3rdparty/linux/PXA/CSCLib.a
	else
	EXTRA_DEPENDENCIES = ../../../3rdparty/linux/x86/CSCLib.a
	endif
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
OBJS = $(OBJ_DIR)/GfxSDL.o \
    $(OBJ_DIR)/GfxFramebuffer.o \
    $(OBJ_DIR)/AlphaBlending.o \
    $(OBJ_DIR)/LibVideoMixer.o \
    $(OBJ_DIR)/GfxVideoWindow.o \
    $(OBJ_DIR)/GfxPrepare.o \
    $(OBJ_DIR)/GfxPresentOvl.o

#include external configuration
include $(TOP)/Makefile.rules
