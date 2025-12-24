#ifndef RENDERERS_VULKAN_QUEUES_QUEUE_FAMILY_INDICIES_H
#define RENDERERS_VULKAN_QUEUES_QUEUE_FAMILY_INDICIES_H

#include <renderers/vulkan/renderer.h>

#include <vulkan/vulkan.h>

#include <stdbool.h>
#include <stdint.h>

enum {
	
	GRAPHICS_FAMILY_INDEX = 0,
	PRESENT_FAMILY_INDEX,
};

#define MAX_INDICIES 8

typedef struct {
	
	bool has_value;
	uint32_t value;
} optional_uint32_t;

typedef struct {
	
	optional_uint32_t index_list[MAX_INDICIES];
	size_t num_indicies;

} queue_family_indicies;

void get_queue_families(vulkan_renderer_t *vk_renderer, VkPhysicalDevice device, queue_family_indicies *indicies);

bool is_complete(queue_family_indicies *indicies);

#endif // RENDERERS_VULKAN_QUEUES_QUEUE_FAMILY_INDICIES_H

