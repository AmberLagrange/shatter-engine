#ifndef SWAP_CHAIN_SYNC_OBJECTS_H
#define SWAP_CHAIN_SYNC_OBJECTS_H

#include <common/core.h>

#include <vulkan_renderer.h>

shatter_status_t create_sync_objects(vulkan_renderer_t *vk_renderer);
shatter_status_t create_fences(vulkan_renderer_t *vk_renderer);
shatter_status_t create_acquire_semaphores(vulkan_renderer_t *vk_renderer);
shatter_status_t create_submit_semaphores(vulkan_renderer_t *vk_renderer);

shatter_status_t cleanup_sync_objects(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_fences(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_acquire_semaphores(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_submit_semaphores(vulkan_renderer_t *vk_renderer);

shatter_status_t wait_for_in_flight_fence(vulkan_renderer_t *vk_renderer);

#endif // SWAP_CHAIN_SYNC_OBJECTS_H

