#ifndef COMMANDS_IMAGE_COMMANDS_H
#define COMMANDS_IMAGE_COMMANDS_H

#include <common/core.h>

#include <vulkan/vulkan.h>

#include <stddef.h>
#include <stdint.h>

typedef struct image_commands_s {
	
	VkCommandPool *command_pool;
	
	VkCommandBuffer *command_buffer_list;
	size_t num_command_buffers;
} image_commands_t;

typedef struct vulkan_renderer_s vulkan_renderer_t;

shatter_status_t create_image_commands(vulkan_renderer_t *vk_renderer, image_commands_t *image_commands,
									   VkCommandPool *command_pool, size_t num_command_buffers);

shatter_status_t destroy_image_commands(vulkan_renderer_t *vk_renderer, image_commands_t *image_commands);

shatter_status_t record_image_command(vulkan_renderer_t *vk_renderer, image_commands_t *image_commands,
									  uint32_t command_index, uint32_t image_index);

#endif // COMMANDS_IMAGE_COMMANDS_H

