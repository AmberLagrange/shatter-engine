#include <common/core.h>

#include <vulkan_renderer.h>

#include <queues/queue_family_indices.h>
#include <queues/required_queue_families.h>

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

void get_queue_families(vulkan_renderer_t *vk_renderer, VkPhysicalDevice device, queue_family_indices_t *indices) {
	
	for (size_t index = 0; index < MAX_INDICES; ++index) {
		
		indices->index_list[index].has_value = false;
	}
	
	uint32_t num_families = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &num_families, NULL);
	
	if (num_families >= MAX_INDICES) {
		
		log_error("Not enough space for the family indices.\n");
		return;
	}
	
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
				
				indices->index_list[required_family.family_index].has_value = true;
				indices->index_list[required_family.family_index].value = family_index;
				++num_found_families;
				break;
			}
		}
	}
	
	indices->num_indices = num_found_families;
	free(family_list);
}

bool is_exclusive_graphics(queue_family_indices_t *indices) {
	
	return (indices->index_list[GRAPHICS_FAMILY_INDEX].value == indices->index_list[PRESENT_FAMILY_INDEX].value);
}

uint32_t *unwrap_indices(queue_family_indices_t *indices, size_t *num_indices) {
	
	*num_indices = indices->num_indices;
	uint32_t *unwrapped_indices = malloc(sizeof(uint32_t) * (*num_indices));
	
	size_t found_index = 0;
	for (size_t index = 0; index < *num_indices; ++index) {
		
		if (indices->index_list[index].has_value) {
			
			unwrapped_indices[found_index] = indices->index_list[index].value;
			++found_index;
		}
	}
	
	assert(*num_indices == found_index);	
	return unwrapped_indices;
}

