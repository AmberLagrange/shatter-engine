#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include <common/core.h>

#include <dynamic_loader/dynamic_loader.h>

#include <renderer/renderer_config.h>

#include <GLFW/glfw3.h>

typedef shatter_status_t (*init_api_t)(void *, renderer_config_t *, GLFWwindow **);
typedef shatter_status_t (*loop_api_t)(void *);
typedef shatter_status_t (*cleanup_api_t)(void *);

typedef struct renderer_s {
	
	dynamic_loader_t api_loader;
	void *api_renderer;
	
	GLFWwindow *rendering_window;
} renderer_t;

shatter_status_t init_renderer(renderer_t *renderer, renderer_config_t *renderer_config);
shatter_status_t loop_renderer(renderer_t *renderer);
shatter_status_t cleanup_renderer(renderer_t *renderer);

shatter_status_t renderer_run(renderer_t *renderer, renderer_config_t *renderer_config);

#endif // RENDERER_RENDERER_H

