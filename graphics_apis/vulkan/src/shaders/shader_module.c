#include <common/core.h>

#include <shaders/shader_module.h>

shatter_status_t create_shader_module(VkShaderModule *shader_module, vulkan_renderer_t *vk_renderer,
									  const char *bytecode, size_t code_size) {
	
	VkShaderModuleCreateInfo create_info = {
		
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code_size,
		.pCode = (uint32_t *)(bytecode)
	};
	
	if (vkCreateShaderModule(vk_renderer->logical_device, &create_info, NULL, shader_module)) {
		
		log_error("Failed to create shader module.\n");
		return SHATTER_VULKAN_SHADER_MODULE_INIT_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

