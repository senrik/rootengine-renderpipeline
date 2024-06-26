#include <roots_math.h>
// internal functions
double pitch(const rt_quat*);
double roll(const rt_quat*);
double yaw(const rt_quat*);
double clamp(const double, const double, const double);

void vec3_init(rt_vec3* _vec, const double _value) {
	_vec->x = _value;
	_vec->y = _value;
	_vec->z = _value;
}
void vec3_copy_init(rt_vec3* _vec, const rt_vec3* _other) {
	_vec->x = _other->x;
	_vec->y = _other->y;
	_vec->z = _other->z;
}
rt_vec3 vec3_normalize(const rt_vec3* _vec) {
	double _mag = sqrt((_vec->x * _vec->x) + (_vec->y * _vec->y) + (_vec->z * _vec->z));
	return (rt_vec3) { _vec->x / _mag, _vec->y / _mag, _vec->z / _mag };
}
rt_vec3 vec3_diff(const rt_vec3* _vecl, const rt_vec3* _vecr) {
	return (rt_vec3) { _vecl->x - _vecr->x, _vecl->y - _vecr->y, _vecl->z - _vecr->z };
}
rt_vec3 vec3_sum(const rt_vec3* _vecl, const rt_vec3* _vecr) {
	return (rt_vec3) {_vecl->x + _vecr->x, _vecl->y + _vecr->y,_vecl->z + _vecr->z};
}
rt_vec3 vec3_scale(const rt_vec3* _vec, const double _scale) {
	return (rt_vec3) { _vec->x *_scale, _vec->y* _scale, _vec->z *_scale };
}
rt_vec3 vec3_cross_prod(const rt_vec3* _vecl, const rt_vec3* _vecr) {

	return (rt_vec3) { _vecl->y* _vecr->z - _vecr->y * _vecl->z, _vecl->z* _vecr->x - _vecr->z * _vecl->x, _vecl->x* _vecr->y - _vecr->x * _vecl->y };

}
double vec3_dot_prod(const rt_vec3* _vecl, const rt_vec3* _vecr) {
	return (_vecl->x * _vecr->x) + (_vecl->y * _vecr->y) + (_vecl->z * _vecr->z);
}
rt_vec3 vec3_cos(const rt_vec3* _vec) {
	return (rt_vec3) { cos(_vec->x), cos(_vec->y), cos(_vec->z) };
}
rt_vec3 vec3_sin(const rt_vec3* _vec) {
	return (rt_vec3) { sin(_vec->x), sin(_vec->y), sin(_vec->z) };
}
rt_vec3 vec3_rotate(const rt_vec3* _vec, const rt_quat* _quat) {
	rt_vec3 _quatVec = (rt_vec3){_quat->x, _quat->y, _quat->z};
	rt_vec3 _uv = vec3_cross_prod(&_quatVec, _vec);
	rt_vec3 _uuv = vec3_cross_prod(&_quatVec, &_uv);

	rt_vec3 _scale = vec3_scale(&_uv, _quat->w);
	rt_vec3 _sum = vec3_sum(&_scale, &_uuv);
	rt_vec3 _sScale = vec3_scale(&_sum, 2.0f);
	return vec3_sum(_vec, &_sScale);
}

rt_vec4 vec4_normalize(const rt_vec4* _vec) {
	double _mag = sqrt((_vec->x * _vec->x) + (_vec->y * _vec->y) + (_vec->z * _vec->z) + (_vec->w * _vec->w));
	return (rt_vec4) { _vec->x / _mag, _vec->y / _mag, _vec->z / _mag, _vec->w/ _mag };
}
rt_vec4 vec4_diff(const rt_vec4* _vecl, const rt_vec4* _vecr) {
	return (rt_vec4) { _vecl->x - _vecr->x, _vecl->y - _vecr->y, _vecl->z - _vecr->z, _vecl->w - _vecr->w };
}
rt_vec4 vec4_sum(const rt_vec4* _vecl , const rt_vec4* _vecr) {
	return (rt_vec4) { _vecl->x + _vecr->x, _vecl->y + _vecr->y, _vecl->z + _vecr->z, _vecl->w + _vecr->w };
}
rt_vec4 vec4_scale(const rt_vec4* _vec, const double _scale) {
	return (rt_vec4) { _vec->x* _scale, _vec->y* _scale, _vec->z* _scale, _vec->w *_scale };
}


void quat_init(rt_quat* _quat) {
	_quat->x = 0.0f;
	_quat->y = 0.0f;
	_quat->z = 0.0f;
	_quat->w = 1.0f;
	
}
rt_vec3 euler_angles(const rt_quat* _quat) {
	return (rt_vec3) { pitch(_quat), yaw(_quat), roll(_quat) };
}
// Implementation taken from the GLM library
rt_quat euler_to_quat(const rt_vec3* _vec) {
	rt_vec3 _halfScale = vec3_scale(_vec, 0.5f);
	rt_vec3 c = vec3_cos(&_halfScale);
	rt_vec3 s = vec3_sin(&_halfScale);
	return (rt_quat) { s.x* c.y* c.z - c.x * s.y * s.z, c.x* s.y* c.z + s.x * c.y * s.z, c.x* c.y* s.z - s.x * s.y * c.z, c.x* c.y* c.z + s.x * s.y * s.z };
}
rt_quat compose_quat(const rt_quat* _quatl, const rt_quat* _quatr) {

}
// Implementation taken from the GLM library
double pitch(const rt_quat* q) {
	return atan(2 * (q->y * q->z + q->w * q->x), q->w * q->w - q->x * q->x - q->y * q->y + q->z * q->z);
}
double roll(const rt_quat* q) {
	return atan(2 * (q->x * q->y + q->w * q->z), q->w * q->w + q->x * q->x - q->y * q->y - q->z * q->z);
}
double yaw(const rt_quat* q) {
	
	return asin(clamp(-2 * (q->x * q->z - q->w * q->y), -1, 1));
}
double clamp(const double _v, const double _min, const double _max) {
	if (_v > _max) {
		return _max;
	}else if (_v < _min) {
		return _min;
	}
	else {
		return _v;
	}
}

void mat4_init(rt_mat4* _mat, const double _d) {
	for (int i = 0; i < 4; i++) {
		_mat->cols[i].v[i] = _d;
	}
}
void mat4_translate(rt_mat4* _mat, const rt_vec3* _vec) {

}
rt_mat4 quat_to_mat4(const rt_quat* _quat) {

}
rt_mat4 dot_product(const rt_mat4* _matr, const rt_mat4* _matl) {

}
rt_mat4 cross_product(const rt_mat4* _matr, const rt_mat4* _matl) {

}
