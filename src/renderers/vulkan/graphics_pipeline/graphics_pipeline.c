#include <common/core.h>

#include <renderers/vulkan/graphics_pipeline/graphics_pipeline.h>

#include <renderers/vulkan/shaders/shader_module.h>
#include <renderers/vulkan/shaders/load_shader.h>

#include <stdlib.h>

shatter_status_t create_graphics_pipeline(vulkan_renderer_t *vk_renderer) {
	
	log_message(stdout, "\nCreating graphics pipeline.\n");
	shatter_status_t status = SHATTER_SUCCESS;
	
	size_t vertex_len;
	char *vertex_bytecode = get_shader_bytecode("spv/vertex/basic_vertex.spv", &vertex_len);
	
	VkShaderModule vertex_module = { 0 };
	if (create_shader_module(&vertex_module, vk_renderer, vertex_bytecode, vertex_len)) {
		
		log_message(stderr, "Failed to create the vertex shader module.\n");
		status = SHATTER_VULKAN_SHADER_MODULE_INIT_FAILURE;
		goto cleanup_vertex_bytecode;
	}
	
	size_t fragment_len;
	char *fragment_bytecode = get_shader_bytecode("spv/fragment/basic_fragment.spv", &fragment_len);
	
	VkShaderModule fragment_module = { 0 };
	if (create_shader_module(&fragment_module, vk_renderer, fragment_bytecode, fragment_len)) {
		
		log_message(stderr, "Failed to create the fragment shader module.\n");
		status = SHATTER_VULKAN_SHADER_MODULE_INIT_FAILURE;
		goto cleanup_vertex_module;
	}
	log_message(stdout, "Graphics pipeline created.\n");
	
	vkDestroyShaderModule(vk_renderer->logical_device, fragment_module, NULL);
cleanup_vertex_module:
	vkDestroyShaderModule(vk_renderer->logical_device, vertex_module, NULL);

	free(fragment_bytecode);
cleanup_vertex_bytecode:
	free(vertex_bytecode);
	
	return status;
}

