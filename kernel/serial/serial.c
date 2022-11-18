#include <levi/arch/x86_64/io.h>
#include <levi/serial/serial.h>

#define DLL(x) (x) /** Divisor Latch Low Byte **/
#define DLH(x) (x + 1) /** Divisor Latch High Byte **/
#define FCR(x) (x + 2) /** FIFO Control Register **/
#define LCR(x) (x + 3) /** Line Control Register **/
#define LSR(x) (x + 5) /** Line Status Register **/

#define DLAB_SET (1 << 7)
#define FCR_ENABLE_FIFO (0x1)
#define FCR_CLR_RECEIVE (0x1 << 1)
#define FCR_CLR_TRANSMIT (0x1 << 2)
#define FCR_TRIGGER_14 (0xC0)
#define LCR_8_BITS 0x3

#define DLAB_115200_HIGH 0x0
#define DLAB_115200_LOW 0x1

#define LSR_EMPTY_TRANSMITTER_HOLDING (0x1 << 5)
#define LSR_DATA_RDY (0x1)

void serial_init_port(u16 port)
{
    outb(LCR(port), DLAB_SET); // Set DLAB to 1
    outb(DLL(port), DLAB_115200_HIGH); // Divisor low byte for 115200 bps
    outb(DLH(port), DLAB_115200_LOW); // Divisor high byte for 115200 bps
    outb(LCR(port), LCR_8_BITS); // 00000011: No parity and 8 bits
    outb(FCR(port),
         FCR_TRIGGER_14 | FCR_CLR_RECEIVE | FCR_CLR_RECEIVE
             | FCR_ENABLE_FIFO); // 11000111: interrupt trigger level 14 bytes,
                                 // Clear transmit fifo (bit 2), Clear Receive
                                 // fifo (bit 1), enable fifo (bit 0)
}

s64 serial_write(u16 port, void *data, u64 size)
{
    u8 *buffer = (u8 *)data;
    u64 w = 0;

    for (; w < size; ++w)
    {
        // Waiting for the serial to be availabe for writing
        while ((inb(LSR(port)) & LSR_EMPTY_TRANSMITTER_HOLDING) == 0)
        {
            asm volatile("pause");
        }

        // Write the data to the serial port
        outb(port, buffer[w]);
    }

    return w;
}

s64 serial_read(u16 port, void *data, u64 size)
{
    u8 *buffer = (u8 *)data;

    u64 w = 0;

    for (; w < size; ++w)
    {
        // Checking if there are data to read

        while ((inb(LSR(COM1)) & LSR_DATA_RDY) == 0)
        {
            asm volatile("pause");
        }

        buffer[w] = inb(port);
    }

    return w;
}
