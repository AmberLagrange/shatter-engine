#include <common/core.h>

#include <opengl_renderer.h>

#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define MAX_INFO_LOG_LEN 512

shatter_status_t init_opengl_renderer(opengl_renderer_t **opengl_renderer_ptr, renderer_config_t *renderer_config) {
	
	log_trace("\n");
	log_trace("Initializing opengl renderer.\n");
	
	*opengl_renderer_ptr = malloc(sizeof(opengl_renderer_t));
	opengl_renderer_t *opengl_renderer = *opengl_renderer_ptr;
	
	opengl_renderer->renderer_config = renderer_config;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	opengl_renderer->renderer_config->rendering_window =
		glfwCreateWindow(opengl_renderer->renderer_config->width, opengl_renderer->renderer_config->height,
						 opengl_renderer->renderer_config->title, NULL, NULL);
	
	if (!opengl_renderer->renderer_config->rendering_window) {
		
		log_error("Failed to create GWLFW window.\n");
		return SHATTER_GLFW_WINDOW_FAILURE;
	}
	
	glfwMakeContextCurrent(opengl_renderer->renderer_config->rendering_window);
	if (!gladLoadGL()) {
		
		log_critical("Failed to load bindings from GLAD.\n");
		return SHATTER_OPENGL_RENDERER_INIT_FAILURE;
	}
	
	const char *vertex_shader_code =
		"#version 450 core\n"
		"\n"
		"layout (location = 0) in vec2 pos;\n"
		"layout (location = 1) in vec3 color;\n"
		"\n"
		"layout (location = 0) out vec3 frag_color;"
		"\n"
		"void main() {\n"
		"	\n"
		"	gl_Position = vec4(pos.x, pos.y, 1.0, 1.0);\n"
		"	frag_color = color;\n"
		"}\0";
	
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
	glCompileShader(vertex_shader);
	
	int success;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		
		char info_log[MAX_INFO_LOG_LEN];
		glGetShaderInfoLog(vertex_shader, MAX_INFO_LOG_LEN, NULL, info_log);
		log_error("Vertex shader failed to compile.\n");
		log_error("%s", info_log);
	}
	
	const char *fragment_shader_code =
		"#version 450\n"
		"\n"
		"layout(location = 0) in vec3 frag_color;"
		"\n"
		"layout(location = 0) out vec4 out_color;\n"
		"\n"
		"void main() {\n"
		"	\n"
		"	out_color = vec4(frag_color, 1.0);\n"
		"}\0";
	
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, (const char **)(&fragment_shader_code), NULL);
	glCompileShader(fragment_shader);
	
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		
		char info_log[MAX_INFO_LOG_LEN ];
		glGetShaderInfoLog(fragment_shader, MAX_INFO_LOG_LEN, NULL, info_log);
		log_error("Fragment shader failed to compile.\n");
		log_error("%s", info_log);
	}
	
	opengl_renderer->shader_program = glCreateProgram();
	glAttachShader(opengl_renderer->shader_program, vertex_shader);
	glAttachShader(opengl_renderer->shader_program, fragment_shader);
	glLinkProgram(opengl_renderer->shader_program);
	
	glGetProgramiv(opengl_renderer->shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		
		char info_log[MAX_INFO_LOG_LEN];
		glGetProgramInfoLog(opengl_renderer->shader_program, MAX_INFO_LOG_LEN, NULL, info_log);
		log_error("Shader failed to link.\n");
		log_error("%s", info_log);
	}
	
	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);
	
	float vertices[3 * 5] = {
		
		 0.0f,  0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	};
	
	glGenVertexArrays(1, &(opengl_renderer->vertex_array_object));
	
	glGenBuffers(1, &(opengl_renderer->vertex_buffer_object));
	
	glBindVertexArray(opengl_renderer->vertex_array_object);
	
	glBindBuffer(GL_ARRAY_BUFFER, opengl_renderer->vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);
	
	log_trace("Initialized opengl renderer.\n");
	return SHATTER_SUCCESS;
}

shatter_status_t loop_opengl_renderer(opengl_renderer_t *opengl_renderer) {
	
	glUseProgram(opengl_renderer->shader_program);
	glBindVertexArray(opengl_renderer->vertex_array_object);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glfwSwapBuffers(opengl_renderer->renderer_config->rendering_window);
	return SHATTER_SUCCESS;
}

shatter_status_t cleanup_opengl_renderer(opengl_renderer_t *opengl_renderer) {
	
	log_trace("\n");
	log_trace("Cleaning up opengl renderer.\n");
	
	glDeleteBuffers(1, &(opengl_renderer->vertex_buffer_object));
	glDeleteVertexArrays(1, &(opengl_renderer->vertex_array_object));
	glDeleteProgram(opengl_renderer->shader_program);
	
	glfwDestroyWindow(opengl_renderer->renderer_config->rendering_window);
	free(opengl_renderer);
	
	log_trace("Cleaned up opengl renderer.\n");
	return SHATTER_SUCCESS;
}

