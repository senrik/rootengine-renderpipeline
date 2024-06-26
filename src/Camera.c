#include "Camera.h"

void Camera_Init(Camera* _cam) {
	_cam->position = (rt_vec3){ 0.0f, 0.0f, -3.0f };
	
	
	quat_init(&_cam->rotation);
	_cam->target = (rt_vec3){ 0.0f, 0.0f, 0.0f };

	
	rt_vec3 _diff = vec3_diff(&_cam->target, &_cam->position);
	vec3_copy_init(&_cam->forwardAxis, &_diff);
	
	_cam->upAxis = vec3_rotate(&(rt_vec3){ 0.0f, 1.0f, 0.0f }, &_cam->rotation);
	_cam->rightAxis = vec3_rotate(&(rt_vec3){ 1.0f, 0.0f, 0.0f }, &_cam->rotation);
	_cam->rotationSpeed = 1.5f;
	_cam->translationSpeed = 2.0f;
	
	mat4_init(&_cam->viewMat, 1.0f);
}

//rt_vec3 Camera_GetAxis() {
//	return upAxis;
//
//}


rt_mat4 Camera_GetViewMatrix(Camera* _cam) {
	// We update the view matrix on this call because it only needs to be updated when it is needed.
	mat4_init(&_cam->viewMat, 1.0f);
	mat4_translate(&_cam->viewMat, &_cam->position);
	_cam->viewMat = dot_product(quat_to_mat(_cam->rotation), &_cam->viewMat); // dot product
	return _cam->viewMat;
}

void Camera_Update(Camera* _cam) {
}

void Camera_ResetRotation(Camera* _cam) {
	quat_init(&_cam->rotation);
}

void Camera_Translate(Camera* _cam, rt_vec3 _axis, const float deltaTime) {
	rt_vec3 _norm = vec3_normalize(&_axis);
	rt_vec3 _scale = vec3_scale(&_norm, _cam->translationSpeed * deltaTime);
	rt_vec3 _rot = vec3_rotate(&_scale, &_cam->rotation);
	_cam->position = vec3_sum(&_cam->position, &_rot);
	// target is in world-space coordinates
	_cam->target = vec3_sum(&_cam->target, &_rot);
	//_cam->UpdatePosition(glm::normalize(_axis) * deltaTime * _cam->GetTranslationSpeed());
}

void Camera_Rotate(Camera* _cam, rt_vec3 _eulerVec, const float deltaTime) {
	rt_vec3 _norm = vec3_normalize(&_eulerVec);
	rt_vec3 _scale = vec3_scale(&_norm, _cam->rotationSpeed * deltaTime);
	rt_quat _euQuat = euler_to_quat(&_scale);
	_cam->rotation = compose_quat(&_euQuat, &_cam->rotation); // need to combine with the current rotation, not override it 
	_cam->target = vec3_rotate(&(rt_vec3) { 0.0f, 0.0f, 1.0f }, & _cam->rotation);
	rt_vec3 _diff = vec3_diff(&_cam->target, &_cam->position);
	_cam->forwardAxis = vec3_normalize(&_diff);
	_cam->upAxis = vec3_rotate(&(rt_vec3) { 0.0f, 1.0f, 0.0f }, & _cam->rotation);
	_cam->rightAxis = vec3_rotate(&(rt_vec3) { 1.0f, 0.0f, 0.0f }, & _cam->rotation);
}
