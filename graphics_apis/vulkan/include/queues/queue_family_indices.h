#ifndef QUEUES_QUEUE_FAMILY_INDICES_H
#define QUEUES_QUEUE_FAMILY_INDICES_H

#include <stdbool.h>
#include <stdint.h>

#include <vulkan/vulkan.h>

#define MAX_INDICES 8

enum {
	
	GRAPHICS_FAMILY_INDEX = 0,
	PRESENT_FAMILY_INDEX,
};

typedef struct opetional_uint32_s {
	
	bool has_value;
	uint32_t value;
} optional_uint32_t;

typedef struct queue_family_indices_s {
	
	optional_uint32_t index_list[MAX_INDICES];
	size_t num_indices;

} queue_family_indices_t;

// Found in <renderers/vulkan/vulkan_renderer.h>
struct vulkan_renderer_s;
typedef struct vulkan_renderer_s vulkan_renderer_t;

void get_queue_families(vulkan_renderer_t *vk_renderer, VkPhysicalDevice device, queue_family_indices_t *indices);

bool is_exclusive_graphics(queue_family_indices_t *indices);

uint32_t *unwrap_indices(queue_family_indices_t *indices, size_t *num_indices);

#endif // QUEUES_QUEUE_FAMILY_INDICES_H

