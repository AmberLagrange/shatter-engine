#include <common/core.h>

#include <buffers/uniform_buffer_object.h>

#include <graphics_pipeline/descriptor_sets.h>

#include <string.h>

shatter_status_t create_descriptor_sets(vulkan_renderer_t *vk_renderer) {
	
	vk_renderer->num_descriptor_sets = vk_renderer->num_in_flight_frames;
	
	vk_renderer->descriptor_set_list = malloc(sizeof(VkDescriptorSet) * vk_renderer->num_descriptor_sets);
	
	VkDescriptorSetLayout *set_layout_list = malloc(sizeof(VkDescriptorSetLayout) * vk_renderer->num_descriptor_sets);
	for (size_t layout_index = 0; layout_index < vk_renderer->num_descriptor_sets; ++layout_index) {
		
		set_layout_list[layout_index] = vk_renderer->descriptor_set_layout;
	}
	
	VkDescriptorSetAllocateInfo allocate_info = {
		
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		
		.descriptorPool = vk_renderer->descriptor_pool,
		
		.descriptorSetCount = vk_renderer->num_descriptor_sets,
		.pSetLayouts = set_layout_list,
	};
	
	shatter_status_t status = SHATTER_SUCCESS;
	if (vkAllocateDescriptorSets(vk_renderer->logical_device, &allocate_info, vk_renderer->descriptor_set_list) != VK_SUCCESS) {
		
		log_error("Failed to allocate descriptor sets.\n");
		status = SHATTER_VULKAN_DESCRIPTOR_SET_INIT_FAILURE;
		goto cleanup;
	}
	
	for (size_t set_index = 0; set_index < vk_renderer->num_descriptor_sets; ++set_index) {
		
		VkDescriptorBufferInfo descriptor_buffer_info = {
			
			.buffer = vk_renderer->uniform_buffer_list[set_index].vulkan_buffer,
			.offset = 0,
			.range = sizeof(uniform_buffer_object_t),
		};
		
		VkWriteDescriptorSet write_descriptor_set = {
			
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			
			.dstSet = vk_renderer->descriptor_set_list[set_index],
			.dstBinding = 0,
			.dstArrayElement = 0,
			
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = 1,
			
			.pBufferInfo = &descriptor_buffer_info,
			.pImageInfo = NULL,
			.pTexelBufferView = NULL,
		};
		
		vkUpdateDescriptorSets(vk_renderer->logical_device, 1, &write_descriptor_set, 0, NULL);
	}
	
cleanup:
	free(set_layout_list);
	return status;
}

shatter_status_t cleanup_descriptor_sets(vulkan_renderer_t *vk_renderer) {
	
	free(vk_renderer->descriptor_set_list);
	return SHATTER_SUCCESS;
}

