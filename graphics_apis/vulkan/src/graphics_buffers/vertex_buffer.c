#include <common/core.h>

#include <devices/physical.h>

#include <graphics_buffers/vertex_buffer.h>

#include <string.h>

const vertex_t vertices[3] = {
	{ {  0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
	{ {  0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f } },
	{ { -0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f } },
};

shatter_status_t get_vertex_binding_description(VkVertexInputBindingDescription *binding_description) {
	
	binding_description->binding = 0;
	binding_description->stride = sizeof(vertex_t);
	binding_description->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	
	return SHATTER_SUCCESS;
}

shatter_status_t get_vertex_attribute_descriptions(VkVertexInputAttributeDescription *attribute_description_list) {
	
	attribute_description_list[VERTEX_POSITION_ATTRIBUTE].binding  = 0;
	attribute_description_list[VERTEX_POSITION_ATTRIBUTE].location = VERTEX_POSITION_ATTRIBUTE;
	attribute_description_list[VERTEX_POSITION_ATTRIBUTE].format   = VK_FORMAT_R32G32_SFLOAT;
	attribute_description_list[VERTEX_POSITION_ATTRIBUTE].offset   = offsetof(vertex_t, pos);
	
	attribute_description_list[VERTEX_COLOR_ATTRIBUTE].binding  = 0;
	attribute_description_list[VERTEX_COLOR_ATTRIBUTE].location = VERTEX_COLOR_ATTRIBUTE;
	attribute_description_list[VERTEX_COLOR_ATTRIBUTE].format   = VK_FORMAT_R32G32B32_SFLOAT;
	attribute_description_list[VERTEX_COLOR_ATTRIBUTE].offset   = offsetof(vertex_t, color);
	
	return SHATTER_SUCCESS;
}

shatter_status_t create_vertex_buffers(vulkan_renderer_t *vk_renderer) {
	
	VkBufferCreateInfo buffer_info = {
		
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		
		.size = sizeof(vertices),
		.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.flags = 0,
	};
	
	if (vkCreateBuffer(vk_renderer->logical_device, &buffer_info, NULL, &(vk_renderer->vertex_buffer)) != VK_SUCCESS) {
		
		log_error("Failed to create the vertex buffer.\n");
		return SHATTER_VULKAN_VERTEX_BUFFER_INIT_FAILURE;
	}
	
	VkMemoryRequirements vertex_buffer_memory_requirements;
	vkGetBufferMemoryRequirements(vk_renderer->logical_device, vk_renderer->vertex_buffer,
								  &vertex_buffer_memory_requirements);
	
	uint32_t memory_type;
	if (get_physical_device_memory_type(vk_renderer, vertex_buffer_memory_requirements.memoryTypeBits,
										VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
										&memory_type)) {
		
		log_error("Could not find a suitable memory type for the vertex buffer.\n");
		return SHATTER_VULKAN_VERTEX_BUFFER_INIT_FAILURE;
	}
	
	VkMemoryAllocateInfo allocate_info = {
		
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		
		.allocationSize = vertex_buffer_memory_requirements.size,
		.memoryTypeIndex = memory_type,
	};
	
	if (vkAllocateMemory(vk_renderer->logical_device, &allocate_info,
						 NULL, &(vk_renderer->vertex_buffer_memory)) != VK_SUCCESS) {
		
		log_error("Failed to allocate memory for the vertex buffer.\n");
		return SHATTER_VULKAN_VERTEX_BUFFER_ALLOCATE_FAILURE;
	}
	
	vkBindBufferMemory(vk_renderer->logical_device, vk_renderer->vertex_buffer,
					   vk_renderer->vertex_buffer_memory, 0);
	
	void *device_memory;
	vkMapMemory(vk_renderer->logical_device, vk_renderer->vertex_buffer_memory, 0,
				buffer_info.size, 0, &device_memory);
	memcpy(device_memory, vertices, (size_t)(buffer_info.size));
	vkUnmapMemory(vk_renderer->logical_device, vk_renderer->vertex_buffer_memory);
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_vertex_buffers(vulkan_renderer_t *vk_renderer) {
	
	vkFreeMemory(vk_renderer->logical_device, vk_renderer->vertex_buffer_memory, NULL);
	vkDestroyBuffer(vk_renderer->logical_device, vk_renderer->vertex_buffer, NULL);
	
	return SHATTER_SUCCESS;
}

