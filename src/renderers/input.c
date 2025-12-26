#include <common/core.h>

#include <renderers/input.h>

void escape_callback(GLFWwindow *rendering_window, int key, int scancode, int action, int mods) {
	
	UNUSED(scancode);
	UNUSED(mods);
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		
		glfwSetWindowShouldClose(rendering_window, GLFW_TRUE);
	}
}

