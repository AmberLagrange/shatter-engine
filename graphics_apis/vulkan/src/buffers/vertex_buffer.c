#include <common/core.h>

#include <buffers/vertex_buffer.h>

#include <commands/copy_command.h>

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

shatter_status_t create_vertex_buffer(vulkan_renderer_t *vk_renderer) {
	
	VkDeviceSize buffer_size = sizeof(vertices);
	
	// ---------- Staging Buffer ---------- //
	
	buffer_t staging_buffer = {
		
		.size = buffer_size,
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	};
	
	if (create_buffer(vk_renderer, &staging_buffer)) {
		
		log_error("Failed to create the vertex staging buffer.\n");
		return SHATTER_VULKAN_STAGING_BUFFER_INIT_FAILURE;
	}
	
	if (allocate_buffer_memory(vk_renderer, &staging_buffer)) {
		
		log_error("Failed to allocate memory for the staging buffer.\n");
		return SHATTER_VULKAN_STAGING_BUFFER_ALLOCATE_FAILURE;
	}
	
	if (memcpy_buffer_to_device(vk_renderer, &staging_buffer, (void *)(vertices))) {
		
		log_error("Failed to copy vertex data to the staging buffer.\n");
		return SHATTER_VULKAN_STAGING_BUFFER_COPY_FAILURE;
	}
	
	// ---------- Vertex Buffer ---------- //
	
	vk_renderer->vertex_buffer.size = buffer_size;
	vk_renderer->vertex_buffer.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	vk_renderer->vertex_buffer.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	
	if (create_buffer(vk_renderer, &(vk_renderer->vertex_buffer))) {
		
		log_error("Failed to create vertex buffer.\n");
		return SHATTER_VULKAN_VERTEX_BUFFER_INIT_FAILURE;
	}
	
	if (allocate_buffer_memory(vk_renderer, &(vk_renderer->vertex_buffer))) {
		
		log_error("Failed to allocate memory for the vertex buffer.\n");
		return SHATTER_VULKAN_VERTEX_BUFFER_ALLOCATE_FAILURE;
	}
	
	copy_command_t copy_command;
		
	create_copy_command(vk_renderer, &copy_command, &(vk_renderer->command_pool));
	record_copy_command(vk_renderer, &copy_command, &(vk_renderer->vertex_buffer), &staging_buffer);
	cleanup_copy_command(vk_renderer, &copy_command);
	cleanup_buffer(vk_renderer, &staging_buffer);
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_vertex_buffer(vulkan_renderer_t *vk_renderer) {
	
	cleanup_buffer(vk_renderer, &(vk_renderer->vertex_buffer));
	return SHATTER_SUCCESS;
}

