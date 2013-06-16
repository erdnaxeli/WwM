#include <stdlib.h>

#include "global.h"
#include "tag.h"
#include "logger.h"

extern struct conf global;

static struct tlist *tag = NULL;

/* Intialize the tag list 'tags'. If tags already contains a struct tlist (tags
 * is not null), tag_init DO NOT free it.
 *
 * @return 1 on success, 0 on error */
int tag_init()
{
    logger(TRACE, "tag_init");
    tag = malloc(sizeof(struct tlist));

    if (tag == NULL) {
        return 0;
    } else {
        tag->state = malloc(sizeof(struct split_state));
        tag->state->v = NULL;
        tag->state->parent = NULL;
        tag->state->right = NULL;
        tag->state->left = NULL;
        tag->wfocused = NULL;
        tag->wins = NULL;
        tag->next = NULL;
        return 1;
    }
}

/* Add a window to the current tag (first in the list), in the frame which
 * contain the focused window. */
void add_win_to_tag(xcb_window_t *w)
{
    logger(TRACE, "add_win_to_tag");

    if (w == NULL) {
        logger(ERROR, "add_win_to_tag: bad argument");
        return;
    }

    // set the border color and width
    xcb_change_window_attributes(global.c, *w, XCB_CW_BORDER_PIXEL,
            (uint32_t []){ global.screen->white_pixel });
    xcb_configure_window(global.c, *w,
            XCB_CONFIG_WINDOW_BORDER_WIDTH,
            (uint32_t []){ 1 });

    if (tag->wfocused == NULL) {
        logger(TRACE, "add_win_to_tag: this is the first window");
        tag->state->v = w;
        set_win_geometry(global.margin_l, global.margin_t,
                global.screen->width_in_pixels
                    - (2 + global.margin_l + global.margin_r),
                global.screen->height_in_pixels
                    - (2 + global.margin_t + global.margin_b),
                *w);
    } else {
        // xcb is asynchronous, so we take advantage of it
        logger(TRACE, "add_win_to_tag: asking geometry information for %d",
                *tag->wfocused);
        xcb_get_geometry_cookie_t geom_cookie = xcb_get_geometry(global.c, *tag->wfocused);

        struct split_state *node = get_lnode(tag->state, tag->wfocused);
        logger(TRACE, "add_win_to_tag: node get");

        if (node == NULL) {
            logger(CRITICAL, "add_win_to_tag: focused window not found");
            exit(EXIT_FAILURE);
        }

        tag->wins = add_win_to_list(tag->wins, tag->wfocused);
        node->v = w;

        /* We have done all we can, now we must get this value.
         * TODO: take care of the possible error (the NULL pointer at present)*/
        xcb_get_geometry_reply_t *geom = xcb_get_geometry_reply(global.c,
                geom_cookie, NULL);
        set_win_geometry(geom->x, geom->y, geom->width, geom->height, *w);
        free(geom);
    }

    xcb_map_window(global.c, *w);
    tag->wfocused = w;
    logger(TRACE, "add_win_to_tag: window added");
}


// split a frame in a given direction
void split_frame(enum direction d)
{
    logger(TRACE, "split_frame");

    if (tag->wfocused == NULL) {
        logger(ERROR, "split_frame: there is no window");
        return;
    }

    // xcb is asynchronous, so we take advantage of it
    logger(TRACE, "split_frame: asking geometry information for %d", *tag->wfocused);
    xcb_get_geometry_cookie_t geom_cookie = xcb_get_geometry(global.c, *tag->wfocused);

    if (!wlist_lenght(tag->wins) >= 1) {
        logger(WARN, "split_frame: no more windows");
        return;
    }

    struct split_state *node = get_lnode(tag->state, tag->wfocused);

    // setting new children
    node->right = malloc(sizeof(struct split_state));
    node->right->v = node->v;
    node->right->parent = node;

    node->left = malloc(sizeof(struct split_state));
    node->left->v = tag->wins->elt;
    node->left->parent = node;

    // setting new value of the node
    node->d = d;
    xcb_get_geometry_reply_t *geom = xcb_get_geometry_reply(global.c,
            geom_cookie, NULL);

    int *value = malloc(sizeof(int));
    switch (d) {
        case HORIZONTAL:
            *value = geom->x + geom->width / 2;
            node->v = value;
            set_win_geometry(geom->x, geom->y,
                    geom->width / 2, geom->height, *(tag->wfocused));
            set_win_geometry(geom->x + geom->width / 2, geom->y,
                    geom->width / 2, geom->height, *(tag->wins->elt));
            break;
        case VERTICAL:
            *value = geom->y + geom->height / 2;
            node->v = value;
            set_win_geometry(geom->x, geom->y,
                    geom->width, geom->height / 2, *(tag->wfocused));
            set_win_geometry(geom->x, geom->y + geom->height / 2,
                    geom->width, geom->height / 2, *(tag->wins->elt));
            break;
        default:
            logger(CRITICAL, "split_frame: unknown direction (in wich parallel universe are you ?");
    }

    free(geom);

    // we shift the non mapped windows
    tag->wins = tag->wins->next;
}
