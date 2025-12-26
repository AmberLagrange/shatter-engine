#ifndef INSTANCE_EXTENSIONS_H
#define INSTANCE_EXTENSIONS_H

#include <vulkan_renderer.h>

#include <stdint.h>

char **get_required_extensions(vulkan_renderer_t *vk_renderer, uint32_t *num_extensions);

#endif // INSTANCE_EXTENSIONS_H

