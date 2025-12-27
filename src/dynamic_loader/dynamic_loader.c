#include <common/core.h>

#include <dynamic_loader/dynamic_loader.h>

#include <dlfcn.h>

shatter_status_t load_library(dynamic_loader_t *dyn_loader) {
	
	// It's possible for dlopen to return NULL as a valid handle.
	// Calling dlerror prior to running dlopen will clear any errors,
	// then calling dlerror after dlopen will return a non-NULL string
	// in the event of errors. This lets the program know if a NULL
	// handle is valid or not.
	
	dlerror();
	dyn_loader->handle = dlopen(dyn_loader->filepath, RTLD_NOW | RTLD_GLOBAL);
	char *dl_error_msg = dlerror();

	if (!dyn_loader->handle && dl_error_msg) {
		
		log_critical("Failed to open library %s.\n", dyn_loader->filepath);
		log_critical("%s\n", dl_error_msg);
		return SHATTER_DYNAMIC_LIBRARY_LOAD_FAILURE;
	}
	
	dlerror();
	vtable_t *vtable = dlsym(dyn_loader->handle, "api_vtable");
	dl_error_msg = dlerror();
	
	if (!vtable && dl_error_msg) {
		
		log_critical("Failed to get api vtable from %s.\n");
		log_critical("%s\n", dl_error_msg);
		return SHATTER_DYNAMIC_LIBRARY_LOAD_VTABLE_FAILURE;
	};
	
	dyn_loader->api_vtable = *(vtable_t *)dlsym(dyn_loader->handle, "api_vtable");
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

