#ifndef RENDERERS_VULKAN_VULKAN_H
#define RENDERERS_VULKAN_VULKAN_H

#include <renderers/vulkan/vulkan.h>

enum {
	
	SHATTER_VULKAN_SUCCESS = 0,
	SHATTER_VULKAN_INSTANCE_FAILURE,
	SHATTER_VULKAN_LAYER_FAILURE,
};

// Vulkan Helpers

VkResult create_debug_utils_messenger_ext(
	
	VkInstance *instance,
	const VkDebugUtilsMessengerCreateInfoEXT *create_info,
	const VkAllocationCallbacks *allocator,
	VkDebugUtilsMessengerEXT *debug_messenger);

void destroy_debug_utils_messenger_ext(
	
	VkInstance *instance,
	VkDebugUtilsMessengerEXT debug_messenger,
	const VkAllocationCallbacks *allocator);

void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT *create_info);

#endif // RENDERER_VULKAN_VULKAN_H

