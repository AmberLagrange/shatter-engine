#include <common/core.h>

#include <vulkan_renderer.h>

#include <commands/copy_command.h>

#include <stdlib.h>

shatter_status_t create_copy_command(vulkan_renderer_t *vk_renderer, copy_command_t *copy_command,
									   VkCommandPool *command_pool) {
	
	log_trace("Creating copy command buffer.\n");
		
	copy_command->command_pool = command_pool;

	VkCommandBufferAllocateInfo command_buffer_allocate_info = {
		
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		
		.commandPool = *command_pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	
	if (vkAllocateCommandBuffers(vk_renderer->logical_device, &command_buffer_allocate_info,
								 &(copy_command->command_buffer)) != VK_SUCCESS) {
		
		log_error("Failed to allocate image command buffers.\n");
		return SHATTER_VULKAN_COMMAND_BUFFER_INIT_FAILURE;
	}
	
	log_trace("Created image command buffers.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_copy_command(vulkan_renderer_t *vk_renderer, copy_command_t *copy_command) {
	
	vkFreeCommandBuffers(vk_renderer->logical_device, *(copy_command->command_pool), 1, &(copy_command->command_buffer));
	return SHATTER_SUCCESS;
}

shatter_status_t record_copy_command(vulkan_renderer_t *vk_renderer, copy_command_t *copy_command,
									 buffer_t *dst_buffer, buffer_t *src_buffer) {
	
	VkCommandBuffer command_buffer = copy_command->command_buffer;
	
	vkResetCommandBuffer(command_buffer, 0);
	
	VkCommandBufferBeginInfo command_buffer_begin_info = {
		
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};
	
	vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
	
	VkBufferCopy copy_region = {
		
		.srcOffset = 0,
		.dstOffset = 0,
		.size = dst_buffer->size,
	};
	
	vkCmdCopyBuffer(command_buffer, src_buffer->vulkan_buffer, dst_buffer->vulkan_buffer, 1, &copy_region);
	vkEndCommandBuffer(command_buffer);
	
	VkSubmitInfo submit_info = {
		
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		
		.commandBufferCount = 1,
		.pCommandBuffers = &command_buffer,
	};
	
	vkQueueSubmit(vk_renderer->graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
	vkQueueWaitIdle(vk_renderer->graphics_queue);
	
	return SHATTER_SUCCESS;
}

