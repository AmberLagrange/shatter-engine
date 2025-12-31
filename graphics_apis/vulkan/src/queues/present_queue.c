#include <common/core.h>

#include <queues/present_queue.h>

#include <swap_chain/swap_chain.h>

#include <vulkan/vulkan.h>

shatter_status_t submit_present_queue(vulkan_renderer_t *vk_renderer, uint32_t image_index) {
	
	VkSemaphore wait_semaphore_list[1] = { vk_renderer->submit_image_semaphore_list[image_index] };
	VkSwapchainKHR swap_chain_list[1] = { vk_renderer->swap_chain };

	VkPresentInfoKHR present_info = {
		
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		
		.pWaitSemaphores = wait_semaphore_list,
		.waitSemaphoreCount = 1,
		
		.pSwapchains = swap_chain_list,
		.pImageIndices = &image_index,
		.swapchainCount = 1,
		
		.pResults = NULL,
	};
	
	VkResult result = vkQueuePresentKHR(vk_renderer->present_queue, &present_info);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vk_renderer->frame_buffer_resized) {
		
		recreate_swap_chain(vk_renderer);
		return SHATTER_VULKAN_SWAP_CHAIN_RECREATION;
	} else if (result != VK_SUCCESS) {
		
		return SHATTER_VULKAN_PRESENT_QUEUE_SUBMITION_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

