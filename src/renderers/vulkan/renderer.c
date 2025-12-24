#include <renderers/glfw.h>
#include <renderers/renderer_config.h>
#include <renderers/vulkan/callbacks.h>
#include <renderers/vulkan/renderer.h>
#include <renderers/vulkan/vulkan.h>
#include <renderers/vulkan/devices/logical.h>
#include <renderers/vulkan/devices/physical.h>
#include <renderers/vulkan/surfaces/surface.h>

#include <logging/logger.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VALIDATION_LAYERS 8

static const char *VALIDATION_LAYERS[MAX_VALIDATION_LAYERS] = {
	"VK_LAYER_KHRONOS_validation",
}; // Remaining will be initialized to NULL

int vulkan_renderer_init(vulkan_renderer_t *vk_renderer, renderer_config_t *config) {
	
	if (init_glfw()) {
		
		log_message(stderr, "Failed to initialize GLFW.\n");
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	memcpy(&(vk_renderer->config), config, sizeof(renderer_config_t));
	vk_renderer->rendering_window =
		glfwCreateWindow(vk_renderer->config.width, vk_renderer->config.height, vk_renderer->config.title, NULL, NULL);
	
	if (!vk_renderer->rendering_window) {
		
		log_message(stderr, "Failed to create GLFW Window.\n");
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	vk_renderer->num_validation_layers = 0;
	init_validation_layers(vk_renderer);
	
	if (create_vulkan_instance(vk_renderer)) {
		
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	if (create_surface(vk_renderer)) {
		
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	if (setup_vulkan_debug_messenger(vk_renderer)) {
		
		log_message(stderr, "Failed to set up the debug messenger.\n");
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	vk_renderer->physical_device = VK_NULL_HANDLE;
	if (setup_physical_device(vk_renderer)) {
		
		log_message(stderr, "Failed to set up a physical device.\n");
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	if (create_logical_device(vk_renderer)) {
		
		log_message(stderr, "Failed to create a logical device.\n");
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	log_message(stdout, "Renderer Initialization Complete.\n");
	return SHATTER_RENDERER_SUCCESS;
}

int vulkan_renderer_loop(vulkan_renderer_t *vk_renderer) {
	
	(void)vk_renderer;
	log_message(stdout, "Running Renderer Loop.\n");
	return SHATTER_RENDERER_SUCCESS;
}

int vulkan_renderer_cleanup(vulkan_renderer_t *vk_renderer) {
	
	vkDestroyDevice(vk_renderer->logical_device, NULL);
	
	cleanup_vulkan_debug_messenger(vk_renderer);
	
	vkDestroySurfaceKHR(vk_renderer->vulkan_instance, vk_renderer->rendering_surface, NULL);
	
	vkDestroyInstance(vk_renderer->vulkan_instance, NULL);
	
	glfwDestroyWindow(vk_renderer->rendering_window);
	
	if (terminate_glfw()) {
		
		log_message(stderr, "Failed to terminate GLFW.\n");
		return SHATTER_RENDERER_CLEANUP_FAILURE;
	}
	
	log_message(stdout, "Renderer Cleanup Complete.\n");
	return SHATTER_RENDERER_SUCCESS;
}

int setup_vulkan_debug_messenger(vulkan_renderer_t *vk_renderer) {
	
	if (!(vk_renderer->validation_layers_enabled)) {
		
		return SHATTER_RENDERER_SUCCESS;
	}
	
	VkDebugUtilsMessengerCreateInfoEXT create_info;
	populate_debug_messenger_create_info(&create_info);
	
	if (create_debug_utils_messenger_ext(&(vk_renderer->vulkan_instance), &create_info, NULL, &(vk_renderer->debug_messenger))
		!= VK_SUCCESS) {
		
		log_message(stderr, "Could not create the debug messenger.\n");
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	return SHATTER_RENDERER_SUCCESS;
}

int cleanup_vulkan_debug_messenger(vulkan_renderer_t *vk_renderer) {
	
	if (vk_renderer->validation_layers_enabled) {
		
		destroy_debug_utils_messenger_ext(&(vk_renderer->vulkan_instance), vk_renderer->debug_messenger, NULL);
	}
	
	return SHATTER_RENDERER_SUCCESS;
}

int create_vulkan_instance(vulkan_renderer_t *vk_renderer) {
	
	int status = SHATTER_VULKAN_SUCCESS;
	
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

// Renderer Helpers

size_t get_num_validation_layers(void) {
	
	size_t validation_index = 0;
	
	log_message(stdout, "Validation layers:\n");
	while (VALIDATION_LAYERS[validation_index]) {
		
		log_message(stdout, "\t%s\n", VALIDATION_LAYERS[validation_index]);
		++validation_index;
	}
	
	return validation_index;
}

int init_validation_layers(vulkan_renderer_t *vk_renderer) {
	
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
	return SHATTER_VULKAN_SUCCESS;
}

int destroy_validation_layers(vulkan_renderer_t *vk_renderer) {
	
	if (!vk_renderer || vk_renderer->validation_layers) {
		
		return SHATTER_VULKAN_LAYER_FAILURE;
	}
	
	for (size_t validation_index = 0; validation_index < vk_renderer->num_validation_layers; ++validation_index) {
		
		free(vk_renderer->validation_layers[validation_index]);
	}
	free(vk_renderer->validation_layers);
	
	return SHATTER_VULKAN_SUCCESS;
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

