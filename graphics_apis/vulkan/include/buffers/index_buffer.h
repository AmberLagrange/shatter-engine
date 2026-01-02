#ifndef BUFFERS_INDEX_BUFFER_H
#define BUFFERS_INDEX_BUFFER_H

#include <common/core.h>

#include <vulkan_renderer.h>

#include <buffers/buffer.h>

#include <stdint.h>

extern const uint32_t indices[6];

shatter_status_t create_index_buffer(vulkan_renderer_t *vk_renderer, buffer_t *index_buffer);
shatter_status_t cleanup_index_buffer(vulkan_renderer_t *vk_renderer, buffer_t *index_buffer);

#endif // BUFFERS_INDEX_BUFFER_H

