#include <common/core.h>

#include <renderers/glfw.h>
#include <renderers/renderer_config.h>

#include <renderers/vulkan/debug_utils.h>
#include <renderers/vulkan/instance.h>
#include <renderers/vulkan/renderer.h>
#include <renderers/vulkan/validation_layers.h>

#include <renderers/vulkan/devices/logical.h>
#include <renderers/vulkan/devices/physical.h>

#include <renderers/vulkan/surfaces/surface.h>

#include <string.h>

shatter_status_t vulkan_renderer_init(vulkan_renderer_t *vk_renderer, renderer_config_t *config) {
	
	if (init_glfw()) {
		
		log_message(stderr, "\nFailed to initialize GLFW.\n");
		return SHATTER_GLFW_INIT_FAILURE;
	}
	
	memcpy(&(vk_renderer->config), config, sizeof(renderer_config_t));
	vk_renderer->rendering_window =
		glfwCreateWindow(vk_renderer->config.width, vk_renderer->config.height, vk_renderer->config.title, NULL, NULL);
	
	if (!vk_renderer->rendering_window) {
		
		log_message(stderr, "\nFailed to create GLFW Window.\n");
		return SHATTER_GLFW_WINDOW_FAILURE;
	}
	
	vk_renderer->num_validation_layers = 0;
	init_validation_layers(vk_renderer);
	
	if (create_vulkan_instance(vk_renderer)) {
		
		return SHATTER_VULKAN_INSTANCE_FAILURE;
	}
	
	if (create_surface(vk_renderer)) {
		
		return SHATTER_SURFACE_INIT_FAILURE;
	}
	
	if (setup_vulkan_debug_messenger(vk_renderer)) {
		
		log_message(stderr, "\nFailed to set up the debug messenger.\n");
		return SHATTER_VULKAN_DEBUT_SETUP_FAILURE;
	}
	
	vk_renderer->physical_device = VK_NULL_HANDLE;
	if (choose_physical_device(vk_renderer)) {
		
		log_message(stderr, "\nFailed to set up a physical device.\n");
		return SHATTER_VULKAN_PHYSICAL_DEVICE_CHOICE_FAILURE;
	}
	
	if (create_logical_device(vk_renderer)) {
		
		log_message(stderr, "\nFailed to create a logical device.\n");
		return SHATTER_VULKAN_LOGICAL_DEVICE_INIT_FAILURE;
	}
	
	log_message(stdout, "\nRenderer Initialization Complete.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t vulkan_renderer_loop(vulkan_renderer_t *vk_renderer) {
	
	(void)vk_renderer;
	log_message(stdout, "\nRunning Renderer Loop.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t vulkan_renderer_cleanup(vulkan_renderer_t *vk_renderer) {
	
	vkDestroyDevice(vk_renderer->logical_device, NULL);
	
	cleanup_vulkan_debug_messenger(vk_renderer);
	
	vkDestroySurfaceKHR(vk_renderer->vulkan_instance, vk_renderer->rendering_surface, NULL);
	
	vkDestroyInstance(vk_renderer->vulkan_instance, NULL);
	
	glfwDestroyWindow(vk_renderer->rendering_window);
	
	if (terminate_glfw()) {
		
		log_message(stderr, "\nFailed to terminate GLFW.\n");
		return SHATTER_RENDERER_CLEANUP_FAILURE;
	}
	
	log_message(stdout, "\nRenderer Cleanup Complete.\n");
	return SHATTER_SUCCESS;
}

