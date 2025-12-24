#ifndef RENDERERS_VULKAN_QUEUES_REQUIRED_QUEUE_FAMILIES_H
#define RENDERERS_VULKAN_QUEUES_REQUIRED_QUEUE_FAMILIES_H

#define MAX_INDICIES 8

typedef struct required_queue_family_s {
	
	uint32_t family_index;
	bool (*condition_lambda)(VkQueueFamilyProperties *, ...);
	const char *debug_string;
} required_queue_family_t;

extern const required_queue_family_t REQUIRED_FAMILY_LIST[MAX_INDICIES];

size_t get_num_required_families(void);
bool has_required_queue_families(queue_family_indicies_t *indicies);

// ---------- Condition Lambdas ---------- //

bool has_graphics(VkQueueFamilyProperties *family, ...);
bool can_present(VkQueueFamilyProperties *family, ...);

#endif // RENDERERS_VULKAN_QUEUES_REQUIRED_QUEUE_FAMILIES_H

