#include <common/core.h>

#include <dynamic_loader/dynamic_loader.h>

#include <dlfcn.h>

const char *get_curr_api(dynamic_loader_t *dyn_loader) {
	
	return dyn_loader->filepath_list[dyn_loader->curr_api_index];
}

shatter_status_t request_api(dynamic_loader_t *dyn_loader, graphics_api_index_t api_index) {
	
	if (api_index >= NUM_API_INDICIES) {
		
		log_error("Invalid graphics API index.\n");
		return SHATTER_DYNAMIC_LIBRARY_INVALID_API_INDEX;
	}
	
	dyn_loader->requested_api_index = api_index;
	return SHATTER_SUCCESS;
}

const char *get_requested_api(dynamic_loader_t *dyn_loader) {
	
	return dyn_loader->filepath_list[dyn_loader->requested_api_index];
}

shatter_status_t load_library(dynamic_loader_t *dyn_loader) {
	
	const char *requested_api = get_requested_api(dyn_loader);
	dyn_loader->handle = dlopen(requested_api, RTLD_NOW | RTLD_GLOBAL);
	if (!dyn_loader->handle) {
		
		log_critical("Failed to open library %s.\n", requested_api);
		log_critical("%s\n", dlerror());
		return SHATTER_DYNAMIC_LIBRARY_LOAD_FAILURE;
	}	
	log_info("Loaded:\t%s.\n", requested_api);
	
	vtable_t *vtable = dlsym(dyn_loader->handle, "api_vtable");
	if (!vtable) {
		
		log_critical("Failed to get api vtable from %s.\n", requested_api);
		log_critical("%s\n", dlerror());
		return SHATTER_DYNAMIC_LIBRARY_LOAD_VTABLE_FAILURE;
	};
	
	dyn_loader->curr_api_index = dyn_loader->requested_api_index;
	dyn_loader->api_vtable = *(vtable_t *)dlsym(dyn_loader->handle, "api_vtable");
	return SHATTER_SUCCESS;
}

shatter_status_t unload_library(dynamic_loader_t *dyn_loader) {
	
	dlclose(dyn_loader->handle);
	log_info("Unloaded:\t%s.\n", get_curr_api(dyn_loader));
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

