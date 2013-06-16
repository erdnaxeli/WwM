#ifndef GLOBAL_H
#define GLOBAL_H

#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <xcb/xcb_atom.h>

struct conf {
    xcb_connection_t *c;
    xcb_screen_t *screen;
    xcb_drawable_t root;
};

#endif // GLOBAL_H
