#ifndef ROOTS_MATH_H
#define ROOTS_MATH_H
#include <math.h>


typedef struct rt_vec2 {
	union {
		struct {
			double x, y;
		};
		double v[2];
	}; 
} rt_vec2;

typedef struct rt_vec3 {
	union {
		struct { double x, y, z; };
		double v[3];
	};
} rt_vec3;

typedef struct rt_vec4 {
	union {
		struct { double x, y, z, w; };
		double v[4];
	};
} rt_vec4;

typedef struct rt_quat {
	union {
		struct { double x, y, z, w; };
		double v[4];
	};
} rt_quat;

typedef struct rt_mat4 {
	union {
		struct {
			double m00, m10, m20, m30,
				m01, m11, m21, m31,
				m02, m12, m22, m32,
				m03, m13, m23, m33;
		};
		rt_vec4 cols[4];
		double v[16];
	};
} rt_mat4;


void vec3_init(rt_vec3*, const double);
void vec3_copy_init(rt_vec3*, const rt_vec3*);
rt_vec3 vec3_normalize(const rt_vec3*);
rt_vec3 vec3_diff(const rt_vec3*, const rt_vec3*);
rt_vec3 vec3_sum(const rt_vec3*, const rt_vec3*);
rt_vec3 vec3_scale(const rt_vec3*, const double);
rt_vec3 vec3_cross_prod(const rt_vec3*, const rt_vec3*);
double vec3_dot_prod(const rt_vec3*, const rt_vec3*);
rt_vec3 vec3_cos(const rt_vec3*);
rt_vec3 vec3_sin(const rt_vec3*);
rt_vec3 vec3_rotate(const rt_vec3*, const rt_quat*);

rt_vec4 vec4_normalize(const rt_vec4*);
rt_vec4 vec4_diff(const rt_vec4*, const rt_vec4*);
rt_vec4 vec4_sum(const rt_vec4*, const rt_vec4*);
rt_vec4 vec4_scale(const rt_vec4*, const double);

void quat_init(rt_quat*);
rt_vec3 euler_angles(const rt_quat*);
rt_quat euler_to_quat(const rt_vec3*);
rt_quat compose_quat(const rt_quat*, const rt_quat*);

void mat4_init(rt_mat4*, const double);
void mat4_translate(rt_mat4*, const rt_vec3*);
rt_mat4 quat_to_mat4(const rt_quat*);
rt_mat4 dot_product(const rt_mat4*, const rt_mat4*);
rt_mat4 cross_product(const rt_mat4*, const rt_mat4*);

#endif // !ROOTS_MATH_H