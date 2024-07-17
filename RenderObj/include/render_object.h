#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H
#include <stdlib.h>
#include <roots_math.h>
#include <data_types.h>
#include <glad/glad.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Shader {
	uint ID;
} Shader;

typedef struct Vertex {
	rt_vec3 position;
	rt_vec3 normal;
	rt_vec2 textureCoords;
} Vertex;

typedef struct Texture {
	int t_width, t_height, nrChannels;
	
	unsigned char* textureData;
} Texture;

typedef struct Mesh {

	uint totalSpan;
	// temporary buffer
	Vertex* vertices;
	uint vertCount;
	uint vertSize;
	// temporary buffer
	uint* indices;
	uint indicesCount;
	uint indicesSize;
	// temporary buffer
	Texture* textures;
	uint textureCount;
	uint textureSize;

	uint materialIndex;
	uint VBO, VAO, EBO, texture;
	
	float* rawVertices;
	uint rawVertCount;
	uint rawVertSize;
} Mesh;

typedef struct RenderObj {

	Mesh* objMeshes;
	uint meshesCount;
	// shader
	Shader objShader;
	// world space coordinates
	rt_vec3 position;
	// world scale values
	rt_vec3 scale;
	rt_quat rotation;
} RenderObj;

void Shader_Empty_Init(Shader*);
void Shader_Init(Shader*, const char*, const char*);
void Shader_Use(const Shader*);
void Shader_setBool(const Shader*, const char*, const uint, const uint ); // last param is a a boolean
void Shader_setInt(const Shader*, const char*, const uint, const int );
void Shader_setFloat(const Shader*, const char*, const uint, const float );
void Shader_setMat4(const Shader*, const char*, const uint, const rt_mat4 );
void Shader_Terminate(const Shader*);

void Texture_Init(Texture*, const unsigned char*);
void Texture_Terminate(Texture*);

// Mesh_init >> Mesh_RawVertsInit >> Mesh_LoadTexture >> Mesh_Register
void Mesh_Empty_Init(Mesh*);
void Mesh_Init(Mesh*, const float* verts, unsigned int _vertCount, unsigned int* indices, unsigned int _indicesCount);
void Mesh_Draw(Mesh*);
void Mesh_LoadTexture(Mesh*, const unsigned char*);
void Mesh_Register(Mesh*);
void Mesh_RawVertsInit(Mesh*);
void Mesh_Terminate(Mesh*);

void RenderObj_ReadFBX(RenderObj*, const char*);
void RenderObj_Init(RenderObj*);
void RenderObj_AddMesh(RenderObj*, const float* verts, unsigned int _vertCount, unsigned int* indices, unsigned int _indicesCount);
void RenderObj_Register(RenderObj*);
void RenderObj_Draw(RenderObj*);
void RenderObj_Terminate(RenderObj*);

#ifdef __cplusplus
}
#endif
#endif