CC=gcc
CFLAGS=-Wall -Werror -pedantic -Wextra -std=gnu99 -fno-builtin -fno-stack-protector -ffreestanding -I../liblevi/include
LFLAGS=-nostdlib -static -T ../liblevi/linker.ld

TARGET=init.elf

OBJECTS=main.o

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) ../liblevi/liblevi.a -o $(TARGET) $(LFLAGS)

clean:
	$(RM) $(OBJECTS) $(TARGET)