#ifndef COMMANDS_COMMAND_POOL_H
#define COMMANDS_COMMAND_POOL_H

#include <common/core.h>

#include <vulkan_renderer.h>

#include <queues/queue_family_indices.h>

#include <vulkan/vulkan.h>

#include <stddef.h>
#include <stdint.h>

shatter_status_t create_command_pool(vulkan_renderer_t *vk_renderer, VkCommandPool *command_pool,
									 queue_family_index_t queue_family_index);

shatter_status_t cleanup_command_pool(vulkan_renderer_t *vk_renderer, VkCommandPool *command_pool);

#endif // COMMANDS_COMMAND_POOL_H

