#include <stdlib.h>

#include "global.h"
#include "logger.h"
#include "tag.h"

extern struct conf global;

void subscribe_events()
{
    const static uint32_t events[] = {
        XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
    };
    /*
        XCB_EVENT_MASK_KEY_PRESS            | XCB_EVENT_MASK_KEY_RELEASE    |
        XCB_EVENT_MASK_BUTTON_PRESS         | XCB_EVENT_MASK_BUTTON_RELEASE |
        XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
    };*/

    xcb_change_window_attributes(global.c, global.screen->root, XCB_CW_EVENT_MASK, events);
}

void event_handler()
{
    xcb_generic_event_t *ev;

    while ((ev = xcb_wait_for_event (global.c))) {
        switch (ev->response_type & ~0x80) {
            case XCB_MAP_REQUEST:
            {
                xcb_map_request_event_t *rev = (xcb_map_request_event_t *)ev;
                logger(INFO, "XCB_MAP_REQUEST : %d", rev->window);
                xcb_configure_window(global.c, rev->window,
                        XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y   |
                        XCB_CONFIG_WINDOW_BORDER_WIDTH,
                        (const uint32_t []){ 0, 300, 1 });
                xcb_change_window_attributes(global.c, rev->window,
                        XCB_CW_BORDER_PIXEL,
                        (const uint32_t []){ global.screen->white_pixel });

                xcb_map_window(global.c, rev->window);
                xcb_flush(global.c);

                xcb_window_t *w = malloc(sizeof(xcb_window_t));
                *w = rev->window;
                add_win_to_tag(w);
                break;
            }
            case XCB_CREATE_NOTIFY:
                logger(INFO, "XCB_CREATE_NOTIFY\n");
                break;
            case XCB_CONFIGURE_NOTIFY:
                logger(INFO, "XCB_CONFIGURE_REQUEST\n");
                break;
            case XCB_BUTTON_PRESS:
                //xcb_button_press_event_t *press = (xcb_button_press_event_t *)e;
                logger(INFO, "XCB_BUTTON_PRESS\n");
                break;
            case XCB_KEY_PRESS:
                logger(INFO, "XCB_KEY_PRESS\n");
                break;
            default:
                break;
        }

        free (ev);
    }
}
