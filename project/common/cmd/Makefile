#
# Rules for building library
#

# ----------------------------------------------------------------------------
# common rules
# ----------------------------------------------------------------------------
ROOT_PATH := ../../..

include $(ROOT_PATH)/gcc.mk

INCLUDE_PATHS += -I$(ROOT_PATH)/project
INCLUDE_PATHS += -I$(PRJ_BOARD)
INCLUDE_PATHS += -I$(PRJ_ROOT_PATH)

CC_FLAGS += -include common/prj_conf_opt.h

# ----------------------------------------------------------------------------
# library and objects
# ----------------------------------------------------------------------------
LIBS := libcmd.a

DIRS := .
DIRS += fft
DIRS += tls

SRCS := $(sort $(basename $(foreach dir,$(DIRS),$(wildcard $(dir)/*.[csS]))))

OBJS := $(addsuffix .o,$(SRCS))

# library make rules
include $(LIB_MAKE_RULES)

