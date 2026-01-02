#ifndef GRAPHICS_BUFFERS_VERTEX_BUFFER_H
#define GRAPHICS_BUFFERS_VERTEX_BUFFER_H

#include <vulkan_renderer.h>

#include <vulkan/vulkan.h>

#include <cglm/cglm.h>

#include <stddef.h>

typedef enum attribute_indices_e {
	
	VERTEX_POSITION_ATTRIBUTE = 0,
	VERTEX_COLOR_ATTRIBUTE,
	
	NUM_VERTEX_ATTRIBUTES
} attribute_indicies_t;

typedef struct vertex_s {
	
	vec2 pos;
	vec3 color;
} vertex_t;

extern const vertex_t vertices[3];

shatter_status_t get_vertex_binding_description(VkVertexInputBindingDescription *binding_description);

// The number of attributes should be the same, so we don't need to know the size of the array
shatter_status_t get_vertex_attribute_descriptions(VkVertexInputAttributeDescription *attribute_description_list);

shatter_status_t create_vertex_buffer(vulkan_renderer_t *vk_renderer);
shatter_status_t cleanup_vertex_buffer(vulkan_renderer_t *vk_renderer);

#endif // GRAPHICS_BUFFERS_VERTEX_BUFFER_H

