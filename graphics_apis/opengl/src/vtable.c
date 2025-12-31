#include <dynamic_loader/vtable.h>

#include <opengl_renderer.h>

vtable_t api_vtable = {
	
	.init_api_renderer    = (init_api_renderer_t)(&init_opengl_renderer),
	.loop_api_renderer    = (loop_api_renderer_t)(&loop_opengl_renderer),
	.cleanup_api_renderer = (cleanup_api_renderer_t)(&cleanup_opengl_renderer),
	
	.api_frame_buffer_resize_callback = 
		(api_frame_buffer_resize_callback_t)(&opengl_frame_buffer_resize_callback),
};

