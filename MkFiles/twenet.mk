##############################################################################
# Copyright (C) 2019 Mono Wireless Inc. All Rights Reserved.
# Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE
# AGREEMENT). 
##############################################################################
# TWENET CONFIGURATIONS
##############################################################################

##############################################################################
# Select the network stack (e.g. MAC, ZBPRO)
JENNIC_STACK ?= MAC
# Specify device type (e.g. CR (Coordinator/router), ED (End Device))
DEVICE_TYPE ?= ED

##############################################################################
# Path definitions
# Select definitions for either single or multiple targets
##############################################################################

# Use if application directory contains multiple targets
ifneq ($(PROJNAME),)
APP_BASE           	?= $(abspath ../..)
APP_SRC_DIR 	    = $(APP_BASE)/$(TARGET_DIR)/Source
APP_COMMON_SRC_DIR	= $(APP_BASE)/Common/Source
else
APP_BASE           	?= $(abspath ..)
APP_SRC_DIR 	    = $(APP_BASE)
endif

##############################################################################
# Application Source files
##############################################################################

# set APP_TWENET_BASE
ifeq ($(TWENET_DIR),DEV)
#$(error dummy error)
APP_TWENET_BASE    = $(MWSDK_PATH)/Wks_TWENET/libTWENET
TWENET_VER = DEV
else
APP_TWENET_BASE    = $(MWSDK_PATH)/TWENET/$(TWENET_DIR)
include $(APP_TWENET_BASE)/Version.mk
endif

# TWENET Additional dirs
APP_STACK_SRC_DIR_ADD1 = $(APP_TWENET_BASE)/include/ToCoNet
APP_STACK_SRC_DIR_ADD2 = $(APP_TWENET_BASE)/include/ToCoNetUtils 
ifeq ($(TWENET_HAS_STGS),1)
APP_STACK_SRC_DIR_ADD3 = $(APP_TWENET_BASE)/include/TWENETstgs 
endif

# MWX C++ library
ifeq ($(MWX),1)
$(info !!!use MWX C++ template library)  
PATH_MWX=$(APP_TWENET_BASE)/src/mwx

  # add source files for compile.
  ifeq ($(TARGET_TYPE),bin)
    ifeq ($(TWENET_HAS_MWX),1)
      ifeq ($(MWX_RECOMP_LIB),1) # if recompile everything
      APP_MWX_SRC_DIR = $(PATH_MWX)
      include $(APP_MWX_SRC_DIR)/mwx_sources.mk
      endif #MWX_RECOMP_LIB
    #LDFLAGS += -L$(APP_MWX_SRC_DIR) # maynot be necessary
    else #TWENET_HAS_MWX
    $(error !!!TWENET library does not have MWX.)
    endif #TWENET_HAS_MWX
  endif #
endif #MWX=1

# EASTL library
ifeq ($(EASTL),1)
PATH_EASTL=$(APP_TWENET_BASE)/src/EASTL
endif

##############################################################################
# Additional Application Library
##############################################################################
ifeq ($(TOCONET_DEBUG),1)
TWENET_DEBUG=1
endif

ifeq ($(TWENET_DEBUG),1)
ADDITIONAL_LIBS += $(APP_TWENET_BASE)/lib/libTWENET_$(_TWELITE)_DBG.a
ADDITIONAL_LIBS += $(APP_TWENET_BASE)/lib/libTWENEText_$(_TWELITE)_DBG.a
ADDITIONAL_LIBS += $(APP_TWENET_BASE)/lib/libTWENETutils_$(_TWELITE).a
  ifeq ($(TWENET_HAS_STGS),1)
  ADDITIONAL_LIBS += $(APP_TWENET_BASE)/lib/libTWENETstgs_$(_TWELITE).a
  endif
TARGET_SUFF += _TDBG
else
ADDITIONAL_LIBS += $(APP_TWENET_BASE)/lib/libTWENET_$(_TWELITE).a
ADDITIONAL_LIBS += $(APP_TWENET_BASE)/lib/libTWENEText_$(_TWELITE).a
ADDITIONAL_LIBS += $(APP_TWENET_BASE)/lib/libTWENETutils_$(_TWELITE).a
  ifeq ($(TWENET_HAS_STGS),1)
  ADDITIONAL_LIBS += $(APP_TWENET_BASE)/lib/libTWENETstgs_$(_TWELITE).a
  endif
endif

ifeq ($(MWX_RECOMP_LIB),0)
  ADDITIONAL_LIBS += $(APP_TWENET_BASE)/lib/libmwx_$(_TWELITE).a
endif

ifeq ($(EASTL),1)
  ADDITIONAL_LIBS += $(APP_TWENET_BASE)/lib/libEASTL_$(_TWELITE).a
endif

##############################################################################
# Additional Application Source directories
# Define any additional application directories outside the application directory
# e.g. for AppQueueApi
##############################################################################
ADDITIONAL_SRC_DIR = $(APP_COMMON_SRC_DIR)

##############################################################################
# Standard Application header search paths
##############################################################################
INCFLAGS += -I$(APP_SRC_DIR)

ifneq ($(PROJNAME),)
INCFLAGS += -I$(APP_COMMON_SRC_DIR)
INCFLAGS += -I$(APP_SRC_DIR)/..
endif

# stack source lib
INCFLAGS += -I$(APP_STACK_SRC_DIR_ADD1)
INCFLAGS += -I$(APP_STACK_SRC_DIR_ADD2)
INCFLAGS += -I$(APP_STACK_SRC_DIR_ADD3)

# chiplib
INCFLAGS += -I$(COMPONENTS_BASE_DIR)
INCFLAGS += -I$(COMPONENTS_BASE_DIR)/MicroSpecific/Include
INCFLAGS += -I$(COMPONENTS_BASE_DIR)/Utilities/Include

# mwx
ifeq ($(MWX),1)
INCFLAGS += -I$(PATH_MWX)
endif

# eastl
ifeq ($(EASTL),1)
INCFLAGS += -I$(PATH_EASTL)/include
INCFLAGS += -I$(PATH_EASTL)/test/packages/EAAssert/include
INCFLAGS += -I$(PATH_EASTL)/test/packages/EABase/include/Common
INCFLAGS += -I$(PATH_EASTL)/test/packages/EAMain/include
INCFLAGS += -I$(PATH_EASTL)/test/packages/EAStdC/include
INCFLAGS += -I$(PATH_EASTL)/test/packages/EATest/include
INCFLAGS += -I$(PATH_EASTL)/test/packages/EAThread/include
endif

##############################################################################
