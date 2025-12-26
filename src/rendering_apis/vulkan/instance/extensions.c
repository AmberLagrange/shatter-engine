#include <common/core.h>

#include <rendering_apis/vulkan/instance/extensions.h>

#include <stdlib.h>
#include <string.h>

#include <vulkan/vulkan.h>

char **get_required_extensions(vulkan_renderer_t *vk_renderer, uint32_t *num_extensions) {
	
	uint32_t num_glfw_extensions = 0;
	const char **glfw_extension_names = glfwGetRequiredInstanceExtensions(&num_glfw_extensions);
	*num_extensions = num_glfw_extensions;
	
	if (vk_renderer->validation_layers_enabled) {
		
		(*num_extensions)++;
	}
	
	char **extension_names = malloc(sizeof(char *) * (*num_extensions));
	for (size_t extension_index = 0; extension_index < num_glfw_extensions; ++extension_index) {
		
		const char *extension_name = glfw_extension_names[extension_index];
		size_t extension_name_len = strlen(extension_name) + 1; // Include NULL terminator
		
		extension_names[extension_index] = malloc(sizeof(char) * extension_name_len);
		strncpy(extension_names[extension_index], extension_name, extension_name_len);
	}
	
	if (vk_renderer->validation_layers_enabled) {
		
		const char *extension_name = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
		size_t extension_name_len = strlen(extension_name) + 1; // Include NULL terminator
		
		extension_names[num_glfw_extensions] = malloc(sizeof(char) * extension_name_len);
		strncpy(extension_names[num_glfw_extensions], extension_name, extension_name_len);
	}
	
	return extension_names;
}

