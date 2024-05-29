#include <stdio.h>
#include <math.h>
#include <vector>
#include <glad/glad.h> //manages function pointers for OpenGL
#include <GLFW/glfw3.h> // Abstraction layer for targeting multiple systems with OpenGL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "render_object.hpp"



#pragma region Callback functions Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
#pragma endregion



void processInput(GLFWwindow* window, float deltaTime);

// Render Pipeline boiler plate code
int RenderPipeline_BP();

void RenderCache_Init();
void RenderCache_Add(const RenderObj*);
void RenderCache_Add(const float* verts, unsigned int _vertCount, unsigned int* indices, unsigned int _indicesCount, const float _xPos, const float _yPos, const float _zPos);
void RenderCache_Draw(glm::mat4, glm::mat4,float);
void RenderCache_Clear();
void RenderCache_AddMesh(const char*, ufbx_load_opts*, ufbx_error*);
#define SCRN_WIDTH 800
#define SCRN_HEIGHT 600
#define RENDER_CACHE_SIZE 200

static RenderObj* renderCache;
static unsigned int cacheSize = 0;
static GLFWwindow* window;
static Camera* mainCamera;
float rotationSpeed = 30.0f;

int main(int argc, char* argv[]) {
  
#pragma region Boiler Plate
	RenderPipeline_BP();
#pragma endregion

	renderCache = (RenderObj*)malloc(sizeof(RenderObj) * RENDER_CACHE_SIZE);

#pragma region Cube
	float cubeVerts[] = {
		-0.5f, -0.5f, -0.5f,     0.0f, 0.0f,     0.0f, -1.0f,  0.0f, // 0 - bottom left
		 0.5f, -0.5f, -0.5f,     1.0f, 0.0f,     0.0f, -1.0f,  0.0f, // 1 - bottom right
		 0.5f,  0.5f, -0.5f,     1.0f, 1.0f,     0.0f,  1.0f,  0.0f, // 2 - top right
		-0.5f,  0.5f, -0.5f,     0.0f, 1.0f,     0.0f,  1.0f,  0.0f, // 3 - top left

		-0.5f, -0.5f,  0.5f,     1.0f, 1.0f,     0.0f, -1.0f,  0.0f, // 4 - bottom left
		 0.5f, -0.5f,  0.5f,     0.0f, 1.0f,     0.0f, -1.0f,  0.0f, // 5 - bottom right
		 0.5f,  0.5f,  0.5f,     0.0f, 0.0f,     0.0f,  1.0f,  0.0f, // 6 - top right
		-0.5f,  0.5f,  0.5f,     1.0f, 0.0f,     0.0f,  1.0f,  0.0f, // 7 - top left

	};
	unsigned int cubeIndices[] = {
		3, 1, 0,
		1, 2, 3,

		7, 4, 5,
		5, 6, 7,

		7, 3, 2,
		2, 6, 7,

		4, 0, 1,
		1, 5, 4,

		2, 1, 5,
		5, 6, 2,

		3, 0, 4,
		4, 7, 3

	};

	//RenderCache_Add(cubeVerts, 40, cubeIndices, 36, 0.0f, -0.5f, 0.0f);
	
#pragma endregion
	
#pragma region Diamond
	float diamondVerts[] = {
		 // position			// texcoords     // normal
		 0.0f,  0.5f,  0.0f,    0.5f,  0.5f,     0.0f,  1.0f,  0.0f,// 0 - top
		-0.5f,  0.0f, -0.5f,    0.0f,  0.0f,     0.0f,  0.0f,  1.0f, // 1 - back left
		 0.5f,  0.0f, -0.5f,    0.0f,  1.0f,     0.0f,  0.0f,  1.0f, // 2 - back right
		-0.5f,  0.0f,  0.5f,    0.0f,  1.0f,     0.0f,  0.0f,  1.0f, // 3 - front left
		 0.5f,  0.0f,  0.5f,    1.0f,  0.0f,     0.0f,  0.0f,  1.0f, // 4 - front right
		 0.0f, -0.5f,  0.0f,    0.5f,  0.5f,     0.0f, -1.0f,  0.0f, // 5 - bottom
		
	};
	unsigned int diamondIndices[] = {
		0, 1, 2,
		0, 2, 4,
		0, 1, 3,
		0, 3, 4,
		5, 2, 1,
		5, 4, 2,
		5, 3, 1,
		5, 4, 3,
	};
	

	RenderCache_Add(diamondVerts, 48, diamondIndices, 24, -1.0f, 0.5f, 0.0f);
	
#pragma endregion
	
#pragma region V22
	
	ufbx_load_opts opts = { 0 };
	ufbx_error fbx_error;
	
	//RenderCache_AddMesh("cv22_rig01_export08.fbx", &opts, &fbx_error);
#pragma endregion
	
	RenderCache_Init();
	float deltaTime = 0;
	while (!glfwWindowShouldClose(window)) {
		float timeValue = (float)glfwGetTime();
		deltaTime = timeValue - deltaTime;
		// INPUT
		processInput(window, deltaTime);


		// RENDER CALLS
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set the camera space
		glm::mat4 view = Camera_GetViewMatrix(mainCamera);
		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(45.0f), (float)SCRN_WIDTH / (float)SCRN_HEIGHT, 0.01f, 1000.0f);

		RenderCache_Draw(view, proj, timeValue);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
		// Update delta time
		deltaTime = timeValue;
	}
	RenderCache_Clear();
	free(mainCamera);
	glfwTerminate();
	return 0;
}

