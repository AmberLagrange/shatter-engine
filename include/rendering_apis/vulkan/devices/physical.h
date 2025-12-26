#ifndef RENDERING_APIS_VULKAN_DEVICES_PHYSICAL_H
#define RENDERING_APIS_VULKAN_DEVICES_PHYSICAL_H

#include <common/core.h>

#include <rendering_apis/vulkan/renderer.h>

#include <stdbool.h>

shatter_status_t choose_physical_device(vulkan_renderer_t *vk_renderer);
bool is_physical_device_suitable(vulkan_renderer_t *vk_renderer, VkPhysicalDevice device);

#endif // RENDERING_APIS_VULKAN_DEVICES_PHYSICAL_H

