#
# Copyright (C) 2015 MediaTek Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#

#
# Makefile for misc devices that really don't fit anywhere else.
#
ifneq ($(CONFIG_PINCTRL_MT6757),y)
# to use mtk legacy pinctrl
subdir-ccflags-y += -I$(DRVGEN_OUT)/inc
endif
MTK_CUSTOM_PATH  := $(srctree)/drivers/misc/mediatek

ifeq ($(CONFIG_ARCH_MTK_PROJECT), "titan6757_c10_n")
ccflags-y += -DGPIO_EXPANDER_SUPPORT
$(info kd_camera_hw: macro GPIO_EXPANDER_SUPPORT defined)
endif

ifeq ($(CONFIG_ARCH_MTK_PROJECT), "titan6757_jade_n")
ccflags-y += -DJADE_PHONE_SUPPORT
$(info kd_camera_hw: macro JADE_PHONE_SUPPORT defined)
endif

ccflags-y += -I$(MTK_CUSTOM_PATH)/imgsensor/inc \

obj-y				+= kd_camera_hw.o
