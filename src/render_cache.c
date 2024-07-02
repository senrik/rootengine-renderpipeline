#include <render_cache.h>

void RenderCache_Init(RenderCache* _cache) {
	_cache->cache = (RenderObj*)malloc(sizeof(RenderObj*) * RENDER_CACHE_SIZE);
	_cache->cacheCount = 0;
}

void RenderCache_Terminate(RenderCache* _cache) {
	for (int i = 0; i < _cache->cacheCount; i++) {
		RenderObj_Terminate(&_cache->cache[i]);
	}
	free(_cache->cache);
}

void RenderCache_Obj_Add(RenderCache* _cache, const RenderObj* _obj) {
	_cache->cache[_cache->cacheCount] = *_obj;
	_cache->cacheCount++;
}

void RenderCache_Raw_Add(RenderCache* _cache, const float* verts, unsigned int _vertCount, unsigned int* indices, unsigned int _indicesCount, const float _xPos, const float _yPos, const float _zPos) {
	// TODO
	RenderObj obj;
	RenderObj_Init(&obj);
	obj.meshesCount = 1;
	obj.objMeshes = (Mesh*)malloc(sizeof(Mesh) * obj.meshesCount);

	for (int mi = 0; mi < obj.meshesCount; mi++) {
		Mesh* _mesh = &obj.objMeshes[mi];
		_mesh->totalSpan = 8;

		_mesh->indicesCount = _indicesCount;
		_mesh->indicesSize = sizeof(uint32_t) * _mesh->indicesCount;
		_mesh->indices = (uint32_t*)malloc(_mesh->indicesSize);
		for (int i = 0; i < _mesh->indicesCount; i++) {
			_mesh->indices[i] = indices[i];
		}

		_mesh->vertCount = _vertCount / _mesh->totalSpan;
		_mesh->vertSize = sizeof(Vertex) * _mesh->vertCount;
		_mesh->vertices = (Vertex*)malloc(_mesh->vertSize);
		for (int vi = 0; vi < _vertCount; vi += 8) {
			_mesh->vertices[vi / _mesh->totalSpan].position.x = verts[vi];
			_mesh->vertices[vi / _mesh->totalSpan].position.y = verts[vi + 1];
			_mesh->vertices[vi / _mesh->totalSpan].position.z = verts[vi + 2];

			_mesh->vertices[vi / _mesh->totalSpan].textureCoords.x = verts[vi + 3];
			_mesh->vertices[vi / _mesh->totalSpan].textureCoords.y = verts[vi + 4];

			_mesh->vertices[vi / _mesh->totalSpan].normal.x = verts[vi + 5];
			_mesh->vertices[vi / _mesh->totalSpan].normal.y = verts[vi + 6];
			_mesh->vertices[vi / _mesh->totalSpan].normal.z = verts[vi + 7];
		}

		_mesh->textureCount = 1;
		_mesh->textureSize = sizeof(Texture) * _mesh->textureCount;
		_mesh->textures = (Texture*)malloc(_mesh->textureSize);
		for (int ti = 0; ti < _mesh->textureCount; ti++) {
			Texture* _texture = &_mesh->textures[ti];
			_texture->textureData = stbi_load("./assets/container.jpg", &_texture->t_width, &_texture->t_height, &_texture->nrChannels, 0);
		}

		Mesh_RawVertsInit(_mesh);
	}

	obj.objShader = Shader("v_shader.vertshader", "f_shader.fragshader");
	obj.xPos = _xPos;
	obj.yPos = _yPos;
	obj.zPos = _zPos;
	obj.xScale = 1.0f;
	obj.yScale = 1.0f;
	obj.zScale = 1.0f;
	obj.rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	RenderCache_Add(&obj);
}

void RenderCache_Draw(RenderCache* _cache, rt_mat4 _m1, rt_mat4 _m2, float _f) {
	// TODO
}