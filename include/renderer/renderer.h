#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include <common/core.h>

#include <renderer/renderer_config.h>

#include <GLFW/glfw3.h>

typedef struct renderer_s {
	
	void *api_renderer;
	GLFWwindow *rendering_window;
} renderer_t;

shatter_status_t init_renderer(renderer_t *renderer, renderer_config_t *config);
shatter_status_t loop_renderer(renderer_t *renderer);
shatter_status_t cleanup_renderer(renderer_t *renderer);

#endif // RENDERER_RENDERER_H

