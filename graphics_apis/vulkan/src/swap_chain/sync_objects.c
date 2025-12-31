#include <common/core.h>

#include <vulkan_renderer.h>

#include <swap_chain/sync_objects.h>

#include <vulkan/vulkan.h>

#include <stdint.h>
#include <stdlib.h>

// ---------- Creation ---------- //

shatter_status_t create_sync_objects(vulkan_renderer_t *vk_renderer) {
	
	if (create_fences(vk_renderer)) {
		
		log_error("Failed to create in flight fences.\n");
		return SHATTER_VULKAN_FENCE_INIT_FAILURE;
	}
	
	if (create_acquire_semaphores(vk_renderer)) {
		
		log_error("Failed to create the image acquisition semaphores.\n");
		return SHATTER_VULKAN_SEMAPHORE_INIT_FAILURE;
	}
	
	if (create_submit_semaphores(vk_renderer)) {
		
		log_error("Failed to create the image submission semaphores.\n");
		return SHATTER_VULKAN_SEMAPHORE_INIT_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t create_fences(vulkan_renderer_t *vk_renderer) {
	
	vk_renderer->in_flight_fence_list = malloc(sizeof(VkFence) * vk_renderer->num_in_flight_frames);
	
	VkFenceCreateInfo fence_info = {
		
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT,
	};
	
	for (size_t fence_index = 0; fence_index < vk_renderer->num_in_flight_frames; ++fence_index) {
		
		VkFence *fence_ptr = &(vk_renderer->in_flight_fence_list[fence_index]);
		if (vkCreateFence(vk_renderer->logical_device, &fence_info, NULL, fence_ptr) != VK_SUCCESS) {
			
			return SHATTER_VULKAN_FENCE_INIT_FAILURE;
		}
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t create_acquire_semaphores(vulkan_renderer_t *vk_renderer) {
	
	vk_renderer->acquire_image_semaphore_list = malloc(sizeof(VkSemaphore) * vk_renderer->num_in_flight_frames);
	
	VkSemaphoreCreateInfo semaphore_info = {
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};
	
	for (size_t acquire_index = 0; acquire_index < vk_renderer->num_in_flight_frames; ++acquire_index) {
		
		VkSemaphore *acquire_semaphore_ptr = &(vk_renderer->acquire_image_semaphore_list[acquire_index]);
		if (vkCreateSemaphore(vk_renderer->logical_device, &semaphore_info, NULL, acquire_semaphore_ptr) != VK_SUCCESS) {
			
			return SHATTER_VULKAN_SEMAPHORE_INIT_FAILURE;
		};
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t create_submit_semaphores(vulkan_renderer_t *vk_renderer) {
	
	vk_renderer->submit_image_semaphore_list = malloc(sizeof(VkSemaphore) * vk_renderer->num_swap_chain_images);
	
	VkSemaphoreCreateInfo semaphore_info = {
		
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};
	
	for (size_t submit_index = 0; submit_index < vk_renderer->num_swap_chain_images; ++submit_index) {
		
		VkSemaphore *submit_semaphore_ptr = &(vk_renderer->submit_image_semaphore_list[submit_index]);
		if (vkCreateSemaphore(vk_renderer->logical_device, &semaphore_info, NULL, submit_semaphore_ptr) != VK_SUCCESS) {
			
			return SHATTER_VULKAN_SEMAPHORE_INIT_FAILURE;
		}
	}
	
	return SHATTER_SUCCESS;
}

// ---------- Cleanup ---------- //

shatter_status_t cleanup_sync_objects(vulkan_renderer_t *vk_renderer) {
	
	cleanup_submit_semaphores(vk_renderer);
	cleanup_acquire_semaphores(vk_renderer);
	cleanup_fences(vk_renderer);
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_fences(vulkan_renderer_t *vk_renderer) {
	
	for (size_t fence_index = 0; fence_index < vk_renderer->num_in_flight_frames; ++fence_index) {
		
		VkFence fence = vk_renderer->in_flight_fence_list[fence_index];
		vkDestroyFence(vk_renderer->logical_device, fence, NULL);
	}
	
	free(vk_renderer->in_flight_fence_list);
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_acquire_semaphores(vulkan_renderer_t *vk_renderer) {
	
	for (size_t acquire_index = 0; acquire_index < vk_renderer->num_in_flight_frames; ++acquire_index) {
		
		VkSemaphore acquire_semaphore = vk_renderer->acquire_image_semaphore_list[acquire_index];
		vkDestroySemaphore(vk_renderer->logical_device, acquire_semaphore, NULL);
	}
	
	free(vk_renderer->acquire_image_semaphore_list);
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_submit_semaphores(vulkan_renderer_t *vk_renderer) {
	
	for (size_t submit_index = 0; submit_index < vk_renderer->num_swap_chain_images; ++submit_index) {
		
		VkSemaphore submit_semaphore = vk_renderer->submit_image_semaphore_list[submit_index];
		vkDestroySemaphore(vk_renderer->logical_device, submit_semaphore, NULL);
	}
	
	free(vk_renderer->submit_image_semaphore_list);
	return SHATTER_SUCCESS;
}

// ---------- Fence Functions ---------- //

shatter_status_t wait_for_in_flight_fence(vulkan_renderer_t *vk_renderer) {
	
	VkFence *in_flight_fence = &(vk_renderer->in_flight_fence_list[vk_renderer->current_frame]);
	vkWaitForFences(vk_renderer->logical_device, 1, in_flight_fence, VK_TRUE, UINT64_MAX);
	
	return SHATTER_SUCCESS;
}

shatter_status_t reset_in_flight_fence(vulkan_renderer_t *vk_renderer) {
	
	VkFence *in_flight_fence = &(vk_renderer->in_flight_fence_list[vk_renderer->current_frame]);
	vkResetFences(vk_renderer->logical_device, 1, in_flight_fence);
	return SHATTER_SUCCESS;
}

