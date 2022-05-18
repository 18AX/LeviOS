#ifndef KBD_HEADER
#define KBD_HEADER

#define KEY_PRESS 1
#define KEY_RELEASE 0

#include "levi/types.h"

enum keycode
{
    KEYCODE_A = 0,
    KEYCODE_B,
    KEYCODE_C,
    KEYCODE_D,
    KEYCODE_E,
    KEYCODE_F,
    KEYCODE_G,
    KEYCODE_H,
    KEYCODE_I,
    KEYCODE_J,
    KEYCODE_K,
    KEYCODE_L,
    KEYCODE_M,
    KEYCODE_N,
    KEYCODE_O,
    KEYCODE_P,
    KEYCODE_Q,
    KEYCODE_R,
    KEYCODE_S,
    KEYCODE_T,
    KEYCODE_U,
    KEYCODE_V,
    KEYCODE_W,
    KEYCODE_X,
    KEYCODE_Y,
    KEYCODE_Z,
    KEYCODE_ARROW_LEFT,
    KEYCODE_ARROW_RIGHT,
    KEYCODE_ARROW_UP,
    KEYCODE_ARROW_DOWN,
    KEYCODE_UNKNOWN,
};

void kbd_init(void);

void on_key_press(enum keycode key);

void on_key_release(enum keycode key);

u32 key_state(enum keycode key);

u64 get_kbd_state();

#endif