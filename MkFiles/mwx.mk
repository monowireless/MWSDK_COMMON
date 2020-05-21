##########################################################################
# Copyright (C) 2019 Mono Wireless Inc. All Rights Reserved.
# Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE
# AGREEMENT). 
##########################################################################

##########################################################################
# USE MWX library
MWX=1

##########################################################################
# SET PROJNAME and TARGET_DIR
# assume Wks_MWX/TARGET_DIR/build dir structure.
PROJNAME=
DIRUP_LST = $(subst /, ,$(abspath ..))
TARGET_DIR = $(word $(words $(DIRUP_LST)), $(DIRUP_LST))

##########################################################################
# LOAD chipsel.mk
include $(MWSDK_PATH)/MkFiles/chipsel.mk

##########################################################################
# add default source
#APPSRC_CXX += $(TARGET_DIR).cpp
APPSRC_CXX += $(subst ../,,$(wildcard ../*.cpp))
_APPSRC_CXX = $(sort $(APPSRC_CXX))
APPSRC_CXX := $(_APPSRC_CXX)
APPSRC += $(subst ../,,$(wildcard ../*.c))
_APPSRC = $(sort $(APPSRC))
APPSRC := $(_APPSRC)

##########################################################################
# add default source
TARGET_TYPE ?= bin
TOCONET_DEBUG ?= 0

##########################################################################
# LOAD others
include $(MWSDK_PATH)/MkFiles/twenet.mk
include $(MWSDK_PATH)/MkFiles/rules.mk
#########################################################################
