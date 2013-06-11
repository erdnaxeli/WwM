#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>
#include <stdlib.h>
#include <stdio.h>

void subscribe_events(xcb_connection_t *c, xcb_drawable_t root)
{
    const static uint32_t events[] = {
        XCB_EVENT_MASK_KEY_PRESS    | XCB_EVENT_MASK_KEY_RELEASE    |
        XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE
    };

    xcb_change_window_attributes(c, root, XCB_CW_EVENT_MASK, events);
}

void event_handler(xcb_connection_t *c, xcb_drawable_t root)
{
    xcb_generic_event_t *e;

    for (;;) {
        e = xcb_wait_for_event (c);

        switch (e->response_type & ~0x80) {
            case XCB_CONFIGURE_NOTIFY:
                printf("XCB_CONFIGURE_NOTIFY\n");
                break;
            case XCB_BUTTON_PRESS:
                //xcb_button_press_event_t *press = (xcb_button_press_event_t *)e;
                printf("XCB_BUTTON_PRESS\n");
                break;
            case XCB_KEY_PRESS:
                printf("XCB_KEY_PRESS\n");
                break;
            default:
                break;
        }

        free (e);
    }
}
