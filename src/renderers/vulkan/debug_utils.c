#include <common/core.h>

#include <renderers/vulkan/callbacks.h>
#include <renderers/vulkan/debug_utils.h>

#include <string.h>

#include <vulkan/vulkan.h>

VkResult create_debug_utils_messenger_ext(
	
	VkInstance *instance,
	const VkDebugUtilsMessengerCreateInfoEXT *create_info,
	const VkAllocationCallbacks *allocator,
	VkDebugUtilsMessengerEXT *debug_messenger) {
	
	PFN_vkCreateDebugUtilsMessengerEXT function_callback =
	(PFN_vkCreateDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(*instance, "vkCreateDebugUtilsMessengerEXT"));
	
	if (function_callback == NULL) {
		
		log_error("Extension not present.\n");
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
		
	return function_callback(*instance, create_info, allocator, debug_messenger);
}

void destroy_debug_utils_messenger_ext(
	
	VkInstance *instance,
	VkDebugUtilsMessengerEXT debug_messenger,
	const VkAllocationCallbacks *allocator) {
	
	PFN_vkDestroyDebugUtilsMessengerEXT function_callback =
	(PFN_vkDestroyDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(*instance, "vkDestroyDebugUtilsMessengerEXT"));

	if (function_callback == NULL) {
		
		return;
	}
	
	function_callback(*instance, debug_messenger, allocator);
}

void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT *create_info) {
	
	memset(create_info, 0, sizeof(VkDebugUtilsMessengerCreateInfoEXT));
	
	create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	create_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	
	create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	
	create_info->pfnUserCallback = debug_callback;
}

shatter_status_t setup_vulkan_debug_messenger(vulkan_renderer_t *vk_renderer) {
	
	if (!(vk_renderer->validation_layers_enabled)) {
		
		return SHATTER_SUCCESS;
	}
	
	VkDebugUtilsMessengerCreateInfoEXT create_info;
	populate_debug_messenger_create_info(&create_info);
	
	if (create_debug_utils_messenger_ext(&(vk_renderer->vulkan_instance), &create_info, NULL, &(vk_renderer->debug_messenger))
		!= VK_SUCCESS) {
		
		log_error("Could not create the debug messenger.\n");
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_vulkan_debug_messenger(vulkan_renderer_t *vk_renderer) {
	
	if (vk_renderer->validation_layers_enabled) {
		
		destroy_debug_utils_messenger_ext(&(vk_renderer->vulkan_instance), vk_renderer->debug_messenger, NULL);
	}
	
	return SHATTER_SUCCESS;
}

