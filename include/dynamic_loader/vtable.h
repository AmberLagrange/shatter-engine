#ifndef DYNAMIC_LOADER_VTABLE_H
#define DYNAMIC_LOADER_VTABLE_H

#include <common/core.h>

#include <renderer/renderer_config.h>

typedef struct GLFWwindow GLFWwindow;

typedef shatter_status_t (*init_api_renderer_t)(void *, renderer_config_t *);
typedef shatter_status_t (*loop_api_renderer_t)(void *);
typedef shatter_status_t (*cleanup_api_renderer_t)(void *);

typedef shatter_status_t (*api_frame_buffer_resize_callback_t)(void *);

typedef struct vtable_s {
	
	init_api_renderer_t    init_api_renderer;
	loop_api_renderer_t    loop_api_renderer;
	cleanup_api_renderer_t cleanup_api_renderer;
	
	api_frame_buffer_resize_callback_t api_frame_buffer_resize_callback;
} vtable_t;

#endif // DYNAMIC_LOADER_VTABLE_H

