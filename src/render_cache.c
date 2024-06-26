#include <render_cache.h>

void RenderCache_Init(RenderCache* _cache) {
	_cache->cache = (RenderObj*)malloc(sizeof(RenderObj*) * RENDER_CACHE_SIZE);
	_cache->cacheCount = 0;
}

void RenderCache_Terminate(RenderCache* _cache) {
	for (int i = 0; i < _cache->cacheCount; i++) {
		RenderObj_Terminate(_cache->cache[i]);
	}
	free(_cache->cache);
}

void RenderCache_Obj_Add(RenderCache* _cache, const RenderObj* _obj) {
	_cache->cache[_cache->cacheCount] = *_obj;
	_cache->cacheCount++;
}

void RenderCache_Raw_Add(RenderCache* _cache, const float* verts, unsigned int _vertCount, unsigned int* indices, unsigned int _indicesCount, const float _xPos, const float _yPos, const float _zPos) {
	// TODO
}

void RenderCache_Draw(RenderCache* _cache, rt_mat4 _m1, rt_mat4 _m2, float _f) {
	// TODO
}