#ifndef RENDERER_H
#define RENDERER_H

#include <common/core.h>

#include <buffers/buffer.h>

#include <commands/image_commands.h>

#include <renderer/renderer_config.h>

#include <queues/queue_family_indices.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_IN_FLIGHT_FRAMES 2

typedef struct vulkan_renderer_s {
	
	renderer_config_t *renderer_config;
	
	VkInstance vulkan_instance;
	VkDebugUtilsMessengerEXT debug_messenger;
	
	VkSurfaceKHR rendering_surface;
	
	queue_family_indices_t queue_family_indices;
	
	VkPhysicalDevice physical_device;
	VkDevice logical_device;
	
	VkQueue graphics_queue;
	VkQueue present_queue;
	
	VkSwapchainKHR swap_chain;
	VkImage *swap_chain_image_list;
	size_t num_swap_chain_images;
	VkFormat swap_chain_image_format;
	VkExtent2D swap_chain_extent;
	
	VkFramebuffer *swap_chain_frame_buffer_list;
	VkImageView *swap_chain_image_view_list;
	
	bool frame_buffer_resized;
	
	VkRenderPass render_pass;
	VkPipelineLayout pipeline_layout;
	VkPipeline graphics_pipeline;
	
	buffer_t vertex_buffer;
	buffer_t index_buffer;
	
	VkCommandPool command_pool;
	image_commands_t image_commands;
	
	VkSemaphore *acquire_image_semaphore_list;
	VkFence *in_flight_fence_list;
	VkSemaphore *submit_image_semaphore_list;
	
	uint32_t current_frame;
	uint32_t num_in_flight_frames;
	
	char **validation_layers;
	size_t num_validation_layers;
	bool validation_layers_enabled;
} vulkan_renderer_t;

shatter_status_t init_vulkan_renderer(vulkan_renderer_t **vk_renderer_ptr, renderer_config_t *renderer_config);
shatter_status_t loop_vulkan_renderer(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_vulkan_renderer(vulkan_renderer_t *vk_renderer);

shatter_status_t draw_frame(vulkan_renderer_t *vk_renderer);

// ---------- Callbacks ---------- //

shatter_status_t vulkan_frame_buffer_resize_callback(vulkan_renderer_t *vk_renderer);

#endif // RENDERER_H

