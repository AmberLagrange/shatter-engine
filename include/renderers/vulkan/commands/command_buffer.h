#ifndef RENDERERS_VULKAN_COMMANDS_COMMAND_BUFFER_H
#define RENDERERS_VULKAN_COMMANDS_COMMAND_BUFFER_H

#include <common/core.h>

#include <renderers/vulkan/renderer.h>

#include <vulkan/vulkan.h>

#include <stdint.h>

shatter_status_t create_command_buffer(vulkan_renderer_t *vk_renderer);
shatter_status_t record_command_buffer(vulkan_renderer_t *vk_renderer, VkCommandBuffer command_buffer, uint32_t image_index);

#endif // RENDERERS_VULKAN_COMMANDS_COMMAND_BUFFER_H

