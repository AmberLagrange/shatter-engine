#ifndef RENDERING_APIS_VULKAN_INSTANCE_EXTENSIONS_H
#define RENDERING_APIS_VULKAN_INSTANCE_EXTENSIONS_H

#include <rendering_apis/vulkan/renderer.h>

#include <stdint.h>

char **get_required_extensions(vulkan_renderer_t *vk_renderer, uint32_t *num_extensions);

#endif // RENDERING_APIS_VULKAN_INSTANCE_EXTENSIONS_H

