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
	
	memcpy(&(vk_renderer->renderer_config), renderer_config, sizeof(renderer_config_t));
	
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
	
	if (create_command_buffer(vk_renderer)) {
		
		log_error("Failed to create command buffer.\n");
		return SHATTER_VULKAN_COMMAND_BUFFER_INIT_FAILURE;
	}
	
	if (create_sync_objects(vk_renderer)) {
		
		log_error("Failed to create sync objects.\n");
		return SHATTER_VULKAN_SYNC_OBJECT_INIT_FAILURE;
	}
	
	log_trace("\n");
	log_trace("Renderer Initialization Complete.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t loop_vulkan_renderer(vulkan_renderer_t *vk_renderer) {
	
	if (draw_frame(vk_renderer)) {
		
		log_critical("Failed to draw frame.\n");
		return SHATTER_VULKAN_DRAW_FRAME_FAILURE;
	}
	
	vkDeviceWaitIdle(vk_renderer->logical_device);
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_vulkan_renderer(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	log_trace("Cleaning up Vulkan renderer.\n");
	
	vkDestroyFence(vk_renderer->logical_device, vk_renderer->in_flight_fence, NULL);
	log_trace("Destroyed in flight fence.\n");
	
	vkDestroySemaphore(vk_renderer->logical_device, vk_renderer->render_finished_semaphore, NULL);
	log_trace("Destroyed render finished semaphore.\n");
	
	vkDestroySemaphore(vk_renderer->logical_device, vk_renderer->image_available_semaphore, NULL);
	log_trace("Destroyed image available semaphore.\n");
	
	vkDestroyCommandPool(vk_renderer->logical_device, vk_renderer->command_pool, NULL);
	log_trace("Destroyed command pool.\n");
	
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
	
	free(vk_renderer);
	log_trace("Renderer Cleanup Complete.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t draw_frame(vulkan_renderer_t *vk_renderer) {
	
	vkWaitForFences(vk_renderer->logical_device, 1, &(vk_renderer->in_flight_fence), VK_TRUE, UINT64_MAX);
	vkResetFences(vk_renderer->logical_device, 1, &(vk_renderer->in_flight_fence));
	
	uint32_t image_index;
	vkAcquireNextImageKHR(vk_renderer->logical_device, vk_renderer->swap_chain, UINT64_MAX,
						  vk_renderer->image_available_semaphore, VK_NULL_HANDLE, &image_index);
	
	vkResetCommandBuffer(vk_renderer->command_buffer, 0);
	record_command_buffer(vk_renderer, vk_renderer->command_buffer, image_index);
	
	VkSemaphore wait_semaphore_list[1] = { vk_renderer->image_available_semaphore };
	VkPipelineStageFlags wait_stage_list[1] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	
	VkSemaphore signal_semaphore_list[1] = { vk_renderer->render_finished_semaphore };
	
	VkSubmitInfo submit_info = {
		
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = wait_semaphore_list,
		.pWaitDstStageMask = wait_stage_list,
		
		.commandBufferCount = 1,
		.pCommandBuffers = &(vk_renderer->command_buffer),
		
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = signal_semaphore_list,
	};
	
	if (vkQueueSubmit(vk_renderer->graphics_queue, 1, &submit_info, vk_renderer->in_flight_fence) != VK_SUCCESS) {
		
		log_error("Failed to submit draw command buffer.\n");
		return SHATTER_VULKAN_DRAW_FRAME_FAILURE;
	}
	
	VkSwapchainKHR swap_chain_list[1] = { vk_renderer->swap_chain };
	VkPresentInfoKHR present_info = {
		
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = signal_semaphore_list,
		
		.swapchainCount = 1,
		.pSwapchains = swap_chain_list,
		.pImageIndices = &image_index,
		
		.pResults = NULL,
	};
	
	vkQueuePresentKHR(vk_renderer->present_queue, &present_info);
	
	return SHATTER_SUCCESS;
}

