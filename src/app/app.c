#include <common/core.h>

#include <renderers/renderer.h>

#include <stdlib.h> // EXIT_SUCCESS
#include <string.h>

int main(int argc, char **argv) {
	
	int status = EXIT_SUCCESS;
	
	renderer_t renderer;
	renderer_config_t config = { 800, 600, "Vulkan Renderer" };
	
	if (argc >= 2 && strcmp(argv[1], "--verbose") == 0) {
		
		enable_logging();
	}
	
	if (renderer_init(&renderer, &config)) {
		
		log_message(stderr, "\nFailed to initialize Renderer.\n");
		status = EXIT_FAILURE;
		goto exit;
	}
	
	if (renderer_loop(&renderer)) {
		
		log_message(stderr, "\nError occurred in the rendering loop.\n");
		status = EXIT_FAILURE;
		goto cleanup;
	}

cleanup:
	
	if (renderer_cleanup(&renderer)) {
		
		log_message(stderr, "\nFailed to cleanup Vulkan Renderer.\n");
		status = EXIT_FAILURE;
		goto exit;
	}
	
exit:
	return status;
}

