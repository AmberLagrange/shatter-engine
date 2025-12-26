#ifndef RENDERING_APIS_VULKAN_FRAME_BUFFER_FRAME_BUFFER_H
#define RENDERING_APIS_VULKAN_FRAME_BUFFER_FRAME_BUFFER_H

#include <common/core.h>

#include <rendering_apis/vulkan/renderer.h>

shatter_status_t create_frame_buffers(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_frame_buffers(vulkan_renderer_t *vk_renderer);

#endif // RENDERING_APIS_VULKAN_FRAME_BUFFER_FRAME_BUFFER_H

