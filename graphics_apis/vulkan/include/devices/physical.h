#ifndef DEVICES_PHYSICAL_H
#define DEVICES_PHYSICAL_H

#include <common/core.h>

#include <vulkan_renderer.h>

#include <stdbool.h>
#include <stdint.h>

shatter_status_t choose_physical_device(vulkan_renderer_t *vk_renderer);

bool is_physical_device_suitable(vulkan_renderer_t *vk_renderer, VkPhysicalDevice device);

shatter_status_t get_physical_device_memory_type(vulkan_renderer_t *vk_renderer, uint32_t memory_type_filter,
												 VkMemoryPropertyFlags properties, uint32_t *memory_type);

#endif // DEVICES_PHYSICAL_H

