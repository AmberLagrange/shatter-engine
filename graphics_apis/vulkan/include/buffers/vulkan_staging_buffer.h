#ifndef BUFFERS_VULKAN_STAGING_BUFFER_H
#define BUFFERS_VULKAN_STAGING_BUFFER_H

#include <common/core.h>

#include <vulkan_renderer.h>

#include <buffers/buffer_info.h>
#include <buffers/vulkan_buffer.h>

shatter_status_t create_vulkan_buffer_with_staging(vulkan_renderer_t *vk_renderer,
												   vulkan_buffer_t *buffer, buffer_info_t *buffer_info);

#endif // BUFFERS_VULKAN_STAGING_BUFFER_H

