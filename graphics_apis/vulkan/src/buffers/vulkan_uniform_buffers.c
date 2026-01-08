#include <common/core.h>

#include <buffers/uniform_buffer_object.h>
#include <buffers/vulkan_buffer.h>
#include <buffers/vulkan_uniform_buffers.h>

#include <vulkan/vulkan.h>

#include <stdlib.h>

shatter_status_t create_vulkan_uniform_buffers(vulkan_renderer_t *vk_renderer) {
	
	VkDeviceSize uniform_buffer_size = sizeof(uniform_buffer_object_t);
	
	vk_renderer->num_uniform_buffers = vk_renderer->num_in_flight_frames;
	vk_renderer->uniform_buffer_list = malloc(sizeof(vulkan_buffer_t) * vk_renderer->num_uniform_buffers);
	vk_renderer->uniform_buffer_memory_map_list = malloc(sizeof(uniform_buffer_object_t *) * vk_renderer->num_uniform_buffers);
	
	for (size_t buffer_index = 0; buffer_index < vk_renderer->num_in_flight_frames; ++buffer_index) {
		
		vulkan_buffer_t *curr_buffer = &(vk_renderer->uniform_buffer_list[buffer_index]);
		curr_buffer->size = uniform_buffer_size;
		curr_buffer->usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		curr_buffer->properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		
		create_vulkan_buffer(vk_renderer, curr_buffer, vk_renderer->uniform_buffer_info);
		allocate_buffer_memory(vk_renderer, curr_buffer);
		
		map_buffer_memory(vk_renderer, curr_buffer, uniform_buffer_size,
						  (void **)(&(vk_renderer->uniform_buffer_memory_map_list[buffer_index])));
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_vulkan_uniform_buffers(vulkan_renderer_t *vk_renderer) {
	
	for (size_t buffer_index = 0; buffer_index < vk_renderer->num_uniform_buffers; ++buffer_index) {
		
		cleanup_vulkan_buffer(vk_renderer, &(vk_renderer->uniform_buffer_list[buffer_index]));
	}
	
	free(vk_renderer->uniform_buffer_memory_map_list);
	free(vk_renderer->uniform_buffer_list);
	
	return SHATTER_SUCCESS;
}

