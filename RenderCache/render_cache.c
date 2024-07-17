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

void RenderCache_Raw_Add(RenderCache* _cache, const float* verts, unsigned int _vertCount, unsigned int* indices, unsigned int _indicesCount, const rt_vec3 _Pos) {
	RenderObj obj;
	RenderObj_Init(&obj);
	
	RenderObj_AddMesh(&obj, verts, _vertCount, indices, _indicesCount);

	for (int mi = 0; mi < obj.meshesCount; mi++) {
		/*Mesh* _mesh = &obj.objMeshes[mi];

		_mesh->textureCount = 1;
		_mesh->textureSize = sizeof(Texture) * _mesh->textureCount;
		_mesh->textures = (Texture*)malloc(_mesh->textureSize);
		for (int ti = 0; ti < _mesh->textureCount; ti++) {
			Texture* _texture = &_mesh->textures[ti];
			_texture->textureData = stbi_load("./assets/container.jpg", &_texture->t_width, &_texture->t_height, &_texture->nrChannels, 0);
		}

		Mesh_RawVertsInit(_mesh);*/

		Mesh_LoadTexture(&obj.objMeshes[mi], "./assets/container.jpg");
		Mesh_RawVertsInit(&obj.objMeshes[mi]);
		Mesh_Register(&obj.objMeshes[mi]);
	}

	//Shader_Init(&obj.objShader, DEFAULT_VERT_PATH, DEFAULT_FRAG_PATH);
	obj.position.x = _Pos.x;
	obj.position.y = _Pos.y;
	obj.position.z = _Pos.z;
	obj.scale.x = 1.0f;
	obj.scale.y = 1.0f;
	obj.scale.z = 1.0f;
	quat_init(&obj.rotation);

	RenderCache_Obj_Add(_cache, &obj);
}


void RenderCache_Draw(RenderCache* _cache, rt_mat4 _m1, rt_mat4 _m2, float _f) {
	// TODO
}