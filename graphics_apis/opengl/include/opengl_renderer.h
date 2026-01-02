#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <common/core.h>

#include <renderer/renderer_properties.h>

typedef struct opengl_renderer_s {
	
	renderer_properties_t *properties;
	
	unsigned int vertex_array_object;
	unsigned int index_buffer_object;
	unsigned int vertex_buffer_object;
	
	unsigned int shader_program;
} opengl_renderer_t;

shatter_status_t init_opengl_renderer(opengl_renderer_t **opengl_renderer_ptr, renderer_properties_t *properties);
shatter_status_t loop_opengl_renderer(opengl_renderer_t *opengl_renderer);
shatter_status_t cleanup_opengl_renderer(opengl_renderer_t *opengl_renderer);

// ---------- Callbacks ---------- //

shatter_status_t opengl_frame_buffer_resize_callback(opengl_renderer_t *opengl_renderer);

#endif // OPENGL_RENDERER_H

