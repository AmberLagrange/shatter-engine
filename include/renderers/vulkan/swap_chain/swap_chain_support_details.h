#ifndef RENDERERS_VULKAN_SWAP_CHAIN_SWAP_CHAIN_SUPPORT_DETAILS_H
#define RENDERERS_VULKAN_SWAP_CHAIN_SWAP_CHAIN_SUPPORT_DETAILS_H

#include <renderers/vulkan/renderer.h>

#include <stdint.h>

#include <vulkan/vulkan.h>

typedef struct swap_chain_support_details_s {
	
	VkSurfaceCapabilitiesKHR surface_capabilities;
	
	VkSurfaceFormatKHR *surface_format_list;
	uint32_t num_surface_formats;
	
	VkPresentModeKHR *present_mode_list;
	uint32_t num_present_modes;
} swap_chain_support_details_t;

swap_chain_support_details_t query_swap_chain_support(vulkan_renderer_t * vk_renderer, VkPhysicalDevice device);

#endif // RENDERERS_VULKAN_SWAP_CHAIN_SWAP_CHAIN_SUPPORT_DETAILS_H

