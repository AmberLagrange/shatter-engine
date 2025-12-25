#ifndef RENDERERS_VULKAN_SWAP_CHAIN_PRESENTATION_MODE_H
#define RENDERERS_VULKAN_SWAP_CHAIN_PRESENTATION_MODE_H

#include <stddef.h>

#include <vulkan/vulkan.h>

VkPresentModeKHR select_swap_present_mode(const VkPresentModeKHR *present_mode_list, size_t num_modes);

#endif // RENDERERS_VULKAN_SWAP_CHAIN_PRESENTATION_MODE_H

