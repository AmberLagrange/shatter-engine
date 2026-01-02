#ifndef GRAPHICS_BUFFERS_STAGING_BUFFER_H
#define GRAPHICS_BUFFERS_STAGING_BUFFER_H

#include <common/core.h>

#include <vulkan_renderer.h>

#include <buffers/buffer.h>

#include <vulkan/vulkan.h>

shatter_status_t create_buffer_with_staging(vulkan_renderer_t *vk_renderer, buffer_t *buffer);

#endif // GRAPHICS_BUFFERS_STAGING_BUFFER_H

