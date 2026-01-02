#ifndef BUFFERS_BUFFER_INFO_H
#define BUFFERS_BUFFER_INFO_H

#include <common/core.h>

#include <stddef.h>

typedef enum attribute_indices_e {
	
	VERTEX_POSITION_ATTRIBUTE = 0,
	VERTEX_COLOR_ATTRIBUTE,
	
	NUM_VERTEX_ATTRIBUTES
} attribute_indicies_t;

typedef struct buffer_info_s {
	
	void *data;
	size_t size;
	size_t num_elements;
} buffer_info_t;

#endif // BUFFERS_BUFFER_INFO_H

