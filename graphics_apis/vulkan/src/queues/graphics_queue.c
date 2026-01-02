#include <common/core.h>

#include <queues/graphics_queue.h>

#include <vulkan/vulkan.h>

#include <stdint.h>

shatter_status_t submit_graphics_queue(vulkan_renderer_t *vk_renderer, uint32_t image_index) {
	
	uint32_t current_frame = vk_renderer->current_frame;
	
	VkFence in_flight_fence = vk_renderer->in_flight_fence_list[vk_renderer->current_frame];
	
	VkSemaphore wait_semaphore_list[1] = { vk_renderer->acquire_image_semaphore_list[current_frame] };
	VkPipelineStageFlags wait_stage_masks[1] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	
	VkCommandBuffer command_buffer_list[1] = { vk_renderer->image_commands.command_buffer_list[current_frame] };
	
	VkSemaphore signal_semaphore_list[1] = { vk_renderer->submit_image_semaphore_list[image_index] };
	
	VkSubmitInfo submit_info = {
		
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		
		.pWaitSemaphores = wait_semaphore_list,
		.waitSemaphoreCount = 1,
		.pWaitDstStageMask = wait_stage_masks,
		
		.pCommandBuffers = command_buffer_list,
		.commandBufferCount = 1,
		
		.pSignalSemaphores = signal_semaphore_list,
		.signalSemaphoreCount = 1
	};
	
	if (vkQueueSubmit(vk_renderer->graphics_queue, 1, &submit_info, in_flight_fence) != VK_SUCCESS) {
		
		log_error("Failed to submit to the graphics queue.\n");
		return SHATTER_VULKAN_DRAW_FRAME_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

