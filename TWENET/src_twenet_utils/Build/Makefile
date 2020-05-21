#############################################################################
#
# ToCoNet Library
#
#############################################################################
#
#############################################################################

# Version
include ../Version.mk

# Application target name
PROJNAME=lib
TARGET_DIR=TWENETutils
TARGET_NAME=TWENETutils

CFLAGS += -DToCoNet_LIB_BUILD

ifeq ($(TOCONET_DEBUG),1)
CFLAGS += -DTOCONET_DEBUG
TOCONET_DEBUG_SUFF = _DBG
endif

##############################################################################
# Default DK2 development kit target hardware
#JENNIC_PCB ?= DEVKIT2

##############################################################################
# Select the network stack (e.g. MAC, ZBPRO)

JENNIC_STACK ?= MAC
# Specify device type (e.g. CR (Coordinator/router), ED (End Device))
DEVICE_TYPE ?= ED

##############################################################################
# Debug options define DEBUG for HW debug
#DEBUG ?=HW
#
#
# Define which UART to use for debug
#DEBUG_PORT ?= UART1

##############################################################################
# Define TRACE to use with DBG module
#TRACE ?=1

##############################################################################
# Path definitions

# Choose SDK by pre-defined parameter JENNIC_PCB.
include $(abspath ../../../../MkFiles/chipsel.mk)

# Select definitions for either single or multiple targets

# Use if application directory contains multiple targets
APP_BASE           	= $(abspath ../..)
APP_BLD_DIR			= $(APP_BASE)/$(TARGET_DIR)/Build
APP_SRC_DIR 	    = ../Source

APP_COMMON_SRC_DIR = $(APP_BASE)/$(TARGET_DIR)/AES

# TOCOS:additonal directories
APP_COMMON_SRC_DIR_ADD1 = $(APP_SRC_DIR)/AES
APP_COMMON_SRC_DIR_ADD2 = $(APP_SRC_DIR)/fixmath
APP_COMMON_SRC_DIR_ADD3 =
APP_COMMON_SRC_DIR_ADD4 = 

##############################################################################
# Application Source files

# Note: Path to source file is found using vpath below, so only .c filename is required
APPSRC += serial.c
APPSRC += uart.c
APPSRC += ByteQueue.c
APPSRC += ccitt8.c
APPSRC += fprintf.c
APPSRC += sprintf.c
APPSRC += randMT.c
APPSRC += regex.c

APPSRC += utils.c
APPSRC += btnMgr.c
APPSRC += serialInputMgr.c

APPSRC += aes_modes.c
APPSRC += aescrypt.c
APPSRC += aeskey.c
APPSRC += aestab.c
APPSRC += ccm.c

APPSRC += fix16.c
APPSRC += fix16_exp.c
APPSRC += fix16_sqrt.c
APPSRC += fix16_trig.c
APPSRC += fract32.c
APPSRC += uint32.c

APPSRC += Version.c

#APPSRC += $(TARGET_DIR).c

##############################################################################
# Additional Application Source directories
# Define any additional application directories outside the application directory
# e.g. for AppQueueApi

ADDITIONAL_SRC_DIR += $(COMPONENTS_BASE_DIR)/Utilities/Source
ADDITIONAL_SRC_DIR += $(APP_COMMON_SRC_DIR)

##############################################################################
# Standard Application header search paths

INCFLAGS += -I$(APP_SRC_DIR)
INCFLAGS += -I$(APP_SRC_DIR)/..
INCFLAGS += -I$(APP_COMMON_SRC_DIR)

INCFLAGS += -I$(APP_COMMON_SRC_DIR_ADD1)
INCFLAGS += -I$(APP_COMMON_SRC_DIR_ADD2)
INCFLAGS += -I$(APP_COMMON_SRC_DIR_ADD3)
INCFLAGS += -I$(APP_COMMON_SRC_DIR_ADD4)

##############################################################################
# Application libraries
# Specify additional Component libraries

