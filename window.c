#include <stdlib.h>

#include "window.h"
#include "logger.h"

// @return the leaf node containing w in tree
struct splite_state *get_lnode(struct splite_state *tree, xcb_window_t *w)
{
    struct splite_state *tmp;
    logger(DEBUG, "get_lnode: searching for %d", *w);

    if (tree->right == NULL) { // tree->left == NULL too
        logger(DEBUG, "get_lnode: end of the tree");

        if (tree->v == NULL) {
            logger(DEBUG, "get_lnode: window not found");
            return tree;
        }
        else {
            xcb_window_t *win = (xcb_window_t *)tree->v;
            logger(DEBUG, "get_lnode: testing %d", *win);

            if (*win == *w) {
                logger(DEBUG, "get_lnode: window found");
                return tree;
            } else {
                logger(DEBUG, "get_lnode: window not found");
                return NULL;
            }
        }
    } else {
        logger(DEBUG, "get_lnode: searching in childs");
        if ((tmp = get_lnode(tree->right, w)) != NULL ||
                    (tmp = get_lnode(tree->left, w)) != NULL) {
            logger(DEBUG, "get_lnode: window found");
            return tmp;
        } else {
            logger(DEBUG, "get_lnode: window not found");
            return NULL;
        }
    }
}

/* Add a window to the end of the given list.
 *
 * @return a pointer to this list */
struct wlist *add_win_to_list(struct wlist *list, xcb_window_t *w)
{
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
