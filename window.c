#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wayland-client.h>
#include "window_data/headers/shm_manager.h"
#include "window_data/headers/xdg-shell.h"
#include "window_data/raw_images/neo_futuristic-back.h"

#define WL_NOOP(name) static void name() {}

WL_NOOP(blank) // blank func for unused events in listeners

struct our_state {
    struct wl_compositor *compositor;
    struct wl_shm *shm;
    struct wl_seat *seat;
    struct wl_pointer *pointer;
    struct xdg_wm_base *xdg_wm_base;
    uint32_t configure_event_serial;
};



static void registry_handle_global(void *data,
        struct wl_registry *wl_registry, 
        uint32_t name, 
        const char *interface, 
        uint32_t version)
{
    
    struct our_state *state = data;
                                    
    if(strcmp(interface, wl_compositor_interface.name) == 0) {
        state->compositor = wl_registry_bind(
            wl_registry, name, &wl_compositor_interface, 6); 
    }

    if(strcmp(interface, wl_shm_interface.name) == 0) {
        state->shm = wl_registry_bind(wl_registry, name, &wl_shm_interface, 2);    

    }

    if(strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xdg_wm_base = wl_registry_bind(wl_registry, name, &xdg_wm_base_interface, 7);
    }

    if(strcmp(interface, wl_seat_interface.name) == 0) {
        state->seat = wl_registry_bind(wl_registry, name, &wl_seat_interface, 9);
    }
}


static void registry_handle_global_remove(void *data, struct wl_registry *registry,
		uint32_t name)
{
	//blank
}

static const struct wl_registry_listener registry_listener = {
    .global = registry_handle_global,
    .global_remove = registry_handle_global_remove,
};



static void xdg_surface_configure_handler(void *data, 
		      struct xdg_surface *xdg_surface,
		      uint32_t serial) {

    struct our_state *state = data;
    state->configure_event_serial = serial; 

}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure_handler
};


static void xdg_wm_base_ping_to_pong (void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping_to_pong
};






static void wl_seat_capabilities (void *data, struct wl_seat *seat, uint32_t capabilities) {

    struct our_state *state = data;
    printf("capability: %d\n", capabilities);

    if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
        if (!state->pointer) {
            state->pointer = wl_seat_get_pointer(seat);
        }
        else if (state->pointer) { // if mouse disconnected from system
            wl_seat_release(seat);
            state->pointer = NULL;
        }
    }
    
}



static const struct wl_seat_listener seat_listener = {
    .capabilities = wl_seat_capabilities,
    .name = (void *)blank,
};

static void pointer_enter(void *data,
                          struct wl_pointer *wl_pointer,
                          uint32_t serial,
                          struct wl_surface *surface,
                          wl_fixed_t surface_x,
                          wl_fixed_t surface_y) {
   
    printf("Enter event, serial: %u, surface address: %p, surface_x: %f, surface_y: %f\n", 
           serial, (void *)surface, wl_fixed_to_double(surface_x), wl_fixed_to_double(surface_y));
}

static void pointer_leave(void *data,
                          struct wl_pointer *wl_pointer,
                          uint32_t serial,
                          struct wl_surface *surface) {
    printf("Leave event, serial: %u, surface address: %p\n", 
           serial, (void *)surface);
}

static void pointer_motion(void *data,
                           struct wl_pointer *wl_pointer,
                           uint32_t time,
                           wl_fixed_t surface_x,
                           wl_fixed_t surface_y) {
    printf("Motion event, time: %u, surface_x: %f, surface_y: %f\n", 
           time, wl_fixed_to_double(surface_x), wl_fixed_to_double(surface_y));
}

static void pointer_button(void *data,
                           struct wl_pointer *wl_pointer,
                           uint32_t serial,
                           uint32_t time,
                           uint32_t button,
                           uint32_t state) {
    printf("Button event, serial: %u, time: %u, button: %u, state: %u\n", 
           serial, time, button, state);
}

