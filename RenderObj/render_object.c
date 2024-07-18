#include "render_object.h"


void Shader_Empty_Init(Shader* _shader) {
	_shader->ID = -1;
}

void Shader_Init(Shader* _shader, const char* vertexCode, const char* fragmentCode) {


	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, vertexCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, fragmentCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
	}

	_shader->ID = glCreateProgram();
	glAttachShader(_shader->ID, vertex);
	glAttachShader(_shader->ID, fragment);
	glLinkProgram(_shader->ID);
	glGetProgramiv(_shader->ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(_shader->ID, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader_Use(const Shader* _shader) {
	glUseProgram(_shader->ID);
}

void Shader_setBool(const Shader* _shader, const char* name, const uint _nameLen, const uint value) {
	glUniform1i(glGetUniformLocation(_shader->ID, name), (int)value);
}

void Shader_setInt(const Shader* _shader, const char* name, const uint _nameLen, int value){
	glUniform1i(glGetUniformLocation(_shader->ID, name), value);
}

void Shader_setFloat(const Shader* _shader, const char* name, const uint _nameLen, float value) {
	glUniform1f(glGetUniformLocation(_shader->ID, name), value);
}

void Shader_setMat4(const Shader* _shader, const char* name, const uint _nameLen, const rt_mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(_shader->ID, name), 1, GL_FALSE, &value.cols[0].v[0]);
}

void Shader_Terminate(const Shader* _shader) {
	glDeleteProgram(_shader->ID);
}


#pragma region Mesh

void Mesh_RawVertsInit(Mesh* _mesh) {
	_mesh->rawVertCount = _mesh->vertCount * _mesh->totalSpan;
	_mesh->rawVertSize = sizeof(float) * _mesh->rawVertCount;
	_mesh->rawVertices = (float*)malloc(sizeof(float) * _mesh->rawVertSize);
	for (int j = 0; j < _mesh->vertCount; j++) {
		size_t ri = j * 8;
		// position
		_mesh->rawVertices[ri] = _mesh->vertices[j].position.x;
		_mesh->rawVertices[ri + 1] = _mesh->vertices[j].position.y;
		_mesh->rawVertices[ri + 2] = _mesh->vertices[j].position.z;
		// uv coord
		_mesh->rawVertices[ri + 3] = _mesh->vertices[j].textureCoords.x;
		_mesh->rawVertices[ri + 4] = _mesh->vertices[j].textureCoords.y;
		// normal
		_mesh->rawVertices[ri + 5] = _mesh->vertices[j].normal.x;
		_mesh->rawVertices[ri + 6] = _mesh->vertices[j].normal.y;
		_mesh->rawVertices[ri + 7] = _mesh->vertices[j].normal.z;
	}

	free(_mesh->vertices);
	//free(_mesh->indices);
}

void Mesh_Empty_Init(Mesh* _mesh) {
	_mesh->totalSpan = 0;
	_mesh->vertices = 0;
	_mesh->vertCount = 0;
	_mesh->vertSize = 0;
	_mesh->indices = 0;
	_mesh->indicesCount = 0;
	_mesh->indicesSize = 0;
	_mesh->rawVertices = 0;
	_mesh->rawVertCount = 0;
	_mesh->rawVertSize = 0;
	_mesh->textures = 0;
	_mesh->textureCount = 0;
	_mesh->textureSize = 0;
}



void Mesh_Init(Mesh* _mesh, const float* verts, unsigned int _vertCount, unsigned int* indices, unsigned int _indicesCount) {
	
	_mesh->totalSpan= 8;
	_mesh->indicesCount = _indicesCount;
	_mesh->indicesSize = sizeof(uint) * _mesh->indicesCount;
	_mesh->indices = (uint*)malloc(_mesh->indicesSize);
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

	_mesh->textures = 0;
	_mesh->textureCount = 0;
	_mesh->textureSize = 0;
}

void Mesh_Register(Mesh* _mesh) {
	glGenVertexArrays(1, &_mesh->VAO);
	glGenBuffers(1, &_mesh->VBO);

	glBindVertexArray(_mesh->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, _mesh->rawVertSize, _mesh->rawVertices, GL_STATIC_DRAW);

	if (_mesh->indicesCount > 0) {
		glGenBuffers(1, &_mesh->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh->indicesSize, _mesh->indices, GL_STATIC_DRAW);
	}

	// vertex position values
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _mesh->totalSpan * sizeof(float), (void*)0);

	// texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, _mesh->totalSpan * sizeof(float), (void*)(3 * sizeof(float)));

	// vertex normal values
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, _mesh->totalSpan * sizeof(float), (void*)(5 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (_mesh->textures[0].textureData != NULL) {
		glGenTextures(1, &_mesh->texture);
		glBindTexture(GL_TEXTURE_2D, _mesh->texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _mesh->textures[0].t_width, _mesh->textures[0].t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, _mesh->textures[0].textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("Failed to load texture!\n");
	}
}
void Mesh_LoadTexture(Mesh* _mesh, char* _pathToTexture) {
	if (_mesh->textures == NULL) {
		_mesh->textures = (Texture*)malloc(sizeof(Texture) * 4);
		_mesh->textureSize = 4;
	}
	
	_mesh->textureCount++;
}
void Mesh_Draw(Mesh* _mesh) {
	if (_mesh->textureCount > 0) {
		glBindTexture(GL_TEXTURE_2D, _mesh->texture);
	}
	glBindVertexArray(_mesh->VAO);
	if (_mesh->indicesCount > 0) {
		glDrawElements(GL_TRIANGLES, _mesh->indicesCount, GL_UNSIGNED_INT, 0);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, _mesh->rawVertCount / _mesh->totalSpan);
	}
}
void Mesh_Terminate(Mesh* _mesh) {
	free(_mesh->vertices);
	free(_mesh->indices);
	free(_mesh->textures);

	glDeleteBuffers(1, &_mesh->VBO);
	glDeleteBuffers(1, &_mesh->VAO);

	free(_mesh->rawVertices);
}
#pragma endregion

#pragma region Texture
void Texture_Init(Texture* _texture, const unsigned char* _textureData) {
	_texture->textureData = _textureData;
}

void Texture_Terminate(Texture* _texture) {
	free(_texture->textureData);
}
#pragma endregion

#pragma region RenderObj

void RenderObj_Init(RenderObj* _obj) {
	_obj->objMeshes = 0;
	_obj->meshesCount = 0;
	if (_obj->meshesCount > 0) {
		free(_obj->objMeshes);
		_obj->meshesCount = 0;
		_obj->objMeshes = (Mesh*)malloc(sizeof(Mesh) * MESHES_PER_RENDEROBJ);
	}
	Shader_Empty_Init(&_obj->objShader);
}

void RenderObj_AddMesh(RenderObj* _obj, Mesh* _mesh) {
	if (_obj->objMeshes == NULL) {
		_obj->objMeshes = (Mesh*)malloc(sizeof(Mesh) * MESHES_PER_RENDEROBJ);
		_obj->meshesCount = 0;
	}

	if (_obj->meshesCount + 1 < MESHES_PER_RENDEROBJ) {
		_obj->objMeshes[_obj->meshesCount] = *_mesh;

		_obj->meshesCount++;
	}
	else {
		printf("ERROR::RENDEROBJ::MAX_MESHES_REACHED");
	}
}

void RenderObj_Add_Raw_Mesh(RenderObj* _obj,const float* verts, unsigned int _vertCount, unsigned int* indices, unsigned int _indicesCount) {
	if (_obj->objMeshes == NULL) {
		_obj->objMeshes = (Mesh*)malloc(sizeof(Mesh) * MESHES_PER_RENDEROBJ);
		_obj->meshesCount = 0;
	}
	if (_obj->meshesCount + 1 <  MESHES_PER_RENDEROBJ) {
		Mesh_Init(&_obj->objMeshes[_obj->meshesCount], verts, _vertCount, indices, _indicesCount);
		//Mesh_LoadTexture(&_obj->objMeshes[_obj->meshesCount], "./assets/container.jpg");
		_obj->meshesCount++;
	} else {
		printf("ERROR::RENDEROBJ::MAX_MESHES_REACHED");
	}
	
}

void RenderObj_Register(RenderObj* _obj) {
	for (int mi = 0; mi < _obj->meshesCount; mi++) {

		Mesh_RawVertsInit(&_obj->objMeshes[mi]);
		// Raw verts, texture, and shader need to be 
		Mesh_Register(&_obj->objMeshes[mi]);
	}
}

void RenderObj_Draw(RenderObj* _obj) {
	//_obj->objShader.use();
	if (_obj->meshesCount > 0) {
		for (size_t i = 0; i < _obj->meshesCount; i++) {
			Mesh_Draw(&_obj->objMeshes[i]);
			glBindVertexArray(0);
		}
	}
}

void RenderObj_Terminate(RenderObj* _obj) {
	for (int i = 0; i < _obj->meshesCount; i++) {
		Mesh_Terminate(&_obj->objMeshes[i]);
	}
	free(_obj->objMeshes);
	Shader_Terminate(&_obj->objShader);
	
}
#pragma endregion