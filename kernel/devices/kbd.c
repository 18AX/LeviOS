#include "levi/devices/kbd.h"

#include "levi/devices/kbd/intel8042.h"

static u64 key_state_mask = 0x0;

void kbd_init()
{
#if x86_64
    intel8042_init();
#endif
}

void on_key_press(enum keycode key)
{
    key_state_mask |= (1 << key);
}

void on_key_release(enum keycode key)
{
    key_state_mask &= ~(1 << key);
}

u32 key_state(enum keycode key)
{
    return ((key_state_mask & (1 << key)) != 0) ? KEY_PRESS : KEY_RELEASE;
}

u64 get_state()
{
    return key_state_mask;
}
