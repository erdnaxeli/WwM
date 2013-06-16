#include <stdlib.h>

#include "global.h"
#include "window.h"
#include "logger.h"

extern struct conf global;

// @return the leaf node containing w in tree
struct split_state *get_lnode(struct split_state *tree, xcb_window_t *w)
{
    struct split_state *tmp;
    logger(TRACE, "get_lnode: searching for %d", *w);

    if (tree->right == NULL) { // tree->left == NULL too
        logger(TRACE, "get_lnode: end of the tree");

        if (tree->v == NULL) {
            logger(TRACE, "get_lnode: window not found");
            return tree;
        }
        else {
            xcb_window_t *win = (xcb_window_t *)tree->v;
            logger(TRACE, "get_lnode: testing %d", *win);

            if (*win == *w) {
                logger(TRACE, "get_lnode: window found");
                return tree;
            } else {
                logger(TRACE, "get_lnode: window not found");
                return NULL;
            }
        }
    } else {
        logger(TRACE, "get_lnode: searching in childs");
        if ((tmp = get_lnode(tree->right, w)) != NULL ||
                    (tmp = get_lnode(tree->left, w)) != NULL) {
            logger(TRACE, "get_lnode: window found");
            return tmp;
        } else {
            logger(TRACE, "get_lnode: window not found");
            return NULL;
        }
    }
}

/* Add a window to the end of the given list.
 *
 * @return a pointer to this list */
struct wlist *add_win_to_list(struct wlist *list, xcb_window_t *w)
{
    logger(TRACE, "add_win_to_list");
    struct wlist *save = list;

    if (list == NULL) {
        save = malloc(sizeof(struct wlist));
        save->elt = w;
        save->next = NULL;
    }
    else {
        while (list->next != NULL)
            list = list->next;

        list->next = malloc(sizeof(struct wlist));
        list->next->elt = w;
        list->next->next = NULL;
    }

    return save;
}

void set_win_geometry(int16_t x, int16_t y, uint16_t width, uint16_t height,
        xcb_drawable_t w)
{
    logger(DEBUG, "set_win_geometry: position = (%d, %d), size = (%d, %d), window = %d", x, y, width, height, w);
    xcb_configure_window(global.c, w,
            XCB_CONFIG_WINDOW_X     | XCB_CONFIG_WINDOW_Y       |
            XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
            (uint32_t []){ x, y, width, height });
}

int wlist_lenght(struct wlist *list)
{
    logger(TRACE, "wlist_lenght");
    int i = 0;

    while (list != NULL) {
        list = list->next;
        i++;
    }

    logger(TRACE, "wlist_lenght: %d", i);
    return i;
}
