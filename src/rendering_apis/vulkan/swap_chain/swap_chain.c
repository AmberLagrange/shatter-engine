#include <common/core.h>

#include <rendering_apis/vulkan/queues/queue_family_indicies.h>

#include <rendering_apis/vulkan/swap_chain/present_mode.h>
#include <rendering_apis/vulkan/swap_chain/surface_format.h>
#include <rendering_apis/vulkan/swap_chain/swap_chain.h>
#include <rendering_apis/vulkan/swap_chain/swap_chain_support_details.h>
#include <rendering_apis/vulkan/swap_chain/swap_extent.h>

#include <stdint.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>

shatter_status_t create_swap_chain(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	log_trace("Creating swapchain:\n");
	shatter_status_t status = SHATTER_SUCCESS;
	
	swap_chain_support_details_t swap_support;
	query_swap_chain_support(&swap_support, vk_renderer, vk_renderer->physical_device);
	
	uint32_t num_images = swap_support.surface_capabilities.minImageCount + 1;
	uint32_t max_images = swap_support.surface_capabilities.maxImageCount;
	
	// 0 signifies no maximum
	if (max_images != 0) {
		
		// Clamp num_images to be at most max_images
		num_images = (num_images > max_images) ? max_images : num_images;
	}
	
	VkSurfaceFormatKHR surface_format =
		select_swap_surface_format(swap_support.surface_format_list, swap_support.num_surface_formats);
	
	VkPresentModeKHR present_mode =
		select_swap_present_mode(swap_support.present_mode_list, swap_support.num_present_modes);
	
	VkExtent2D extent = select_swap_extent(vk_renderer, &(swap_support.surface_capabilities));
	
	VkSwapchainCreateInfoKHR create_info = {
		
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = vk_renderer->rendering_surface,
		
		.minImageCount = num_images,
		.imageFormat = surface_format.format,
		.imageColorSpace = surface_format.colorSpace,
		.imageExtent = extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		
		.preTransform = swap_support.surface_capabilities.currentTransform,
		
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		
		.presentMode = present_mode,
		.clipped = VK_TRUE,
		
		.oldSwapchain = VK_NULL_HANDLE
	};
	
	if (is_exclusive_graphics(&(vk_renderer->queue_family_indicies))) {
		
		log_trace("\tExclusive sharing mode.\n");
		
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		create_info.queueFamilyIndexCount = 0;
		create_info.pQueueFamilyIndices = NULL;
	} else {
		
		log_trace("Concurrent sharing mode.\n");
		
		size_t num_indicies;
		uint32_t *unwrapped_indicies = unwrap_indicies(&(vk_renderer->queue_family_indicies), &num_indicies);
		
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = num_indicies;
		create_info.pQueueFamilyIndices = unwrapped_indicies;
		
		free(unwrapped_indicies);
	}
	
	if (vkCreateSwapchainKHR(vk_renderer->logical_device, &create_info, NULL, &(vk_renderer->swap_chain)) != VK_SUCCESS) {
		
		log_error("Failed to create the swap chain.\n");
		status = SHATTER_VULKAN_SWAP_CHAIN_INIT_FAILURE;
		goto cleanup;
	};
	
	vkGetSwapchainImagesKHR(vk_renderer->logical_device, vk_renderer->swap_chain,
							(uint32_t *)&(vk_renderer->num_swap_chain_images), NULL);
	
	vk_renderer->swap_chain_image_list = malloc(sizeof(VkImage) * vk_renderer->num_swap_chain_images);
	
	vkGetSwapchainImagesKHR(vk_renderer->logical_device, vk_renderer->swap_chain,
							(uint32_t *)&(vk_renderer->num_swap_chain_images), vk_renderer->swap_chain_image_list);
	
	vk_renderer->swap_chain_image_format = surface_format.format;
	vk_renderer->swap_chain_extent = extent;
	log_info("Created swap chain.\n");
	
cleanup:
	cleanup_swap_chain_support_details(&swap_support);
	return status;
}

