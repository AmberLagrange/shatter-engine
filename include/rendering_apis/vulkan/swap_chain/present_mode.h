#ifndef RENDERING_APIS_VULKAN_SWAP_CHAIN_PRESENTATION_MODE_H
#define RENDERING_APIS_VULKAN_SWAP_CHAIN_PRESENTATION_MODE_H

#include <stddef.h>

#include <vulkan/vulkan.h>

VkPresentModeKHR select_swap_present_mode(const VkPresentModeKHR *present_mode_list, size_t num_modes);

#endif // RENDERING_APIS_VULKAN_SWAP_CHAIN_PRESENTATION_MODE_H

