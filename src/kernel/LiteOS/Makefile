#
# Rules for building library
#

# ----------------------------------------------------------------------------
# common rules
# ----------------------------------------------------------------------------
ROOT_PATH := ../../..

include $(ROOT_PATH)/gcc.mk

# ----------------------------------------------------------------------------
# library and objects
# ----------------------------------------------------------------------------
LIBS := libliteos.a

DIRS := arch/arm/arm-m/src
DIRS += arch/arm/arm-m/armv7-m
DIRS += arch/arm/arm-m/armv7-m/gcc
DIRS += kernel
DIRS += kernel/base/core
DIRS += kernel/base/ipc
DIRS += kernel/base/mem/common
DIRS += kernel/base/mem/bestfit_little
DIRS += kernel/base/mem/membox
DIRS += kernel/base/misc
DIRS += kernel/base/om
DIRS += utils/src

SRCS := $(sort $(basename $(foreach dir,$(DIRS),$(wildcard $(dir)/*.[csS]))))

OBJS := $(addsuffix .o,$(SRCS))

# library make rules
include $(LIB_MAKE_RULES)
