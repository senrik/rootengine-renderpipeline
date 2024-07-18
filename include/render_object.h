#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H
#include <ufbx.h>
#include <stb_image.h>

typedef struct Shader {
	unsigned int ID;
} Shader;

typedef struct Vertex {
	// position - vec3
	// normal - vec3
	// texture Coords - vec2
} Vertex;

typedef struct Texture {
	int t_width, t_height, nrChannels;
	
	unsigned char* textureData;
} Texture;

typedef struct Mesh {

	uint32_t totalSpan;
	// temporary buffer
	Vertex* vertices;
	uint32_t vertCount;
	uint32_t vertSize;
	// temporary buffer
	uint32_t* indices;
	uint32_t indicesCount;
	uint32_t indicesSize;
	// temporary buffer
	Texture* textures;
	uint32_t textureCount;
	uint32_t textureSize;

	uint32_t materialIndex;
	unsigned int VBO, VAO, EBO, texture;

	float* rawVertices;
	size_t rawVertCount;
	size_t rawVertSize;
} Mesh;

typedef struct RenderObj {

	Mesh* objMeshes;
	uint32_t meshesCount;
	// shader
	Shader objShader;
	// world space coordinates
	float xPos, yPos, zPos;
	// world scale values
	float xScale, yScale, zScale;
	// rotation quaternion
} RenderObj;

void Texture_Init(Texture*, const char*);
void Texture_Terminate(Texture*);

void Mesh_Init(Mesh* _mesh);
void Mesh_Draw(Mesh*);
void Mesh_RawVertsInit(Mesh*);
void Mesh_Terminate(Mesh*);

void RenderObj_ReadFBX(RenderObj*, ufbx_scene*, const char*);
void RenderObj_Init(RenderObj*);
void RenderObj_Draw(RenderObj*);
void RenderObj_Terminate(RenderObj*);

#endif