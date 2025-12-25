#ifndef RENDERERS_VULKAN_SHADER_SHADER_MODULE_H
#define RENDERERS_VULKAN_SHADER_SHADER_MODULE_H

#include <common/core.h>

#include <renderers/vulkan/renderer.h>

#include <vulkan/vulkan.h>

#include <stddef.h>

shatter_status_t create_shader_module(VkShaderModule *shader_module, vulkan_renderer_t *vk_renderer,
									  const char *bytecode, size_t len);

#endif // RENDERERS_VULKAN_SHADER_SHADER_MODULE_H

