##############################################################################
# Copyright (C) 2019 Mono Wireless Inc. All Rights Reserved.
# Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE
# AGREEMENT). 
##############################################################################
# MAC CONFIGURATIONS
##############################################################################
$(error not work now. need adjustment)
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
APP_BASE           	= $(abspath ../..)
APP_BLD_DIR			= $(APP_BASE)/$(TARGET_DIR)/Build
APP_SRC_DIR 	    = $(APP_BASE)/$(TARGET_DIR)/Source

APP_COMMON_SRC_DIR = $(APP_BASE)/Common/Source

# TOCOS:additonal directories
APP_STACK_SRC_DIR_ADD1 = 
APP_STACK_SRC_DIR_ADD2 = 

##############################################################################
# Additional Application Library

#ADDITIONAL_LIBS += 

##############################################################################
# Additional Application Source directories
# Define any additional application directories outside the application directory
# e.g. for AppQueueApi

ADDITIONAL_SRC_DIR = $(APP_COMMON_SRC_DIR)

##############################################################################
# Standard Application header search paths

INCFLAGS += -I$(APP_SRC_DIR)
INCFLAGS += -I$(APP_SRC_DIR)/..
INCFLAGS += -I$(APP_COMMON_SRC_DIR)

#INCFLAGS += -I$(APP_STACK_SRC_DIR_ADD1)
#INCFLAGS += -I$(APP_STACK_SRC_DIR_ADD2)

INCFLAGS += -I$(COMPONENTS_BASE_DIR)
INCFLAGS += -I$(COMPONENTS_BASE_DIR)/Utilities/Include

##############################################################################
