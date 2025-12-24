#ifndef RENDERERS_VULKAN_RENDERER_H
#define RENDERERS_VULKAN_RENDERER_H

#include <renderers/renderer_config.h>

#include <stdbool.h>
#include <stdint.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

enum {
	
	SHATTER_RENDERER_SUCCESS = 0,
	SHATTER_RENDERER_INIT_FAILURE,
	SHATTER_RENDERER_CLEANUP_FAILURE,
	SHATTER_RENDERER_LOOP_ERROR,
};

typedef struct {
	
	renderer_config_t config;
	
	GLFWwindow *rendering_window;
	
	VkInstance vulkan_instance;
	VkDebugUtilsMessengerEXT debug_messenger;
	
	VkSurfaceKHR rendering_surface;
	
	VkPhysicalDevice physical_device;
	VkDevice logical_device;
	
	VkQueue graphics_queue;
	
	char **validation_layers;
	size_t num_validation_layers;
	bool validation_layers_enabled;
} vulkan_renderer_t;

int vulkan_renderer_init(vulkan_renderer_t *vk_renderer, renderer_config_t *config);
int vulkan_renderer_loop(vulkan_renderer_t *vk_renderer);
int vulkan_renderer_cleanup(vulkan_renderer_t *vk_renderer);

int setup_vulkan_debug_messenger(vulkan_renderer_t *vk_renderer);
int cleanup_vulkan_debug_messenger(vulkan_renderer_t *vk_renderer);

int create_vulkan_instance(vulkan_renderer_t *vk_renderer);

// Renderer Helpers

size_t get_num_validation_layers(void);
int init_validation_layers(vulkan_renderer_t *vk_renderer);
int destroy_validation_layers(vulkan_renderer_t *vk_renderer);
bool check_validation_layer_support(vulkan_renderer_t *vk_renderer);

char **get_required_extensions(vulkan_renderer_t *vk_renderer, uint32_t *num_extensions);

#endif // RENDERERS_VULKAN_RENDERER_H

