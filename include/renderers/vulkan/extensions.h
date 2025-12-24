#ifndef RENDERERS_VULKAN_EXTENSIONS_H
#define RENDERERS_VULKAN_EXTENSIONS_H

#include <renderers/vulkan/renderer.h>

#include <stdint.h>

char **get_required_extensions(vulkan_renderer_t *vk_renderer, uint32_t *num_extensions);

#endif // RENDERERS_VULKAN_EXTENSIONS_H

