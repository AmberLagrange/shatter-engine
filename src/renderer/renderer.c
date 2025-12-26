#include <common/core.h>

#include <renderer/renderer.h>

#include <window/glfw.h>
#include <window/input.h>

#include <stdlib.h>

extern shatter_status_t init_api_renderer(void **api_renderer, renderer_config_t *config, GLFWwindow **rendering_window_ptr);
extern shatter_status_t loop_api_renderer(void *api_renderer);
extern shatter_status_t cleanup_api_renderer(void *api_renderer);

shatter_status_t init_renderer(renderer_t *renderer, renderer_config_t *config) {
	
	if (init_glfw()) {
		
		log_error("Failed to initialize GLFW.\n");
		return SHATTER_GLFW_INIT_FAILURE;
	}
	
	return init_api_renderer(&(renderer->api_renderer), config, &(renderer->rendering_window));
}

shatter_status_t loop_renderer(renderer_t *renderer) {
	
	glfwSetKeyCallback(renderer->rendering_window, escape_callback);
	return loop_api_renderer(renderer->api_renderer);
}

shatter_status_t cleanup_renderer(renderer_t *renderer) {
	
	int status = cleanup_api_renderer(renderer->api_renderer);
	
	if (terminate_glfw()) {
		
		log_error("Falied to terminate GLFW.\n");
		return SHATTER_GLFW_TERMINATE_FAILURE;
	}
	
	return status;
}

