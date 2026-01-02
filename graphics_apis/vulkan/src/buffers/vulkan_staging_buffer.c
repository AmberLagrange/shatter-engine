#include <common/core.h>

#include <buffers/vulkan_staging_buffer.h>

#include <commands/copy_command.h>

shatter_status_t create_vulkan_buffer_with_staging(vulkan_renderer_t *vk_renderer,
												   vulkan_buffer_t *buffer, buffer_info_t *buffer_info) {
	
	shatter_status_t status = SHATTER_SUCCESS;
	
	// ---------- Staging Buffer ---------- //
	
	vulkan_buffer_t staging_buffer = {
		
		.size = buffer_info->size,
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	};
	
	if (create_vulkan_buffer(vk_renderer, &staging_buffer, buffer_info)) {
		
		log_error("Failed to create the vertex staging buffer.\n");
		return SHATTER_VULKAN_STAGING_BUFFER_INIT_FAILURE;
	}
	
	if (allocate_buffer_memory(vk_renderer, &staging_buffer)) {
		
		log_error("Failed to allocate memory for the staging buffer.\n");
		status = SHATTER_VULKAN_STAGING_BUFFER_ALLOCATE_FAILURE;
		goto cleanup_staging_buffer;
	}
	
	if (memcpy_buffer_to_device(vk_renderer, &staging_buffer, buffer_info)) {
		
		log_error("Failed to copy data to the staging buffer.\n");
		status = SHATTER_VULKAN_STAGING_BUFFER_COPY_FAILURE;
		goto cleanup_staging_buffer;
	}
	
	// ---------- Provided Buffer ---------- //
	
	if (create_vulkan_buffer(vk_renderer, buffer, buffer_info)) {
		
		log_error("Failed to create the buffer.\n");
		status = SHATTER_VULKAN_PROVIDED_BUFFER_INIT_FAILURE;
		goto cleanup_staging_buffer;
	}
	
	if (allocate_buffer_memory(vk_renderer, buffer)) {
		
		log_error("Failed to allocate memory for the buffer.\n");
		status = SHATTER_VULKAN_PROVIDED_BUFFER_ALLOCATE_FAILURE;
		cleanup_vulkan_buffer(vk_renderer, buffer);
		goto cleanup_staging_buffer;
	}
	
	if (memcpy_buffer_to_buffer(vk_renderer, buffer, &staging_buffer)) {
		
		log_error("Failed to copy data from the staging buffer.\n");
		cleanup_vulkan_buffer(vk_renderer, buffer);
		status = SHATTER_VULKAN_PROVIDED_BUFFER_COPY_FAILURE;
	}
	
cleanup_staging_buffer:
	cleanup_vulkan_buffer(vk_renderer, &staging_buffer);
	
	return status;
}

