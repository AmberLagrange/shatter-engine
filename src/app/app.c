#include <common/core.h>

#include <renderers/renderer.h>

#include <stdlib.h> // EXIT_SUCCESS
#include <string.h>

void parse_args(int argc, char **argv) {
	
	for (size_t arg_index = 1; arg_index < (size_t)argc; ++arg_index) {
		
		if (strcmp(argv[arg_index], "--verbose") == 0) {
			
			init_logging();
			enable_logging();
		}
		
		if (strcmp(argv[arg_index], "--enable-prefix") == 0) {
			
			enable_log_prefixes();
		}
		
		if (strcmp(argv[arg_index], "--enable-color") == 0) {
			
			enable_log_colors();
			enable_log_prefix_colors();
			enable_log_message_colors();
		}
		
		if (strcmp(argv[arg_index], "--test-logging") == 0) {
			
			test_logging();
		}
	}
}

int main(int argc, char **argv) {
	
	parse_args(argc, argv);
	
	int status = EXIT_SUCCESS;
	
	renderer_t renderer;
	renderer_config_t config = { 800, 600, "Vulkan Renderer" };
	
	if (renderer_init(&renderer, &config)) {
		
		log_error("Failed to initialize Renderer.\n");
		status = EXIT_FAILURE;
		goto exit;
	}
	
	if (renderer_loop(&renderer)) {
		
		log_error("Error occurred in the rendering loop.\n");
		status = EXIT_FAILURE;
		goto cleanup;
	}

cleanup:
	
	if (renderer_cleanup(&renderer)) {
		
		log_error("Failed to cleanup Vulkan Renderer.\n");
		status = EXIT_FAILURE;
		goto exit;
	}
	
exit:
	return status;
}

