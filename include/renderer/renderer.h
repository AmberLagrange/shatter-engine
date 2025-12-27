#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include <common/core.h>

#include <dynamic_loader/dynamic_loader.h>

#include <renderer/renderer_config.h>

#include <GLFW/glfw3.h>

#include <stdbool.h>

typedef shatter_status_t (*init_api_t)(void *, renderer_config_t *);
typedef shatter_status_t (*loop_api_t)(void *);
typedef shatter_status_t (*cleanup_api_t)(void *);

typedef struct renderer_s {
	
	renderer_config_t *renderer_config;
	
	dynamic_loader_t* api_loader;
	void *api_renderer;
	
	bool is_running;
	bool needs_reload;
	bool reload_error;
} renderer_t;

shatter_status_t renderer_run(renderer_t *renderer);

#endif // RENDERER_RENDERER_H

