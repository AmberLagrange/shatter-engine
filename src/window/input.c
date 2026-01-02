#include <common/core.h>

#include <dynamic_loader/dynamic_loader.h>

#include <renderer/abstract_renderer.h>

#include <window/input.h>

void renderer_key_callback(GLFWwindow *rendering_window, int key, int scancode, int action, int mods) {
	
	UNUSED(scancode);
	UNUSED(mods);
	
	abstract_renderer_t *renderer = glfwGetWindowUserPointer(rendering_window);
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		
		renderer->is_running = false;
	}
	
	if (key == GLFW_KEY_R && action == GLFW_RELEASE) {
		
		renderer->needs_reload = true;
	}
	
	if (key == GLFW_KEY_V && action == GLFW_RELEASE) {
		
		request_api(renderer->api_loader, VULKAN_API_INDEX);
		renderer->needs_reload = true;
	}
	
	if (key == GLFW_KEY_O && action == GLFW_RELEASE) {
		
		request_api(renderer->api_loader, OPENGL_API_INDEX);
		renderer->needs_reload = true;
	}
}

