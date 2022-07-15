CC=gcc
CFLAGS=-Wall -Werror -pedantic -Wextra -std=gnu99 -fno-builtin -fno-stack-protector -ffreestanding -Iinclude

TARGET=liblevi.a

OBJECTS=crt0.o \
		start.o \
		syscall.o \
		string.o \
		stdio.o \

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	ar rcs $(TARGET) $(OBJECTS)

clean:
	$(RM) $(OBJECTS) $(TARGET)