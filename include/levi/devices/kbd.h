#ifndef KBD_HEADER
#define KBD_HEADER

#define KEY_UP 1
#define KEY_DOWN 0

#include "levi/types.h"

enum keycode
{
    A = 0,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    ARROW_LEFT,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN
};

void on_key_up(enum keycode key);

void on_key_down(enum keycode key);

u32 key_state(enum keycode key);

#endif