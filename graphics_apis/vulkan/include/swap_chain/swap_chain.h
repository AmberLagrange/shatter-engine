#ifndef SWAP_CHAIN_SWAP_CHAIN_H
#define SWAP_CHAIN_SWAP_CHAIN_H

#include <common/core.h>

#include <vulkan_renderer.h>

#include <stdint.h>

shatter_status_t create_swap_chain(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_swap_chain(vulkan_renderer_t *vk_renderer);

shatter_status_t get_next_swap_chain_image(vulkan_renderer_t *vk_renderer, uint32_t *image_index);

#endif // SWAP_CHAIN_SWAP_CHAIN_H

