#include <common/core.h>

#include <queues/queue_family_indices.h>

#include <swap_chain/frame_buffer.h>
#include <swap_chain/image_view.h>
#include <swap_chain/present_mode.h>
#include <swap_chain/render_pass.h>
#include <swap_chain/surface_format.h>
#include <swap_chain/swap_chain.h>
#include <swap_chain/swap_chain_support_details.h>
#include <swap_chain/swap_extent.h>
#include <swap_chain/sync_objects.h>

#include <stdlib.h>

#include <vulkan/vulkan.h>

shatter_status_t create_swap_chain(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	log_trace("Creating swapchain:\n");
	shatter_status_t status = SHATTER_SUCCESS;
	
	// ---------- Swap Chain ---------- //
	
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
	vk_renderer->swap_chain_image_format = surface_format.format;
	
	VkPresentModeKHR present_mode =
		select_swap_present_mode(swap_support.present_mode_list, swap_support.num_present_modes);
	
	vk_renderer->swap_chain_extent = select_swap_extent(vk_renderer, &(swap_support.surface_capabilities));
	
	VkSwapchainCreateInfoKHR create_info = {
		
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = vk_renderer->rendering_surface,
		
		.minImageCount = num_images,
		.imageFormat = surface_format.format,
		.imageColorSpace = surface_format.colorSpace,
		.imageExtent = vk_renderer->swap_chain_extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		
		.preTransform = swap_support.surface_capabilities.currentTransform,
		
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		
		.presentMode = present_mode,
		.clipped = VK_TRUE,
		
		.oldSwapchain = VK_NULL_HANDLE
	};
	
	if (is_exclusive_graphics(&(vk_renderer->queue_family_indices))) {
		
		log_trace("\tExclusive sharing mode.\n");
		
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		create_info.queueFamilyIndexCount = 0;
		create_info.pQueueFamilyIndices = NULL;
	} else {
		
		log_trace("Concurrent sharing mode.\n");
		
		size_t num_indices;
		uint32_t *unwrapped_indices = unwrap_indices(&(vk_renderer->queue_family_indices), &num_indices);
		
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = num_indices;
		create_info.pQueueFamilyIndices = unwrapped_indices;
		
		free(unwrapped_indices);
	}
	
	if (vkCreateSwapchainKHR(vk_renderer->logical_device, &create_info, NULL, &(vk_renderer->swap_chain)) != VK_SUCCESS) {
		
		log_error("Failed to create the swap chain.\n");
		status = SHATTER_VULKAN_SWAP_CHAIN_INIT_FAILURE;
		goto cleanup;
	};
	
	// ---------- Swap Chain Images ---------- //
	
	vk_renderer->num_swap_chain_images = 0;
	vkGetSwapchainImagesKHR(vk_renderer->logical_device, vk_renderer->swap_chain,
							(uint32_t *)&(vk_renderer->num_swap_chain_images), NULL);
	
	vk_renderer->swap_chain_image_list = malloc(sizeof(VkImage) * vk_renderer->num_swap_chain_images);
	
	vkGetSwapchainImagesKHR(vk_renderer->logical_device, vk_renderer->swap_chain,
							(uint32_t *)&(vk_renderer->num_swap_chain_images), vk_renderer->swap_chain_image_list);
	
	// ---------- Swap Chain Image Views ---------- //
	
	if (create_image_views(vk_renderer)) {
		
		log_error("Failed to create swap chain image views.\n");
		status = SHATTER_VULKAN_IMAGE_VIEW_INIT_FAILURE;
		goto cleanup;
	}
	
	// ---------- Render Pass ---------- //
	
	if (create_render_pass(vk_renderer)) {
		
		log_error("Failed to create swap chain render pass.\n");
		status = SHATTER_VULKAN_RENDER_PASS_INIT_FAILURE;
		goto cleanup;
	}
	
	// ---------- Frame Buffers ---------- //
	
	if (create_frame_buffers(vk_renderer)) {
		
		log_error("Failed to create swap chain frame buffers.\n");
		status = SHATTER_VULKAN_FRAME_BUFFER_INIT_FAILURE;
		goto cleanup;
	}
	
	// ---------- Sync Objects ---------- //
	
	if (create_sync_objects(vk_renderer)) {
		
		log_error("Failed to create swap chain sync objects.\n");
		status = SHATTER_VULKAN_SYNC_OBJECT_INIT_FAILURE;
		goto cleanup;
	}
	
	// ---------- Finish --------- //
	
	vk_renderer->frame_buffer_resized = false;
	log_trace("Created swap chain.\n");
cleanup:
	cleanup_swap_chain_support_details(&swap_support);
	return status;
}

shatter_status_t cleanup_swap_chain(vulkan_renderer_t *vk_renderer) {
	
	// ---------- Sync Objects ---------- //
	
	cleanup_sync_objects(vk_renderer);
	
	// ---------- Frame Buffers ---------- //
	
	cleanup_frame_buffers(vk_renderer);
	
	// ---------- Render Pass ---------- //
	
	cleanup_render_pass(vk_renderer);
	
	// ---------- Swap Chain Image Views ---------- //
	
	cleanup_image_views(vk_renderer);
	
	// ---------- Swap Chain Images ----------- //
	
	free(vk_renderer->swap_chain_image_list);
	
	// ---------- Swap Chain ---------- //
	vkDestroySwapchainKHR(vk_renderer->logical_device, vk_renderer->swap_chain, NULL);
	return SHATTER_SUCCESS;
}

shatter_status_t recreate_swap_chain(vulkan_renderer_t *vk_renderer) {
	
	log_trace("Recreate swap chain.\n");
	vkDeviceWaitIdle(vk_renderer->logical_device);
	
	cleanup_swap_chain(vk_renderer);
	
	if (create_swap_chain(vk_renderer)) {
		
		log_error("Swap chain recreation failed.\n");
		return SHATTER_VULKAN_SWAP_CHAIN_RECREATION_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t get_next_swap_chain_image(vulkan_renderer_t *vk_renderer, uint32_t *image_index) {
	
	VkResult result = vkAcquireNextImageKHR(vk_renderer->logical_device, vk_renderer->swap_chain,
											UINT64_MAX, vk_renderer->acquire_image_semaphore_list[vk_renderer->current_frame],
											VK_NULL_HANDLE, image_index);
	
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vk_renderer->frame_buffer_resized) {
		
		recreate_swap_chain(vk_renderer);
		return SHATTER_VULKAN_SWAP_CHAIN_RECREATION;
	} else if (result != VK_SUCCESS) {
		
		return SHATTER_VULKAN_SWAP_CHAIN_ACQUISITION_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

