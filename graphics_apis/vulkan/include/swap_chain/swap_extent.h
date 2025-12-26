#ifndef SWAP_CHAIN_SWAP_EXTENT_H
#define SWAP_CHAIN_SWAP_EXTENT_H

#include <vulkan_renderer.h>

#include <vulkan/vulkan.h>

VkExtent2D select_swap_extent(vulkan_renderer_t *vk_renderer, const VkSurfaceCapabilitiesKHR *surface_capabilities);

#endif // SWAP_CHAIN_SWAP_EXTENT_H

