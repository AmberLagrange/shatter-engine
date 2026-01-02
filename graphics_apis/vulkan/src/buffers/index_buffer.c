#include <common/core.h>

#include <buffers/index_buffer.h>
#include <buffers/staging_buffer.h>

#include <commands/copy_command.h>

const uint32_t indices[6] = {
	
	0, 1, 2,
	2, 3, 0,
};

shatter_status_t create_index_buffer(vulkan_renderer_t *vk_renderer, buffer_t *index_buffer) {
	
	index_buffer->size = sizeof(indices);
	index_buffer->usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	index_buffer->properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	
	index_buffer->data = (void *)(indices);
	index_buffer->num_elements = sizeof(indices) / sizeof(indices[0]);
	
	if (create_buffer_with_staging(vk_renderer, index_buffer)) {
		
		log_error("Failed to create index buffer.\n");
		return SHATTER_VULKAN_INDEX_BUFFER_INIT_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_index_buffer(vulkan_renderer_t *vk_renderer, buffer_t *index_buffer) {
	
	cleanup_buffer(vk_renderer, index_buffer);
	return SHATTER_SUCCESS;
}

