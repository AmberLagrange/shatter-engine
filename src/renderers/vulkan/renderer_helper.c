#include <common/core.h>

#include <renderers/vulkan/callbacks.h>
#include <renderers/vulkan/renderer_helper.h>

#include <stdlib.h>
#include <string.h>

#include <vulkan/vulkan.h>

// ---------- static const values ---------- //

#define MAX_VALIDATION_LAYERS 8

static const char *VALIDATION_LAYERS[MAX_VALIDATION_LAYERS] = {
	"VK_LAYER_KHRONOS_validation",
}; // Remaining will be initialized to NULL

// ---------- Helper Functions ---------- //

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
	log_message(stdout, "Number of extensions: %d\n", num_extensions);
	
	VkExtensionProperties *extension_properties_list = malloc(sizeof(VkExtensionProperties) * num_extensions);
	vkEnumerateInstanceExtensionProperties(NULL, &num_extensions, extension_properties_list);
	
	log_message(stdout, "Available extensions:\n");
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
		
		log_message(stderr, "Failed to create Vulkan instance.\n");
		status = SHATTER_VULKAN_INSTANCE_FAILURE;
		goto cleanup;
	}
	
	log_message(stdout, "Vulkan Instance Created.\n");	
cleanup:
	
	free(extension_properties_list);
	
	for (size_t extension_index = 0; extension_index < num_extensions; ++extension_index) {
		
		free(extension_names[extension_index]);
	}
	free(extension_names);
	
	return status;
}

// ---------- Validation Layers ---------- //

size_t get_num_validation_layers(void) {
	
	size_t validation_index = 0;
	
	log_message(stdout, "Validation layers:\n");
	while (VALIDATION_LAYERS[validation_index]) {
		
		log_message(stdout, "\t%s\n", VALIDATION_LAYERS[validation_index]);
		++validation_index;
	}
	
	return validation_index;
}

shatter_status_t init_validation_layers(vulkan_renderer_t *vk_renderer) {
	
#ifdef NDEBUG
	vk_renderer->num_validation_layers = 0;
	vk_renderer->validation_layers_enabled = false;
#else
	size_t num_validation_layers = get_num_validation_layers();
	vk_renderer->validation_layers = malloc(sizeof(char *) * num_validation_layers);

	for (size_t validation_index = 0; validation_index < num_validation_layers; ++validation_index) {
		
		const char *validation_layer = VALIDATION_LAYERS[validation_index];
		size_t validation_layer_len = strlen(validation_layer) + 1; // Include NULL terminator
		
		vk_renderer->validation_layers[validation_index] = malloc(sizeof(char) * validation_layer_len);
		strncpy(vk_renderer->validation_layers[validation_index], validation_layer, validation_layer_len);
	}

	vk_renderer->num_validation_layers = num_validation_layers;
	vk_renderer->validation_layers_enabled = true;
	
	if (!check_validation_layer_support(vk_renderer)) {
		
		return SHATTER_VULKAN_LAYER_FAILURE;
	}
#endif
	return SHATTER_SUCCESS;
}

shatter_status_t destroy_validation_layers(vulkan_renderer_t *vk_renderer) {
	
	if (!vk_renderer || vk_renderer->validation_layers) {
		
		return SHATTER_VULKAN_LAYER_FAILURE;
	}
	
	for (size_t validation_index = 0; validation_index < vk_renderer->num_validation_layers; ++validation_index) {
		
		free(vk_renderer->validation_layers[validation_index]);
	}
	free(vk_renderer->validation_layers);
	
	return SHATTER_SUCCESS;
}

bool check_validation_layer_support(vulkan_renderer_t *vk_renderer) {
	
	uint32_t num_available_layers;
	vkEnumerateInstanceLayerProperties(&num_available_layers, NULL);
	
	VkLayerProperties *available_layers = malloc(sizeof(VkLayerProperties) * num_available_layers);
	vkEnumerateInstanceLayerProperties(&num_available_layers, available_layers);
	
	for (size_t renderer_layer_index = 0; renderer_layer_index < vk_renderer->num_validation_layers; ++renderer_layer_index) {
		
		const char *renderer_layer = vk_renderer->validation_layers[renderer_layer_index];
		bool layer_found = false;
		
		for (size_t available_layer_index = 0; available_layer_index < num_available_layers; ++available_layer_index) {
			
			const char *available_layer = available_layers[available_layer_index].layerName;
			
			if (strcmp(renderer_layer, available_layer) == 0) {
				
				layer_found = true;
				break;
			}
		}
		
		if (!layer_found) { // If any of the layers are missing, return false
			
			log_message(stderr, "Validation layer %s not found.\n", renderer_layer);
			return false;
		}
	}
	
	log_message(stdout, "All validation layers found.\n");
	free(available_layers);
	return true; // All layers are available
}

// ---------- Extensions ---------- //

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

// ---------- Debug Utils ---------- //

VkResult create_debug_utils_messenger_ext(
	
	VkInstance *instance,
	const VkDebugUtilsMessengerCreateInfoEXT *create_info,
	const VkAllocationCallbacks *allocator,
	VkDebugUtilsMessengerEXT *debug_messenger) {
	
	PFN_vkCreateDebugUtilsMessengerEXT function_callback =
	(PFN_vkCreateDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(*instance, "vkCreateDebugUtilsMessengerEXT"));
	
	if (function_callback == NULL) {
		
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
		
		log_message(stderr, "Could not create the debug messenger.\n");
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

