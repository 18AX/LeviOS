CC=gcc
CFLAGS=-I../include -Wall -Werror -Wextra -std=gnu99 -ffreestanding -fno-stack-protector -fno-pic -fpie -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-red-zone
LFLAGS=-nostdlib -T linker.ld -Wl,-static,-pie,--no-dynamic-linker,-ztext -static-pie -nostdlib -z max-page-size=0x1000

BUILDDIR=build

TARGET=$(BUILDDIR)/levi

.PHONY: all iso clean x86_64

KERNEL_OBJ= kmain.c.o \
	panic.c.o \
	boot/stivale2.c.o \
	utils/string.c.o \
	utils/stdio.c.o \
	memory/memory.c.o \
	memory/page_alloc.c.o \
	memory/kalloc.c.o \
	interrupts/interrupts.c.o \
	proc/process.c.o \
	fs/fs.c.o \
	fs/file.c.o \
	utils/stringmap.c.o \
	drivers/drivers.c.o \
	drivers/serial/serial.c.o \
	devices/kbd.c.o \

# x86_64

x86_64_OBJ= $(KERNEL_OBJ) \
	arch/x86_64/vmm.c.o \
	arch/x86_64/arch.c.o \
	arch/x86_64/idt.c.o \
	arch/x86_64/isr_handler.S.o \
	arch/x86_64/pic.c.o \

x86_64: CFLAGS += -Dx86_64
x86_64: $(addprefix $(BUILDDIR)/, $(x86_64_OBJ))
	$(CC) $^ -o $(TARGET) $(LFLAGS)


$(BUILDDIR)/%.c.o: %.c
	$(HIDE) mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.S.o: %.S
	$(HIDE) mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(BUILDDIR)