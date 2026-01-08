#ifndef DYNAMIC_LOADER_VTABLE_H
#define DYNAMIC_LOADER_VTABLE_H

#include <common/core.h>

#include <buffers/buffer_info.h>

#include <renderer/renderer_properties.h>

#include <stddef.h>

typedef struct GLFWwindow GLFWwindow;

typedef shatter_status_t (*create_api_renderer_t)(void **);
typedef shatter_status_t (*init_api_renderer_t)(void *, renderer_properties_t *);
typedef shatter_status_t (*loop_api_renderer_t)(void *);
typedef shatter_status_t (*cleanup_api_renderer_t)(void *);

typedef shatter_status_t (*submit_api_buffer_info_t)(void *, buffer_info_t *);

typedef shatter_status_t (*api_frame_buffer_resize_callback_t)(void *);

typedef struct vtable_s {
	
	create_api_renderer_t  create_api_renderer;
	init_api_renderer_t    init_api_renderer;
	loop_api_renderer_t    loop_api_renderer;
	cleanup_api_renderer_t cleanup_api_renderer;
	
	submit_api_buffer_info_t submit_api_vertex_info;
	submit_api_buffer_info_t submit_api_index_info;
	submit_api_buffer_info_t submit_api_uniform_buffer_info;
	
	api_frame_buffer_resize_callback_t api_frame_buffer_resize_callback;
} vtable_t;

#endif // DYNAMIC_LOADER_VTABLE_H

