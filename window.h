#ifndef WINDOW_H
#define WINDOW_H

#include <xcb/xcb.h>

enum direction {
    HORIZONTAL,
    VERTICAL
};

/* This struct represent the split state of the desktop.
 * It is basically a kdtree.
 *
 * The value v is either a int in an inner node (split's x or y) or a
 * xcb_window_t list in a leaf node. d is the direction (horizontal or verticale
 * of the split, and has of course no sens in a leaf node.
 * We know if we are in an inner node if one of the children (right or left) is
 * not null. */
struct splite_sate;

struct splite_state {
    void *v;
    enum direction d;
    struct splite_sate *right;
    struct splite_sate *left;
};

// A list of xcb_window_t
struct wlist;

struct wlist {
    xcb_window_t elt;
    struct wlist *next;
};

#endif // WINDOW_H
