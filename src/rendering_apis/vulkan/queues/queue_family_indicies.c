#include <common/core.h>

#include <rendering_apis/vulkan/renderer.h>

#include <rendering_apis/vulkan/queues/queue_family_indicies.h>
#include <rendering_apis/vulkan/queues/required_queue_families.h>

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

void get_queue_families(vulkan_renderer_t *vk_renderer, VkPhysicalDevice device, queue_family_indicies_t *indicies) {
	
	for (size_t index = 0; index < MAX_INDICIES; ++index) {
		
		indicies->index_list[index].has_value = false;
	}
	
	uint32_t num_families = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &num_families, NULL);
	
	VkQueueFamilyProperties *family_list = malloc(sizeof(VkQueueFamilyProperties) * num_families);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &num_families, family_list);
	
	size_t num_required_families = get_num_required_families();
	size_t num_found_families = 0;
	
	log_trace("Required families:\n");
	
	for (size_t required_index = 0; required_index < num_required_families; ++required_index) {
		
		required_queue_family_t required_family = REQUIRED_FAMILY_LIST[required_index];
		log_trace("\t%s\n", required_family.debug_string);
		
		for (size_t family_index = 0; family_index < num_families; ++family_index) {
			
			VkQueueFamilyProperties family = family_list[family_index];
			if (required_family.condition_lambda(&family, vk_renderer, device, family_index)) {
				
				indicies->index_list[required_family.family_index].has_value = true;
				indicies->index_list[required_family.family_index].value = family_index;
				++num_found_families;
				break;
			}
		}
	}
	
	indicies->num_indicies = num_found_families;
	free(family_list);
}

bool is_exclusive_graphics(queue_family_indicies_t *indicies) {
	
	return (indicies->index_list[GRAPHICS_FAMILY_INDEX].value == indicies->index_list[PRESENT_FAMILY_INDEX].value);
}

uint32_t *unwrap_indicies(queue_family_indicies_t *indicies, size_t *num_indicies) {
	
	*num_indicies = indicies->num_indicies;
	uint32_t *unwrapped_indicies = malloc(sizeof(uint32_t) * (*num_indicies));
	
	size_t found_index = 0;
	for (size_t index = 0; index < *num_indicies; ++index) {
		
		if (indicies->index_list[index].has_value) {
			
			unwrapped_indicies[found_index] = indicies->index_list[index].value;
			++found_index;
		}
	}
	
	assert(*num_indicies == found_index);	
	return unwrapped_indicies;
}

