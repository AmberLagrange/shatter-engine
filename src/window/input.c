#include <common/core.h>

#include <renderer/renderer.h>

#include <window/input.h>

void renderer_key_callback(GLFWwindow *rendering_window, int key, int scancode, int action, int mods) {
	
	UNUSED(scancode);
	UNUSED(mods);
	
	
	if (action != GLFW_PRESS) {
		
		return;
	}
	renderer_t *renderer = glfwGetWindowUserPointer(rendering_window);
	
	switch(key) {
		
		case GLFW_KEY_ESCAPE:
			
			renderer->is_running = false;
			break;
		
		case GLFW_KEY_R:
			
			renderer->needs_reload = true;
			break;
		
		default:
			
			break;
	}
}

