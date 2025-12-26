#include <common/core.h>

#include <rendering_apis/vulkan/swap_chain/swap_chain_support_details.h>

#include <stdlib.h>
#include <string.h>

shatter_status_t query_swap_chain_support(swap_chain_support_details_t *support_details,
										  vulkan_renderer_t *vk_renderer,
										  VkPhysicalDevice device) {
	
	memset(support_details, 0, sizeof(swap_chain_support_details_t));
	
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vk_renderer->rendering_surface,
											  &(support_details->surface_capabilities));
	
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_renderer->rendering_surface,
										 &(support_details->num_surface_formats), NULL);
	
	if (support_details->num_surface_formats != 0) {
		
		support_details->surface_format_list = malloc(sizeof(VkSurfaceFormatKHR) * support_details->num_surface_formats);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_renderer->rendering_surface,
											 &(support_details->num_surface_formats),
											 support_details->surface_format_list);
	}
	
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_renderer->rendering_surface,
											  &(support_details->num_present_modes), NULL);
	
	if (support_details->num_present_modes != 0) {
		
		support_details->present_mode_list = malloc(sizeof(VkPresentModeKHR) * support_details->num_present_modes);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_renderer->rendering_surface,
												  &(support_details->num_present_modes),
												  support_details->present_mode_list);
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_swap_chain_support_details(swap_chain_support_details_t *support_details) {
	
	free(support_details->surface_format_list);
	free(support_details->present_mode_list);
	
	return SHATTER_SUCCESS;
}

