#include <common/core.h>

#include <vulkan_renderer.h>

#include <commands/command_buffer.h>
#include <commands/command_pool.h>

#include <debug/debug_utils.h>
#include <debug/validation_layers.h>

#include <devices/logical.h>
#include <devices/physical.h>

#include <graphics_buffers/vertex_buffer.h>

#include <graphics_pipeline/graphics_pipeline.h>

#include <instance/instance.h>

#include <queues/graphics_queue.h>
#include <queues/present_queue.h>

#include <renderer/renderer_config.h>

#include <surfaces/surface.h>

#include <swap_chain/swap_chain.h>
#include <swap_chain/sync_objects.h>

#include <stdlib.h>
#include <string.h>

shatter_status_t init_vulkan_renderer(vulkan_renderer_t **vk_renderer_ptr,
									  renderer_config_t *renderer_config) {
	
	log_trace("Initializing Vulkan Renderer.\n");
	
	vulkan_renderer_t *vk_renderer = malloc(sizeof(vulkan_renderer_t));
	*vk_renderer_ptr = vk_renderer;
	
	vk_renderer->renderer_config = renderer_config;
	
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	vk_renderer->renderer_config->rendering_window = glfwCreateWindow(
			vk_renderer->renderer_config->width,
			vk_renderer->renderer_config->height,
			vk_renderer->renderer_config->title,
			NULL, NULL);
	
	if (!vk_renderer->renderer_config->rendering_window) {
		
		log_error("Failed to create GLFW window.\n");
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
		
		log_error("Failed to set up the debug messenger.\n");
		return SHATTER_VULKAN_DEBUG_SETUP_FAILURE;
	}
	
	vk_renderer->physical_device = VK_NULL_HANDLE;
	if (choose_physical_device(vk_renderer)) {
		
		log_error("Failed to set up a physical device.\n");
		return SHATTER_VULKAN_PHYSICAL_DEVICE_CHOICE_FAILURE;
	}
	
	if (create_logical_device(vk_renderer)) {
		
		log_error("Failed to create a logical device.\n");
		return SHATTER_VULKAN_LOGICAL_DEVICE_INIT_FAILURE;
	}
	
	vk_renderer->num_in_flight_frames = MAX_IN_FLIGHT_FRAMES; // 2
	if (create_swap_chain(vk_renderer)) {
		
		log_error("Failed to create the swap chain.\n");
		return SHATTER_VULKAN_SWAP_CHAIN_INIT_FAILURE;
	}
	
	if (create_graphics_pipeline(vk_renderer)) {
		
		log_error("Failed to create the graphics pipeline.\n");
		return SHATTER_VULKAN_GRAPHICS_PIPELINE_INIT_FAILURE;
	}
	
	if (create_command_pool(vk_renderer)) {
		
		log_error("Failed to create command pool.\n");
		return SHATTER_VULKAN_COMMAND_POOL_INIT_FAILURE;
	}
	
	if (create_vertex_buffers(vk_renderer)) {
		
		log_error("Failed to create vertex buffers.\n");
		return SHATTER_VULKAN_VERTEX_BUFFER_INIT_FAILURE;
	}
	
	if (create_command_buffers(vk_renderer)) {
		
		log_error("Failed to create command buffer.\n");
		return SHATTER_VULKAN_COMMAND_BUFFER_INIT_FAILURE;
	}
	
	vk_renderer->current_frame = 0;
	
	log_trace("\n");
	log_trace("Renderer Initialization Complete.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t loop_vulkan_renderer(vulkan_renderer_t *vk_renderer) {
	
	shatter_status_t status = draw_frame(vk_renderer);
	if (status != SHATTER_SUCCESS && status != SHATTER_VULKAN_SWAP_CHAIN_RECREATION) {
		
		log_critical("Failed to draw frame.\n");
		return SHATTER_VULKAN_DRAW_FRAME_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_vulkan_renderer(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	log_trace("Cleaning up Vulkan renderer.\n");
	
	vkDeviceWaitIdle(vk_renderer->logical_device);
	
	vkDestroyCommandPool(vk_renderer->logical_device, vk_renderer->command_pool, NULL);
	log_trace("Destroyed command pools.\n");
	
	cleanup_vertex_buffers(vk_renderer);
	log_trace("Destroyed vertex buffer.\n");
	
	cleanup_command_buffers(vk_renderer);
	log_trace("Destroyed command buffers.\n");
	
	vkDestroyPipeline(vk_renderer->logical_device, vk_renderer->graphics_pipeline, NULL);
	log_trace("Destroyed graphics pipeline.\n");
	
	vkDestroyPipelineLayout(vk_renderer->logical_device, vk_renderer->pipeline_layout, NULL);
	log_trace("Destroyed pipeline layout.\n");
	
	cleanup_swap_chain(vk_renderer);
	log_trace("Destroyed swapchain.\n");
	
	vkDestroyDevice(vk_renderer->logical_device, NULL);
	log_trace("Destroyed logical device.\n");
	
	cleanup_vulkan_debug_messenger(vk_renderer);
	log_trace("Destroyed Vulkan debug messenger.\n");
	
	vkDestroySurfaceKHR(vk_renderer->vulkan_instance, vk_renderer->rendering_surface, NULL);
	log_trace("Destroyed surface.\n");
	
	vkDestroyInstance(vk_renderer->vulkan_instance, NULL);
	log_trace("Destroyed Vulkan instance.\n");
	
	glfwDestroyWindow(vk_renderer->renderer_config->rendering_window);
	log_trace("Desroyed GLFW window.\n");
	
	free(vk_renderer);
	log_trace("Renderer Cleanup Complete.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t draw_frame(vulkan_renderer_t *vk_renderer) {
	
	uint32_t current_frame = vk_renderer->current_frame;
	uint32_t image_index;
	
	wait_for_in_flight_fence(vk_renderer);
	
	if (get_next_swap_chain_image(vk_renderer, &image_index)) {
		
		return SHATTER_VULKAN_SWAP_CHAIN_RECREATION;
	}
	
	reset_in_flight_fence(vk_renderer);
	record_command_buffer(vk_renderer, vk_renderer->command_buffer_list[current_frame], image_index);
	submit_graphics_queue(vk_renderer, image_index);
	submit_present_queue(vk_renderer, image_index);
	
	vk_renderer->current_frame = (current_frame + 1) % vk_renderer->num_in_flight_frames;
	
	return SHATTER_SUCCESS;
}

// ---------- Callbacks ---------- //

shatter_status_t vulkan_frame_buffer_resize_callback(vulkan_renderer_t *vk_renderer) {
	
	vk_renderer->frame_buffer_resized = true;
	return SHATTER_SUCCESS;
}

