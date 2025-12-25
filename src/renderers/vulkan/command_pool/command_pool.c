#include <common/core.h>

#include <renderers/vulkan/command_pool/command_pool.h>

#include <vulkan/vulkan.h>

shatter_status_t create_command_pool(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	log_trace("Creating command pool.\n");
	
	UNUSED(vk_renderer);
	
	log_info("Created command pool.\n");
	return SHATTER_SUCCESS;
}