static void pointer_axis(void *data,
                         struct wl_pointer *wl_pointer,
                         uint32_t time,
                         uint32_t axis,
                         wl_fixed_t value) {
    printf("Axis event, time: %u, axis: %u, value: %f\n", 
           time, axis, wl_fixed_to_double(value));
}

static void pointer_frame(void *data,
                          struct wl_pointer *wl_pointer) {
    printf("Frame event\n");
}

static void pointer_axis_source(void *data,
                                struct wl_pointer *wl_pointer,
                                uint32_t axis_source) {
    printf("Axis source event, axis_source: %u\n", 
           axis_source);
}

static const struct wl_pointer_listener pointer_listener = {
   .enter = pointer_enter,
   .leave = pointer_leave,
   .motion = pointer_motion,
   .leave = pointer_leave,
   .button = pointer_button,
   .axis = pointer_axis,
   .frame = pointer_frame,
   .axis_source = pointer_axis_source,
};

static struct wl_buffer * draw_frame(struct our_state *state) { 
    const int width = 1920, height = 1080;
    const int stride = width * 4; // 4 is bytes for one pixel in WL_SHM_FORMAT_XRGB8888
    const int shm_pool_size = height * stride * 2; // *2 for double-buffering

    int fd = allocate_shm_file(shm_pool_size);
    uint8_t *pool_data = mmap(NULL, shm_pool_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    struct wl_shm_pool *pool = wl_shm_create_pool(state->shm, fd, shm_pool_size);

    int index = 0;
    int offset = height * stride * index;
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, offset, width, height, stride, WL_SHM_FORMAT_XRGB8888);

    uint32_t *pixels = (uint32_t *)&pool_data[offset];

    uint8_t *image_pixels = wl_lab_data_raw_images_neo_futuristic_back_raw; // var name from imported image.c

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // enumerating index in 1D massive (4 bytes for pixel)
            int image_idx = (y * width + x) * 4;
            
            // separate channels
            uint8_t r = image_pixels[image_idx + 0];
            uint8_t g = image_pixels[image_idx + 1];
            uint8_t b = image_pixels[image_idx + 2];
            uint8_t a = image_pixels[image_idx + 3];

            // packing channels into 32 byte format
            pixels[y * width + x] = (a << 24) | (r << 16) | (g << 8) | b;
        }
    }

    munmap(pool_data, shm_pool_size);
    return buffer;
     


}


int
main(int argc, char *argv[])
{
    struct our_state state = { 0 }; // { 0 } - set all fields to NULL
	struct wl_display *display = wl_display_connect(NULL);
    if (display) {
    printf("connected!\n");

    } else {
        printf("error to connect\n");
        return 1;
    }

	struct wl_registry *registry = wl_display_get_registry(display);

    wl_registry_add_listener(registry, &registry_listener, &state);
    wl_display_roundtrip(display);
    wl_seat_add_listener(state.seat, &seat_listener, &state);
    xdg_wm_base_add_listener(state.xdg_wm_base, &xdg_wm_base_listener, NULL);


    struct wl_surface *surface = wl_compositor_create_surface(state.compositor);
    struct xdg_surface *xdg_surface = xdg_wm_base_get_xdg_surface(state.xdg_wm_base, surface);
    struct xdg_toplevel *xdg_toplevel = xdg_surface_get_toplevel(xdg_surface);


    xdg_surface_add_listener(xdg_surface, &xdg_surface_listener, &state);
    wl_surface_commit(surface); // first commit: -configure me
    wl_display_roundtrip(display);

    wl_pointer_add_listener(state.pointer, &pointer_listener, NULL);
    wl_display_roundtrip(display); 


    printf("serial: %d", state.configure_event_serial);

    xdg_surface_ack_configure(xdg_surface, state.configure_event_serial); // accept configuration.

    struct wl_buffer *buffer = draw_frame(&state);
    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_damage_buffer(surface, 0, 0, UINT32_MAX, UINT32_MAX);

    wl_surface_commit(surface);


    while (wl_display_dispatch(display) != -1) {
        // standing in shaking and fear
    }

	return 0;
}
