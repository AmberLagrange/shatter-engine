#ifndef SWAP_CHAIN_RENDER_PASS_H
#define SWAP_CHAIN_RENDER_PASS_H

#include <common/core.h>

#include <vulkan_renderer.h>

shatter_status_t create_render_pass(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_render_pass(vulkan_renderer_t *vk_renderer);

#endif // SWAP_CHAIN_RENDER_PASS_H

