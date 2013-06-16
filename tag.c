#include <stdlib.h>

#include "tag.h"
#include "logger.h"

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

    if (tag->wfocused == NULL) {
        logger(INFO, "add_win_to_tag: this is the first window");
        tag->state->v = w;
    } else {
        struct split_state *node = get_lnode(tag->state, tag->wfocused);
        logger(DEBUG, "add_win_to_tag: node get");

        if (node != NULL) {
            add_win_to_list(tag->wins, (xcb_window_t *)node->v);
        } else {
            logger(CRITICAL, "add_win_to_tag: focused window not found");
            exit(EXIT_FAILURE);
        }

        logger(INFO, "add_win_to_tag");
        node->v = w;
    }

    tag->wfocused = w;
}
