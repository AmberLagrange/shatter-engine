#ifndef RENDERERS_VULKAN_QUEUES_QUEUE_FAMILY_INDICIES_H
#define RENDERERS_VULKAN_QUEUES_QUEUE_FAMILY_INDICIES_H

#include <stdbool.h>
#include <stdint.h>

#include <vulkan/vulkan.h>

enum {
	
	GRAPHICS_FAMILY_INDEX = 0,
	PRESENT_FAMILY_INDEX,
};

#define MAX_INDICIES 8

typedef struct opetional_uint32_s {
	
	bool has_value;
	uint32_t value;
} optional_uint32_t;

typedef struct queue_family_indicies_S {
	
	optional_uint32_t index_list[MAX_INDICIES];
	size_t num_indicies;

} queue_family_indicies_t;

// Found in <renderers/vulkan/renderer.h>
struct vulkan_renderer_s;
typedef struct vulkan_renderer_s vulkan_renderer_t;

void get_queue_families(vulkan_renderer_t *vk_renderer, VkPhysicalDevice device, queue_family_indicies_t *indicies);

bool is_complete(queue_family_indicies_t *indicies);

#endif // RENDERERS_VULKAN_QUEUES_QUEUE_FAMILY_INDICIES_H

