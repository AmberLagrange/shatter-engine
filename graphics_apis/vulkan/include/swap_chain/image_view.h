#ifndef IMAGE_VIEW_IMAGE_VIEW_H
#define IMAGE_VIEW_IMAGE_VIEW_H

#include <common/core.h>

#include <vulkan_renderer.h>

shatter_status_t create_image_views(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_image_views(vulkan_renderer_t *vk_renderer);

#endif // IMAGE_VIEW_IMAGE_VIEW_H

