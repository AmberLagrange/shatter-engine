#ifndef RENDERER_RENDERER_CONFIG_H
#define RENDERER_RENDERER_CONFIG_H

#include <stdint.h>

#define MAX_TITLE_LENGTH 256

typedef struct renderer_config_s {
	
	uint32_t width, height;
	char title[MAX_TITLE_LENGTH];
	const char *binary_path;
} renderer_config_t;

#endif // RENDERER_RENDERER_CONFIG_H

