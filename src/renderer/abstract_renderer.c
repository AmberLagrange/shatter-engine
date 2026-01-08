#include <common/core.h>

#include <buffers/uniform_buffer_object.h>

#include <dynamic_loader/dynamic_loader.h>
#include <dynamic_loader/vtable.h>

#include <renderer/abstract_renderer.h>

#include <window/glfw.h>
#include <window/input.h>
#include <window/window.h>

#include <cglm/cglm.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>

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
	
	submit_abstract_vertex_info(renderer);
	submit_abstract_index_info(renderer);
	submit_abstract_uniform_buffer_info(renderer);
	
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
	
	vec3 rotation_axis = { 0.0f, 0.0f, 1.0f };
	uniform_buffer_object_t *ubo_ptr = renderer->uniform_buffer_info->data;
	mat4 *model = &(ubo_ptr->model);
	
	struct timeval start_time;
	struct timeval curr_time;
	double delta_time;
	
	gettimeofday(&start_time, NULL);
	
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
		
		gettimeofday(&curr_time, NULL);
		delta_time = (curr_time.tv_sec - start_time.tv_sec) + 1e-6 * (curr_time.tv_usec - start_time.tv_usec);
		
		glm_mat4_identity(*model); // Reset the model matrix so that the rotations don't compound
		glm_rotate(*model, glm_rad(90.0f) * delta_time, rotation_axis);
		
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

// ---------- Submit Buffer Info ---------- //

shatter_status_t submit_abstract_vertex_info(abstract_renderer_t *renderer) {
	
	submit_api_buffer_info_t submit_api_vertex_info = renderer->api_loader->api_vtable.submit_api_vertex_info;
	return submit_api_vertex_info(renderer->api_renderer, renderer->vertex_info);
}

shatter_status_t submit_abstract_index_info(abstract_renderer_t *renderer) {
	
	submit_api_buffer_info_t submit_api_index_info = renderer->api_loader->api_vtable.submit_api_index_info;
	return submit_api_index_info(renderer->api_renderer, renderer->index_info);
}

shatter_status_t submit_abstract_uniform_buffer_info(abstract_renderer_t *renderer) {
	
	submit_api_buffer_info_t submit_api_uniform_buffer_info = renderer->api_loader->api_vtable.submit_api_uniform_buffer_info;
	return submit_api_uniform_buffer_info(renderer->api_renderer, renderer->uniform_buffer_info);
}

