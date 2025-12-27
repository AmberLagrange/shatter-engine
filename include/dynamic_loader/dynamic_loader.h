#ifndef DYNAMIC_LOADER_DYNAMIC_LOADER_H
#define DYNAMIC_LOADER_DYNAMIC_LOADER_H

#include <common/core.h>

#include <dynamic_loader/vtable.h>

typedef enum graphics_api_index_s {
	
	OPENGL_API_INDEX = 0,
	VULKAN_API_INDEX,
	
	NUM_API_INDICES
} graphics_api_index_t;

typedef struct dynamic_loader_s {
	
	const char *filepath_list[NUM_API_INDICES];
	graphics_api_index_t curr_api_index;
	graphics_api_index_t requested_api_index;
	
	void *handle;
	vtable_t api_vtable;
} dynamic_loader_t;

const char *get_curr_api(dynamic_loader_t *dyn_loader);
shatter_status_t request_api(dynamic_loader_t *dyn_loader, graphics_api_index_t api_index);
const char *get_requested_api(dynamic_loader_t *dyn_loader);

shatter_status_t   load_library(dynamic_loader_t *dyn_loader);
shatter_status_t unload_library(dynamic_loader_t *dyn_loader);
shatter_status_t reload_library(dynamic_loader_t *dyn_loader);

#endif // DYNAMIC_LOAD_DYNAMIC_LOADER_H

