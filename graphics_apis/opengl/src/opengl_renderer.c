#include <common/core.h>

#include <opengl_renderer.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define MAX_INFO_LOG_LEN 512

shatter_status_t init_window(opengl_renderer_t *opengl_renderer) {
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	opengl_renderer->properties->rendering_window =
		glfwCreateWindow(opengl_renderer->properties->width, opengl_renderer->properties->height,
						 opengl_renderer->properties->title, NULL, NULL);
	
	if (!opengl_renderer->properties->rendering_window) {
		
		log_error("Failed to create GWLFW window.\n");
		return SHATTER_GLFW_WINDOW_FAILURE;
	}
	
	glfwMakeContextCurrent(opengl_renderer->properties->rendering_window);
	return SHATTER_SUCCESS;
}

shatter_status_t init_glad(void) {
	
	if (!gladLoadGL()) {
		
		log_critical("Failed to load bindings from GLAD.\n");
		return SHATTER_OPENGL_GLAD_INIT_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t get_absolute_filepath(opengl_renderer_t *opengl_renderer, const char *filepath, char *absolute_filepath) {
	
	size_t directory_filepath_len = strlen(opengl_renderer->properties->directory_filepath);
	assert(directory_filepath_len < MAX_FILEPATH_LEN);
	
	strncpy(absolute_filepath, opengl_renderer->properties->directory_filepath, MAX_FILEPATH_LEN);
	strncat(absolute_filepath, filepath, MAX_FILEPATH_LEN - directory_filepath_len);
	return SHATTER_SUCCESS;
}

shatter_status_t create_shader(unsigned int *shader, const char *shader_filepath, GLenum shader_type) {
	
	FILE *file_ptr = fopen(shader_filepath, "r");
	
	if (!file_ptr) {
		
		return SHATTER_OPENGL_SHADER_READ_FAILURE;
	}
	
	fseek(file_ptr, 0L, SEEK_END);
	size_t file_size = ftell(file_ptr);
	fseek(file_ptr, 0L, SEEK_SET);
	
	char *shader_code = malloc(file_size + 1);
	fread(shader_code, sizeof(char), file_size, file_ptr);
	shader_code[file_size] = '\0';
	fclose(file_ptr);
	
	*shader = glCreateShader(shader_type);
	glShaderSource(*shader, 1, (const char **)(&shader_code), NULL);
	glCompileShader(*shader);
	free(shader_code);
	
	int success;
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		
		char info_log[MAX_INFO_LOG_LEN];
		glGetShaderInfoLog(*shader, MAX_INFO_LOG_LEN, NULL, info_log);
		log_error("Shader failed to compile.\n");
		log_error("%s", info_log);
		return SHATTER_OPENGL_SHADER_INIT_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t create_shader_program(opengl_renderer_t *opengl_renderer,
									   unsigned int vertex_shader, unsigned int fragment_shader) {
	
	opengl_renderer->shader_program = glCreateProgram();
	glAttachShader(opengl_renderer->shader_program, vertex_shader);
	glAttachShader(opengl_renderer->shader_program, fragment_shader);
	glLinkProgram(opengl_renderer->shader_program);
	
	int success;
	glGetProgramiv(opengl_renderer->shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		
		char info_log[MAX_INFO_LOG_LEN];
		glGetProgramInfoLog(opengl_renderer->shader_program, MAX_INFO_LOG_LEN, NULL, info_log);
		log_error("Shader failed to link.\n");
		log_error("%s", info_log);
		return SHATTER_OPENGL_SHADER_PROGRAM_INIT_FAILURE;
	}
	
	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);
	
	return SHATTER_SUCCESS;
}

shatter_status_t init_vertex_arrays(opengl_renderer_t *opengl_renderer) {
	
	glGenVertexArrays(1, &(opengl_renderer->vertex_array_object));
	glBindVertexArray(opengl_renderer->vertex_array_object);
	
	return SHATTER_SUCCESS;
}

shatter_status_t init_vertex_buffers(opengl_renderer_t *opengl_renderer) {
	
	glBindVertexArray(opengl_renderer->vertex_array_object);

	glGenBuffers(1, &(opengl_renderer->vertex_buffer_object));
	glBindBuffer(GL_ARRAY_BUFFER, opengl_renderer->vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, opengl_renderer->properties->vertex_buffer_info->size,
				 opengl_renderer->properties->vertex_buffer_info->data, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
	
	return SHATTER_SUCCESS;
}

shatter_status_t init_index_buffers(opengl_renderer_t *opengl_renderer) {
	
	glGenBuffers(1, &(opengl_renderer->index_buffer_object));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, opengl_renderer->index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, opengl_renderer->properties->index_buffer_info->size,
				 opengl_renderer->properties->index_buffer_info->data, GL_STATIC_DRAW);
	
	return SHATTER_SUCCESS;
}

shatter_status_t init_opengl_renderer(opengl_renderer_t **opengl_renderer_ptr, renderer_properties_t *properties) {
	
	log_trace("\n");
	log_trace("Initializing opengl renderer.\n");
	
	*opengl_renderer_ptr = malloc(sizeof(opengl_renderer_t));
	opengl_renderer_t *opengl_renderer = *opengl_renderer_ptr;
	
	opengl_renderer->properties = properties;
	
	if (init_window(opengl_renderer)) {
		
		return SHATTER_OPENGL_RENDERER_INIT_FAILURE;
	}

	if (init_glad()) {
		
		return SHATTER_OPENGL_RENDERER_INIT_FAILURE;
	}
	
	unsigned int vertex_shader;
	char vertex_filepath[MAX_FILEPATH_LEN + 1];
	get_absolute_filepath(opengl_renderer, "/shaders/vertex/basic_shader.vert", vertex_filepath);
	
	if (create_shader(&vertex_shader, vertex_filepath, GL_VERTEX_SHADER)) {
		
		return SHATTER_OPENGL_RENDERER_INIT_FAILURE;
	}
	
	unsigned int fragment_shader;
	char fragment_filepath[MAX_FILEPATH_LEN + 1];
	get_absolute_filepath(opengl_renderer, "/shaders/fragment/basic_shader.frag", fragment_filepath);
	
	if (create_shader(&fragment_shader, fragment_filepath, GL_FRAGMENT_SHADER)) {
		
		return SHATTER_OPENGL_RENDERER_INIT_FAILURE;
	}
	
	if (create_shader_program(opengl_renderer, vertex_shader, fragment_shader)) {
		
		return SHATTER_OPENGL_RENDERER_INIT_FAILURE;
	}
	
	if (init_vertex_arrays(opengl_renderer)) {
		
		return SHATTER_OPENGL_RENDERER_INIT_FAILURE;
	}
	
	if (init_vertex_buffers(opengl_renderer)) {
		
		return SHATTER_OPENGL_RENDERER_INIT_FAILURE;
	}
	
	if (init_index_buffers(opengl_renderer)) {
		
		return SHATTER_OPENGL_RENDERER_INIT_FAILURE;
	}
	
	log_trace("Initialized opengl renderer.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t loop_opengl_renderer(opengl_renderer_t *opengl_renderer) {
	
	glUseProgram(opengl_renderer->shader_program);
	glBindVertexArray(opengl_renderer->vertex_array_object);
	glDrawElements(GL_TRIANGLES, opengl_renderer->properties->index_buffer_info->num_elements, GL_UNSIGNED_INT, (void *)(0));
	glfwSwapBuffers(opengl_renderer->properties->rendering_window);
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_opengl_renderer(opengl_renderer_t *opengl_renderer) {
	
	log_trace("\n");
	log_trace("Cleaning up opengl renderer.\n");
	
	glDeleteBuffers(1, &(opengl_renderer->vertex_buffer_object));
	glDeleteVertexArrays(1, &(opengl_renderer->vertex_array_object));
	glDeleteProgram(opengl_renderer->shader_program);
	
	glfwDestroyWindow(opengl_renderer->properties->rendering_window);
	free(opengl_renderer);
	
	log_trace("Cleaned up opengl renderer.\n");
	return SHATTER_SUCCESS;
}

// ---------- Callbacks ---------- //

shatter_status_t opengl_frame_buffer_resize_callback(opengl_renderer_t *opengl_renderer) {
	
	UNUSED(opengl_renderer);
	return SHATTER_SUCCESS;
}

