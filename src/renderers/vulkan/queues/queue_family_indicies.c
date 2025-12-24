#include <renderers/vulkan/queues/queue_family_indicies.h>

#include <logging/logger.h>

#include <stdarg.h>
#include <stdlib.h>

typedef struct {
	
	uint32_t family_index;
	bool (*condition_lambda)(VkQueueFamilyProperties *, ...);
	const char *debug_string;
} queue_family;

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

static const queue_family REQUIRED_FAMILY_LIST[MAX_INDICIES] = {
	
	{ GRAPHICS_FAMILY_INDEX, has_graphics, "Graphics Family" },
	{ PRESENT_FAMILY_INDEX, can_present, "Present Family" },
};

size_t get_num_required_families(void) {
	
	size_t required_family_index = 0;
	
	while (REQUIRED_FAMILY_LIST[required_family_index].debug_string) {
		
		++required_family_index;
	}
	
	return required_family_index;
}

void get_queue_families(vulkan_renderer_t *vk_renderer, VkPhysicalDevice device, queue_family_indicies *indicies) {
	
	for (size_t index = 0; index < MAX_INDICIES; ++index) {
		
		indicies->index_list[index].has_value = false;
	}
	
	uint32_t num_families = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &num_families, NULL);
	
	VkQueueFamilyProperties *family_list = malloc(sizeof(VkQueueFamilyProperties) * num_families);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &num_families, family_list);
	
	size_t num_required_families = get_num_required_families();
	size_t num_found_families = 0;
	
	for (size_t required_index = 0; required_index < num_required_families; ++required_index) {
		
		for (size_t family_index = 0; family_index < num_families; ++family_index) {
			
			VkQueueFamilyProperties family = family_list[family_index];
			if (REQUIRED_FAMILY_LIST[required_index].condition_lambda(&family, vk_renderer, device, family_index)) {
				
				indicies->index_list[REQUIRED_FAMILY_LIST[required_index].family_index].has_value = true;
				indicies->index_list[REQUIRED_FAMILY_LIST[required_index].family_index].value = family_index;
				++num_found_families;
				break;
			}
		}
	}
	
	indicies->num_indicies = num_found_families;
	free(family_list);
}

bool is_complete(queue_family_indicies *indicies) {
	
	size_t num_required_families = get_num_required_families();
	
	for (size_t required_index = 0; required_index < num_required_families; ++required_index) {
		
		if (!indicies->index_list[REQUIRED_FAMILY_LIST[required_index].family_index].has_value) {
			
			return false;
		}
	}
	
	log_message(stdout, "Has all the required families.\n");
	for (size_t required_index = 0; required_index < num_required_families; ++required_index) {
		
		log_message(stdout, "\t%s\n", REQUIRED_FAMILY_LIST[required_index].debug_string);
	}
	
	return true;
}

