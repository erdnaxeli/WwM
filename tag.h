#ifndef TAG_H
#define TAG_H

#include "window.h"

/* A list of tags :
 *  - state : a splite_state tree, wich contain the splits of the screen and
 *  the mapped windows
 *  - wins : list of *not* mapped windows
 *  - next : pretty obvious, isn't it ? */
struct tlist;

struct tlist {
    struct splite_state state;
    struct wlist wins;
    struct tlist *next;
};

#endif // TAG_H
