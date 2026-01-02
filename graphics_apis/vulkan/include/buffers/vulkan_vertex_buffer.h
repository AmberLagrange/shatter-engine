#ifndef BUFFERS_VULKAN_VERTEX_BUFFER_H
#define BUFFERS_VULKAN_VERTEX_BUFFER_H

#include <vulkan_renderer.h>

#include <buffers/buffer_info.h>
#include <buffers/vulkan_buffer.h>

#include <vulkan/vulkan.h>

shatter_status_t get_vertex_binding_description(VkVertexInputBindingDescription *binding_description);

// The number of attributes should be the same, so we don't need to know the size of the array
shatter_status_t get_vertex_attribute_descriptions(VkVertexInputAttributeDescription *attribute_description_list);

shatter_status_t create_vulkan_vertex_buffer(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_vulkan_vertex_buffer(vulkan_renderer_t *vk_renderer);

#endif // BUFFERS_VULKAN_VERTEX_BUFFER_H

