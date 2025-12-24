#ifndef RENDERERS_VULKAN_DEVICES_EXTENSIONS_H
#define RENDERERS_VULKAN_DEVICES_EXTENSIONS_H

#include <renderers/vulkan/devices/physical.h>

#include <stdbool.h>

bool check_device_extension_support(VkPhysicalDevice device);

#endif // RENDERERS_VULKAN_DEVICES_EXTENSIONS_H

