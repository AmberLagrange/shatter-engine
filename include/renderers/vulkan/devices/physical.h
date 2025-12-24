#ifndef RENDERERS_VULKAN_DEVICES_PHYSICAL_H
#define RENDERERS_VULKAN_DEVICES_PHYSICAL_H

#include <renderers/vulkan/renderer.h>

#include <stdbool.h>

enum {
	
	SHATTER_PHYSICAL_DEVICE_SUCCESS = 0,
	SHATTER_PHYSICAL_DEVICE_SETUP_FAILURE,
};

int setup_physical_device(vulkan_renderer_t *vk_renderer);
bool is_physical_device_suitable(vulkan_renderer_t *vk_renderer, VkPhysicalDevice device);

#endif // RENDERERS_VULKAN_DEVICES_PHYSICAL_H

