CC=gcc
CFLAGS=-I../include -Wall -Werror -Wextra -std=gnu99 -ffreestanding -fno-stack-protector -fno-pic -fpie -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-red-zone
LFLAGS=-nostdlib -T linker.ld -Wl,-static,-pie,--no-dynamic-linker,-ztext -static-pie -nostdlib -z max-page-size=0x1000

BUILDDIR=build

TARGET=$(BUILDDIR)/levi

OBJ= kmain.c.o \
	panic.c.o \
	boot/stivale2.c.o \
	arch/x86_64/arch.c.o

OBJECTS= $(addprefix $(BUILDDIR)/, $(OBJ))

.PHONY: all iso clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LFLAGS)

$(BUILDDIR)/%.c.o: %.c
	$(HIDE) mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.S.o: %.S
	$(HIDE) mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(TARGET)