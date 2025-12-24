#include <renderers/renderer.h>

#include <stdlib.h>

// Temporarily just include the vulkan renderer
// and use it directly in the init function
// TODO: Abstract this more with shared libraries
#include <renderers/vulkan/renderer.h>

int renderer_init(renderer_t *renderer, renderer_config_t *config) {
	
	renderer->api_renderer = malloc(sizeof(vulkan_renderer_t));
	
	renderer->renderer_init_callback = (int (*)(void *, renderer_config_t *))(&vulkan_renderer_init);
	renderer->renderer_loop_callback = (int (*)(void *))(&vulkan_renderer_loop);
	renderer->renderer_cleanup_callback = (int (*)(void *))(&vulkan_renderer_cleanup);
	
	return renderer->renderer_init_callback(renderer->api_renderer, config);
}

int renderer_loop(renderer_t *renderer) {
	
	return renderer->renderer_loop_callback(renderer->api_renderer);
}

int renderer_cleanup(renderer_t *renderer) {
	
	int status = renderer->renderer_cleanup_callback(renderer->api_renderer);
	free(renderer->api_renderer);
	return status;
}

