#include <common/core.h>

#include <debug/callbacks.h>

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
	
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
	void *user_data) {
	
	(void)(message_type);
	(void)(user_data);

	if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		
		log_error("Validation layer: %s\n", callback_data->pMessage);
	}
	
	return VK_FALSE;
}

