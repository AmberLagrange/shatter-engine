#include <common/core.h>

#include <renderers/vulkan/surfaces/surface.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

shatter_status_t create_surface(vulkan_renderer_t *vk_renderer) {
	
	if (glfwCreateWindowSurface(vk_renderer->vulkan_instance, vk_renderer->rendering_window,
								NULL, &(vk_renderer->rendering_surface))) {
		
		log_message(stderr, "Failed to create a rendering surface.\n");
		return SHATTER_SURFACE_INIT_FAILURE;
	}
	
	log_message(stdout, "\nRendering surface created.\n");
	return SHATTER_SUCCESS;
}

