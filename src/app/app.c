#include <common/core.h>

#include <buffers/buffer_info.h>
#include <buffers/vertex_buffer_info.h>

#include <dynamic_loader/dynamic_loader.h>

#include <renderer/abstract_renderer.h>

#include <window/glfw.h>

#include <libgen.h> // dirname

#include <signal.h>
#include <stdint.h>
#include <stdlib.h> // EXIT_SUCCESS
#include <string.h>

// ---------- SIGINT Handling ---------- //

volatile bool sigint_signaled = false;

static void sigint_handler(int _) {
	
	UNUSED(_);
	sigint_signaled = true;
}

// ---------- Parsing Args ---------- //

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
		} else if (strcmp(argv[arg_index], "--enable-log-prefixes") == 0) {
			
			enable_log_prefixes();
		} else if (strcmp(argv[arg_index], "--enable-log-colors") == 0) {
			
			enable_log_colors();
		} else if (strcmp(argv[arg_index], "--enable-log-prefix-colors") == 0) {
			
			enable_log_colors();
			enable_log_prefixes();
			enable_log_prefix_colors();
		} else if (strcmp(argv[arg_index], "--enable-log-message-colors") == 0) {
			
			enable_log_colors();
			enable_log_message_colors();
		} else if (strcmp(argv[arg_index], "--test-logging") == 0) {
			
			test_logging();
		}
	}
}

// ---------- Application ---------- //

const vertex_t vertices[4] = {
	
	{ { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
	{ {  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
	{ {  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f } },
	{ { -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f } },
};

const uint32_t indices[6] = {
	
	0, 1, 2,
	2, 3, 0,
};

int main(int argc, char **argv) {
	
	parse_args(argc, argv);
	
	char binary_filepath[MAX_FILEPATH_LEN + 1];
	realpath(argv[0], binary_filepath);
	
	char directory_filepath[MAX_FILEPATH_LEN + 1];
	strncpy(directory_filepath, binary_filepath, MAX_FILEPATH_LEN);
	dirname(directory_filepath);
	size_t directory_filepath_len = strlen(directory_filepath);
	
	char vulkan_api_filepath[MAX_FILEPATH_LEN + 1];
	strncpy(vulkan_api_filepath, directory_filepath, MAX_FILEPATH_LEN);
	strncat(vulkan_api_filepath, "/", 1);
	strncat(vulkan_api_filepath, vulkan_api_library_filepath, MAX_FILEPATH_LEN - directory_filepath_len - 1);
	
	char opengl_api_filepath[MAX_FILEPATH_LEN + 1];
	strncpy(opengl_api_filepath, directory_filepath, MAX_FILEPATH_LEN);
	strncat(opengl_api_filepath, "/", 1);
	strncat(opengl_api_filepath, opengl_api_library_filepath, MAX_FILEPATH_LEN - directory_filepath_len - 1);
	
	dynamic_loader_t api_loader = {
		
		.filepath_list[VULKAN_API_INDEX] = vulkan_api_filepath,
		.filepath_list[OPENGL_API_INDEX] = opengl_api_filepath,
		
		.requested_api_index = VULKAN_API_INDEX,
	};
	buffer_info_t vertex_buffer_info = {
		
		.data = (void *)(vertices),
		.size = sizeof(vertices),
		.num_elements = sizeof(vertices) / sizeof(vertices[0]),
	};
	
	buffer_info_t index_buffer_info = {
		
		.data = (void *)(indices),
		.size = sizeof(indices),
		.num_elements = sizeof(indices) / sizeof(indices[0]),
	};
	
	renderer_properties_t renderer_properties = {
		
		.width  = 800,
		.height = 600,
		.title   = "Vulkan Renderer",
		
		.directory_filepath = directory_filepath,
		
		.vertex_buffer_info = &vertex_buffer_info,
		.index_buffer_info  = &index_buffer_info,
	};
	
	abstract_renderer_t renderer = {
		
		.properties = &renderer_properties,
		.api_loader = &api_loader,
	};
	
	init_glfw();
	
	int status = SHATTER_SUCCESS;
	
	if (load_library(renderer.api_loader)) {
		
		status = SHATTER_RENDERER_RUN_FAILURE;
		goto exit;
	}
	
	if (init_abstract_renderer(&renderer)) {
		
		log_error("Failed to initialize Renderer.\n");
		status = SHATTER_RENDERER_RUN_FAILURE;
		goto cleanup;
	}
	
	signal(SIGINT, &sigint_handler);	
	
	if (loop_abstract_renderer(&renderer)) {
		
		log_error("Error occurred in the rendering loop.\n");
		status = SHATTER_RENDERER_RUN_FAILURE;
	}
	
cleanup:
	if (cleanup_abstract_renderer(&renderer)) {
		
		log_error("Failed to cleanup Renderer.\n");
		status = SHATTER_RENDERER_RUN_FAILURE;
		goto exit;
	}
	
exit:
	terminate_glfw();
	
	return status;
}

