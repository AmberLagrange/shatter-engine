#include <common/core.h>

#include <buffers/buffer_info.h>
#include <buffers/uniform_buffer_object.h>
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

char *get_abs_path(const char *directory_filepath, const char *relative_filepath, char *string_buffer, size_t buffer_len) {
	
	size_t directory_len = strlen(directory_filepath);
	
	strncpy(string_buffer, directory_filepath, buffer_len);
	strncat(string_buffer, "/", buffer_len - directory_len);
	strncat(string_buffer, relative_filepath, buffer_len - directory_len - 1);
	
	return string_buffer;
}

int main(int argc, char **argv) {
	
	parse_args(argc, argv);
	
	char directory_filepath[MAX_FILEPATH_LEN + 1];
	realpath(argv[0], directory_filepath);
	dirname(directory_filepath);
	
	char vulkan_api_filepath[MAX_FILEPATH_LEN + 1];
	get_abs_path(directory_filepath, vulkan_api_library_filepath, vulkan_api_filepath, MAX_FILEPATH_LEN);
	
	char opengl_api_filepath[MAX_FILEPATH_LEN + 1];
	get_abs_path(directory_filepath, opengl_api_library_filepath, opengl_api_filepath, MAX_FILEPATH_LEN);
	
	dynamic_loader_t api_loader = {
		
		.filepath_list[VULKAN_API_INDEX] = vulkan_api_filepath,
		.filepath_list[OPENGL_API_INDEX] = opengl_api_filepath,
		
		.requested_api_index = VULKAN_API_INDEX,
	};
	
	renderer_properties_t renderer_properties = {
		
		.width  = 800,
		.height = 600,
		.title   = "Vulkan Renderer",
		
		.directory_filepath = directory_filepath,
	};
	
	buffer_info_t vertex_info = {
		
		.data = (void *)(vertices),
		.size = sizeof(vertices),
		.num_elements = sizeof(vertices) / sizeof(vertices[0]),
	};
	
	buffer_info_t index_info = {
		
		.data = (void *)(indices),
		.size = sizeof(indices),
		.num_elements = sizeof(indices) / sizeof(indices[0]),
	};
	
	uniform_buffer_object_t ubo;
	
	glm_mat4_identity(ubo.model);
	
	vec3 eye    = { 0.0f, 0.0f, 2.0f };
	vec3 center = { 0.0f, 0.0f, 0.0f };
	vec3 up     = { 0.0f, 1.0f, 0.0f };
	
	glm_mat4_identity(ubo.view);
	glm_lookat(eye, center, up, ubo.view);
	
	glm_mat4_identity(ubo.projection);
	glm_perspective(glm_rad(45.0f), (float)renderer_properties.width / (float)renderer_properties.height,
					0.1f, 10.0f, ubo.projection);
	
	buffer_info_t uniform_buffer_info = {
		
		.data = (void *)(&ubo),
		.size = sizeof(ubo),
		.num_elements = 1,
	};
	
	abstract_renderer_t renderer = {
		
		.properties = &renderer_properties,
		.api_loader = &api_loader,
		
		.vertex_info = &vertex_info,
		.index_info = &index_info,
		.uniform_buffer_info = &uniform_buffer_info,
	};
	
	init_glfw();
	
	int status = SHATTER_SUCCESS;
	
	if (load_library(renderer.api_loader)) {
		
		status = SHATTER_RENDERER_RUN_FAILURE;
		goto exit;
	}
	
	create_abstract_renderer(&renderer);
	
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

