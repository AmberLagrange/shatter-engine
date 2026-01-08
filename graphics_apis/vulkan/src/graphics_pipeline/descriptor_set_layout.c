#include <common/core.h>

#include <graphics_pipeline/descriptor_set_layout.h>

#include <vulkan/vulkan.h>

shatter_status_t create_descriptor_set_layout(vulkan_renderer_t *vk_renderer) {
	
	VkDescriptorSetLayoutBinding uniform_buffer_object_binding = {
		
		.binding = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
		
		.pImmutableSamplers = NULL,
	};
	
	VkDescriptorSetLayoutCreateInfo descriptor_set_info = {
		
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		
		.bindingCount = 1,
		.pBindings = &uniform_buffer_object_binding,
	};
	
	if (vkCreateDescriptorSetLayout(vk_renderer->logical_device, &descriptor_set_info,
									NULL, &(vk_renderer->descriptor_set_layout)) != VK_SUCCESS) {
		
		log_error("Failed to create the descriptor set layout.\n");
		return SHATTER_VULKAN_DESCRIPTOR_SET_LAYOUT_INIT_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_descriptor_set_layout(vulkan_renderer_t *vk_renderer) {
	
	vkDestroyDescriptorSetLayout(vk_renderer->logical_device, vk_renderer->descriptor_set_layout, NULL);
	return SHATTER_SUCCESS;
}


