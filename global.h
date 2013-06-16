#ifndef GLOBAL_H
#define GLOBAL_H

#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <xcb/xcb_atom.h>
#include <xcb/xcb_keysyms.h>

struct conf {
    xcb_connection_t *c; // connection
    xcb_screen_t *screen; // screen
    xcb_drawable_t root; // root window
    xcb_key_symbols_t *keysyms; // key symbol table

    int16_t margin_t; // top
    int16_t margin_r; // right
    int16_t margin_b; // bottom
    int16_t margin_l; // left
};

#endif // GLOBAL_H
