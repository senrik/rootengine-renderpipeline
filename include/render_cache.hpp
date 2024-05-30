#ifndef RENDER_CACHE_H
#define RENDER_CACHE_H
#include <render_object.hpp>
class RenderCache {
public:
#define RENDER_CACHE_SIZE 200
	RenderCache();
	void Add(const RenderObj*);
	void Add(const float* verts, unsigned int _vertCount, unsigned int* indices, unsigned int _indicesCount, const float _xPos, const float _yPos, const float _zPos);
	void Draw(glm::mat4, glm::mat4, float);
	void Clear();
	~RenderCache();
private:
	RenderObj* cache;
	unsigned int cacheCount;
};
#endif
