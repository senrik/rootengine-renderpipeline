#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <ufbx.h>
#include <stb_image.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

class Shader {
public:
	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	void terminateShader();
private:
	unsigned int ID;
};

typedef struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
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
public:

	Mesh* objMeshes;
	uint32_t meshesCount;
	// shader
	Shader objShader;
	// world space coordinates
	float xPos, yPos, zPos;
	// world scale values
	float xScale, yScale, zScale;
	glm::quat rotation;
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

glm::vec3 ufbx_to_glm_vec3(ufbx_vec3);
glm::vec2 ufbx_to_glm_vec2(ufbx_vec2);

#endif