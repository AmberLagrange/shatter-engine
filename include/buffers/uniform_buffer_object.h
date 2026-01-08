#ifndef BUFFERS_UNIFORM_BUFFER_OBJECT_H
#define BUFFERS_UNIFORM_BUFFER_OBJECT_H

#include <cglm/cglm.h>

typedef struct uniform_buffer_object_s {
	
	mat4 model;
	mat4 view;
	mat4 projection;
} uniform_buffer_object_t;

#endif // BUFFERS_UNIFORM_BUFFER_OBJECT_H

