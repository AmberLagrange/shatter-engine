#include <common/core.h>

#include <renderers/vulkan/devices/extensions.h>
#include <renderers/vulkan/devices/physical.h>

#include <renderers/vulkan/queues/queue_family_indicies.h>
#include <renderers/vulkan/queues/required_queue_families.h>

#include <renderers/vulkan/swap_chain/swap_chain_support_details.h>

#include <stdlib.h>

shatter_status_t choose_physical_device(vulkan_renderer_t *vk_renderer) {
	
	log_trace("\n");
	
	int status = SHATTER_SUCCESS;
	
	uint32_t num_devices = 0;
	vkEnumeratePhysicalDevices(vk_renderer->vulkan_instance, &num_devices, NULL);
	
	if (num_devices == 0) {
		
		log_error("Unable to find any GPUs with Vulkan support.\n");
		status = SHATTER_VULKAN_PHYSICAL_DEVICE_CHOICE_FAILURE;
		goto exit;
	}
	
	VkPhysicalDevice *device_list = malloc(sizeof(VkPhysicalDevice) * num_devices);
	vkEnumeratePhysicalDevices(vk_renderer->vulkan_instance, &num_devices, device_list);
	
	VkPhysicalDeviceProperties device_properties;
	for (size_t device_index = 0; device_index < num_devices; ++device_index) {
		
		VkPhysicalDevice device = device_list[device_index];
		vkGetPhysicalDeviceProperties(device, &device_properties);
		
		log_trace("Checking physical device:\n");
		log_trace("\t%s\n", device_properties.deviceName);
		if (is_physical_device_suitable(vk_renderer, device)) {
			
			vk_renderer->physical_device = device;
			break;
		}
	}
	
	if (vk_renderer->physical_device == VK_NULL_HANDLE) {
		
		log_error("Failed to find a suitable GPU.\n");
		status = SHATTER_VULKAN_PHYSICAL_DEVICE_CHOICE_FAILURE;
		goto cleanup;
	}
	
	log_info("Physical device selected:\n");
	log_info("\t%s\n", device_properties.deviceName);
	
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
	
	queue_family_indicies_t family_indicies;
	get_queue_families(vk_renderer, device, &family_indicies);
	
	if (device_properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		
		log_error("Device is not a discrete gpu.\n");
		return false;
	}
	
	if (!device_features.geometryShader) {
		
		log_error("Device does not have a geometry shader feature.\n");
		return false;
	}
	
	if (!has_required_queue_families(&family_indicies)) {
		
		log_error("Device does not have all required queue families.\n");
		return false;
	}
	
	if (!check_device_extension_support(device)) {
		
		log_error("Device does not support required extensions.\n");
		return false;
	}
	
	swap_chain_support_details_t support_details;
	query_swap_chain_support(&support_details, vk_renderer, device);
	
	if ((support_details.num_surface_formats == 0) || (support_details.num_present_modes == 0)) {
		
		log_error("Device does not have adequate swap chain support.\n");
		return false;
	}
	
	vk_renderer->queue_family_indicies = family_indicies;
	
	return true;
}

