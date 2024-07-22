#include "Camera.h"

//void Camera_Init(Camera* _cam) {
//	_cam->position = glm::vec3(0.0f, 0.0f, -3.0f);
//	_cam->rotation = glm::quat();
//	_cam->target = glm::vec3(0.0f, 0.0f, 0.0f);
//	_cam->forwardAxis = glm::normalize(_cam->target - _cam->position);
//	_cam->upAxis = _cam->rotation * glm::vec3(0.0f, 1.0f, 0.0f);
//	_cam->rightAxis = _cam->rotation * glm::vec3(1.0f, 0.0f, 0.0f);
//	_cam->rotationSpeed = 1.5f;
//	_cam->translationSpeed = 2.0f;
//	
//	_cam->viewMat = glm::mat4(1.0f);
//}
//
////glm::vec3 Camera_GetAxis() {
////	return upAxis;
////
////}
//
//
//glm::mat4 Camera_GetViewMatrix(Camera* _cam) {
//	// We update the view matrix on this call because it only needs to be updated when it is needed.
//	_cam->viewMat = glm::mat4(1.0f);
//	_cam->viewMat = glm::translate(_cam->viewMat, _cam->position);
//	_cam->viewMat = glm::mat4_cast(_cam->rotation) * _cam->viewMat;
//	return _cam->viewMat;
//}
//
//void Camera_Update(Camera* _cam) {
//}
//
//void Camera_ResetRotation(Camera* _cam) {
//	_cam->rotation = glm::quat();
//}
//
//void Camera_Translate(Camera* _cam, glm::vec3 _axis, const float deltaTime) {
//	_cam->position += ( glm::normalize(_axis) * _cam->translationSpeed * deltaTime)*_cam->rotation;
//	// target is in world-space coordinates
//	_cam->target += (glm::normalize(_axis) * _cam->translationSpeed * deltaTime) * _cam->rotation;
//	//_cam->UpdatePosition(glm::normalize(_axis) * deltaTime * _cam->GetTranslationSpeed());
//}
//
//void Camera_Rotate(Camera* _cam, glm::vec3 _eulerVec, const float deltaTime) {
//	_cam->rotation = glm::quat(glm::normalize(_eulerVec) * _cam->rotationSpeed * deltaTime)*_cam->rotation;
//	_cam->target = _cam->rotation* glm::vec3(0.0f, 0.0f, 1.0f);
//	_cam->forwardAxis = glm::normalize(_cam->target - _cam->position);
//	_cam->upAxis = _cam->rotation * glm::vec3(0.0f, 1.0f, 0.0f);
//	_cam->rightAxis = _cam->rotation * glm::vec3(1.0f, 0.0f, 0.0f);
//}