AUDIO_ROOT=$(srctree)/techpack/audio

KBUILD_OPTIONS+=  AUDIO_ROOT=$(AUDIO_ROOT)

all: modules

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(M) clean

%:
	$(MAKE) -C $(KERNEL_SRC) M=$(M) $@ $(KBUILD_OPTIONS)

