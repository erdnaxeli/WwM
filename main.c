/* WwM - Windows with Mustaches, because mustaches are cools
 *
 * by Alexandre Morignot <erdnaxeli@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>

#include "event.h"

int main ()
{
    xcb_connection_t *c = xcb_connect(NULL, NULL);

    if (c == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
    
    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup (c)).data;
    xcb_drawable_t root = screen->root;

    /* This return an error if an other window manager is running (don't as me
     * why). */
    xcb_change_window_attributes(c, root, XCB_CW_EVENT_MASK,
            (const uint32_t []){ XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT });
    xcb_aux_sync(c);

    /* If there is any event, it must be an error (this is too soon for other
     * event). */
    if (xcb_poll_for_event(c) != NULL) {
        printf("Another window manager is running");
        exit(EXIT_FAILURE);
    }

    xcb_window_t w = xcb_generate_id (c);
    //xcb_create_window (c, screen->root_depth, w, screen->root, 10, 10, 250, 150, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, 0, NULL);
    //xcb_map_window (c, w);

    xcb_flush (c);
    subscribe_events(c, root);
    xcb_flush (c);
    event_handler(c, w);
    /*
    int *shit = &screen->white_pixel;
    xcb_void_cookie_t cookie = xcb_change_window_attributes (c,
                                                    root,
                                                    XCB_CW_BACK_PIXEL,
                                                    shit);

    xcb_gcontext_t foreground = xcb_generate_id (c);
    uint32_t mask = XCB_GC_BACKGROUND;
    uint32_t values[1] = {screen->white_pixel};
    //xcb_create_gc (c, foreground, root, mask, values);

    //mask = XCB_CW_BACK_PIXEL;
    //values[0] = screen->white_pixel;

    xcb_window_t w = xcb_generate_id (c);
    //xcb_create_window (c, screen->root_depth, w, screen->root, 10, 10, 250, 150, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, 0, NULL);

    mask = XCB_GC_BACKGROUND;
    values[0] = screen->black_pixel;
    //xcb_create_gc (c, foreground, w, mask, values);

    xcb_map_window (c, root);
    */

    return 0;
}
