#include <common/core.h>

#include <surfaces/surface.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

shatter_status_t create_surface(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	log_trace("Creating rendering surface.\n");
	
	if (glfwCreateWindowSurface(vk_renderer->vulkan_instance, vk_renderer->properties->rendering_window,
								NULL, &(vk_renderer->rendering_surface))) {
		
		log_error("Failed to create a rendering surface.\n");
		return SHATTER_SURFACE_INIT_FAILURE;
	}
	
	log_trace("Rendering surface created.\n");
	return SHATTER_SUCCESS;
}

