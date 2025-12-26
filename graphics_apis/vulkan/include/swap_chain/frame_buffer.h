#ifndef FRAME_BUFFER_FRAME_BUFFER_H
#define FRAME_BUFFER_FRAME_BUFFER_H

#include <common/core.h>

#include <vulkan_renderer.h>

shatter_status_t create_frame_buffers(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_frame_buffers(vulkan_renderer_t *vk_renderer);

#endif // FRAME_BUFFER_FRAME_BUFFER_H

