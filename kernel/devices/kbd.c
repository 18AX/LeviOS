#include "levi/devices/kbd.h"

static u64 key_state_mask = 0x0;

void on_key_up(enum keycode key)
{
    key_state_mask &= (1 << key);
}

void on_key_down(enum keycode key)
{
    key_state_mask &= ~(1 << key);
}

u32 key_state(enum keycode key)
{
    return (key_state_mask & key) != 0;
}
