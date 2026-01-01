#include <common/core.h>

#include <shaders/load_shader.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ALIGNMENT sizeof(uint32_t)

char *get_shader_bytecode(const char *filepath, size_t *code_size) {
	
	FILE *file_ptr = fopen(filepath, "rb");
	
	if (!file_ptr) {
		
		log_error("Could not open %s.\n", filepath);
		return NULL;
	}
	
	fseek(file_ptr, 0L, SEEK_END);
	*code_size = ftell(file_ptr);
	fseek(file_ptr, 0L, SEEK_SET);
	
	// Ensure a proper alignment
	int32_t remainder = (*code_size % ALIGNMENT);
	if (remainder != 0) {
		
		*code_size += (ALIGNMENT - remainder);
	}
	
	char *bytecode = malloc(*code_size);
	fread(bytecode, sizeof(char), *code_size, file_ptr);
	
	fclose(file_ptr);
	return bytecode;
}

