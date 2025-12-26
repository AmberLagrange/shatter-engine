#include <common/core.h>

#include <dynamic_loader/dynamic_loader.h>

#include <dlfcn.h>

shatter_status_t load_library(dynamic_loader_t *dyn_loader) {
	
	dyn_loader->handle = dlopen(dyn_loader->filepath, RTLD_NOW | RTLD_GLOBAL);
	if (!dyn_loader->handle) {
		
		log_critical("Failed to open library %s.\n", dyn_loader->filepath);
		log_critical("%s\n", dlerror());
		return SHATTER_DYNAMIC_LIBRARY_LOAD_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

shatter_status_t unload_library(dynamic_loader_t *dyn_loader) {
	
	dlclose(dyn_loader->handle);
	return SHATTER_SUCCESS;
}

shatter_status_t reload_library(dynamic_loader_t *dyn_loader) {
	
	if (unload_library(dyn_loader)) {
		
		return SHATTER_DYNAMIC_LIBRARY_RELOAD_FAILURE;
	}
	
	if (load_library(dyn_loader)) {
		
		return SHATTER_DYNAMIC_LIBRARY_RELOAD_FAILURE;
	}
	
	return SHATTER_SUCCESS;
}

void *get_function(dynamic_loader_t *dyn_loader, const char *function_name) {
	
	void *function_ptr = dlsym(dyn_loader->handle, function_name);
	
	if (!function_ptr) {
		
		log_critical("Failed to find function %s.\n", function_name);
		return NULL;
	}
	
	return function_ptr;
}

