#include <common/core.h>

#include <renderers/vulkan/swap_chain/swap_chain_support_details.h>

swap_chain_support_details_t query_swap_chain_support(vulkan_renderer_t *vk_renderer, VkPhysicalDevice device) {
	
	swap_chain_support_details_t support_details = { 0 };
	
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vk_renderer->rendering_surface, &support_details.surface_capabilities);
	
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_renderer->rendering_surface,
										 &support_details.num_surface_formats, NULL);
	if (support_details.num_surface_formats != 0) {
		
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_renderer->rendering_surface,
											 &support_details.num_surface_formats,
											 support_details.surface_format_list);
	}
	
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_renderer->rendering_surface,
											  &support_details.num_present_modes, NULL);
	
	if (support_details.num_present_modes != 0) {
		
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_renderer->rendering_surface,
												  &support_details.num_present_modes,
												  support_details.present_mode_list);
	}
	
	return support_details;
}

