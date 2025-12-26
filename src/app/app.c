#include <common/core.h>

#include <renderer/renderer.h>

#include <stdlib.h> // EXIT_SUCCESS
#include <string.h>

void parse_args(int argc, char **argv) {
	
	for (size_t arg_index = 1; arg_index < (size_t)argc; ++arg_index) {
		
		// Not the most elegant solution, but good enough
		if (strcmp(argv[arg_index], "--log-level=debug") == 0) {
			
			enable_logging();
			init_logging();
			set_log_level(LOG_LEVEL_DEBUG);
		} else if (strcmp(argv[arg_index], "--log-level=trace") == 0) {
			
			enable_logging();
			init_logging();
			set_log_level(LOG_LEVEL_TRACE);
		} else if (strcmp(argv[arg_index], "--log-level=info") == 0) {
			
			enable_logging();
			init_logging();
			set_log_level(LOG_LEVEL_INFO);
		} else if (strcmp(argv[arg_index], "--log-level=warning") == 0) {
			
			enable_logging();
			init_logging();
			set_log_level(LOG_LEVEL_WARNING);
		} else if (strcmp(argv[arg_index], "--log-level=error") == 0) {
			
			enable_logging();
			init_logging();
			set_log_level(LOG_LEVEL_ERROR);
		} else if (strcmp(argv[arg_index], "--log-level=critical") == 0) {
			
			enable_logging();
			init_logging();
			set_log_level(LOG_LEVEL_CRITICAL);
		} else if (strcmp(argv[arg_index], "--enable-prefix") == 0) {
			
			enable_log_prefixes();
		} else if (strcmp(argv[arg_index], "--enable-color") == 0) {
			
			enable_log_colors();
			enable_log_prefix_colors();
			enable_log_message_colors();
		} else if (strcmp(argv[arg_index], "--test-logging") == 0) {
			
			test_logging();
		}
	}
}

int main(int argc, char **argv) {
	
	parse_args(argc, argv);
	
	int status = EXIT_SUCCESS;
	
	renderer_t renderer;
	renderer_config_t config = { 800, 600, "Vulkan Renderer" };
	
	if (init_renderer(&renderer, &config)) {
		
		log_error("Failed to initialize Renderer.\n");
		status = EXIT_FAILURE;
		goto exit;
	}
	
	if (loop_renderer(&renderer)) {
		
		log_error("Error occurred in the rendering loop.\n");
		status = EXIT_FAILURE;
		goto cleanup;
	}

cleanup:
	
	if (cleanup_renderer(&renderer)) {
		
		log_error("Failed to cleanup Vulkan Renderer.\n");
		status = EXIT_FAILURE;
		goto exit;
	}
	
exit:
	return status;
}

