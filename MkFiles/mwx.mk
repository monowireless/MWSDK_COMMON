##########################################################################
# Copyright (C) 2019 Mono Wireless Inc. All Rights Reserved.
# Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE
# AGREEMENT). 
##########################################################################

##########################################################################
# USE MWX library
MWX=1
EASTL?=1

# MWX_RECOMP_LIB   1:always recompile, 0:use precompiled lib
MWX_RECOMP_LIB?=0

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

# if NO_SRC_INCL_FROM_TOPDIR is set 0, skip including source files on the
# top directory.
NO_SRC_INCL_FROM_TOPDIR ?= 0

# add source files on the top directory.
ifneq ($(NO_SRC_INCL_FROM_TOPDIR),1)
APPSRC_CXX += $(subst ../,,$(wildcard ../*.cpp))
_APPSRC_CXX = $(sort $(APPSRC_CXX))
APPSRC_CXX := $(_APPSRC_CXX)
APPSRC += $(subst ../,,$(wildcard ../*.c))
_APPSRC = $(sort $(APPSRC))
APPSRC := $(_APPSRC)
endif

##########################################################################
# add default source
TARGET_TYPE ?= bin
TOCONET_DEBUG ?= 0

##########################################################################
# LOAD others
include $(MWSDK_PATH)/MkFiles/twenet.mk
include $(MWSDK_PATH)/MkFiles/rules.mk
#########################################################################
