#ifndef RENDERER_RENDERER_PROPERTIES_H
#define RENDERER_RENDERER_PROPERTIES_H

#include <buffers/buffer_info.h>

#include <stdint.h>

typedef struct GLFWwindow GLFWwindow;

typedef struct renderer_properties_s {
	
	uint32_t width, height;
	char title[MAX_TITLE_LENGTH + 1];
	
	GLFWwindow *rendering_window;
	
	const char *directory_filepath;
	
	buffer_info_t *vertex_buffer_info;
	buffer_info_t *index_buffer_info;
} renderer_properties_t;

#endif // RENDERER_RENDERER_PROPERTIES_H

