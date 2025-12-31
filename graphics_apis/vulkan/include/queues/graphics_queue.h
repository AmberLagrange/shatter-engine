#ifndef QUEUES_GRAPHICS_QUEUES_H
#define QUEUES_GRAPHICS_QUEUES_H

#include <common/core.h>

#include <vulkan_renderer.h>

#include <stdint.h>

shatter_status_t submit_graphics_queue(vulkan_renderer_t *vk_renderer, uint32_t image_index);

#endif // QUEUES_GRAPHICS_QUEUES_H

