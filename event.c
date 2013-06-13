#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>
#include <stdlib.h>
#include <stdio.h>

#include "logger.h"

void subscribe_events(xcb_connection_t *c, xcb_drawable_t root)
{
    const static uint32_t events[] = {
        XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
    };
    /*
        XCB_EVENT_MASK_KEY_PRESS            | XCB_EVENT_MASK_KEY_RELEASE    |
        XCB_EVENT_MASK_BUTTON_PRESS         | XCB_EVENT_MASK_BUTTON_RELEASE |
        XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
    };*/

    xcb_change_window_attributes(c, root, XCB_CW_EVENT_MASK, events);
}

void event_handler(xcb_connection_t *c, xcb_screen_t *screen, xcb_drawable_t root)
{
    xcb_generic_event_t *ev;

    while ((ev = xcb_wait_for_event (c))) {
        switch (ev->response_type & ~0x80) {
            case XCB_MAP_REQUEST:
            {
                xcb_map_request_event_t *rev = (xcb_map_request_event_t *)ev;
                logger(INFO, "XCB_MAP_REQUEST : %d\n", rev->window);
                xcb_configure_window(c, rev->window,
                        XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y   |
                        XCB_CONFIG_WINDOW_BORDER_WIDTH,
                        (const uint32_t []){ 0, 300, 1 });
                xcb_change_window_attributes(c, rev->window,
                        XCB_CW_BORDER_PIXEL,
                        (const uint32_t []){ screen->white_pixel });

                xcb_map_window(c, rev->window);
                xcb_flush(c);
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
