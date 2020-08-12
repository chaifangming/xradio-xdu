#
# project local config options, override the global config options
#

# ----------------------------------------------------------------------------
# override global config options
# ----------------------------------------------------------------------------
# enable/disable XIP, default to y
export __CONFIG_XIP := y

# enable/disable OTA, default to n
export __CONFIG_OTA := y

export __CONFIG_OS_FREERTOS := n

export __CONFIG_OS_LITEOS := y

export __CONFIG_LWIP_V1 := n
export __CONFIG_LWIP_V2 := y
