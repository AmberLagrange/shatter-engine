#include <common/core.h>

#include <renderers/glfw.h>
#include <renderers/renderer_config.h>

#include <renderers/vulkan/debug_utils.h>
#include <renderers/vulkan/instance.h>
#include <renderers/vulkan/renderer.h>
#include <renderers/vulkan/validation_layers.h>

#include <renderers/vulkan/devices/logical.h>
#include <renderers/vulkan/devices/physical.h>

#include <renderers/vulkan/graphics_pipeline/graphics_pipeline.h>

#include <renderers/vulkan/render_pass/render_pass.h>

#include <renderers/vulkan/surfaces/surface.h>

#include <renderers/vulkan/swap_chain/frame_buffer.h>
#include <renderers/vulkan/swap_chain/image_view.h>
#include <renderers/vulkan/swap_chain/swap_chain.h>

#include <stdlib.h>
#include <string.h>

shatter_status_t vulkan_renderer_init(vulkan_renderer_t *vk_renderer, renderer_config_t *config) {
	
	if (init_glfw()) {
		
		log_error("Failed to initialize GLFW.\n");
		return SHATTER_GLFW_INIT_FAILURE;
	}
	
	memcpy(&(vk_renderer->config), config, sizeof(renderer_config_t));
	vk_renderer->rendering_window =
		glfwCreateWindow(vk_renderer->config.width, vk_renderer->config.height, vk_renderer->config.title, NULL, NULL);
	
	if (!vk_renderer->rendering_window) {
		
		log_error("Failed to create GLFW Window.\n");
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

	if (create_swap_chain(vk_renderer)) {
		
		log_error("Failed to create the swap chain.\n");
		return SHATTER_VULKAN_SWAP_CHAIN_INIT_FAILURE;
	}
	
	if (create_image_views(vk_renderer)) {
		
		log_error("Failed to create the image views.\n");
		return SHATTER_VULKAN_IMAGE_VIEW_INIT_FAILURE;
	}
	
	if (create_render_pass(vk_renderer)) {
		
		log_error("Failed to create the render pass.\n");
		return SHATTER_VULKAN_RENDER_PASS_INIT_FAILURE;
	}
	
	if (create_graphics_pipeline(vk_renderer)) {
		
		log_error("Failed to create the graphics pipeline.\n");
		return SHATTER_VULKAN_GRAPHICS_PIPELINE_INIT_FAILURE;
	}
	
	if (create_frame_buffers(vk_renderer)) {
		
		log_error("Failed to create frame buffers.\n");
		return SHATTER_VULKAN_FRAME_BUFFER_INIT_FAILURE;
	}
	
	log_info("Renderer Initialization Complete.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t vulkan_renderer_loop(vulkan_renderer_t *vk_renderer) {
	
	(void)vk_renderer;
	log_trace("\n");
	log_trace("Running Renderer Loop.\n");
	
	return SHATTER_SUCCESS;
}

shatter_status_t vulkan_renderer_cleanup(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	
	cleanup_frame_buffers(vk_renderer);
	log_trace("Destroyed frame buffers.\n");
	
	vkDestroyPipeline(vk_renderer->logical_device, vk_renderer->graphics_pipeline, NULL);
	log_trace("Destroyed graphics pipeline.\n");
	
	vkDestroyPipelineLayout(vk_renderer->logical_device, vk_renderer->pipeline_layout, NULL);
	log_trace("Destroyed pipeline layout.\n");
	
	vkDestroyRenderPass(vk_renderer->logical_device, vk_renderer->render_pass, NULL);
	log_trace("Destroyed render pass.\n");
	
	cleanup_image_views(vk_renderer);
	log_trace("Destroyed image views.\n");
	
	free(vk_renderer->swap_chain_image_list);
	
	vkDestroySwapchainKHR(vk_renderer->logical_device, vk_renderer->swap_chain, NULL);
	log_trace("Destroyed swapchain.\n");
	
	vkDestroyDevice(vk_renderer->logical_device, NULL);
	log_trace("Destroyed logical device.\n");
	
	cleanup_vulkan_debug_messenger(vk_renderer);
	log_trace("Destroyed Vulkan debug messenger.\n");
	
	vkDestroySurfaceKHR(vk_renderer->vulkan_instance, vk_renderer->rendering_surface, NULL);
	log_trace("Destroyed surface.\n");
	
	vkDestroyInstance(vk_renderer->vulkan_instance, NULL);
	log_trace("Destroyed Vulkan instance.\n");
	
	glfwDestroyWindow(vk_renderer->rendering_window);
	log_trace("Destroyed GLFW window.\n");
	
	if (terminate_glfw()) {
		
		log_error("Failed to terminate GLFW.\n");
		return SHATTER_RENDERER_CLEANUP_FAILURE;
	}
	
	log_info("Renderer Cleanup Complete.\n");
	return SHATTER_SUCCESS;
}

