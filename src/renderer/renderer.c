#include <common/core.h>

#include <renderer/renderer.h>

#include <window/glfw.h>
#include <window/input.h>

#include <stdlib.h>

shatter_status_t init_renderer(renderer_t *renderer, renderer_config_t *renderer_config) {
	
	if (init_glfw()) {
		
		log_error("Failed to initialize GLFW.\n");
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	renderer->api_loader.filepath = renderer_config->api_filepath;
	
	if (load_library(&(renderer->api_loader))) {
		
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	init_api_t init_api_renderer = (init_api_t)(get_function(&(renderer->api_loader), "init_api_renderer"));
	
	if (!init_api_renderer) {
		
		return SHATTER_RENDERER_INIT_FAILURE;
	}
	
	return init_api_renderer(&(renderer->api_renderer), renderer_config, &(renderer->rendering_window));
}

shatter_status_t loop_renderer(renderer_t *renderer) {
	
	loop_api_t loop_api_renderer = (loop_api_t)(get_function(&(renderer->api_loader), "loop_api_renderer"));
	
	if (!loop_api_renderer) {
		
		return SHATTER_RENDERER_LOOP_FAILURE;
	}
	
	glfwSetKeyCallback(renderer->rendering_window, escape_callback);
	
	while (!glfwWindowShouldClose(renderer->rendering_window)) {
		
		glfwPollEvents();
		shatter_status_t status = loop_api_renderer(renderer->api_renderer);
		if (status) {
			
			return status;
		}
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_renderer(renderer_t *renderer) {
	
	cleanup_api_t cleanup_api_renderer = (cleanup_api_t)(get_function(&(renderer->api_loader), "cleanup_api_renderer"));
	
	if (!cleanup_api_renderer) {
		
		return SHATTER_RENDERER_CLEANUP_FAILURE;
	}
	
	int status = cleanup_api_renderer(renderer->api_renderer);
	
	if (unload_library(&(renderer->api_loader))) {
		
		return SHATTER_RENDERER_CLEANUP_FAILURE;
	}
	
	if (terminate_glfw()) {
		
		return SHATTER_RENDERER_CLEANUP_FAILURE;
	}
	
	return status;
}

shatter_status_t renderer_run(renderer_t *renderer, renderer_config_t *renderer_config) {
	
	int status = SHATTER_SUCCESS;
	
	if (init_renderer(renderer, renderer_config)) {
		
		log_error("Failed to initialize Renderer.\n");
		status = SHATTER_RENDERER_RUN_FAILURE;
		goto exit;
	}
	
	if (loop_renderer(renderer)) {
		
		log_error("Error occurred in the rendering loop.\n");
		status = SHATTER_RENDERER_RUN_FAILURE;
		goto cleanup;
	}

cleanup:
	
	if (cleanup_renderer(renderer)) {
		
		log_error("Failed to cleanup Renderer.\n");
		status = SHATTER_RENDERER_RUN_FAILURE;
		goto exit;
	}
	
exit:
	return status;
}


