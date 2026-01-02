#include <common/core.h>

#include <dynamic_loader/dynamic_loader.h>
#include <dynamic_loader/vtable.h>

#include <renderer/abstract_renderer.h>

#include <window/glfw.h>
#include <window/input.h>
#include <window/window.h>

#include <stdbool.h>
#include <stdlib.h>

extern volatile bool sigint_signaled;

shatter_status_t create_abstract_renderer(abstract_renderer_t *renderer) {
	
	create_api_renderer_t create_api_renderer = renderer->api_loader->api_vtable.create_api_renderer;
	return create_api_renderer(&(renderer->api_renderer));
}

shatter_status_t init_abstract_renderer(abstract_renderer_t *renderer) {
	
	renderer->is_running = true;
	renderer->needs_reload = false;
	renderer->reload_error = false;
	
	init_api_renderer_t init_api_renderer = renderer->api_loader->api_vtable.init_api_renderer;
	
	if (init_api_renderer(renderer->api_renderer, renderer->properties)) {
		
		log_error("Failed to initialize the renderer.\n");
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	glfwSetWindowUserPointer(renderer->properties->rendering_window, renderer);
	glfwSetKeyCallback(renderer->properties->rendering_window, renderer_key_callback);
	glfwSetFramebufferSizeCallback(renderer->properties->rendering_window, frame_buffer_resize_callback);
	return SHATTER_SUCCESS;
}

shatter_status_t reload_abstract_renderer(abstract_renderer_t *renderer) {
	
	if (cleanup_abstract_renderer(renderer)) {
		
		return SHATTER_RENDERER_RELOAD_FAILURE;
	}
	
	shatter_status_t reload_status = reload_library(renderer->api_loader);
	if (reload_status == SHATTER_DYNAMIC_LIBRARY_RELOAD_RECOVERED) {
		
		renderer->reload_error = true;
	} else if (reload_status == SHATTER_DYNAMIC_LIBRARY_RELOAD_FAILURE) {
		
		renderer->reload_error = true;
		return SHATTER_RENDERER_RELOAD_FAILURE;
	}
	
	create_abstract_renderer(renderer);
	
	if (init_abstract_renderer(renderer)) {
		
		return SHATTER_RENDERER_RELOAD_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t loop_abstract_renderer(abstract_renderer_t *renderer) {
	
	GLFWwindow *rendering_window = renderer->properties->rendering_window;
	
	loop_api_renderer_t loop_api_renderer = renderer->api_loader->api_vtable.loop_api_renderer;
	
	while (!glfwWindowShouldClose(rendering_window) && renderer->is_running && !sigint_signaled) {
		
		glfwPollEvents();
		if (renderer->needs_reload) {
			
			if (reload_abstract_renderer(renderer)) {
				
				renderer->is_running = false;
				return SHATTER_RENDERER_RELOAD_FAILURE;
			}
			
			// Update the loop function to be the new API
			loop_api_renderer = renderer->api_loader->api_vtable.loop_api_renderer;
		}
		
		shatter_status_t status = loop_api_renderer(renderer->api_renderer);
		if (status) {
			
			return status;
		}
	}
	
	renderer->is_running = false;
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_abstract_renderer(abstract_renderer_t *renderer) {
	
	if (renderer->reload_error) {
		
		return SHATTER_RENDERER_CLEANUP_NONE;
	}
	
	glfwSetKeyCallback(renderer->properties->rendering_window, NULL);
	cleanup_api_renderer_t cleanup_api_renderer = renderer->api_loader->api_vtable.cleanup_api_renderer;
	
	shatter_status_t status = cleanup_api_renderer(renderer->api_renderer);
	
	if (unload_library(renderer->api_loader)) {
		
		return SHATTER_RENDERER_CLEANUP_FAILURE;
	}
	
	return status;
}

