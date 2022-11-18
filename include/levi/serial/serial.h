#ifndef SERIAL_HEADER
#define SERIAL_HEADER

/**
 * Serial ports are mainly used for debugging. The logging system use serial.
 */

#include <levi/types.h>

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

/**
 * @brief Initilaze a serial port
 *
 * @param port port to be initialized, possible values are COM1, COM2, COM3,
 * COM4
 */
void serial_init_port(u16 port);

s64 serial_write(u16 port, void *data, u64 len);
s64 serial_read(u16 port, void *data, u64 len);

#endif