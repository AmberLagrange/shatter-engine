#ifndef RENDERING_APIS_VULKAN_IMAGE_VIEW_IMAGE_VIEW_H
#define RENDERING_APIS_VULKAN_IMAGE_VIEW_IMAGE_VIEW_H

#include <common/core.h>

#include <rendering_apis/vulkan/renderer.h>

shatter_status_t create_image_views(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_image_views(vulkan_renderer_t *vk_renderer);

#endif //RENDERING_APIS_VULKAN_IMAGE_VIEW_IMAGE_VIEW_H

