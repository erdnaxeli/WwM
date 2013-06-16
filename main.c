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
    set_logger_level(TRACE);

    logger(INFO, "Connecting to X");
    global.c = xcb_connect(NULL, NULL);

    if (global.c == NULL) {
        logger(CRITICAL, "Cannot open display\n");
        exit(EXIT_FAILURE);
    }
    
    global.screen = xcb_setup_roots_iterator(xcb_get_setup (global.c)).data;

    /* This return an error if an other window manager is running (don't as me
     * why). */
    xcb_change_window_attributes(global.c, global.screen->root, XCB_CW_EVENT_MASK,
            (uint32_t []){ XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT });
    xcb_aux_sync(global.c);

    /* If there is any event, it must be an error (this is too soon for other
     * event). */
    if (xcb_poll_for_event(global.c) != NULL) {
        logger(CRITICAL, "Another window manager is running");
        /* TODO: uncomment this before merge it to stable
         * exit(EXIT_FAILURE); */
    }

    xcb_flush(global.c);
    subscribe_events();

    if (!tag_init()) {
        logger(CRITICAL, "Could not intialize the tags list");
        exit(EXIT_FAILURE);
    }

    xcb_flush(global.c);

    logger(INFO, "Reading configuration");
    // TODO: read a configuration and set it
    global.margin_t = 15;
    global.margin_r = 0;
    global.margin_b = 0;
    global.margin_l = 0;

    exec_cmd("urxvt");

    logger(INFO, "Starting event loop");
    event_loop();

    logger(INFO, "Disconnecting from X");
    xcb_disconnect(global.c);

    logger(INFO, "Bye");
    return 0;
}
