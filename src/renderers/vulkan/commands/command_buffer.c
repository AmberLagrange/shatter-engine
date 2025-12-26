#include <common/core.h>

#include <renderers/vulkan/commands/command_buffer.h>

#include <vulkan/vulkan.h>

shatter_status_t create_command_buffer(vulkan_renderer_t *vk_renderer) {
	
	log_trace("Creating command buffer.\n");
	
	VkCommandBufferAllocateInfo command_buffer_allocate_info = {
		
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		
		.commandPool = vk_renderer->command_pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	
	if (vkAllocateCommandBuffers(vk_renderer->logical_device, &command_buffer_allocate_info,
								 &(vk_renderer->command_buffer)) != VK_SUCCESS) {
		
		log_error("Failed to allocate command buffers.\n");
		return SHATTER_VULKAN_COMMAND_BUFFER_INIT_FAILURE;
	}
	
	log_info("Created command buffer.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t record_command_buffer(vulkan_renderer_t *vk_renderer, VkCommandBuffer command_buffer, uint32_t image_index) {
	
	VkCommandBufferBeginInfo command_buffer_begin_info = {
		
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = 0,
		.pInheritanceInfo = NULL,
	};
	
	if (vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info) != VK_SUCCESS) {
		
		log_error("Failed to begin recording the command buffer.\n");
		return SHATTER_VULKAN_COMMAND_BUFFER_RECORD_FAILURE;
	}
	
	VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
	VkRenderPassBeginInfo render_pass_info = {
		
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		
		.renderPass = vk_renderer->render_pass,
		.framebuffer = vk_renderer->swap_chain_frame_buffer_list[image_index],
		
		.renderArea.offset = { 0, 0 },
		.renderArea.extent = vk_renderer->swap_chain_extent,
		
		.clearValueCount = 1,
		.pClearValues = &clear_color,
	};
	vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_renderer->graphics_pipeline);
	
	VkViewport viewport = {
		
		.x = 0.0f,
		.y = 0.0f,
		
		.width  = (float)(vk_renderer->swap_chain_extent.width),
		.height = (float)(vk_renderer->swap_chain_extent.height),
		
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};
	vkCmdSetViewport(command_buffer, 0, 1, &viewport);
	
	VkRect2D scissor = {
		
		.offset = { 0, 0 },
		.extent = vk_renderer->swap_chain_extent
	};
	vkCmdSetScissor(command_buffer, 0, 1, &scissor);
	
	vkCmdDraw(command_buffer, 3, 1, 0, 0);
	vkCmdEndRenderPass(command_buffer);
	
	if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
		
		log_error("Failed to record the command buffer.\n");
		return SHATTER_VULKAN_COMMAND_BUFFER_RECORD_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