#pragma region Callback functions Implimentations
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
#pragma endregion

int RenderPipeline_BP() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCRN_WIDTH, SCRN_HEIGHT, "Root Engine Render Pipeline", NULL, NULL);
	if (window == NULL) {
		printf("Failed to create GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD.\n");
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	mainCamera = (Camera*)malloc(sizeof(Camera));
	Camera_Init(mainCamera);
}


void RenderCache_Init() {
	for (int i = 0; i < cacheSize; i++) {

		RenderObj_Init(&renderCache[i]);
	}
}

void RenderCache_Add(const RenderObj* _obj) {
	renderCache[cacheSize] = *_obj;
	cacheSize++;
}

void RenderCache_Add(const float* verts, unsigned int _vertCount, unsigned int* indices, unsigned int _indicesCount, const float _xPos, const float _yPos, const float _zPos) {
	RenderObj obj;
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
		for (int vi = 0; vi < _vertCount; vi+=8) {
			_mesh->vertices[vi / _mesh->totalSpan].position.x = verts[vi];
			_mesh->vertices[vi / _mesh->totalSpan].position.y = verts[vi+1];
			_mesh->vertices[vi / _mesh->totalSpan].position.z = verts[vi+2];

			_mesh->vertices[vi / _mesh->totalSpan].textureCoords.x = verts[vi+3];
			_mesh->vertices[vi / _mesh->totalSpan].textureCoords.y = verts[vi+4];

			_mesh->vertices[vi / _mesh->totalSpan].normal.x = verts[vi+5];
			_mesh->vertices[vi / _mesh->totalSpan].normal.y = verts[vi+6];
			_mesh->vertices[vi / _mesh->totalSpan].normal.z = verts[vi+7];
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
	obj.rotation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));

	RenderCache_Add(&obj);
}

void RenderCache_AddMesh(const char* _scene, ufbx_load_opts* opts, ufbx_error* fbx_error) {
	auto scene = ufbx_load_file(_scene, opts, fbx_error);
	if (!scene) {
		fprintf(stderr, "Failed to load: %s\n", fbx_error->description.data);
		exit(1);
	}
	
	RenderObj* _fbxObj = (RenderObj*)malloc(sizeof(RenderObj));
	RenderObj_ReadFBX(_fbxObj, scene, "CV22TS.jpg");
	
	RenderCache_Add(_fbxObj);
}

void RenderCache_Draw(glm::mat4 _view, glm::mat4 _proj, float _time) {
	for (int i = 0;i < cacheSize; i++)
	{
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(renderCache[i].xPos, renderCache[i].yPos, renderCache[i].zPos));
		
		renderCache[i].objShader.setMat4("view", _view);
		renderCache[i].objShader.setMat4("projection", _proj);
		renderCache[i].objShader.setMat4("model", model);
		RenderObj_Draw(&renderCache[i]);
	}
}

void RenderCache_Clear() {
	for (int i = 0; i < cacheSize; i++)
	{
		RenderObj_Terminate(&renderCache[i]);
	}
	free(renderCache);
	cacheSize = 0;
}

void processInput(GLFWwindow* window, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}	

	// ================================== CAMERA MOVEMENT ======================================
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || 
		glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || 
		glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || 
		glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		glm::vec3 _movementVec = glm::vec3(0.0f);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			_movementVec.x = 1.0f;

		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			_movementVec.x = -1.0f;

		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			_movementVec.z = 1.0f;

		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			_movementVec.z = -1.0f;

		}

		Camera_Translate(mainCamera, _movementVec, deltaTime);
	}
	// ================================== CAMERA ROTATION ======================================
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		
		glm::vec3 _eulerRotation = glm::vec3(0.0f);
		// rotation around the x-axis
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			_eulerRotation.x = -1.0f;
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			_eulerRotation.x = 1.0f;
		}

		// rotationa round the y-axis
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			_eulerRotation.y = -1.0f;
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			_eulerRotation.y = 1.0f;
		}
		//printf("Euler Rotation: {%.1F, %.1F, %.1F}\n", _eulerRotation.x, _eulerRotation.y, _eulerRotation.z);
		Camera_Rotate(mainCamera, _eulerRotation, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
		Camera_ResetRotation(mainCamera);
	}
}