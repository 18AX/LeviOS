CC=gcc
CFLAGS=-Wall -Werror -pedantic -Wextra -std=gnu99 -fno-builtin -fno-stack-protector -ffreestanding 
LFLAGS=-nostdlib -static -T init.ld

TARGET=init.elf

OBJECTS=main.o start.o

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LFLAGS)

clean:
	$(RM) $(OBJECTS) $(TARGET)