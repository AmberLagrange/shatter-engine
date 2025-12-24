#ifndef RENDERERS_VULKAN_DEVICES_EXTENSIONS_H
#define RENDERERS_VULKAN_DEVICES_EXTENSIONS_H

#include <stdbool.h>

#include <vulkan/vulkan.h>

#define MAX_REQUIRED_EXTENSIONS 8
extern const char *REQUIRED_EXTENSIONS[MAX_REQUIRED_EXTENSIONS];

size_t get_num_required_extensions(void);
bool check_device_extension_support(VkPhysicalDevice device);

#endif // RENDERERS_VULKAN_DEVICES_EXTENSIONS_H

