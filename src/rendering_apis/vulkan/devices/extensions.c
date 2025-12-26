#include <common/core.h>

#include <rendering_apis/vulkan/devices/extensions.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <vulkan/vulkan.h>

const char *REQUIRED_EXTENSIONS[MAX_REQUIRED_EXTENSIONS] = {
	
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

size_t get_num_required_extensions(void) {
	
	size_t extension_index = 0;
	
	while (REQUIRED_EXTENSIONS[extension_index]) {
		
		++extension_index;
	}
	
	return extension_index;
}

bool check_device_extension_support(VkPhysicalDevice device) {
	
	uint32_t num_extensions = 0;
	vkEnumerateDeviceExtensionProperties(device, NULL, &num_extensions, NULL);
	
	VkExtensionProperties *extension_list = malloc(sizeof(VkExtensionProperties) * num_extensions);
	vkEnumerateDeviceExtensionProperties(device, NULL, &num_extensions, extension_list);
	
	uint32_t num_required_extensions = get_num_required_extensions();
	
	log_trace("Required extensions:\n");
	
	bool has_all_extensions = true;
	for (size_t required_index = 0; required_index < num_required_extensions; ++required_index) {
		
		bool has_extension = false;
		
		const char *required_name = REQUIRED_EXTENSIONS[required_index];
		log_trace("\t%s\n", required_name);
		
		for (size_t extension_index = 0; extension_index < num_extensions; ++extension_index) {
			
			if (strcmp(extension_list[extension_index].extensionName, required_name) == 0) {
				
				has_extension = true;
				break;
			}
		}
		
		if (!has_extension) {
			
			log_error("Could not find extension %s.\n", required_name);
			has_all_extensions = false;
		}
	}
	
	free(extension_list);
	
	if (has_all_extensions) {
		
		log_info("Has all the required extensions.\n");
	}
	return has_all_extensions;
}

