.PHONY: all clean iso kernel test

all: iso

iso: kernel
	./makeiso.sh

kernel:
	$(MAKE) -C kernel

clean:
	$(MAKE) -C kernel clean