#include <renderers/vulkan/swap_chain/surface_format.h>

VkSurfaceFormatKHR select_swap_surface_format(const VkSurfaceFormatKHR *surface_format_list, size_t num_formats) {
	
	for (size_t format_index = 0; format_index < num_formats; ++format_index) {
		
		VkSurfaceFormatKHR format = surface_format_list[format_index];
		
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			
			return format;
		}
	}
	
	// No optimal format found, just return the first one
	return surface_format_list[0];
}

