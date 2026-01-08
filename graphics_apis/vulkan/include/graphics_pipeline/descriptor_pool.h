#ifndef GRAPHICS_PIPELINE_DESCRIPTOR_POOL_H
#define GRAPHICS_PIPELINE_DESCRIPTOR_POOL_H

#include <common/core.h>

#include <vulkan_renderer.h>

shatter_status_t create_descriptor_pool(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_descriptor_pool(vulkan_renderer_t *vk_renderer);

#endif // GRAPHICS_PIPELINE_DESCRIPTOR_POOL_H

