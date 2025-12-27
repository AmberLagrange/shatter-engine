#include <common/core.h>

#include <graphics_pipeline/graphics_pipeline.h>

#include <shaders/load_shader.h>
#include <shaders/shader_module.h>

#include <stdlib.h>
#include <string.h>


shatter_status_t create_graphics_pipeline(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	log_trace("Creating graphics pipeline.\n");
	shatter_status_t status = SHATTER_SUCCESS;
	
	const char *directory_filepath = vk_renderer->renderer_config.directory_filepath;
	size_t directory_filepath_len = strlen(directory_filepath);
	
	// ---------- Vertex Shader ---------- //
	
	char vertex_spv_filepath[MAX_FILEPATH_LEN + 1];
	strncpy(vertex_spv_filepath, directory_filepath, MAX_FILEPATH_LEN);
	strncat(vertex_spv_filepath, "/shaders/spv/vertex/basic_vertex.spv", MAX_FILEPATH_LEN - directory_filepath_len);
	
	size_t vertex_bytecode_len;
	char *vertex_bytecode = get_shader_bytecode(vertex_spv_filepath, &vertex_bytecode_len);
	if (vertex_bytecode == NULL) {
		
		log_error("Failed to read the vertex shader bytecode.\n");
		status = SHATTER_VULKAN_SHADER_READ_FAILURE;
		goto exit;
	}
	
	VkShaderModule vertex_module = { 0 };
	if (create_shader_module(&vertex_module, vk_renderer, vertex_bytecode, vertex_bytecode_len)) {
		
		log_error("Failed to create the vertex shader module.\n");
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
	
	char fragment_spv_filepath[MAX_FILEPATH_LEN + 1];
	strncpy(fragment_spv_filepath, directory_filepath, MAX_FILEPATH_LEN);
	strncat(fragment_spv_filepath, "/shaders/spv/fragment/basic_fragment.spv", MAX_FILEPATH_LEN - directory_filepath_len);
	
	size_t fragment_bytecode_len;
	char *fragment_bytecode = get_shader_bytecode(fragment_spv_filepath, &fragment_bytecode_len);
	if (fragment_bytecode == NULL) {
		
		log_error("Failed to read the fragment shader bytecode.\n");
		status = SHATTER_VULKAN_SHADER_READ_FAILURE;
		goto cleanup_vertex_module;
	}
	
	VkShaderModule fragment_module = { 0 };
	if (create_shader_module(&fragment_module, vk_renderer, fragment_bytecode, fragment_bytecode_len)) {
		
		log_error("Failed to create the fragment shader module.\n");
		status = SHATTER_VULKAN_SHADER_MODULE_INIT_FAILURE;
		goto cleanup_fragment_bytecode;
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
	
	// ---------- Dynamic State ---------- //
	
	VkDynamicState dynamic_state_list[2] = {
		
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
	};
	
	VkPipelineDynamicStateCreateInfo dynamic_state_info = {
		
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		
		.dynamicStateCount = 2,
		.pDynamicStates = dynamic_state_list,
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
	
	VkPipelineRasterizationStateCreateInfo rasterizer_info = {
		
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
	
	VkPipelineMultisampleStateCreateInfo multisampling_info = {
		
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
	
	VkPipelineColorBlendStateCreateInfo color_blending_info = {
		
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
		
		log_error("Failed to create the pipeline layout.\n");
		status = SHATTER_VULKAN_PIPELINE_LAYOUT_INIT_FAILURE;
		goto cleanup_fragment_module;
	}
	
	// ---------- Pipeline ---------- //
	
	VkGraphicsPipelineCreateInfo pipeline_info = {
		
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		
		.stageCount = 2,
		.pStages = shader_stage_info_list,
		
		.pDynamicState       = &dynamic_state_info,
		.pVertexInputState   = &vertex_input_info,
		.pInputAssemblyState = &input_assembly_info,
		.pViewportState      = &viewport_info,
		.pRasterizationState = &rasterizer_info,
		.pMultisampleState   = &multisampling_info,
		.pDepthStencilState  = NULL, // Ignoring for now
		.pColorBlendState    = &color_blending_info,
		
		.layout = vk_renderer->pipeline_layout,
		.renderPass = vk_renderer->render_pass,
		.subpass = 0,
		
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = -1,
	};
	UNUSED(depth_stencil); // Unused for now, as we're passing in NULL
	
	if (vkCreateGraphicsPipelines(vk_renderer->logical_device, VK_NULL_HANDLE, 1, &pipeline_info,
								  NULL, &(vk_renderer->graphics_pipeline)) != VK_SUCCESS) {
		
		log_error("Failed to create the graphics pipeline.\n");
		status = SHATTER_VULKAN_GRAPHICS_PIPELINE_INIT_FAILURE;
		goto cleanup_fragment_module;
	}
	
	// ---------- Success ---------- //
	
	log_trace("Created graphics pipeline.\n");
	
	// ---------- Cleanup ---------- //
	
cleanup_fragment_module:
	vkDestroyShaderModule(vk_renderer->logical_device, fragment_module, NULL);
	
cleanup_fragment_bytecode:
	free(fragment_bytecode);
	
cleanup_vertex_module:
	vkDestroyShaderModule(vk_renderer->logical_device, vertex_module, NULL);
	
cleanup_vertex_bytecode:
	free(vertex_bytecode);
	
exit:
	return status;
}

