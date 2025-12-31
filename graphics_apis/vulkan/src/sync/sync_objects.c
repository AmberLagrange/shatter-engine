#include <common/core.h>

#include <vulkan_renderer.h>

#include <sync/sync_objects.h>

#include <vulkan/vulkan.h>

#include <stdlib.h>

shatter_status_t create_sync_objects(vulkan_renderer_t *vk_renderer) {
	
	vk_renderer->acquire_image_semaphore_list = malloc(sizeof(VkSemaphore) * MAX_IN_FLIGHT_FRAMES);
	vk_renderer->in_flight_fence_list = malloc(sizeof(VkFence) * MAX_IN_FLIGHT_FRAMES);
	
	vk_renderer->submit_image_semaphore_list = malloc(sizeof(VkSemaphore) * vk_renderer->num_swap_chain_images);
	
	VkSemaphoreCreateInfo semaphore_info = {
		
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};
	
	VkFenceCreateInfo fence_info = {
		
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT,
	};
	
	for (size_t frame_index = 0; frame_index < MAX_IN_FLIGHT_FRAMES; ++frame_index) {
		
		if (vkCreateSemaphore(vk_renderer->logical_device, &semaphore_info,
							  NULL, &(vk_renderer->acquire_image_semaphore_list[frame_index])) != VK_SUCCESS) {
			
			log_error("Failed to create image available semaphore.\n");
			return SHATTER_VULKAN_SEMAPHORE_INIT_FAILURE;
		};
		
		if (vkCreateFence(vk_renderer->logical_device, &fence_info,
						  NULL, &(vk_renderer->in_flight_fence_list[frame_index])) != VK_SUCCESS) {
			
			log_error("Failed to create in flight fence.\n");
			return SHATTER_VULKAN_FENCE_INIT_FAILURE;
		}
	}
	
	for (size_t submit_index = 0; submit_index < vk_renderer->num_swap_chain_images; ++submit_index) {
		
		if (vkCreateSemaphore(vk_renderer->logical_device, &semaphore_info,
							  NULL, &(vk_renderer->submit_image_semaphore_list[submit_index])) != VK_SUCCESS) {
			
			log_error("Failed to create image available semaphore.\n");
			return SHATTER_VULKAN_SEMAPHORE_INIT_FAILURE;
		}
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_sync_objects(vulkan_renderer_t *vk_renderer) {
	
	for (size_t submit_index = 0; submit_index < vk_renderer->num_swap_chain_images; ++submit_index) {
		
		vkDestroySemaphore(vk_renderer->logical_device, vk_renderer->submit_image_semaphore_list[submit_index], NULL);
	}
	
	for (size_t frame_index = 0; frame_index < MAX_IN_FLIGHT_FRAMES; ++frame_index) {
		
		vkDestroyFence(vk_renderer->logical_device, vk_renderer->in_flight_fence_list[frame_index], NULL);
		vkDestroySemaphore(vk_renderer->logical_device, vk_renderer->acquire_image_semaphore_list[frame_index], NULL);
	}
	
	free(vk_renderer->submit_image_semaphore_list);
	free(vk_renderer->in_flight_fence_list);
	free(vk_renderer->acquire_image_semaphore_list);
	
	return SHATTER_SUCCESS;
}

