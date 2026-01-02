#ifndef RENDERER_ABSTRACT_RENDERER_H
#define RENDERER_ABSTRACT_RENDERER_H

#include <common/core.h>

#include <buffers/buffer_info.h>

#include <dynamic_loader/dynamic_loader.h>

#include <renderer/renderer_properties.h>

#include <GLFW/glfw3.h>

#include <stdbool.h>

typedef shatter_status_t (*init_api_t)(void *, renderer_properties_t *);
typedef shatter_status_t (*loop_api_t)(void *);
typedef shatter_status_t (*cleanup_api_t)(void *);

typedef struct abstract_renderer_s {
	
	renderer_properties_t *properties;
	
	dynamic_loader_t* api_loader;
	void *api_renderer;
	
	bool is_running;
	bool needs_reload;
	bool reload_error;
} abstract_renderer_t;

shatter_status_t init_abstract_renderer(abstract_renderer_t *renderer);
shatter_status_t loop_abstract_renderer(abstract_renderer_t *renderer);
shatter_status_t cleanup_abstract_renderer(abstract_renderer_t *renderer);

#endif // RENDERER_ABSTRACT_RENDERER_H

