#include <common/core.h>

#include <renderers/vulkan/devices/physical.h>
#include <renderers/vulkan/queues/queue_family_indicies.h>

#include <stdlib.h>

shatter_status_t choose_physical_device(vulkan_renderer_t *vk_renderer) {
	
	log_message(stdout, "Checking physical devices.\n");
	int status = SHATTER_SUCCESS;
	
	uint32_t num_devices = 0;
	vkEnumeratePhysicalDevices(vk_renderer->vulkan_instance, &num_devices, NULL);
	
	if (num_devices == 0) {
		
		log_message(stderr, "Unable to find any GPUs with Vulkan support.\n");
		status = SHATTER_VULKAN_PHYSICAL_DEVICE_CHOICE_FAILURE;
		goto exit;
	}
	
	VkPhysicalDevice *device_list = malloc(sizeof(VkPhysicalDevice) * num_devices);
	vkEnumeratePhysicalDevices(vk_renderer->vulkan_instance, &num_devices, device_list);
	
	for (size_t device_index = 0; device_index < num_devices; ++device_index) {
		
		VkPhysicalDevice device = device_list[device_index];
		if (is_physical_device_suitable(vk_renderer, device)) {
			
			vk_renderer->physical_device = device;
			break;
		}
	}
	
	if (vk_renderer->physical_device == VK_NULL_HANDLE) {
		
		log_message(stderr, "Failed to find a suitable GPU.\n");
		status = SHATTER_VULKAN_PHYSICAL_DEVICE_CHOICE_FAILURE;
		goto cleanup;
	}
	
	VkPhysicalDeviceProperties device_properties;
	vkGetPhysicalDeviceProperties(vk_renderer->physical_device, &device_properties);
	
	log_message(stdout, "Physical device:\n\t%s\n", device_properties.deviceName);
	
cleanup:
	free(device_list);
exit:
	return status;
}

bool is_physical_device_suitable(vulkan_renderer_t *vk_renderer, VkPhysicalDevice device) {
	
	VkPhysicalDeviceProperties device_properties;
	vkGetPhysicalDeviceProperties(device, &device_properties);

	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceFeatures(device, &device_features);
	
	queue_family_indicies indicies;
	get_queue_families(vk_renderer, device, &indicies);
	
	return device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
		&& device_features.geometryShader
		&& is_complete(&indicies);
}

