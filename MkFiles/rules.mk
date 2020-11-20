##############################################################################
# Copyright (C) 2019 Mono Wireless Inc. All Rights Reserved.
# Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE
# AGREEMENT). 
##############################################################################
# COMMON BUILD RULES
##############################################################################

##############################################################################
# VERSION
CFLAGS += -DVERSION_MAIN=$(VERSION_MAIN)
CFLAGS += -DVERSION_SUB=$(VERSION_SUB)
CFLAGS += -DVERSION_VAR=$(VERSION_VAR)

##############################################################################
# OS specific.

# default tool chain
TOOLCHAIN_PATH = ba-elf-ba2-r36379

# Configure for WSL (Windows Subsystem Linux)
ifneq ($(origin WSLENV),undefined)
$(info !!!WSL environment, use Linux toolchain instead.)
TOOLCHAIN_PATH = ba-elf-ba2-r36379.wsl
endif

ifeq ($(OS),Windows_NT)  
ifeq ($(MWX),1)
  TOOLCHAIN_PATH = ba-elf-ba2-r36379.w10
  TOOL_EXE_SUFF=.exe
endif
  CMDPATH=$(realpath $(TOOL_COMMON_BASE_DIR)/MinGW/msys/1.0/bin)
  RM=$(CMDPATH)/rm
  MKDIR=$(CMDPATH)/mkdir
  SLEEP=$(CMDPATH)/sleep
else
  # Configure for non-Windows compiler, compile error with LTO.
  DISABLE_LTO = 1
  RM=rm
  MKDIR=mkdir
  SLEEP=sleep
endif

##############################################################################
# check Tools dir
TOOLCHAIN_FULL_PATH:=$(realpath $(SDK_BASE_DIR)/../../Tools/$(TOOLCHAIN_PATH))
ifeq ($(TOOLCHAIN_FULL_PATH),)
  TOOLCHAIN_FULL_PATH:=$(realpath $(SDK_BASE_DIR)/../../../Tools/$(TOOLCHAIN_PATH))
endif

TOOL_COMMON_BASE_DIR:=$(realpath $(TOOLCHAIN_FULL_PATH)/..)

##############################################################################
# Configure for the selected chip or chip family
include $(SDK_BASE_DIR)/Chip/Common/Build/config.mk
include $(SDK_BASE_DIR)/Platform/Common/Build/config.mk
include $(SDK_BASE_DIR)/Stack/Common/Build/config.mk


##############################################################################
# modify flags
### Change Optimize Level
# -O, -O1 : rather bigger bin file
# -O2     : a bit bigger, better in some case.
# -Os     : default
# -O3     : not sure.
ifneq ($(OPTFLAG),)
$(info "!!! set optflags as $(OPTFLAG).")
CFLAGS  := $(subst -Os,$(OPTFLAG),$(CFLAGS))
LDFLAGS  := $(subst -Os,$(OPTFLAG),$(LDFLAGS))
endif


##############################################################################
# strip space for some values
space:=
space+=
STRIPPED_TARGET_SUFF=$(subst $(space),,$(TARGET_SUFF))
STRIPPED_OBJDIR_SUFF=$(subst $(space),,$(OBJDIR_SUFF))


##############################################################################
# Target
ifeq ($(subst $(space),,$(TARGET_TYPE)),a)
  TARGET0 = lib$(PROJNAME)
  $(warning "NOTICE: building library file.")
else
  TARGET0 = $(PROJNAME)_
endif

ifeq ($(TARGET0),_)
  TARGET0=
endif

ifneq ($(SUBNAME),)
  TARGET = $(TARGET0)$(SUBNAME)
else
  TARGET = $(TARGET0)$(TARGET_DIR)
endif

ifneq ($(APP_TWENET_BASE),)
TARGET_BIN = $(TARGET)_$(_TWELITE)$(STRIPPED_TARGET_SUFF)_$(TWENET_VER)_V$(VERSION_MAIN)-$(VERSION_SUB)-$(VERSION_VAR)
else
TARGET_BIN = $(TARGET)_$(_TWELITE)$(STRIPPED_TARGET_SUFF)_$(VERSION_MAIN)-$(VERSION_SUB)-$(VERSION_VAR)
endif


##############################################################################
# Objects

ifeq ($(DEBUG),HW)
OBJDIR   = objs_$(_TWELITE)$(STRIPPED_OBJDIR_SUFF)_hdbg
else
OBJDIR   = objs_$(_TWELITE)$(STRIPPED_OBJDIR_SUFF)
endif

