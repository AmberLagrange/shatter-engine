#include <renderers/glfw.h>
#include <GLFW/glfw3.h>

#include <logging/logger.h>

#include <stdbool.h>

static bool s_glfw_initialized = false;
static uint32_t s_glfw_instances = 0;

int init_glfw(void) {
	
	if (!s_glfw_initialized) {
		
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		s_glfw_initialized = true;
		
		log_message(stdout, "Initialized GLFW.\n");
	}
	++s_glfw_instances;
	
	return SHATTER_GLFW_SUCCESS;
}

int terminate_glfw(void) {
	
	--s_glfw_instances;
	if (s_glfw_instances == 0) {
		
		glfwTerminate();
		s_glfw_initialized = false;
		log_message(stdout, "Terminated GLFW.\n");
	}
	
	return SHATTER_GLFW_SUCCESS;
}

