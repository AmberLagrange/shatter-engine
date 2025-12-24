#include <renderers/vulkan/devices/logical.h>
#include <renderers/vulkan/queues/queue_family_indicies.h>

#include <logging/logger.h>

int create_logical_device(vulkan_renderer_t *vk_renderer) {
	
	queue_family_indicies indicies;
	get_queue_families(vk_renderer, vk_renderer->physical_device, &indicies);
	
	float queue_priority = 1.0f;
	
	VkDeviceQueueCreateInfo queue_create_info = {
		
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		
		.queueFamilyIndex = indicies.index_list[GRAPHICS_FAMILY_INDEX].value,
		.queueCount = 1,
		
		.pQueuePriorities = &queue_priority
	};
	
	VkPhysicalDeviceFeatures device_features = { 0 };
	
	VkDeviceCreateInfo create_info = {
		
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		
		.pQueueCreateInfos = &queue_create_info,
		.queueCreateInfoCount = 1,
		
		.pEnabledFeatures = &device_features,
		.enabledExtensionCount = 0,
		
		.enabledLayerCount = vk_renderer->num_validation_layers,
		.ppEnabledLayerNames = (const char * const *)vk_renderer->validation_layers,
	};
	
	if (vkCreateDevice(vk_renderer->physical_device, &create_info, NULL, &(vk_renderer->logical_device)) != VK_SUCCESS) {
		
		log_message(stderr, "Failed to create a logical device.\n");
		return SHATTER_LOGICAL_DEVICE_INIT_FAILURE;
	}
	
	vkGetDeviceQueue(vk_renderer->logical_device, indicies.index_list[GRAPHICS_FAMILY_INDEX].value,
					 0, &(vk_renderer->graphics_queue));
	
	log_message(stdout, "Created logical device.\n");
	return SHATTER_LOGICAL_DEVICE_SUCCESS;
}

