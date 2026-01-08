#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <common/core.h>

#include <buffers/buffer_info.h>

#include <renderer/renderer_properties.h>

typedef struct opengl_renderer_s {
	
	renderer_properties_t *properties;
	
	unsigned int vertex_array_object;
	buffer_info_t *vertex_buffer_info;
	
	unsigned int index_buffer_object;
	buffer_info_t *index_buffer_info;
	
	unsigned int uniform_buffer_object;
	buffer_info_t *uniform_buffer_info;
	
	unsigned int vertex_buffer_object;
	
	unsigned int shader_program;
} opengl_renderer_t;

shatter_status_t create_opengl_renderer(opengl_renderer_t **gl_renderer_ptr);
shatter_status_t init_opengl_renderer(opengl_renderer_t *gl_renderer, renderer_properties_t *properties);
shatter_status_t loop_opengl_renderer(opengl_renderer_t *gl_renderer);
shatter_status_t cleanup_opengl_renderer(opengl_renderer_t *gl_renderer);

// ---------- Buffer Submition ---------- //

shatter_status_t submit_opengl_vertex_info(opengl_renderer_t *gl_renderer, buffer_info_t *vertex_info);
shatter_status_t submit_opengl_index_info(opengl_renderer_t *gl_renderer, buffer_info_t *index_info);
shatter_status_t submit_opengl_uniform_buffer_info(opengl_renderer_t *gl_renderer, buffer_info_t *uniform_buffer_info);

// ---------- Callbacks ---------- //

shatter_status_t opengl_frame_buffer_resize_callback(opengl_renderer_t *gl_renderer);

#endif // OPENGL_RENDERER_H

