#ifndef RENDERING_APIS_VULKAN_SWAP_CHAIN_SURFACE_FORMAT_H
#define RENDERING_APIS_VULKAN_SWAP_CHAIN_SURFACE_FORMAT_H

#include <stddef.h>

#include <vulkan/vulkan.h>

VkSurfaceFormatKHR select_swap_surface_format(const VkSurfaceFormatKHR *surface_format_list, size_t num_formats);

#endif // RENDERING_APIS_VULKAN_SWAP_CHAIN_SURFACE_FORMAT_H

