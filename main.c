/* WwM - Windows with Mustaches, because mustaches are cools
 *
 * by Alexandre Morignot <erdnaxeli@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "global.h"
#include "event.h"
#include "logger.h"
#include "tools.h"
#include "tag.h"

// global configuration
struct conf global;

int main ()
{
    set_logger_level(DEBUG);
    global.c = xcb_connect(NULL, NULL);

    if (global.c == NULL) {
        logger(CRITICAL, "Cannot open display\n");
        exit(EXIT_FAILURE);
    }
    
    global.screen = xcb_setup_roots_iterator(xcb_get_setup (global.c)).data;

    /* This return an error if an other window manager is running (don't as me
     * why). */
    xcb_change_window_attributes(global.c, global.screen->root, XCB_CW_EVENT_MASK,
            (const uint32_t []){ XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT });
    xcb_aux_sync(global.c);

    /* If there is any event, it must be an error (this is too soon for other
     * event). */
    if (xcb_poll_for_event(global.c) != NULL) {
        logger(ERROR, "Another window manager is running");
        /* TODO: uncomment this before merge it to stable
         * exit(EXIT_FAILURE); */
    }

    xcb_window_t w = xcb_generate_id (global.c);
    //xcb_create_window (c, screen->root_depth, w, screen->root, 10, 10, 250, 150, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, 0, NULL);
    //xcb_map_window (c, w);

    xcb_flush(global.c);
    subscribe_events();

    if (!tag_init()) {
        logger(CRITICAL, "Could not intialize the tags list");
        exit(EXIT_FAILURE);
    }

    xcb_flush(global.c);
    exec_cmd("urxvt");
    event_handler();

    xcb_disconnect(global.c);
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
