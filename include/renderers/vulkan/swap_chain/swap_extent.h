#ifndef RENDERERS_VULKAN_SWAP_CHAIN_SWAP_EXTENT_H
#define RENDERERS_VULKAN_SWAP_CHAIN_SWAP_EXTENT_H

#include <renderers/vulkan/renderer.h>

#include <vulkan/vulkan.h>

VkExtent2D select_swap_extent(vulkan_renderer_t *vk_renderer, const VkSurfaceCapabilitiesKHR *surface_capabilities);

#endif // RENDERERS_VULKAN_SWAP_CHAIN_SWAP_EXTENT_H

