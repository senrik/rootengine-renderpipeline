#include "render_object.h"

void Shader_Empty_Init(Shader* _shader) {
	_shader->ID = -1;
}

void Shader_Init(Shader* _shader, const char* vertexPath, uint vlength, const char* fragmentPath, uint flength) {

	rt_string vertexCode = read_textfile(vertexPath);
	rt_string fragmentCode = read_textfile(fragmentPath);

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode.data, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode.data, NULL);
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

void Shader_setBool(const Shader* _shader, const char* name, bool value) {
	glUniform1i(glGetUniformLocation(_shader->ID, name), (int)value);
}

void Shader_setInt(const Shader* _shader, const char* name, int value){
	glUniform1i(glGetUniformLocation(_shader->ID, name), value);
}

void Shader_setFloat(const Shader* _shader, const char* name, float value) {
	glUniform1f(glGetUniformLocation(_shader->ID, name), value);
}

void Shader_setMat4(const Shader* _shader, const char* name, const rt_mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(_shader->ID, name), 1, GL_FALSE, &value.cols[0].v[0]);
}

void Shader_terminateShader(const Shader* _shader) {
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
/*
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
*/
void RenderObj_Init(RenderObj* _obj) {
	
	if (_obj->meshesCount > 0) {
		for (int i = 0; i < _obj->meshesCount; i++) {
			if (&_obj->objMeshes[i] == NULL) continue;
			Mesh_Init(&_obj->objMeshes[i]);
		}
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