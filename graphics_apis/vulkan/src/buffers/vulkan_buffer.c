#include <common/core.h>

#include <buffers/vulkan_buffer.h>

#include <commands/copy_command.h>

#include <devices/physical.h>

#include <string.h>

// ---------- Creation and Cleanup ---------- //

shatter_status_t create_vulkan_buffer(vulkan_renderer_t *vk_renderer, vulkan_buffer_t *buffer, buffer_info_t *buffer_info) {
	
	VkBufferCreateInfo buffer_create_info = {
		
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		
		.size = buffer_info->size,
		.usage = buffer->usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.flags = 0,
	};
	
	if (vkCreateBuffer(vk_renderer->logical_device, &buffer_create_info, NULL, &(buffer->vulkan_buffer)) != VK_SUCCESS) {
		
		log_error("Failed to create the buffer.\n");
		return SHATTER_VULKAN_VERTEX_BUFFER_INIT_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_vulkan_buffer(vulkan_renderer_t *vk_renderer, vulkan_buffer_t *buffer) {
	
	vkFreeMemory(vk_renderer->logical_device, buffer->memory, NULL);
	vkDestroyBuffer(vk_renderer->logical_device, buffer->vulkan_buffer, NULL);
	
	return SHATTER_SUCCESS;
}

// ---------- Memory Allocation ---------- //
	
shatter_status_t allocate_buffer_memory(vulkan_renderer_t *vk_renderer, vulkan_buffer_t *buffer) {
	
	vkGetBufferMemoryRequirements(vk_renderer->logical_device, buffer->vulkan_buffer, &(buffer->memory_requirements));
	
	uint32_t memory_type;
	if (get_physical_device_memory_type(vk_renderer, buffer->memory_requirements.memoryTypeBits,
										buffer->properties, &memory_type)) {
		
		log_error("Could not find a suitable memory type for the buffer.\n");
		return SHATTER_VULKAN_BUFFER_ALLOCATE_FAILURE;
	}
	
	VkMemoryAllocateInfo allocate_info = {
		
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		
		.allocationSize = buffer->memory_requirements.size,
		.memoryTypeIndex = memory_type,
	};
	
	if (vkAllocateMemory(vk_renderer->logical_device, &allocate_info, NULL, &(buffer->memory)) != VK_SUCCESS) {
		
		log_error("Failed to allocate memory for the buffer.\n");
		return SHATTER_VULKAN_BUFFER_ALLOCATE_FAILURE;
	}
	
	vkBindBufferMemory(vk_renderer->logical_device, buffer->vulkan_buffer, buffer->memory, 0);
	
	return SHATTER_SUCCESS;
}

// ---------- Memory Copying ---------- //

shatter_status_t memcpy_buffer_to_device(vulkan_renderer_t *vk_renderer, vulkan_buffer_t *buffer, buffer_info_t *buffer_info) {
	
	void *device_memory;
	
	vkMapMemory(vk_renderer->logical_device, buffer->memory, 0, buffer_info->size, 0, &device_memory);
	memcpy(device_memory, buffer_info->data, buffer_info->size);
	vkUnmapMemory(vk_renderer->logical_device, buffer->memory);
	
	return SHATTER_SUCCESS;
}

shatter_status_t memcpy_buffer_to_buffer(vulkan_renderer_t *vk_renderer, vulkan_buffer_t *dst_buffer,
																		 vulkan_buffer_t *src_buffer) {
	
	copy_command_t copy_command;
	create_copy_command(vk_renderer, &copy_command, &(vk_renderer->command_pool));
	
	if (record_copy_command(vk_renderer, &copy_command, dst_buffer, src_buffer)) {
		
		log_error("Failed to copy data to the destination buffer.\n");
		return SHATTER_VULKAN_BUFFER_COPY_FAILURE;
	}
	
	cleanup_copy_command(vk_renderer, &copy_command);
	
	return SHATTER_SUCCESS;
}

