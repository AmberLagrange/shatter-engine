#ifndef RENDERERS_VULKAN_DEVICES_LOGICAL_H
#define RENDERERS_VULKAN_DEVICES_LOGICAL_H

#include <renderers/vulkan/renderer.h>

enum {
	
	SHATTER_LOGICAL_DEVICE_SUCCESS = 0,
	SHATTER_LOGICAL_DEVICE_INIT_FAILURE,
};

int create_logical_device(vulkan_renderer_t *vk_renderer);

#endif // RENDERERS_VULKAN_DEVICES_LOGICAL_H

