#include <common/core.h>

#include <window/glfw.h>
#include <GLFW/glfw3.h>

#include <stdbool.h>

static bool s_glfw_initialized = false;
static uint32_t s_glfw_instances = 0;

shatter_status_t init_glfw(void) {
	
	if (!s_glfw_initialized) {
		
		glfwInit();
		s_glfw_initialized = true;
		
		log_trace("Initialized GLFW.\n");
	}
	++s_glfw_instances;
	
	return SHATTER_SUCCESS;
}

shatter_status_t terminate_glfw(void) {
	
	--s_glfw_instances;
	if (s_glfw_instances == 0) {
		
		glfwTerminate();
		s_glfw_initialized = false;
		log_trace("\n");
		log_trace("Terminated GLFW.\n");
	}
	
	return SHATTER_SUCCESS;
}

