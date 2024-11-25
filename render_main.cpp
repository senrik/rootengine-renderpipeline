#include <stdlib.h>
#include <stdio.h>
#include <Camera.h>
#include <asset_io.h>
#include <render_cache.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#pragma region Callback functions Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
#pragma endregion

void processInput(GLFWwindow* window, float deltaTime);

// Render Pipeline boiler plate code
int RenderPipeline_BP();

#define SCRN_WIDTH 800
#define SCRN_HEIGHT 600
#define RENDER_CACHE_SIZE 200

static GLFWwindow* window;
static Camera* mainCamera;
float rotationSpeed = 30.0f;

int main(int argc, char* argv[]) {

#pragma region Boiler Plate
	RenderPipeline_BP();
#pragma endregion

	RenderCache cache;
	RenderCache_Init(&cache);	
	// Testing RenderObj
	RenderObj plane;
	RenderObj_Init(&plane);
	
	RenderObj_Add_Raw_Mesh(&plane, defaultPlaneVerts, DEFAULT_PLANE_VERT_COUNT, defaultPlaneIndices, DEFAULT_PLANE_VERT_COUNT);
	Texture* _tex = Mesh_GetTexture(&plane.objMeshes[0], 0);
	_tex->textureData = load_image("./assets/container.jpg", &_tex->t_width, &_tex->t_height, &_tex->nrChannels, 0);
	rt_string _vertCode, _fragCode;
	rt_string_init(&_vertCode);
	rt_string_init(&_fragCode);
	read_textfile(&_vertCode, "./assets/shaders/diffuse_texture.vertshader");
	//printf("Vertex Shader Code:\n============================================\n%s\n", _vertCode.data);
	read_textfile(&_fragCode, "./assets/shaders/diffuse_texture.fragshader");
	//printf("Fragment Shader Code:\n============================================\n%s\n", _fragCode.data);
	Shader_Init(&plane.objShader, &_vertCode, &_fragCode);
	RenderObj_Register(&plane);
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
		rt_mat4 view = Camera_GetViewMatrix(mainCamera);
		rt_mat4 proj;
		mat4_init(&proj, 1.0f);
		//proj = glm::perspective(glm::radians(45.0f), (float)SCRN_WIDTH / (float)SCRN_HEIGHT, 0.01f, 1000.0f);

		//RenderCache_Draw(view, proj, timeValue);'
		RenderObj_Draw(&plane);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwPollEvents();
		glfwSwapBuffers(window);
		// Update delta time
		deltaTime = timeValue;
	}
	//RenderCache_Clear();
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
	return 0;
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
		rt_vec3 _movementVec;
		vec3_init(&_movementVec, 0.0f);
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

		//Camera_Translate(mainCamera, _movementVec, deltaTime);
	}
	// ================================== CAMERA ROTATION ======================================
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		
		rt_vec3 _eulerRotation;
		vec3_init(&_eulerRotation, 0.0f);
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
		//Camera_Rotate(mainCamera, _eulerRotation, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
		//Camera_ResetRotation(mainCamera);
	}
}