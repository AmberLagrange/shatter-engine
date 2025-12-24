#ifndef RENDERERS_VULKAN_SURFACES_SURFACE_H
#define RENDERERS_VULKAN_SURFACES_SURFACE_H

#include <renderers/vulkan/renderer.h>

enum {
	
	SHATTER_SURFACE_SUCCESS = 0,
	SHATTER_SURFACE_INIT_FAILURE,
};

int create_surface(vulkan_renderer_t *vk_renderer);

#endif // RENDERERS_VULKAN_SURFACES_SURFACE_H

