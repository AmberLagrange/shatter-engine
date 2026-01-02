#include <common/core.h>

#include <buffers/staging_buffer.h>
#include <buffers/vertex_buffer.h>

const vertex_t vertices[4] = {
	
	{ { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
	{ {  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
	{ {  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f } },
	{ { -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f } },
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

shatter_status_t create_vertex_buffer(vulkan_renderer_t *vk_renderer, buffer_t *vertex_buffer) {
	
	vertex_buffer->size = sizeof(vertices);
	vertex_buffer->usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	vertex_buffer->properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	
	vertex_buffer->data = (void *)(vertices);
	vertex_buffer->num_elements = sizeof(vertices) / sizeof(vertices[0]);
	
	if (create_buffer_with_staging(vk_renderer, vertex_buffer)) {
		
		log_error("Failed to create vertex buffer.\n");
		return SHATTER_VULKAN_VERTEX_BUFFER_INIT_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_vertex_buffer(vulkan_renderer_t *vk_renderer, buffer_t *vertex_buffer) {
	
	cleanup_buffer(vk_renderer, vertex_buffer);
	return SHATTER_SUCCESS;
}

