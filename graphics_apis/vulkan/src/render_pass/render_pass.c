#include <common/core.h>

#include <render_pass/render_pass.h>

#include <vulkan/vulkan.h>

shatter_status_t create_render_pass(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	log_trace("Creating render pass.\n");
	
	// ---------- Color Attachments ---------- //
	
	VkAttachmentDescription color_attachment = {
		
		.format = vk_renderer->swap_chain_image_format,
		
		.samples = VK_SAMPLE_COUNT_1_BIT,
		
		.loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		
		.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout   = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
	};
	
	// ---------- Subpasses ---------- //
	
	VkAttachmentReference color_attachment_reference = {
		
		.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	};
	
	VkSubpassDescription subpass = {
		
		.colorAttachmentCount = 1,
		.pColorAttachments = &color_attachment_reference,
	};
	
	// ---------- Subpass Dependencies ---------- //
	
	VkSubpassDependency subpass_dependency = {
		
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		
		.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.srcAccessMask = 0,
		
		.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
	};
	
	// ---------- Render Pass ---------- //
	
	VkRenderPassCreateInfo render_pass_info = {
		
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		
		.attachmentCount = 1,
		.pAttachments = &color_attachment,
		
		.subpassCount = 1,
		.pSubpasses = &subpass,
		
		.dependencyCount = 1,
		.pDependencies = &subpass_dependency,
	};
	
	if (vkCreateRenderPass(vk_renderer->logical_device, &render_pass_info, NULL, &(vk_renderer->render_pass)) != VK_SUCCESS) {
		
		log_error("Failed to create the render pass.\n");
		return SHATTER_VULKAN_RENDER_PASS_INIT_FAILURE;
	}
	
	log_trace("Created render pass.\n");
	return SHATTER_SUCCESS;
}

