#ifndef GRAPHICS_BUFFERS_BUFFER_H
#define GRAPHICS_BUFFERS_BUFFER_H

#include <vulkan/vulkan.h>

#include <stdint.h>

typedef struct buffer_s {
	
	VkBuffer vulkan_buffer;
	VkDeviceMemory memory;
	
	VkMemoryRequirements memory_requirements;
	VkDeviceSize size;
	VkBufferUsageFlags usage;
	VkMemoryPropertyFlags properties;
	
	void *data;
	size_t num_elements;
} buffer_t;

typedef struct vulkan_renderer_s vulkan_renderer_t;

shatter_status_t create_buffer(vulkan_renderer_t *vk_renderer, buffer_t *buffer);
shatter_status_t cleanup_buffer(vulkan_renderer_t *vk_renderer, buffer_t *buffer);

shatter_status_t allocate_buffer_memory(vulkan_renderer_t *vk_renderer, buffer_t *buffer);

shatter_status_t memcpy_buffer_to_device(vulkan_renderer_t *vk_renderer, buffer_t *buffer, void *data);
shatter_status_t memcpy_buffer_to_buffer(vulkan_renderer_t *vk_renderer, buffer_t *dst_buffer, buffer_t *src_buffer);

#endif // GRAPHICS_BUFFERS_BUFFER_H

