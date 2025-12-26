#include <common/core.h>

#include <devices/extensions.h>
#include <devices/logical.h>
#include <devices/logical_helper.h>
#include <queues/queue_family_indicies.h>

#include <stdbool.h>
#include <stdlib.h>

// ---------- Logical Device Functions ---------- //

shatter_status_t create_logical_device(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	log_trace("Creating logical device.\n");
	
	queue_family_indicies_t families = vk_renderer->queue_family_indicies;
	
	VkDeviceQueueCreateInfo *queue_create_info_list = malloc(sizeof(VkDeviceQueueCreateInfo) * families.num_indicies);
	size_t num_unique_families = 0;
	
	float queue_priority = 1.0f;
	for (size_t family_index = 0; family_index < families.num_indicies; ++family_index) {
		
		VkDeviceQueueCreateInfo queue_create_info = {
			
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			
			.queueFamilyIndex = families.index_list[family_index].value,
			.queueCount = 1,
			
			.pQueuePriorities = &queue_priority
		};
		
		if (!is_unique_queue_family(queue_create_info_list, queue_create_info, num_unique_families)) {
			
			continue;
		}
		
		queue_create_info_list[num_unique_families] = queue_create_info;
		++num_unique_families;
	}
	
	VkPhysicalDeviceFeatures device_features = { 0 };
	
	VkDeviceCreateInfo create_info = {
		
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		
		.queueCreateInfoCount = num_unique_families,
		.pQueueCreateInfos = queue_create_info_list,
		
		.pEnabledFeatures = &device_features,
		
		.enabledExtensionCount = get_num_required_extensions(),
		.ppEnabledExtensionNames = REQUIRED_EXTENSIONS,
		
		.enabledLayerCount = vk_renderer->num_validation_layers,
		.ppEnabledLayerNames = (const char * const *)vk_renderer->validation_layers,
	};
	
	if (vkCreateDevice(vk_renderer->physical_device, &create_info, NULL, &(vk_renderer->logical_device)) != VK_SUCCESS) {
		
		log_error("Failed to create a logical device.\n");
		return SHATTER_VULKAN_LOGICAL_DEVICE_INIT_FAILURE;
	}
	
	vkGetDeviceQueue(vk_renderer->logical_device, families.index_list[GRAPHICS_FAMILY_INDEX].value,
					 0, &(vk_renderer->graphics_queue));
	
	vkGetDeviceQueue(vk_renderer->logical_device, families.index_list[PRESENT_FAMILY_INDEX].value,
					 0, &(vk_renderer->present_queue));
	
	log_info("Created logical device.\n");
	free(queue_create_info_list);
	return SHATTER_SUCCESS;
}

// ---------- Logical Device Helper Functions ---------- //

bool is_unique_queue_family(VkDeviceQueueCreateInfo *queue_create_info_list,
							VkDeviceQueueCreateInfo new_queue_create_info,
							size_t num_queue_create_info) {
	
	for (size_t queue_create_info_index = 0; queue_create_info_index < num_queue_create_info; ++queue_create_info_index) {
		
		if (queue_create_info_list[queue_create_info_index].queueFamilyIndex == new_queue_create_info.queueFamilyIndex) {
			
			return false;
		}
	}
	
	return true;
}

