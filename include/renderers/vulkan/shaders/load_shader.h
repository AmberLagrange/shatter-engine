#ifndef RENDERERS_VULKAN_SHADERS_LOAD_SHADER_H
#define RENDERERS_VULKAN_SHADERS_LOAD_SHADER_H

#include <stddef.h>

char *get_shader_bytecode(const char *file_path, size_t *code_size);

#endif // RENDERERS_VULKAN_SHADERS_LOAD_SHADER_H