#APPOBJS_BASE = $(APPSRC:.c=.o)
APPOBJS = $(APPSRC:%.c=$(OBJDIR)/%.o)

#APPOBJS_CXX_BASE = $(APPSRC_CXX:.cpp=.o)
APPOBJS_CXX = $(APPSRC_CXX:%.cpp=$(OBJDIR)/%.o)


##############################################################################
# Application include path
ifneq ($(APP_COMMON_SRC_DIR_ADD1),)
INCFLAGS += -I$(APP_COMMON_SRC_DIR_ADD1)
endif
ifneq ($(APP_COMMON_SRC_DIR_ADD2),)
INCFLAGS += -I$(APP_COMMON_SRC_DIR_ADD2)
endif
ifneq ($(APP_COMMON_SRC_DIR_ADD3),)
INCFLAGS += -I$(APP_COMMON_SRC_DIR_ADD3)
endif
ifneq ($(APP_COMMON_SRC_DIR_ADD4),)
INCFLAGS += -I$(APP_COMMON_SRC_DIR_ADD4)
endif
ifneq ($(AP_MWX_SRC_DIR),)
INCFLAGS += -I$(APP_MWX_SRC_DIR)
endif
 
##############################################################################
# Application dynamic dependencies
# - set 0 when performing clean.
USE_APPDEPS?=1
ifeq ($(USE_APPDEPS),1)
  APPDEPS = $(APPOBJS:.o=.d) $(APPOBJS_CXX:.o=.d)
  DEPFLAGS = -MT $@ -MMD -MP -MF $(OBJDIR)/$*.d
else
endif


#########################################################################
# C++ compiler

ifneq ($(subst $(space),,$(APPSRC_CXX)),)
$(info !!!g++ is used for compiling .cpp and linking)
BUILD_CXX=1
endif

CXX:=$(TOOL_COMMON_BASE_DIR)/$(TOOLCHAIN_PATH)/bin/$(CROSS_COMPILE)-g++$(TOOL_EXE_SUFF)

# C++11, no-exception, no RTTI (no virtual/dynamic_cast<>)
CXXFLAGS += -std=c++11 -fno-exceptions -fno-non-call-exceptions -ffreestanding -fno-rtti


#########################################################################
# Linker
LDLIBS := $(addsuffix _$(JENNIC_CHIP_FAMILY),$(APPLIBS)) $(LDLIBS)

# C/C++ Linker (if .cpp files are in the build target, use g++ for linking)
ifeq ($(BUILD_CXX),1)
LINKERCMD=$(CXX)
else
LINKERCMD=$(CC)
endif

ifeq (0,1)
$(info APP_SRC_DIR $(APP_SRC_DIR))
$(info APP_COMMON_SRC_DIR $(APP_COMMON_SRC_DIR))
$(info APP_STACK_SRC_DIR_ADD1 $(APP_STACK_SRC_DIR_ADD1))
$(info APP_STACK_SRC_DIR_ADD2 $(APP_STACK_SRC_DIR_ADD2))
$(info APP_COMMON_SRC_DIR_ADD1 $(APP_COMMON_SRC_DIR_ADD1))
$(info APP_MWX_SRC_DIR $(APP_MWX_SRC_DIR))
$(info TARGET_BIN $(TARGET_BIN))
$(info APPOBJS $(APPOBJS))
$(info APPOBJS_CXX $(APPOBJS_CXX))
endif

#########################################################################
# Main Section
.PHONY: all clean cleanall

# Path to directories containing application source 
ifneq ($(PROJNAME),)
vpath % $(APP_SRC_DIR):$(APP_COMMON_SRC_DIR):$(ADDITIONAL_SRC_DIR):$(APP_STACK_SRC_DIR_ADD1):$(APP_STACK_SRC_DIR_ADD2):$(APP_COMMON_SRC_DIR_ADD1):$(APP_COMMON_SRC_DIR_ADD2):$(APP_COMMON_SRC_DIR_ADD3):$(APP_COMMON_SRC_DIR_ADD4):$(APP_MWX_SRC_DIR)
else
vpath % $(APP_SRC_DIR):$(ADDITIONAL_SRC_DIR):$(APP_STACK_SRC_DIR_ADD1):$(APP_STACK_SRC_DIR_ADD2):$(APP_COMMON_SRC_DIR_ADD1):$(APP_COMMON_SRC_DIR_ADD2):$(APP_COMMON_SRC_DIR_ADD3):$(APP_COMMON_SRC_DIR_ADD4):$(APP_MWX_SRC_DIR)
endif

