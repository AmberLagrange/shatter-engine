#include <common/core.h>

#include <renderer/renderer.h>

#include <window/glfw.h>
#include <window/input.h>

#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>

// ---------- Global Check for SIGINT ---------- //

static volatile bool s_is_running = true;

// ---------- Static Function Prototypes ---------- //

static shatter_status_t init_renderer(renderer_t *renderer);
static shatter_status_t loop_renderer(renderer_t *renderer);
static shatter_status_t cleanup_renderer(renderer_t *renderer);
static shatter_status_t reload_renderer(renderer_t *renderer);

static void sigint_handler(int _) {
	
	UNUSED(_);
	s_is_running = false;
}

// ---------- Static Function Definitions ---------- //

static shatter_status_t init_renderer(renderer_t *renderer) {
	
	renderer->is_running = true;
	renderer->needs_reload = false;
	
	if (renderer->api_loader->api_vtable.init_api_renderer(&(renderer->api_renderer), renderer->renderer_config)) {
		
		log_error("Failed to initialize the renderer.\n");
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	glfwSetWindowUserPointer(renderer->renderer_config->rendering_window, renderer);
	glfwSetKeyCallback(renderer->renderer_config->rendering_window, renderer_key_callback);
	return SHATTER_SUCCESS;
}

static shatter_status_t reload_renderer(renderer_t *renderer) {
	
	if (cleanup_renderer(renderer)) {
		
		return SHATTER_RENDERER_RELOAD_FAILURE;
	}
	
	if (load_library(renderer->api_loader)) {
		
		return SHATTER_RENDERER_RELOAD_FAILURE;
	}
	
	if (init_renderer(renderer)) {
		
		return SHATTER_RENDERER_RELOAD_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

static shatter_status_t loop_renderer(renderer_t *renderer) {	
	
	while (!glfwWindowShouldClose(renderer->renderer_config->rendering_window)
			&& renderer->is_running && s_is_running) {
		
		glfwPollEvents();
		if (renderer->needs_reload) {
			
			if (reload_renderer(renderer)) {
				
				return SHATTER_RENDERER_RELOAD_FAILURE;
			}
		}
		
		shatter_status_t status = renderer->api_loader->api_vtable.loop_api_renderer(renderer->api_renderer);
		if (status) {
			
			return status;
		}
	}
	
	renderer->is_running = false;
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_renderer(renderer_t *renderer) {
	
	glfwSetKeyCallback(renderer->renderer_config->rendering_window, NULL);
	shatter_status_t status = renderer->api_loader->api_vtable.cleanup_api_renderer(renderer->api_renderer);
	
	if (unload_library(renderer->api_loader)) {
		
		return SHATTER_RENDERER_CLEANUP_FAILURE;
	}
	
	return status;
}

// ---------- Non Static Functions ---------- //

shatter_status_t renderer_run(renderer_t *renderer) {
	
	int status = SHATTER_SUCCESS;
	
	if (init_glfw()) {
		
		log_error("Failed to initialize GLFW.\n");
		status = SHATTER_RENDERER_RUN_FAILURE;
		goto exit;
	}
	
	if (load_library(renderer->api_loader)) {
		
		status = SHATTER_RENDERER_RUN_FAILURE;
		goto cleanup_glfw_loader;
	}
	
	if (init_renderer(renderer)) {
		
		log_error("Failed to initialize Renderer.\n");
		status = SHATTER_RENDERER_RUN_FAILURE;
		goto cleanup_renderer;
	}
	
	signal(SIGINT, &sigint_handler);
	
	if (loop_renderer(renderer)) {
		
		log_error("Error occurred in the rendering loop.\n");
		status = SHATTER_RENDERER_RUN_FAILURE;
		goto cleanup_renderer;
	}

cleanup_renderer:
	if (cleanup_renderer(renderer)) {
		
		log_error("Failed to cleanup Renderer.\n");
		status = SHATTER_RENDERER_RUN_FAILURE;
		goto exit;
	}
	
cleanup_glfw_loader:
	if (terminate_glfw()) {
		
		return SHATTER_RENDERER_RUN_FAILURE;
	}
	
exit:
	return status;
}


