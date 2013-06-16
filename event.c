#include <stdlib.h>

#include "global.h"
#include "logger.h"
#include "tag.h"

extern struct conf global;

void subscribe_events()
{
    logger(TRACE, "suscribe_events");
    const static uint32_t events[] = {
        XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT    |
        XCB_EVENT_MASK_KEY_PRESS                | XCB_EVENT_MASK_KEY_RELEASE
    };
    /*
        XCB_EVENT_MASK_BUTTON_PRESS         | XCB_EVENT_MASK_BUTTON_RELEASE |
        XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
    };*/

    xcb_change_window_attributes(global.c, global.screen->root,
            XCB_CW_EVENT_MASK, events);
}

void subscribe_events_win(xcb_window_t w)
{
    logger(TRACE, "suscribe_events_win: %d", w);
    const static uint32_t events[] = {
        XCB_EVENT_MASK_KEY_PRESS                | XCB_EVENT_MASK_KEY_RELEASE
    };

    xcb_change_window_attributes(global.c, w, XCB_CW_EVENT_MASK, events);
}

void event_loop()
{
    logger(TRACE, "event_loop");
    xcb_generic_event_t *ev;

    while ((ev = xcb_wait_for_event (global.c))) {
        switch (ev->response_type & ~0x80) {
            case XCB_MAP_REQUEST:
            {
                xcb_map_request_event_t *rev = (xcb_map_request_event_t *)ev;
                logger(DEBUG, "XCB_MAP_REQUEST : %d", rev->window);
                
                xcb_window_t *w = malloc(sizeof(xcb_window_t));
                *w = rev->window;
                add_win_to_tag(w);
                subscribe_events_win(*w);
                break;
            }
            case XCB_CREATE_NOTIFY:
                logger(DEBUG, "XCB_CREATE_NOTIFY");
                break;
            case XCB_CONFIGURE_NOTIFY:
                logger(DEBUG, "XCB_CONFIGURE_REQUEST");
                break;
            case XCB_BUTTON_PRESS:
                //xcb_button_press_event_t *press = (xcb_button_press_event_t *)e;
                logger(DEBUG, "XCB_BUTTON_PRESS");
                break;
            case XCB_KEY_PRESS:
            {
                logger(DEBUG, "XCB_KEY_PRESS: %d");
                break;
            }
            default:
                break;
        }

        xcb_flush(global.c);
        free (ev);
    }
}
