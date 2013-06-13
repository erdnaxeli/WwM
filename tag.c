#include <stdlib.h>

#include "tag.h"

static struct tlist *tags = NULL;

/* Intialize the tag list 'tags'. If tags already contains a struct tlist (tags
 * is not null), tag_init DO NOT free it.
 *
 * @return 1 on success, 0 on error */
int tag_init()
{
    tags = malloc(sizeof(struct tlist));

    if (tags == NULL) {
        return 0;
    } else {
        tags->state = malloc(sizeof(struct splite_state));
        tags->state->v = NULL;
        tags->state->right = NULL;
        tags->state->left = NULL;
        tags->wins = NULL;
        tags->next = NULL;
        return 1;
    }
}
