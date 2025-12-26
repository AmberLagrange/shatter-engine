#include <common/core.h>

#include <rendering_apis/vulkan/swap_chain/swap_extent.h>

#include <stdint.h>

VkExtent2D select_swap_extent(vulkan_renderer_t *vk_renderer, const VkSurfaceCapabilitiesKHR *surface_capabilities) {
	
	if (surface_capabilities->currentExtent.width != UINT32_MAX) {
		
		return surface_capabilities->currentExtent;
	}
	
	uint32_t width, height;
	glfwGetFramebufferSize(vk_renderer->rendering_window, (int *)(&width), (int *)(&height));
	
	uint32_t min_width = surface_capabilities->minImageExtent.width;
	uint32_t max_width = surface_capabilities->maxImageExtent.width;
	
	uint32_t min_height = surface_capabilities->minImageExtent.height;
	uint32_t max_height = surface_capabilities->maxImageExtent.height;
	
	// Clamp width
	width = (width < min_width) ? min_width : width;
	width = (width > max_width) ? max_width : width;

	// Clamp height
	height = (height < min_height) ? min_height : height;
	height = (height > max_height) ? max_height : height;
	
	VkExtent2D real_extent = {
		.width  = (uint32_t)(width),
		.height = (uint32_t)(height),
	};
	
	return real_extent;
}

