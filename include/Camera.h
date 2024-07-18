#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera {
	
	// pos target, forwardAxis, rightAxis, upAxis - vec3
	// rotation - quat
	
	// vewMat mat4
	
	float rotationSpeed, translationSpeed;
} Camera;

void Camera_Init(Camera*);
//void Camera_UpdateRotation(Camera*, glm::quat);
//void Camera_UpdatePosition(Camera*, glm::vec3);
//glm::mat4 Camera_GetViewMatrix(Camera* _cam);
//void Camera_Rotate(Camera*, glm::vec3, const float);
void Camera_ResetRotation(Camera*);
//void Camera_Translate(Camera*, glm::vec3, const float);
void OrbitCamera_Rotate(Camera*, const float[], const float);

#endif // !CAMERA_H