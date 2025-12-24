#ifndef RENDERERS_RENDERER_CONFIG_H
#define RENDERERS_RENDERER_CONFIG_H

#include <stdint.h>

#define MAX_TITLE_LENGTH 256

typedef struct {
	
	uint32_t width, height;
	char title[MAX_TITLE_LENGTH];
} renderer_config_t;

#endif // RENDERERS_RENDERER_CONFIG_H

