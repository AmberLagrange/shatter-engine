#include <common/core.h>

#include <renderers/vulkan/callbacks.h>
#include <renderers/vulkan/debug_utils.h>
#include <renderers/vulkan/extensions.h>
#include <renderers/vulkan/instance.h>

#include <stdlib.h>
#include <string.h>

#include <vulkan/vulkan.h>

shatter_status_t create_vulkan_instance(vulkan_renderer_t *vk_renderer) {
	
	int status = SHATTER_SUCCESS;
	
	VkApplicationInfo application_info = {
		
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Vulkan Renderer",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "No Engine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_0,
		.pNext = NULL,
	};
	
	uint32_t num_extensions = 0;
	char **extension_names = get_required_extensions(vk_renderer, &num_extensions);
	
	VkExtensionProperties *extension_properties_list = malloc(sizeof(VkExtensionProperties) * num_extensions);
	vkEnumerateInstanceExtensionProperties(NULL, &num_extensions, extension_properties_list);
	
	log_message(stdout, "\nEnabled extensions:\n");
	for (size_t extension_index = 0; extension_index < num_extensions; ++extension_index) {
		
		log_message(stdout, "\t%s\n", extension_properties_list[extension_index].extensionName);
	}
	
	VkInstanceCreateInfo create_info = {
		
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		
		.pApplicationInfo = &application_info,
		.enabledExtensionCount = num_extensions,
		
		.ppEnabledExtensionNames = (const char **)extension_names,
		
		.enabledLayerCount = vk_renderer->num_validation_layers,
		.ppEnabledLayerNames = (const char * const *)(vk_renderer->validation_layers)
	};
	
	VkDebugUtilsMessengerCreateInfoEXT debug_create_info = { 0 };
	
	if (vk_renderer->validation_layers_enabled) {
		
		populate_debug_messenger_create_info(&debug_create_info);
		create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_create_info;
	} else {
		
		create_info.pNext = NULL;
	}
	
	if (vkCreateInstance(&create_info, NULL, &(vk_renderer->vulkan_instance)) != VK_SUCCESS) {
		
		log_message(stderr, "\nFailed to create Vulkan instance.\n");
		status = SHATTER_VULKAN_INSTANCE_FAILURE;
		goto cleanup;
	}
	
	log_message(stdout, "\nVulkan Instance Created.\n");	
cleanup:
	
	free(extension_properties_list);
	
	for (size_t extension_index = 0; extension_index < num_extensions; ++extension_index) {
		
		free(extension_names[extension_index]);
	}
	free(extension_names);
	
	return status;
}