#ADDITIONAL_OBJS += $(APP_SRC_DIR)/XXX.o
INCFLAGS += -I$(COMPONENTS_BASE_DIR)
INCFLAGS += -I$(COMPONENTS_BASE_DIR)/Utilities/Include
INCFLAGS += -I$(COMPONENTS_BASE_DIR)/MicroSpecific/Include

##############################################################################

# You should not need to edit below this line

##############################################################################
# Configure for the selected chip or chip family

include $(SDK_BASE_DIR)/Chip/Common/Build/config.mk
include $(SDK_BASE_DIR)/Platform/Common/Build/Config.mk
include $(SDK_BASE_DIR)/Stack/Common/Build/Config.mk

##############################################################################
# C flags
CFLAGS += -DFIXMATH_NO_CACHE # too much memory allocation for exp, sin, ...
#CFLAGS += -DFIXMATH_NO_OVERFLOW
CFLAGS += -Wno-cast-align -Wno-sequence-point

##############################################################################

TARGET = $(PROJNAME)$(TARGET_NAME)_$(TWELITE)

#OBJDIR   = objs_$(JENNIC_CHIP)$(TOCONET_DEBUG_SUFF)
OBJDIR   = objs_$(TWELITE)

APPOBJS_BASE = $(APPSRC:.c=.o)
APPOBJS = $(APPSRC:%.c=$(OBJDIR)/%.o)

##############################################################################
# Application dynamic dependencies

#APPDEPS = $(APPOBJS_BASE:.o=.d)
APPDEPS = $(APPOBJS_BASE:%.o=$(OBJDIR)/%.d)

#########################################################################
# Linker

# Add application libraries before chip specific libraries to linker so
# symbols are resolved correctly (i.e. ordering is significant for GCC)

LDLIBS := $(addsuffix _$(JENNIC_CHIP_FAMILY),$(APPLIBS)) $(LDLIBS)

#########################################################################
# Dependency rules

.PHONY: all clean
# Path to directories containing application source 
vpath % $(APP_SRC_DIR):$(APP_COMMON_SRC_DIR):$(ADDITIONAL_SRC_DIR):$(APP_COMMON_SRC_DIR_ADD1):$(APP_COMMON_SRC_DIR_ADD2):$(APP_COMMON_SRC_DIR_ADD3):$(APP_COMMON_SRC_DIR_ADD4)

# TARGET_BIN = $(TARGET)_$(JENNIC_CHIP)_$(VERSION_MAIN)$(VERSION_SUB)_$(VERSION_VAR)
TARGET_BIN = $(TARGET)$(TOCONET_DEBUG_SUFF)

all: dir $(TARGET_BIN).a ../Source/Version.h

dir:
	-mkdir -p $(OBJDIR)
	
-include $(APPDEPS)
%.d:
	rm -f $*.o

$(OBJDIR)/%.o: %.S
	$(info Assembling $< ...)
	$(CC) -c -o $(subst Source,Build,$@) $(CFLAGS) $(INCFLAGS) $< -MD -MF $(OBJDIR)/$*.d -MP
	@echo

$(OBJDIR)/%.o: %.c 
	$(info Compiling $< ...)
	$(CC) -c -o $(subst Source,Build,$@) $(CFLAGS) $(INCFLAGS) $< -MD -MF $(OBJDIR)/$*.d -MP
	@echo

$(TARGET_BIN).a: $(APPOBJS)
	rm -f $(TARGET).a
	$(AR) $(ARFLAGS) $@ $(APPOBJS) $(ADDITIONAL_OBJS)

../Source/Version.h: ../Version.mk
	@echo '#define VERSION_MAIN $(VERSION_MAIN)' > ../Source/Version.h
	@echo "#define VERSION_SUB $(VERSION_SUB)" >> ../Source/Version.h
	@echo "#define VERSION_VAR $(VERSION_VAR)" >> ../Source/Version.h	
	
#########################################################################

clean:
	rm -f $(APPOBJS) $(APPDEPS) $(TARGET_BIN).a

#########################################################################
