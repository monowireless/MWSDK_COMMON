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
# Debug options define DEBUG for HW debug
#DEBUG ?=HW

# Define which UART to use for debug
#DEBUG_PORT ?= UART1

##############################################################################
# Path definitions
# Select definitions for either single or multiple targets

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
ifeq ($(TWENET_HAS_MWX),1)
$(info !!!use MWX C++ template library)
include $(APP_TWENET_BASE)/src/mwx/mwx_sources.mk
APP_MWX_SRC_DIR = $(APP_TWENET_BASE)/src/mwx
LDFLAGS += -L$(APP_MWX_SRC_DIR)
else
$(error !!!TWENET library does not have MWX.)
endif
endif


##############################################################################
# Additional Application Library

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

##############################################################################
# Additional Application Source directories
# Define any additional application directories outside the application directory
# e.g. for AppQueueApi

ADDITIONAL_SRC_DIR = $(APP_COMMON_SRC_DIR)

##############################################################################
# Standard Application header search paths

INCFLAGS += -I$(APP_SRC_DIR)

ifneq ($(PROJNAME),)
INCFLAGS += -I$(APP_COMMON_SRC_DIR)
INCFLAGS += -I$(APP_SRC_DIR)/..
endif

INCFLAGS += -I$(APP_STACK_SRC_DIR_ADD1)
INCFLAGS += -I$(APP_STACK_SRC_DIR_ADD2)
INCFLAGS += -I$(APP_STACK_SRC_DIR_ADD3)
INCFLAGS += -I$(APP_MWX_SRC_DIR)

INCFLAGS += -I$(COMPONENTS_BASE_DIR)
INCFLAGS += -I$(COMPONENTS_BASE_DIR)/MicroSpecific/Include
INCFLAGS += -I$(COMPONENTS_BASE_DIR)/Utilities/Include

##############################################################################
