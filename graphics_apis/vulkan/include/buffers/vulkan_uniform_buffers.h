#ifndef BUFFERS_VULKAN_UNIFORM_BUFFERS_H
#define BUFFERS_VULKAN_UNIFORM_BUFFERS_H

#include <common/core.h>

#include <vulkan_renderer.h>

shatter_status_t create_vulkan_uniform_buffers(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_vulkan_uniform_buffers(vulkan_renderer_t *vk_renderer);

#endif // BUFFERS_VULKAN_UNIFORM_BUFFERS_H

