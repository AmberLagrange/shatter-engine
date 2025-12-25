#ifndef RENDERERS_VULKAN_IMAGE_VIEW_IMAGE_VIEW_H
#define RENDERERS_VULKAN_IMAGE_VIEW_IMAGE_VIEW_H

#include <common/core.h>

#include <renderers/vulkan/renderer.h>

shatter_status_t create_image_views(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_image_views(vulkan_renderer_t *vk_renderer);

#endif //RENDERERS_VULKAN_IMAGE_VIEW_IMAGE_VIEW_H

