##############################################################################
# Copyright (C) 2019 Mono Wireless Inc. All Rights Reserved.
# Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE
# AGREEMENT). 
##############################################################################
# CHIP DEPENDENT CONFIGURATIONS
##############################################################################
MWSDK_PATH ?= $(realpath $(MWSDK_ROOT))

ifneq ($(words $(MWSDK_PATH)),1)
$(error *** MWSDK_PATH must not have SPACE! "$(MWSDK_PATH)" )
endif

_CUR_DIR=$(realpath .)
ifneq ($(words $(_CUR_DIR)),1)
$(error The current dir must not have SPACE! "$(_CUR_DIR)")
endif

#mkfile_path := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
#$(info $(mkfile_path))
#current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

######################################################################################
# Set SDK_BASE_INFORMATION
######################################################################################
_TWELITE=$(TWELITE)

### TWE-Lite (RED, JN5169)
ifeq ($(_TWELITE),RED)
$(info !!!building for TWELITE RED)
JENNIC_CHIP		= JN5169

### TWE-Lite (BLUE, JN5164)
else ifeq ($(_TWELITE),BLUE)
JENNIC_CHIP		= JN5164
$(info !!!building for TWELITE BLUE)
else

### NOT DEFINED 
$(error ERROR: No TWE_CHIP_MODEL definition.)
endif

#########################################################################
# CHIPLIB SELECTION
#########################################################################

# DEFAULT CHIP SUPPORT LIBRARYR
include $(MWSDK_PATH)/ChipLib/usever.mk
include $(MWSDK_PATH)/TWENET/usever.mk

# set chip library version as CFLAGS
_CHIP_LIB1 = $(subst SW, -DMWCHIPLIBCODE=,$(CHIP_LIB))
_CHIP_LIB2 = $(subst V, -DMWCHIPLIBVER=,$(_CHIP_LIB1))
CFLAGS += $(_CHIP_LIB2)

# set BASE DIR FOR SDK
#SDK_BASE_DIR   	 	= $(abspath $(MWSDK_PATH)/ChipLib/$(CHIP_LIB))
SDK_BASE_DIR   	 	= $(MWSDK_PATH)/ChipLib/$(CHIP_LIB)

#########################################################################
# OTHERS
#########################################################################

ifeq ($(origin TARGET_DIR), undefined)
DIRUP_LST = $(subst /, ,$(abspath ..))
TARGET_DIR = $(word $(words $(DIRUP_LST)), $(DIRUP_LST))
endif

# if defined and PROJNAME is blank, assume $(TARGET_DIR) only.
ifeq ($(origin PROJNAME),undefined)
DIRUPUP_LST = $(subst /, ,$(abspath ../..))
PROJNAME = $(word $(words $(DIRUPUP_LST)), $(DIRUPUP_LST))
endif
