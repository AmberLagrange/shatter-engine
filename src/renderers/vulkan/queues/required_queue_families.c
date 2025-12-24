#include <renderers/vulkan/renderer.h>

#include <renderers/vulkan/queues/queue_family_indicies.h>
#include <renderers/vulkan/queues/required_queue_families.h>

#include <stdarg.h>
#include <stddef.h>

// ---------- Const Values ---------- //

const required_queue_family_t REQUIRED_FAMILY_LIST[MAX_INDICIES] = {
	
	{ GRAPHICS_FAMILY_INDEX, has_graphics, "Graphics Family" },
	{ PRESENT_FAMILY_INDEX, can_present, "Present Family" },
};

// ---------- Helper Functions

size_t get_num_required_families(void) {
	
	size_t required_family_index = 0;
	
	while (REQUIRED_FAMILY_LIST[required_family_index].debug_string) {
		
		++required_family_index;
	}
	
	return required_family_index;
}

// ---------- Conditional Lambdas ---------- //

bool has_graphics(VkQueueFamilyProperties *family, ...) {
	
	return family->queueFlags & VK_QUEUE_GRAPHICS_BIT;
}

bool can_present(VkQueueFamilyProperties *family, ...) {
	
	VkBool32 present_support = VK_FALSE;
	
	va_list args;
	va_start(args, family);
	vulkan_renderer_t *vk_renderer = va_arg(args, vulkan_renderer_t *);
	VkPhysicalDevice device = va_arg(args, VkPhysicalDevice);
	uint32_t index = va_arg(args, uint32_t);
	va_end(args);
	
	vkGetPhysicalDeviceSurfaceSupportKHR(device, index, vk_renderer->rendering_surface, &present_support);
	return present_support == VK_TRUE;
}

