#include <levi/fs/file.h>
#include <levi/interrupts/interrupts.h>
#include <levi/panic.h>
#include <levi/utils/kerr.h>
#include <levi/utils/kprintf.h>

void kerr(u32 code, const char *fmt, ...)
{
    (void)fmt;
    interrupts_disable();

    kprintf("^r!!!!!!!! ^wPANIC 0x%x ^r!!!!!!!!\n", code);
    va_list args;
    va_start(args, fmt);
    __kdprintf(STDOUT_FILENO, fmt, args);
    va_end(args);

    die();
}