#ifndef EVENT_H
#define EVENT_H

void subscribe_events(xcb_connection_t *c, xcb_drawable_t root);
void event_handler(xcb_connection_t *c, xcb_drawable_t root);

#endif //EVENT_H
