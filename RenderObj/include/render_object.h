#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H
#include <stdlib.h>
#include <roots_math.h>
#include <data_struct.h>
#include <glad/glad.h>
#ifdef __cplusplus
extern "C" {
#endif
	// This will be updated in the future to account for PBR rendering, for now it is diffuse and normal maps
#define TEXTURES_PER_MESH 2
typedef struct Shader {
	RTuint ID, vertexLength, fragLength;
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

	RTuint totalSpan;
	// temporary buffer
	Vertex* vertices;
	RTuint vertCount;
	RTuint vertSize;
	// temporary buffer
	RTuint* indices;
	RTuint indicesCount;
	RTuint indicesSize;
	// temporary buffer
	Texture* textures;// diffuse, normal, specular map, metallic map, roughness, transparency
	RTuint textureCount;
	RTuint textureSize;

	RTuint materialIndex;
	RTuint VBO, VAO, EBO, texture;
	
	float* rawVertices;
	RTuint rawVertCount;
	RTuint rawVertSize;
} Mesh;

typedef struct RenderObj {

	Mesh* objMeshes;
	RTuint meshesCount;
	// shader
	Shader objShader;
	// world space coordinates
	rt_vec3 position;
	// world scale values
	rt_vec3 scale;
	rt_quat rotation;
} RenderObj;

void Shader_Empty_Init(Shader*);
void Shader_Init(Shader*, rt_string*, rt_string*);
void Shader_Use(const Shader*);
void Shader_setBool(const Shader*, const char*, const RTuint, const RTuint ); // last param is a a boolean
void Shader_setInt(const Shader*, const char*, const RTuint, const int );
void Shader_setFloat(const Shader*, const char*, const RTuint, const float );
void Shader_setMat4(const Shader*, const char*, const RTuint, const rt_mat4 );
char* Shader_getSource(const Shader*);
void Shader_Terminate(const Shader*);

void Texture_Empty_Init(Texture*);
void Texture_Terminate(Texture*);

// Mesh_init >> Mesh_RawVertsInit >> asset_io load_texture >> Mesh_Register
void Mesh_Empty_Init(Mesh*);
void Mesh_Init(Mesh*, const float* verts, unsigned int _vertCount, unsigned int* indices, unsigned int _indicesCount);
void Mesh_Draw(Mesh*);
Texture* Mesh_GetTexture(Mesh*, RTuint);
void Mesh_Register(Mesh*);
void Mesh_RawVertsInit(Mesh*);
void Mesh_Terminate(Mesh*);

void RenderObj_ReadFBX(RenderObj*, const char*);
void RenderObj_Init(RenderObj*);
void RenderObj_AddMesh(RenderObj*,Mesh*);
void RenderObj_Add_Raw_Mesh(RenderObj*, const float* verts, unsigned int _vertCount, unsigned int* indices, unsigned int _indicesCount);
void RenderObj_Register(RenderObj*);
void RenderObj_Draw(RenderObj*);
void RenderObj_Terminate(RenderObj*);
unsigned char* RenderObj_Serialize(RenderObj*);

#ifdef __cplusplus
}
#endif
#endif