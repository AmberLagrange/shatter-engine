#ifndef RENDERER_RENDERER_CONFIG_H
#define RENDERER_RENDERER_CONFIG_H

#include <stdint.h>

typedef struct GLFWwindow GLFWwindow;

typedef struct renderer_config_s {
	
	uint32_t width, height;
	
	char title[MAX_TITLE_LENGTH + 1];
	
	const char *directory_filepath;
	
	const char *api_filepath;
} renderer_config_t;

#endif // RENDERER_RENDERER_CONFIG_H

