#ifndef RENDERERS_VULKAN_CALLBACKS_H
#define RENDERERS_VULKAN_CALLBACKS_H

#include <vulkan/vulkan.h>

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
	
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
	void *user_data);

#endif // RENDERER_VULKAN_CALLBACKS_H

