#include <common/core.h>

#include <swap_chain/frame_buffer.h>

#include <vulkan/vulkan.h>

#include <stdlib.h>

shatter_status_t create_frame_buffers(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	log_trace("Creating frame buffers.\n");
	
	vk_renderer->num_swap_chain_frame_buffers = vk_renderer->num_swap_chain_image_views;
	vk_renderer->swap_chain_frame_buffer_list = malloc(sizeof(VkFramebuffer) * vk_renderer->num_swap_chain_frame_buffers);
	
	for (size_t frame_buffer_index = 0; frame_buffer_index < vk_renderer->num_swap_chain_frame_buffers; ++frame_buffer_index) {
		
		VkImageView attachment_list[1] = { vk_renderer->swap_chain_image_view_list[frame_buffer_index] };
		
		VkFramebufferCreateInfo frame_buffer_info = {
			
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			
			.renderPass = vk_renderer->render_pass,
			
			.attachmentCount = 1,
			.pAttachments = attachment_list,
			
			.width  = vk_renderer->swap_chain_extent.width,
			.height = vk_renderer->swap_chain_extent.height,
			
			.layers = 1,
		};
		
		if (vkCreateFramebuffer(vk_renderer->logical_device, &frame_buffer_info,
								NULL, &(vk_renderer->swap_chain_frame_buffer_list[frame_buffer_index])) != VK_SUCCESS) {
			
			log_error("Failed to create frame buffer at index: %u\n", frame_buffer_index);
			free(vk_renderer->swap_chain_frame_buffer_list);
			return SHATTER_VULKAN_FRAME_BUFFER_INIT_FAILURE;
		}
	}
	
	log_trace("Created frame buffers.\n");
	return SHATTER_SUCCESS;;
}

shatter_status_t cleanup_frame_buffers(vulkan_renderer_t *vk_renderer) {
	
	for (size_t frame_buffer_index = 0; frame_buffer_index < vk_renderer->num_swap_chain_frame_buffers; ++frame_buffer_index) {
		
		vkDestroyFramebuffer(vk_renderer->logical_device, vk_renderer->swap_chain_frame_buffer_list[frame_buffer_index], NULL);
	}
	
	free(vk_renderer->swap_chain_frame_buffer_list);
	return SHATTER_SUCCESS;
}

