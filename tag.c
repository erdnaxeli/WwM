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
    logger(DEBUG, "tag_init: entering");
    tag = malloc(sizeof(struct tlist));

    if (tag == NULL) {
        return 0;
    } else {
        tag->state = malloc(sizeof(struct split_state));
        tag->state->v = NULL;
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
    logger(DEBUG, "add_win_to_tag: entering");

    // set the border color and width
    xcb_change_window_attributes(global.c, *w, XCB_CW_BORDER_PIXEL,
            (uint32_t []){ global.screen->white_pixel });
    xcb_configure_window(global.c, *w,
            XCB_CONFIG_WINDOW_BORDER_WIDTH,
            (uint32_t []){ 1 });

    if (tag->wfocused == NULL) {
        logger(INFO, "add_win_to_tag: this is the first window");
        tag->state->v = w;
        set_win_geometry(0, 0, global.screen->width_in_pixels - 2,
                global.screen->height_in_pixels - 2, *w);
    } else {
        // xcb is asynchronous, so we take advantage of it
        xcb_get_geometry_cookie_t geom_cookie = xcb_get_geometry(global.c, *w);

        struct split_state *node = get_lnode(tag->state, tag->wfocused);
        logger(DEBUG, "add_win_to_tag: node get");

        if (node != NULL) {
            add_win_to_list(tag->wins, (xcb_window_t *)node->v);
        } else {
            logger(CRITICAL, "add_win_to_tag: focused window not found");
            exit(EXIT_FAILURE);
        }

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
    logger(DEBUG, "add_win_to_tag: window added");
}
