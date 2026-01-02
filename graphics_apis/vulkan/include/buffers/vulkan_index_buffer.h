#ifndef BUFFERS_VULKAN_INDEX_BUFFER_H
#define BUFFERS_VULKAN_INDEX_BUFFER_H

#include <common/core.h>

#include <vulkan_renderer.h>

#include <buffers/buffer_info.h>
#include <buffers/vulkan_buffer.h>

#include <stdint.h>

shatter_status_t create_vulkan_index_buffer(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_vulkan_index_buffer(vulkan_renderer_t *vk_renderer);

#endif // BUFFERS_VULKAN_INDEX_BUFFER_H

