#ifndef RENDERING_APIS_VULKAN_DEBUG_VALIDATION_LAYERS_H
#define RENDERING_APIS_VULKAN_DEBUG_VALIDATION_LAYERS_H

#include <common/core.h>

#include <rendering_apis/vulkan/renderer.h>

#include <stddef.h>

size_t get_num_validation_layers(void);

shatter_status_t init_validation_layers(vulkan_renderer_t *vk_renderer);
shatter_status_t destroy_validation_layers(vulkan_renderer_t *vk_renderer);

bool check_validation_layer_support(vulkan_renderer_t *vk_renderer);

char **get_required_extensions(vulkan_renderer_t *vk_renderer, uint32_t *num_extensions);

#endif // RENDERING_APIS_VULKAN_DEBUG_RENDERER_HELPER_H

