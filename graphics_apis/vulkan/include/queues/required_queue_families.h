#ifndef QUEUES_REQUIRED_QUEUE_FAMILIES_H
#define QUEUES_REQUIRED_QUEUE_FAMILIES_H

#include <queues/queue_family_indices.h>

#include <vulkan/vulkan.h>

typedef struct required_queue_family_s {
	
	uint32_t family_index;
	bool (*condition_lambda)(VkQueueFamilyProperties *, ...);
	const char *debug_string;
} required_queue_family_t;

extern const required_queue_family_t REQUIRED_FAMILY_LIST[MAX_INDICES];

size_t get_num_required_families(void);
bool has_required_queue_families(queue_family_indices_t *indices);

// ---------- Condition Lambdas ---------- //

bool has_graphics(VkQueueFamilyProperties *family, ...);
bool can_present(VkQueueFamilyProperties *family, ...);

#endif // QUEUES_REQUIRED_QUEUE_FAMILIES_H

