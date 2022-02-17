##############################################################################
# Copyright (C) 2019 Mono Wireless Inc. All Rights Reserved.
# Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE
# AGREEMENT). 
##############################################################################
# USER PROJECT BUILD DEFINITION.
##############################################################################


#####################################################################
### set TARGET type (a: library)
TARGET_TYPE=a

#####################################################################
### set TWELITE model
TWELITE ?= BLUE
#TWELITE = RED

#####################################################################
### set application version (MUST SET THIS.)
VERSION_MAIN = 3
VERSION_SUB  = 07
VERSION_VAR  = 00

#####################################################################
### set an additional source file
###   the default file name is dirname.

### for C++ files compiled with g++ (must have .cpp suffix)
APPSRC_CXX += allocator_eastl.cpp
#APPSRC_CXX += assert.cpp
APPSRC_CXX += fixed_pool.cpp
APPSRC_CXX += hashtable.cpp
APPSRC_CXX += intrusive_list.cpp
APPSRC_CXX += numeric_limits.cpp
APPSRC_CXX += red_black_tree.cpp
APPSRC_CXX += string.cpp
#APPSRC_CXX += thread_support.cpp
APPSRC_CXX += EAMemory.cpp
APPSRC_CXX += EASprintf.cpp

### for C files compiled with gcc (must have .c suffix)
#APPSRC += my_c_file.c

### Additional Src/Include Path
# if set, find source files from given dirs.
#
APP_COMMON_SRC_DIR_ADD1 = ../source
APP_COMMON_SRC_DIR_ADD2 = ../test/packages/EAStdC/source
#APP_COMMON_SRC_DIR_ADD3 = 
#APP_COMMON_SRC_DIR_ADD4 =

#####################################################################
### set misc option for compiler

### C++ flags passed to g++
# e.g. CXXFLAGS += -DMY_DEFS
#CXXFLAGS +=

### C++/C flags passed to g++/gcc
# e.g. CFLAGS += -DMY_DEFS
#CFLAGS +=

### include opts
# e.g. INCFLAGS += -I../my_common_src/
PATH_EASTL = ..
INCFLAGS += -I$(PATH_EASTL)/include
INCFLAGS += -I$(PATH_EASTL)/test/packages/EAAssert/include
INCFLAGS += -I$(PATH_EASTL)/test/packages/EABase/include/Common
INCFLAGS += -I$(PATH_EASTL)/test/packages/EAMain/include
INCFLAGS += -I$(PATH_EASTL)/test/packages/EAStdC/include
INCFLAGS += -I$(PATH_EASTL)/test/packages/EATest/include
INCFLAGS += -I$(PATH_EASTL)/test/packages/EAThread/include

### optimize flag (default is -Os, normally no need to change)
#OPTFLAG=-O2

#TWENET_DEBUG=1
#DISABLE_LTO=1

#####################################################################
### must include mwx.mk (the makefile body part.)
MWSDK_PATH?=$(realpath $(MWSDK_ROOT))
include $(MWSDK_PATH)/MkFiles/mwx.mk
#####################################################################

LIBDIR = ../../../lib

deploy:
	-cp -fv  $(TARGET_BIN).a $(LIBDIR)/$(TARGET)_$(_TWELITE)$(STRIPPED_TARGET_SUFF).a
	-touch $(LIBDIR)/$(TARGET)_$(_TWELITE)$(STRIPPED_TARGET_SUFF).$(VERSION_MAIN)-$(VERSION_SUB)-$(VERSION_VAR)
