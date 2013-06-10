#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>

void subscribe_events(xcb_connection_t *c, xcb_drawable_t root)
{
    const static uint32_t value[] = {XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY};
    xcb_configure_window(c, root, XCB_CW_EVENT_MASK, value);
}

void event_handler(xcb_connection_t *c, xcb_screen_t *s, xcb_drawable_t root)
{
}
