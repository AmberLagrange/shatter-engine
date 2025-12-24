#ifndef RENDERERS_VULKAN_RENDERER_HELPER_H
#define RENDERERS_VULKAN_RENDERER_HELPER_H

#include <common/core.h>

#include <renderers/vulkan/renderer.h>

#include <stddef.h> // size_t

#include <vulkan/vulkan.h>

shatter_status_t create_vulkan_instance(vulkan_renderer_t *vk_renderer);

// ---------- Vulkan Validation Layers ---------- //

size_t get_num_validation_layers(void);

shatter_status_t init_validation_layers(vulkan_renderer_t *vk_renderer);
shatter_status_t destroy_validation_layers(vulkan_renderer_t *vk_renderer);

bool check_validation_layer_support(vulkan_renderer_t *vk_renderer);

char **get_required_extensions(vulkan_renderer_t *vk_renderer, uint32_t *num_extensions);

// ---------- Vulkan Debug Utils ---------- //

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

#endif // RENDERERS_VULKAN_RENDERER_HELPER_H

