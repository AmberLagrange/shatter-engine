#ifndef RENDERING_APIS_VULKAN_RENDERER_H
#define RENDERING_APIS_VULKAN_RENDERER_H

#include <common/core.h>

#include <renderer/renderer_config.h>

#include <rendering_apis/vulkan/queues/queue_family_indicies.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <stddef.h>

typedef struct vulkan_renderer_s {
	
	renderer_config_t config;
	
	GLFWwindow *rendering_window;
	
	VkInstance vulkan_instance;
	VkDebugUtilsMessengerEXT debug_messenger;
	
	VkSurfaceKHR rendering_surface;
	
	queue_family_indicies_t queue_family_indicies;
	
	VkPhysicalDevice physical_device;
	VkDevice logical_device;
	
	VkQueue graphics_queue;
	VkQueue present_queue;
	
	VkSwapchainKHR swap_chain;
	VkImage *swap_chain_image_list;
	size_t num_swap_chain_images;
	VkFormat swap_chain_image_format;
	VkExtent2D swap_chain_extent;
	
	VkImageView *swap_chain_image_view_list;
	size_t num_swap_chain_image_views;
	
	VkRenderPass render_pass;
	VkPipelineLayout pipeline_layout;
	VkPipeline graphics_pipeline;
	
	VkFramebuffer *swap_chain_frame_buffer_list;
	size_t num_swap_chain_frame_buffers;
	
	VkCommandPool command_pool;
	VkCommandBuffer command_buffer;
	
	VkSemaphore image_available_semaphore;
	VkSemaphore render_finished_semaphore;
	VkFence in_flight_fence;
	
	char **validation_layers;
	size_t num_validation_layers;
	bool validation_layers_enabled;
} vulkan_renderer_t;

shatter_status_t vulkan_renderer_init(vulkan_renderer_t *vk_renderer, renderer_config_t *config);
shatter_status_t vulkan_renderer_loop(vulkan_renderer_t *vk_renderer);
shatter_status_t vulkan_renderer_cleanup(vulkan_renderer_t *vk_renderer);

shatter_status_t draw_frame(vulkan_renderer_t *vk_renderer);

#endif // RENDERING_APIS_VULKAN_RENDERER_H

