#ifndef DYNAMIC_LOADER_DYNAMIC_LOADER_H
#define DYNAMIC_LOADER_DYNAMIC_LOADER_H

#include <common/core.h>

#include <dynamic_loader/vtable.h>

typedef struct dynamic_loader_s {
	
	const char *filepath;
	void *handle;
	vtable_t api_vtable;
} dynamic_loader_t;

shatter_status_t   load_library(dynamic_loader_t *dyn_loader);
shatter_status_t unload_library(dynamic_loader_t *dyn_loader);
shatter_status_t reload_library(dynamic_loader_t *dyn_loader);

#endif // DYNAMIC_LOAD_DYNAMIC_LOADER_H

