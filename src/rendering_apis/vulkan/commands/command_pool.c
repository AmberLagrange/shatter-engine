#include <common/core.h>

#include <rendering_apis/vulkan/commands/command_pool.h>

#include <vulkan/vulkan.h>

shatter_status_t create_command_pool(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	log_trace("Creating command pool.\n");
	
	VkCommandPoolCreateInfo command_pool_info = {
		
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		
		.queueFamilyIndex = vk_renderer->queue_family_indicies.index_list[GRAPHICS_FAMILY_INDEX].value,
	};
	
	if (vkCreateCommandPool(vk_renderer->logical_device, &command_pool_info,
							NULL, &(vk_renderer->command_pool)) != VK_SUCCESS) {
		
		log_error("Could not create command pool.\n");
		return SHATTER_VULKAN_COMMAND_POOL_INIT_FAILURE;
	}
	
	log_info("Created command pool.\n");
	return SHATTER_SUCCESS;
}

