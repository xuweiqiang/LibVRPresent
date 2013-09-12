###############################################################################
## TAL library template Makefile
## Copyright (c) 2007 Cidana, Inc.
###############################################################################

include config

TOP = $(shell pwd)
LIB_TARGET = lib
BIN_TARGET = bin

LIBWIN32 = libWin32.a

#set to the path where libWin32.a and it's headers are placed
LIBWIN32PATH = ../../3rdparty/linux/libWin32/lib/
LIBWIN32_INC = ../../3rdparty/linux/libWin32/inc/

export TOP LIBWIN32 LIBWIN32_INC LIBWIN32PATH

#make libwin32 first
.PHONY: default clean all test directory

default: 
	$(MAKE) -C gfx_linux/
	
clean: 
	$(MAKE) clean -C gfx_linux/

all: clean default
	@echo "Done."
    