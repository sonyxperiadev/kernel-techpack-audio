AUDIO_ROOT := $(srctree)/techpack/audio

ifeq ($(CONFIG_ARCH_WAIPIO), y)
include $(AUDIO_ROOT)/config/waipioauto.conf
LINUXINCLUDE += -include $(AUDIO_ROOT)/config/waipioautoconf.h
endif

ifeq ($(CONFIG_ARCH_KALAMA), y)
include $(AUDIO_ROOT)/config/kalamaauto.conf
LINUXINCLUDE += -include $(AUDIO_ROOT)/config/kalamaautoconf.h
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
