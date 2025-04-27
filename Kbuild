ifeq ($(filter y, $(CONFIG_ARCH_LAHAINA) $(CONFIG_ARCH_BLAIR)),)
AUDIO_ROOT := $(srctree)/techpack/audio

ifeq ($(CONFIG_ARCH_PARROT), y)
include $(AUDIO_ROOT)/config/waipioauto.conf
LINUXINCLUDE += -include $(AUDIO_ROOT)/config/waipioautoconf.h
endif

LINUXINCLUDE += \
		-I$(AUDIO_ROOT)/include/uapi \
		-I$(AUDIO_ROOT)/include/uapi/audio \
		-I$(AUDIO_ROOT)/include/asoc \
		-I$(AUDIO_ROOT)/include
USERINCLUDE += -I$(AUDIO_ROOT)/include/uapi/audio

obj-y += soc/
obj-y += dsp/
obj-y += ipc/
obj-y += asoc/
else
obj-y += legacy/
endif
