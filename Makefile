.PHONY: all clean iso kernel test


all: iso

iso: x86_64
	./makeiso.sh

x86_64:
	$(MAKE) -C kernel x86_64
	$(MAKE) -C liblevi
	$(MAKE) -C init

test: iso
	bochs

clean:
	$(MAKE) -C kernel clean
	$(MAKE) -C liblevi clean
	$(MAKE) -C init clean
	$(RM) levi.iso
	$(RM) -rf isofiles