#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H
#ifdef __cplusplus
extern "C" {
#endif
#include <roots_math/roots_math.h>
#include <asset_io.h>
#include <glad.h>

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
	float xPos, yPos, zPos;
	// world scale values
	float xScale, yScale, zScale;
	rt_quat rotation;
} RenderObj;

void Shader_Empty_Init(Shader*);
void Shader_Init(Shader*,const char*, uint, const char*, uint);
void Shader_Use(const Shader*);
void Shader_setBool(const Shader*, const char*, const uint, const bool );
void Shader_setInt(const Shader*, const char*, const uint, const int );
void Shader_setFloat(const Shader*, const char*, const uint, const float );
void Shader_setMat4(const Shader*, const char*, const uint, const rt_mat4 );
void Shader_Terminate(const Shader*);

void Texture_Init(Texture*, const char*);
void Texture_Terminate(Texture*);

void Mesh_Init(Mesh*);
void Mesh_Draw(Mesh*);
void Mesh_RawVertsInit(Mesh*);
void Mesh_Terminate(Mesh*);

void RenderObj_ReadFBX(RenderObj*, const char*);
void RenderObj_Init(RenderObj*);
void RenderObj_Draw(RenderObj*);
void RenderObj_Terminate(RenderObj*);

#ifdef __cplusplus
}
#endif
#endif