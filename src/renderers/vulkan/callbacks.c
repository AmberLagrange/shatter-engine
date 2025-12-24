#include <common/core.h>

#include <renderers/vulkan/callbacks.h>

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
	
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
	void *user_data) {
	
	(void)(message_type);
	(void)(user_data);

	if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		
		log_message(stderr, "Validation layer: %s\n", callback_data->pMessage);
	}
	
	return VK_FALSE;
}

