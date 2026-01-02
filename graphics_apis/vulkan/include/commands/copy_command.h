#ifndef COMMANDS_COPY_COMMAND_H
#define COMMANDS_COPY_COMMAND_H

#include <common/core.h>

#include <buffers/vulkan_buffer.h>

#include <vulkan/vulkan.h>

typedef struct copy_command_s {
	
	VkCommandPool *command_pool;
	VkCommandBuffer command_buffer;
} copy_command_t;

typedef struct vulkan_renderer_s vulkan_renderer_t;

shatter_status_t create_copy_command(vulkan_renderer_t *vk_renderer, copy_command_t *copy_command,
									 VkCommandPool *command_pool);

shatter_status_t cleanup_copy_command(vulkan_renderer_t *vk_renderer, copy_command_t *copy_command);

shatter_status_t record_copy_command(vulkan_renderer_t *vk_renderer, copy_command_t *copy_command,
									 vulkan_buffer_t *dst_buffer, vulkan_buffer_t *src_buffer);

#endif // COMMANDS_COPY_COMMAND_H

