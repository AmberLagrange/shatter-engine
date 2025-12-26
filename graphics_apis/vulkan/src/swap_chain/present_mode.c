#include <common/core.h>

#include <swap_chain/present_mode.h>

VkPresentModeKHR select_swap_present_mode(const VkPresentModeKHR *present_mode_list, size_t num_modes) {
	
	for (size_t mode_index = 0; mode_index < num_modes; ++mode_index) {
		
		VkPresentModeKHR mode = present_mode_list[mode_index];
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			
			return mode;
		}
	}
	
	// Only FIFO is guaranteed
	return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
}

