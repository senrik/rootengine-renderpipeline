#ifndef CAMERA_H
#define CAMERA_H
#include <roots_math.h>
#ifdef __cplusplus
extern "C" {
#endif


typedef struct Camera {
	rt_vec3 position, target, forwardAxis, rightAxis, upAxis;
	rt_quat rotation;
	rt_mat4 viewMat;
	float rotationSpeed, translationSpeed;
} Camera;

void Camera_Init(Camera*);
//void Camera_UpdateRotation(Camera*, glm::quat);
//void Camera_UpdatePosition(Camera*, glm::vec3);
rt_mat4 Camera_GetViewMatrix(Camera* _cam);
void Camera_Rotate(Camera*, rt_vec3, const float);
void Camera_ResetRotation(Camera*);
void Camera_Translate(Camera*, rt_vec3, const float);
void OrbitCamera_Rotate(Camera*, const float[], const float);

#ifdef __cplusplus
}
#endif
#endif // !CAMERA_H