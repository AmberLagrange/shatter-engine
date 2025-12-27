#include <dynamic_loader/vtable.h>

#include <vulkan_renderer.h>

vtable_t api_vtable = {
	
	.init_api_renderer    = (init_api_renderer_t)(&init_vulkan_renderer),
	.loop_api_renderer    = (loop_api_renderer_t)(&loop_vulkan_renderer),
	.cleanup_api_renderer = (cleanup_api_renderer_t)(&cleanup_vulkan_renderer),
};

