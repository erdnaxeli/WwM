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

    xcb_change_window_attributes(global.c, global.screen->root,
            XCB_CW_EVENT_MASK, events);
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

        xcb_flush(global.c);
        free (ev);
    }
}
