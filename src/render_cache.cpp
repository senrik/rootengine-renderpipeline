#include <render_cache.hpp>

RenderCache::RenderCache() {
	cache = (RenderObj*)malloc(sizeof(RenderObj*) * RENDER_CACHE_SIZE);
	cacheCount = 0;
}

RenderCache::~RenderCache() {
	for (int i = 0; i < cacheCount; i++) {
		RenderObj_Terminate
	}
}