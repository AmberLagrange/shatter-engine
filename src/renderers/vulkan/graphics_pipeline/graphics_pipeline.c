#include <common/core.h>

#include <renderers/vulkan/graphics_pipeline/graphics_pipeline.h>

#include <renderers/vulkan/shaders/shader_module.h>
#include <renderers/vulkan/shaders/load_shader.h>

#include <stdlib.h>

shatter_status_t create_graphics_pipeline(vulkan_renderer_t *vk_renderer) {
	
	log_message(stdout, "\nCreating graphics pipeline.\n");
	shatter_status_t status = SHATTER_SUCCESS;
	
	// ---------- Vertex Shader ---------- //
	
	size_t vertex_len;
	char *vertex_bytecode = get_shader_bytecode("spv/vertex/basic_vertex.spv", &vertex_len);
	
	VkShaderModule vertex_module = { 0 };
	if (create_shader_module(&vertex_module, vk_renderer, vertex_bytecode, vertex_len)) {
		
		log_message(stderr, "Failed to create the vertex shader module.\n");
		status = SHATTER_VULKAN_SHADER_MODULE_INIT_FAILURE;
		goto cleanup_vertex_bytecode;
	}
	
	VkPipelineShaderStageCreateInfo vertex_shader_stage_info = {
		
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = vertex_module,
		.pName = "main",
		.pSpecializationInfo = NULL,
	};
	
	// ---------- Fragment Shader ---------- //
	
	size_t fragment_len;
	char *fragment_bytecode = get_shader_bytecode("spv/fragment/basic_fragment.spv", &fragment_len);
	
	VkShaderModule fragment_module = { 0 };
	if (create_shader_module(&fragment_module, vk_renderer, fragment_bytecode, fragment_len)) {
		
		log_message(stderr, "Failed to create the fragment shader module.\n");
		status = SHATTER_VULKAN_SHADER_MODULE_INIT_FAILURE;
		goto cleanup_vertex_module;
	}
	
	VkPipelineShaderStageCreateInfo fragment_shader_stage_info = {
		
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = fragment_module,
		.pName = "main",
		.pSpecializationInfo = NULL,
	};
	
	// ---------- Shader Stage ---------- //
	
	VkPipelineShaderStageCreateInfo shader_stage_info_list[2] = {
		
		vertex_shader_stage_info,
		fragment_shader_stage_info,
	};
	
	// ---------- Vertex Input ---------- //
	
	VkPipelineVertexInputStateCreateInfo vertex_input_info = {
		
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		
		.vertexBindingDescriptionCount = 0,
		.pVertexBindingDescriptions = NULL,
		
		.vertexAttributeDescriptionCount = 0,
		.pVertexAttributeDescriptions = NULL,
	};
	
	// ---------- Input Assembly ---------- //
	
	VkPipelineInputAssemblyStateCreateInfo input_assembly_info = {
		
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		
		.primitiveRestartEnable = VK_FALSE,
	};
	
	// ---------- Viewports and Scissors ---------- //
	
	VkViewport viewport = {
		
		.x = 0.0f,
		.y = 0.0f,
		.width  = (float)(vk_renderer->swap_chain_extent.width),
		.height = (float)(vk_renderer->swap_chain_extent.height),
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};
	
	VkRect2D scissor = {
		
		.offset = {0, 0},
		.extent = vk_renderer->swap_chain_extent,
	};
	
	VkPipelineViewportStateCreateInfo viewport_info = {
		
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		
		.viewportCount = 1,
		.pViewports = &viewport,
		
		.scissorCount = 1,
		.pScissors = &scissor,
	};
	
	// ---------- Rasterizer ---------- //
	
	VkPipelineRasterizationStateCreateInfo rasterizer = {
		
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		
		.depthClampEnable = VK_FALSE,
		
		.rasterizerDiscardEnable = VK_FALSE,
		
		.polygonMode = VK_POLYGON_MODE_FILL,
		
		.lineWidth = 1.0f,
		
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_CLOCKWISE,
		
		.depthBiasEnable = VK_FALSE,
		.depthBiasConstantFactor = 0.0f,
		.depthBiasClamp = 0.0f,
		.depthBiasSlopeFactor = 0.0f,
		};
	
	// ---------- Multisampling ---------- //
	
	VkPipelineMultisampleStateCreateInfo multisampling = {
		
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		
		.sampleShadingEnable = VK_FALSE,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.minSampleShading = 1.0f,
		.pSampleMask = NULL,
		.alphaToCoverageEnable = VK_FALSE,
		.alphaToOneEnable = VK_FALSE,
	};
	
	// ---------- Depth and Stencil Testing ---------- //
	
	VkPipelineDepthStencilStateCreateInfo depth_stencil = { 0 };
	
	// ---------- Color Blending ---------- //
	
	VkPipelineColorBlendAttachmentState color_blend_attachment = {
		
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
						  VK_COLOR_COMPONENT_G_BIT |
						  VK_COLOR_COMPONENT_B_BIT |
						  VK_COLOR_COMPONENT_A_BIT,
		
		.blendEnable = VK_TRUE,
		
		.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		.colorBlendOp = VK_BLEND_OP_ADD,
		
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
		.alphaBlendOp = VK_BLEND_OP_ADD,
	};
	
	VkPipelineColorBlendStateCreateInfo color_blending = {
		
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		
		.attachmentCount = 1,
		.pAttachments = &color_blend_attachment,
		
		.blendConstants[0] = 0.0f,
		.blendConstants[1] = 0.0f,
		.blendConstants[2] = 0.0f,
		.blendConstants[3] = 0.0f,
	};
	
	// ---------- Pipeline Layout ---------- //
	
	VkPipelineLayoutCreateInfo pipeline_layout_info = {
		
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		
		.setLayoutCount = 0,
		.pSetLayouts = NULL,
		
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = NULL,
	};
	
	if (vkCreatePipelineLayout(vk_renderer->logical_device, &pipeline_layout_info,
							   NULL, &(vk_renderer->pipeline_layout)) != VK_SUCCESS) {
		
		status = SHATTER_VULKAN_PIPELINE_LAYOUT_INIT_FAILURE;
		goto cleanup;
	}
	
	UNUSED(shader_stage_info_list);
	UNUSED(vertex_input_info);
	UNUSED(input_assembly_info);
	UNUSED(viewport_info);
	UNUSED(rasterizer);
	UNUSED(multisampling);
	UNUSED(depth_stencil);
	UNUSED(color_blending);
	
	// ---------- Success ---------- //
	
	log_message(stdout, "Graphics pipeline created.\n");
	
	// ---------- Cleanup ---------- //
	
cleanup:
	vkDestroyShaderModule(vk_renderer->logical_device, fragment_module, NULL);
cleanup_vertex_module:
	vkDestroyShaderModule(vk_renderer->logical_device, vertex_module, NULL);

	free(fragment_bytecode);
cleanup_vertex_bytecode:
	free(vertex_bytecode);
	
	return status;
}

