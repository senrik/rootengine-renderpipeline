#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct Camera {
	glm::vec3 position, target, forwardAxis, rightAxis, upAxis;
	glm::quat rotation;
	glm::mat4 viewMat;
	float rotationSpeed, translationSpeed;
} Camera;

void Camera_Init(Camera*);
//void Camera_UpdateRotation(Camera*, glm::quat);
//void Camera_UpdatePosition(Camera*, glm::vec3);
glm::mat4 Camera_GetViewMatrix(Camera* _cam);
void Camera_Rotate(Camera*, glm::vec3, const float);
void Camera_ResetRotation(Camera*);
void Camera_Translate(Camera*, glm::vec3, const float);
void OrbitCamera_Rotate(Camera*, const float[], const float);

#endif // !CAMERA_H