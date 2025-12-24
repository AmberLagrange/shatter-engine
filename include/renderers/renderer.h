#ifndef RENDERERS_RENDERER_H
#define RENDERERS_RENDERER_H

#include <renderers/renderer_config.h>

typedef struct {
	
	void *api_renderer;
	
	int (*renderer_init_callback)(void *, renderer_config_t *);
	int (*renderer_loop_callback)(void *);
	int (*renderer_cleanup_callback)(void *);
} renderer_t;

int renderer_init(renderer_t *renderer, renderer_config_t *config);
int renderer_loop(renderer_t *renderer);
int renderer_cleanup(renderer_t *renderer);

#endif // RENDERERS_RENDERER_H

