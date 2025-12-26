#ifndef SWAP_CHAIN_SWAP_CHAIN_SUPPORT_DETAILS_H
#define SWAP_CHAIN_SWAP_CHAIN_SUPPORT_DETAILS_H

#include <common/core.h>

#include <vulkan_renderer.h>

#include <stdint.h>

#include <vulkan/vulkan.h>

typedef struct swap_chain_support_details_s {
	
	VkSurfaceCapabilitiesKHR surface_capabilities;
	
	VkSurfaceFormatKHR *surface_format_list;
	uint32_t num_surface_formats;
	
	VkPresentModeKHR *present_mode_list;
	uint32_t num_present_modes;
} swap_chain_support_details_t;

shatter_status_t query_swap_chain_support(swap_chain_support_details_t *support_details,
										  vulkan_renderer_t * vk_renderer,
										  VkPhysicalDevice device);

shatter_status_t cleanup_swap_chain_support_details(swap_chain_support_details_t *support_details);

#endif // SWAP_CHAIN_SWAP_CHAIN_SUPPORT_DETAILS_H

