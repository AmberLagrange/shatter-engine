#ifndef QUEUES_PRESENT_QUEUE_H
#define QUEUES_PRESENT_QUEUE_H

#include <common/core.h>

#include <vulkan_renderer.h>

#include <stdint.h>

shatter_status_t submit_present_queue(vulkan_renderer_t *vk_renderer, uint32_t image_index);

#endif // QUEUES_PRESENT_QUEUE_H

