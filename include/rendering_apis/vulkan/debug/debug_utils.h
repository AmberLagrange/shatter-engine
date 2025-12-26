#ifndef RENDERING_APIS_VULKAN_DEBUG_DEBUG_UTILS_H
#define RENDERING_APIS_VULKAN_DEBUG_DEBUG_UTILS_H

#include <common/core.h>

#include <rendering_apis/vulkan/renderer.h>

#include <vulkan/vulkan.h>

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

shatter_status_t setup_vulkan_debug_messenger(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_vulkan_debug_messenger(vulkan_renderer_t *vk_renderer);

#endif // RENDERING_APIS_VULKAN_DEBUG_DEBUG_UTILS_H

