#ifndef RENDERERS_RENDERER_H
#define RENDERERS_RENDERER_H

#include <common/core.h>

#include <renderers/renderer_config.h>

typedef struct renderer_s {
	
	void *api_renderer;
	
	shatter_status_t (*renderer_init_callback)(void *, renderer_config_t *);
	shatter_status_t (*renderer_loop_callback)(void *);
	shatter_status_t (*renderer_cleanup_callback)(void *);
} renderer_t;

shatter_status_t renderer_init(renderer_t *renderer, renderer_config_t *config);
shatter_status_t renderer_loop(renderer_t *renderer);
shatter_status_t renderer_cleanup(renderer_t *renderer);

#endif // RENDERERS_RENDERER_H

