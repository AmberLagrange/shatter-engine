#include <common/core.h>

#include <renderers/vulkan/validation_layers.h>

#include <stdlib.h>
#include <string.h>

#include <vulkan/vulkan.h>

// ---------- static const values ---------- //

#define MAX_VALIDATION_LAYERS 8

static const char *VALIDATION_LAYERS[MAX_VALIDATION_LAYERS] = {
	"VK_LAYER_KHRONOS_validation",
}; // Remaining will be initialized to NULL

// ---------- Validation Layer Functions ---------- //

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

