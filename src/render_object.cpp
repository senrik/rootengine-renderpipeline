#include "render_object.hpp"

#pragma region Shader
Shader::Shader() {
	ID = -1;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		printf("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n");
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::terminateShader() {
	glDeleteProgram(ID);
}

#pragma endregion

#pragma region Helper Functions
glm::vec3 ufbx_to_glm_vec3(ufbx_vec3 _v) {
	return glm::vec3((float)_v.x, (float)_v.y, (float)_v.z);
}
glm::vec2 ufbx_to_glm_vec2(ufbx_vec2 _v) {
	return glm::vec2((float)_v.x, (float)_v.y);
}
#pragma endregion

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
	free(_mesh->indices);
}
void Mesh_Init(Mesh* _mesh) {
	
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
}
#pragma endregion

#pragma region Texture
void Texture_Init(Texture* _texture, const char* _textureName) {
	_texture->textureData = stbi_load(_textureName, &_texture->t_width, &_texture->t_height, &_texture->nrChannels, 0);
}

void Texture_Terminate(Texture* _texture) {
	free(_texture->textureData);
}
#pragma endregion

#pragma region RenderObj

void RenderObj_ReadFBX(RenderObj* _obj, ufbx_scene* _scene, const char* _texturePath) {
	// parse the meshes in the fbx scene
	_obj->meshesCount = 0;
	_obj->objMeshes = (Mesh*)malloc(sizeof(Mesh) * _obj->meshesCount);
	size_t readIndex = 0;
	for (size_t mi = 0; mi < _scene->meshes.count; mi++) {
		size_t maxParts = 0;
		size_t maxTriangle = 0;
		ufbx_mesh* mesh = _scene->meshes[mi];
#pragma region Read Sub-Mesh
		const char* node_name = mesh->instances[0]->name.data;
		if (strcmp(node_name, "PILOT1") != 0) continue;
		// Tally the number of parts, and get the max triangles used in the parts
		for (size_t pi = 0; pi < mesh->material_parts.count; pi++)
		{
			ufbx_mesh_part* part = &mesh->material_parts.data[pi];
			if (part->num_triangles == 0) continue;
			maxParts += 1;
			maxTriangle = part->num_triangles < maxTriangle ? maxTriangle : part->num_triangles;
		}
		// temp buffers
		size_t num_tri_indices = mesh->max_face_triangles * 3;
		uint32_t* tri_indices = (uint32_t*)malloc(sizeof(uint32_t) * num_tri_indices);
		Vertex* vertices = (Vertex*)malloc(sizeof(Vertex) * maxTriangle * 3);
		uint32_t* indices = (uint32_t*)malloc(sizeof(uint32_t) * maxTriangle * 3);

		//THESE ARE CAUSING ISSUES, FIX THE DAMN PROBLEM.
		// 
		// result buffers
		Mesh* _meshParts = (Mesh*)malloc(sizeof(Mesh) * maxParts); 
		size_t _meshPartsCount = 0;

		// parse material parts
		for (size_t pi = 0; pi < mesh->material_parts.count; pi++) {
			ufbx_mesh_part* mesh_part = &mesh->material_parts.data[pi];
			if (mesh_part->num_triangles == 0) continue;

			Mesh* part = &_meshParts[_meshPartsCount++];
			size_t num_indices = 0;

			for (size_t fi = 0; fi < mesh_part->num_faces; fi++) {
				ufbx_face face = mesh->faces.data[mesh_part->face_indices.data[fi]];
				size_t num_tris = ufbx_triangulate_face(tri_indices, num_tri_indices, mesh, face);

				ufbx_vec2 default_uv = { 0 };

				for (size_t vi = 0; vi < num_tris; vi++) {
					uint32_t ix = tri_indices[vi];
					Vertex* vert = &vertices[num_indices];

					ufbx_vec3 pos = ufbx_get_vertex_vec3(&mesh->vertex_position, ix);
					ufbx_vec3 normal = ufbx_get_vertex_vec3(&mesh->vertex_normal, ix);
					ufbx_vec2 uv_coord = mesh->vertex_uv.exists ? ufbx_get_vertex_vec2(&mesh->vertex_uv, ix) : default_uv;

					vert->position = ufbx_to_glm_vec3(pos);
					vert->normal = ufbx_to_glm_vec3(normal);
					vert->textureCoords = ufbx_to_glm_vec2(uv_coord);

					num_indices++;
				}
			}
			
			ufbx_vertex_stream streams[1];
			size_t num_streams = 1;

			streams[0].data = vertices;
			streams[0].vertex_count = num_indices;
			streams[0].vertex_size = sizeof(Vertex);

			ufbx_error error;
			size_t num_vertices = ufbx_generate_indices(streams, num_streams, indices, num_indices, NULL, &error);

			if (error.type != UFBX_ERROR_NONE) {
				char buffer[1024];
				ufbx_format_error(buffer, sizeof(buffer), &error);
				fprintf(stderr, "%s\n%s\n", "Failed to generate index buffer", buffer);
				exit(1);
			}

			
			if (mesh_part->index < mesh->materials.count) {
				ufbx_material* material = mesh->materials.data[mesh_part->index];
				part->materialIndex = (uint32_t)material->typed_id;
			} else {
				part->materialIndex = -1;
			}

			part->vertCount = num_vertices;
			part->vertSize = sizeof(Vertex) * num_vertices;
			part->vertices = (Vertex*)malloc(part->vertSize);

			part->indicesCount = num_indices;
			part->indicesSize = sizeof(Vertex) * num_indices;
			part->indices = (uint32_t*)malloc(part->indicesSize);

			part->textureCount = 1;
			part->textureSize = sizeof(Texture) * part->textureCount;
			part->textures = (Texture*)malloc(part->textureSize);

			for (int i = 0; i < part->textureCount; i++) {
				Texture_Init(&part->textures[i], _texturePath);
			}

			part->totalSpan = 8;
			for (int j = 0; j < part->indicesCount; j++) {
				part->indices[j] = indices[j];
			}
			
			for (int j = 0; j < part->vertCount; j++) {
				part->vertices[j].position.x = vertices[j].position.x;
				part->vertices[j].position.y = vertices[j].position.y;
				part->vertices[j].position.z = vertices[j].position.z;

				part->vertices[j].normal.x = vertices[j].normal.x;
				part->vertices[j].normal.y = vertices[j].normal.y;
				part->vertices[j].normal.z = vertices[j].normal.z;

				part->vertices[j].textureCoords.x = vertices[j].textureCoords.x;
				part->vertices[j].textureCoords.y = vertices[j].textureCoords.y;
			}

			Mesh_RawVertsInit(part);

			_obj->objMeshes[_obj->meshesCount] = *part;
			_obj->meshesCount++;
		}


		free(tri_indices);
		free(vertices);
		free(indices);
#pragma endregion
	}

	_obj->xPos = 0.0f;
	_obj->yPos = 0.0f;
	_obj->zPos = 0.0f;
	
	_obj->xScale = 1.0f;
	_obj->yScale = 1.0f;
	_obj->zScale = 1.0f;

	_obj->rotation = glm::quat();
}

void RenderObj_Init(RenderObj* _obj) {
	
	if (_obj->meshesCount > 0) {
		for (int i = 0; i < _obj->meshesCount; i++) {
			if (&_obj->objMeshes[i] == NULL) continue;
			Mesh_Init(&_obj->objMeshes[i]);
		}
	}	
}

void RenderObj_Draw(RenderObj* _obj) {
	_obj->objShader.use();
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
	_obj->objShader.terminateShader();
	
}
#pragma endregion