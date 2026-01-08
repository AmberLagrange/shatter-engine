#include <common/core.h>

#include <graphics_pipeline/descriptor_pool.h>

shatter_status_t create_descriptor_pool(vulkan_renderer_t *vk_renderer) {
	
	VkDescriptorPoolSize pool_size = {
		
		.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = vk_renderer->num_in_flight_frames,
	};
	
	VkDescriptorPoolCreateInfo pool_info = {
		
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		
		.poolSizeCount = 1,
		.pPoolSizes = &pool_size,
		
		.maxSets = vk_renderer->num_in_flight_frames,
		
		.flags = 0,
	};
	
	if (vkCreateDescriptorPool(vk_renderer->logical_device, &pool_info, NULL, &(vk_renderer->descriptor_pool)) != VK_SUCCESS) {
		
		log_error("Failed to create descriptor pool.\n");
		return SHATTER_VULKAN_DESCRIPTOR_POOL_INIT_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_descriptor_pool(vulkan_renderer_t *vk_renderer) {
	
	vkDestroyDescriptorPool(vk_renderer->logical_device, vk_renderer->descriptor_pool, NULL);
	return SHATTER_SUCCESS;
}

