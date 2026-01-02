#include <common/core.h>

#include <vulkan_renderer.h>

#include <commands/image_commands.h>

#include <stdlib.h>

shatter_status_t create_image_commands(vulkan_renderer_t *vk_renderer, image_commands_t *image_commands,
									   VkCommandPool *command_pool, size_t num_command_buffers) {
	
	log_trace("Creating image command buffers.\n");
		
	image_commands->command_buffer_list = malloc(sizeof(VkCommandBuffer) * num_command_buffers);
	image_commands->command_pool = command_pool;
	image_commands->num_command_buffers = num_command_buffers;

	VkCommandBufferAllocateInfo command_buffer_allocate_info = {
		
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		
		.commandPool = *command_pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = num_command_buffers,
	};
	
	if (vkAllocateCommandBuffers(vk_renderer->logical_device, &command_buffer_allocate_info,
								 image_commands->command_buffer_list) != VK_SUCCESS) {
		
		log_error("Failed to allocate image command buffers.\n");
		free(image_commands->command_buffer_list);
		return SHATTER_VULKAN_COMMAND_BUFFER_INIT_FAILURE;
	}
	
	log_trace("Created image command buffers.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_image_commands(vulkan_renderer_t *vk_renderer, image_commands_t *image_commands) {
	
	UNUSED(vk_renderer);
	free(image_commands->command_buffer_list);
	return SHATTER_SUCCESS;
}

shatter_status_t record_image_command(vulkan_renderer_t *vk_renderer, image_commands_t *image_commands,
									  uint32_t command_index, uint32_t image_index) {
	
	VkCommandBuffer command_buffer = image_commands->command_buffer_list[command_index];
	
	vkResetCommandBuffer(command_buffer, 0);
	
	VkCommandBufferBeginInfo command_buffer_begin_info = {
		
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = 0,
		.pInheritanceInfo = NULL,
	};
	
	if (vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info) != VK_SUCCESS) {
		
		log_error("Failed to begin recording the command buffer.\n");
		return SHATTER_VULKAN_COMMAND_BUFFER_RECORD_FAILURE;
	}
	
	VkClearValue clear_color = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};
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
	
	VkBuffer     vertex_buffers[1] = { vk_renderer->vertex_buffer.vulkan_buffer };
	VkDeviceSize offsets[1]        = { 0 };
	vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);
	
	vkCmdDraw(command_buffer, 3, 1, 0, 0);
	vkCmdEndRenderPass(command_buffer);
	
	if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
		
		log_error("Failed to record the command buffer.\n");
		return SHATTER_VULKAN_COMMAND_BUFFER_RECORD_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

