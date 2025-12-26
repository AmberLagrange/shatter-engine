#include <common/core.h>

#include <rendering_apis/vulkan/sync/sync_objects.h>

#include <vulkan/vulkan.h>

shatter_status_t create_sync_objects(vulkan_renderer_t *vk_renderer) {
	
	// ---------- Semaphores ---------- //
	
	VkSemaphoreCreateInfo semaphore_info = {
		
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};
	
	if (vkCreateSemaphore(vk_renderer->logical_device, &semaphore_info,
						  NULL, &(vk_renderer->image_available_semaphore)) != VK_SUCCESS) {
		
		log_error("Failed to create image available semaphore.\n");
		return SHATTER_VULKAN_SEMAPHORE_INIT_FAILURE;
	};

	if (vkCreateSemaphore(vk_renderer->logical_device, &semaphore_info,
						  NULL, &(vk_renderer->render_finished_semaphore)) != VK_SUCCESS) {
		
		log_error("Failed to create render finished semaphore.\n");
		return SHATTER_VULKAN_SEMAPHORE_INIT_FAILURE;
	};
	
	// ---------- Frence ---------- //
	
	VkFenceCreateInfo fence_info = {
		
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT,
	};
	
	if (vkCreateFence(vk_renderer->logical_device, &fence_info, NULL, &(vk_renderer->in_flight_fence)) != VK_SUCCESS) {
		
		log_error("Failed to create in flight fence.\n");
		return SHATTER_VULKAN_FENCE_INIT_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

