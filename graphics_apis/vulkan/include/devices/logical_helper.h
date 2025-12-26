#ifndef DEVICES_LOGICAL_HELPER_H
#define DEVICES_LOGICAL_HELPER_H

#include <stdbool.h>

bool is_unique_queue_family(VkDeviceQueueCreateInfo *queue_create_info_list,
							VkDeviceQueueCreateInfo new_queue_create_info,
							size_t num_queue_create_info);

#endif // DEVICES_LOGICAL_HELPER_H