# in case `make clean all', disable parallel build.
ifneq ($(filter clean,$(MAKECMDGOALS)),)
.NOTPARALLEL:
endif

# all
all: prebld mainbld postbld

mainbld: generate_target

# objdir should make before compiling (for parallel build.)
$(APPOBJS): | objdir
$(APPOBJS_CXX): | objdir

# include appdep
ifeq ($(USE_APPDEPS),1)
$(APPDEPS):
include $(wildcard $(APPDEPS))
endif

# build rules
$(OBJDIR)/%.o: %.S
	$(info Assembling $< ...)
	$(CC) -c -o $(subst Source,Build,$@) $(DEPFLAGS) $(CFLAGS) $(INCFLAGS) $< 
	@echo ""

ifeq ($(USE_APPDEPS),1)
$(OBJDIR)/%.o: %.c $(OBJDIR)/%.d
else
$(OBJDIR)/%.o: %.c
endif
	$(info Compiling $< ...)
	$(CC) -c -o $(subst Source,Build,$@) $(DEPFLAGS) $(CFLAGS) $(INCFLAGS) $(realpath $<)
	@echo ""

ifeq ($(USE_APPDEPS),1)
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/%.d
else
$(OBJDIR)/%.o: %.cpp
endif
	$(info Compiling $< ...)	
	$(CXX) -c -o $(subst Source,Build,$@) $(DEPFLAGS) $(CXXFLAGS) $(CFLAGS) $(INCFLAGS) $(realpath $<)
	@echo ""

# generate elf file
$(OBJDIR)/$(TARGET_BIN).elf: $(APPOBJS) $(APPOBJS_CXX) $(addsuffix _$(JENNIC_CHIP_FAMILY).a,$(addprefix $(COMPONENTS_BASE_DIR)/Library/lib,$(APPLIBS)))
	$(info Linking $@ ...)
	$(LINKERCMD) -Wl,--gc-sections -Wl,-u_AppColdStart -Wl,-u_AppWarmStart $(LDFLAGS) -T$(LINKCMD) -o $@ \
		 $(APPOBJS) $(APPOBJS_CXX) $(ADDITIONAL_OBJS) $(ADDITIONAL_LIBS) \
		 -Wl,--start-group $(addprefix -l,$(LDLIBS))  $(ADDITIONAL_LIBS) -Wl,--end-group \
		 -Wl,-Map,$(OBJDIR)/$(TARGET)_$(_TWELITE)$(BIN_SUFFIX).map

ifeq ($(TARGET_TYPE),bin)
generate_target: $(OBJDIR)/$(TARGET_BIN).elf |prebld
	${SIZE} $<
	$(info Generating binary ...)
	$(RM) -f $(TARGET_BIN).bin
	$(OBJCOPY) -S -O binary $< $(TARGET_BIN).bin

	$(info !!!TARGET=$(TARGET_BIN).$(TARGET_TYPE))
endif

ifeq ($(TARGET_TYPE),a)
generate_target: $(APPOBJS) $(APPOBJS_CXX) |prebld
	$(RM) -f $(TARGET_BIN).a
	$(AR) $(ARFLAGS) $(TARGET_BIN).$(TARGET_TYPE) $(APPOBJS) $(APPOBJS_CXX) $(ADDITIONAL_OBJS)

	$(info !!!TARGET=$(TARGET_BIN).$(TARGET_TYPE))
endif
	
#########################################################################
# create sub-dirs for obj files.
objdir:
	@$(MKDIR) -p $(OBJDIR)
ifeq ($(MWX),1)
	@$(MKDIR) -p $(OBJDIR)/boards
	@$(MKDIR) -p $(OBJDIR)/networks
	@$(MKDIR) -p $(OBJDIR)/sensors
	@$(MKDIR) -p $(OBJDIR)/sensors/legacy
	@$(MKDIR) -p $(OBJDIR)/settings
endif

# note: clean/cleanall will keep a target `.bin' file.
clean:
	@$(RM) -rfv $(OBJDIR)

cleanall:
	@$(RM) -rfv objs_*/
	@echo "finished removing obj_* directories"

prebld:
#	$(SLEEP) 1

postbld: | mainbld
	@echo !!!TARGET=$(TARGET_BIN).$(TARGET_TYPE)


#########################################################################
