#include <dynamic_loader/vtable.h>

#include <opengl_renderer.h>

vtable_t api_vtable = {
	
	.create_api_renderer  = (create_api_renderer_t)(&create_opengl_renderer),
	.init_api_renderer    = (init_api_renderer_t)(&init_opengl_renderer),
	.loop_api_renderer    = (loop_api_renderer_t)(&loop_opengl_renderer),
	.cleanup_api_renderer = (cleanup_api_renderer_t)(&cleanup_opengl_renderer),
	
	.submit_api_vertex_info = (submit_api_buffer_info_t)(&submit_opengl_vertex_info),
	.submit_api_index_info  = (submit_api_buffer_info_t)(&submit_opengl_index_info),
	.submit_api_uniform_buffer_info = (submit_api_buffer_info_t)(&submit_opengl_uniform_buffer_info),
	
	.api_frame_buffer_resize_callback = 
		(api_frame_buffer_resize_callback_t)(&opengl_frame_buffer_resize_callback),
};

