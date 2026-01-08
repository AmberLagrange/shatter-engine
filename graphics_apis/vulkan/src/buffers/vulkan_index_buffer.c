#include <common/core.h>

#include <buffers/vulkan_index_buffer.h>
#include <buffers/vulkan_staging_buffer.h>

shatter_status_t create_vulkan_index_buffer(vulkan_renderer_t *vk_renderer) {
	
	vk_renderer->index_buffer.size = vk_renderer->index_buffer_info->size;
	vk_renderer->index_buffer.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	vk_renderer->index_buffer.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	
	if (create_vulkan_buffer_with_staging(vk_renderer, &(vk_renderer->index_buffer),
										  vk_renderer->index_buffer_info)) {
		
		log_error("Failed to create index buffer.\n");
		return SHATTER_VULKAN_INDEX_BUFFER_INIT_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_vulkan_index_buffer(vulkan_renderer_t *vk_renderer) {
	
	cleanup_vulkan_buffer(vk_renderer, &(vk_renderer->index_buffer));
	return SHATTER_SUCCESS;
}

