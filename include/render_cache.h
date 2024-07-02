#ifndef RENDER_CACHE_H
#define RENDER_CACHE_H
#ifdef __cplusplus
extern "C" {
#endif
#include <render_object.h>
#define RENDER_CACHE_SIZE 200


typedef struct RenderCache {
	RenderObj* cache;
	unsigned int cacheCount;
} RenderCache;

void RenderCache_Init(RenderCache*);
void RenderCache_Obj_Add(RenderCache*, const RenderObj*);
void RenderCache_Raw_Add(RenderCache*, const float*, unsigned int, unsigned int*, unsigned int, const float, const float, const float);
void RenderCache_Draw(RenderCache* , rt_mat4, rt_mat4, float);
void RenderCache_Clear(RenderCache*);
void RenderCache_Terminate(RenderCache*);



#ifdef __cplusplus
}
#endif
#endif
