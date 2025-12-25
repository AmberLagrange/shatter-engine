#include <common/core.h>

#include <renderers/vulkan/swap_chain/image_view.h>

#include <stdlib.h>

shatter_status_t create_image_views(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	log_trace("Creating image views.\n");
	
	vk_renderer->num_swap_chain_image_views = vk_renderer->num_swap_chain_images;
	vk_renderer->swap_chain_image_view_list = malloc(sizeof(VkImageView) * vk_renderer->num_swap_chain_image_views);
	
	for (size_t image_view_index = 0; image_view_index < vk_renderer->num_swap_chain_image_views; ++image_view_index) {
		
		VkImageViewCreateInfo create_info = {
			
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = vk_renderer->swap_chain_image_list[image_view_index],
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = vk_renderer->swap_chain_image_format,
			
			.components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
			.components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
			.components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
			.components.a = VK_COMPONENT_SWIZZLE_IDENTITY,
			
			.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.subresourceRange.baseMipLevel = 0,
			.subresourceRange.levelCount = 1,
			.subresourceRange.baseArrayLayer = 0,
			.subresourceRange.layerCount = 1,
		};
		
		if (vkCreateImageView(vk_renderer->logical_device, &create_info, NULL,
							  &(vk_renderer->swap_chain_image_view_list[image_view_index])) != VK_SUCCESS) {
				
				log_error("Failed to create image views.\n");
				free(vk_renderer->swap_chain_image_view_list);
				return SHATTER_VULKAN_IMAGE_VIEW_INIT_FAILURE;
		}
	}
	
	log_info("Created image views.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_image_views(vulkan_renderer_t *vk_renderer) {
	
	for (size_t image_view_index = 0; image_view_index < vk_renderer->num_swap_chain_image_views; ++image_view_index) {
		
		vkDestroyImageView(vk_renderer->logical_device, vk_renderer->swap_chain_image_view_list[image_view_index], NULL);
	}
	
	free(vk_renderer->swap_chain_image_view_list);
	
	return SHATTER_SUCCESS;
}

