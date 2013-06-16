#ifndef TAG_H
#define TAG_H

#include "window.h"

/* A list of tags :
 *  - state : a split_state tree, wich contain the splits of the screen and
 *  the mapped windows
 *  - wfocused : focused window
 *  - wins : list of *not* mapped windows
 *  - next : pretty obvious, isn't it ? */
struct tlist;

struct tlist {
    struct split_state *state;
    xcb_window_t *wfocused;
    struct wlist *wins;
    struct tlist *next;
};

int tag_init();
void add_win_to_tag(xcb_window_t *w);
void split_frame(enum direction d);

#endif // TAG_H
