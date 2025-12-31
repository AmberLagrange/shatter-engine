#include <common/core.h>

#include <vulkan_renderer.h>

#include <commands/command_buffer.h>
#include <commands/command_pool.h>

#include <debug/debug_utils.h>
#include <debug/validation_layers.h>

#include <devices/logical.h>
#include <devices/physical.h>

#include <graphics_pipeline/graphics_pipeline.h>

#include <instance/instance.h>

#include <render_pass/render_pass.h>

#include <surfaces/surface.h>

#include <swap_chain/frame_buffer.h>
#include <swap_chain/image_view.h>
#include <swap_chain/swap_chain.h>

#include <sync/sync_objects.h>

#include <renderer/renderer_config.h>

#include <stdlib.h>
#include <string.h>

shatter_status_t init_vulkan_renderer(vulkan_renderer_t **vk_renderer_ptr,
									  renderer_config_t *renderer_config) {
	
	log_trace("Initializing Vulkan Renderer.\n");
	
	vulkan_renderer_t *vk_renderer = malloc(sizeof(vulkan_renderer_t));
	*vk_renderer_ptr = vk_renderer;
	
	vk_renderer->renderer_config = renderer_config;
	
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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
	
	if (create_command_pool(vk_renderer)) {
		
		log_error("Failed to create command pool.\n");
		return SHATTER_VULKAN_COMMAND_POOL_INIT_FAILURE;
	}
	
	if (create_command_buffers(vk_renderer)) {
		
		log_error("Failed to create command buffer.\n");
		return SHATTER_VULKAN_COMMAND_BUFFER_INIT_FAILURE;
	}
	
	if (create_sync_objects(vk_renderer)) {
		
		log_error("Failed to create sync objects.\n");
		return SHATTER_VULKAN_SYNC_OBJECT_INIT_FAILURE;
	}
	
	vk_renderer->current_frame = 0;
	
	log_trace("\n");
	log_trace("Renderer Initialization Complete.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t loop_vulkan_renderer(vulkan_renderer_t *vk_renderer) {
	
	if (draw_frame(vk_renderer)) {
		
		log_critical("Failed to draw frame.\n");
		return SHATTER_VULKAN_DRAW_FRAME_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_vulkan_renderer(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	log_trace("Cleaning up Vulkan renderer.\n");
	
	vkDeviceWaitIdle(vk_renderer->logical_device);
	
	cleanup_sync_objects(vk_renderer);
	log_trace("Destroyed sync objects.\n");
	
	vkDestroyCommandPool(vk_renderer->logical_device, vk_renderer->command_pool, NULL);
	log_trace("Destroyed command pools.\n");
	
	cleanup_command_buffers(vk_renderer);
	log_trace("Destroyed command buffers.\n");
	
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
	
	glfwDestroyWindow(vk_renderer->renderer_config->rendering_window);
	log_trace("Desroyed GLFW window.\n");
	
	free(vk_renderer);
	log_trace("Renderer Cleanup Complete.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t draw_frame(vulkan_renderer_t *vk_renderer) {
	
	uint32_t current_frame = vk_renderer->current_frame;
	VkFence *current_fence_ptr = &(vk_renderer->in_flight_fence_list[current_frame]);
	
	vkWaitForFences(vk_renderer->logical_device, 1, current_fence_ptr, VK_TRUE, UINT64_MAX);
	vkResetFences(vk_renderer->logical_device, 1, current_fence_ptr);
	
	VkSemaphore *acquire_semaphore_ptr = &(vk_renderer->acquire_image_semaphore_list[current_frame]);
	
	uint32_t image_index;
	vkAcquireNextImageKHR(vk_renderer->logical_device, vk_renderer->swap_chain, UINT64_MAX,
						  *acquire_semaphore_ptr, VK_NULL_HANDLE, &image_index);
	
	VkCommandBuffer *command_buffer_ptr = &(vk_renderer->command_buffer_list[current_frame]);
	
	vkResetCommandBuffer(*command_buffer_ptr, 0);
	record_command_buffer(vk_renderer, *command_buffer_ptr, image_index);
	
	VkSemaphore *submit_semaphore_ptr = &(vk_renderer->submit_image_semaphore_list[image_index]);
	
	VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submit_info = {
		
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = acquire_semaphore_ptr,
		.pWaitDstStageMask = &wait_stage,
		
		.commandBufferCount = 1,
		.pCommandBuffers = command_buffer_ptr,
		
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = submit_semaphore_ptr,
	};
	
	if (vkQueueSubmit(vk_renderer->graphics_queue, 1, &submit_info, *current_fence_ptr) != VK_SUCCESS) {
		
		log_error("Failed to submit draw command buffer.\n");
		return SHATTER_VULKAN_DRAW_FRAME_FAILURE;
	}
	
	VkSwapchainKHR swap_chain_list[1] = { vk_renderer->swap_chain };
	VkPresentInfoKHR present_info = {
		
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = submit_semaphore_ptr,
		
		.swapchainCount = 1,
		.pSwapchains = swap_chain_list,
		.pImageIndices = &image_index,
		
		.pResults = NULL,
	};
	
	vkQueuePresentKHR(vk_renderer->present_queue, &present_info);
	vk_renderer->current_frame = (current_frame + 1) % MAX_IN_FLIGHT_FRAMES;
	
	return SHATTER_SUCCESS;
}

