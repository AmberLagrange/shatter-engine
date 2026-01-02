#include <common/core.h>

#include <dynamic_loader/vtable.h>

#include <renderer/abstract_renderer.h>

#include <window/window.h>

void frame_buffer_resize_callback(GLFWwindow *rendering_window, int width, int height) {
	
	UNUSED(width);
	UNUSED(height);
	
	abstract_renderer_t *renderer = glfwGetWindowUserPointer(rendering_window);
	
	api_frame_buffer_resize_callback_t api_frame_buffer_resize_callback =
		renderer->api_loader->api_vtable.api_frame_buffer_resize_callback;
	
	api_frame_buffer_resize_callback(renderer->api_renderer);
}

