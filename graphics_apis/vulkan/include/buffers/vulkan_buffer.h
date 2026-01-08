#ifndef BUFFERS_VULKAN_BUFFER_H
#define BUFFERS_VULKAN_BUFFER_H

#include <buffers/buffer_info.h>

#include <vulkan/vulkan.h>

typedef struct vulkan_buffer_s {
	
	VkBuffer vulkan_buffer;
	VkDeviceMemory memory;
	
	VkMemoryRequirements memory_requirements;
	VkDeviceSize size;
	VkBufferUsageFlags usage;
	VkMemoryPropertyFlags properties;
} vulkan_buffer_t;

typedef struct vulkan_renderer_s vulkan_renderer_t;

shatter_status_t create_vulkan_buffer(vulkan_renderer_t *vk_renderer, vulkan_buffer_t *buffer, buffer_info_t *buffer_info);
shatter_status_t cleanup_vulkan_buffer(vulkan_renderer_t *vk_renderer, vulkan_buffer_t *buffer);

shatter_status_t allocate_buffer_memory(vulkan_renderer_t *vk_renderer, vulkan_buffer_t *buffer);

shatter_status_t map_buffer_memory(vulkan_renderer_t *vk_renderer, vulkan_buffer_t *buffer,
								   VkDeviceSize size, void **memory_map);
shatter_status_t unmap_buffer_memory(vulkan_renderer_t *vk_renderer, vulkan_buffer_t *buffer);

shatter_status_t memcpy_buffer_to_device(vulkan_renderer_t *vk_renderer, vulkan_buffer_t *buffer, buffer_info_t *buffer_info);
shatter_status_t memcpy_buffer_to_buffer(vulkan_renderer_t *vk_renderer, vulkan_buffer_t *dst_buffer,
																		 vulkan_buffer_t *src_buffer);

#endif // BUFFERS_VULKAN_BUFFER_H

