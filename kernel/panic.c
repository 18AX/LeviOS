#include "levi/panic.h"

void die()
{
    asm volatile("cli");

    for (;;)
    {
        asm volatile("hlt");
    }
}