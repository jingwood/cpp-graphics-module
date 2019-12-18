///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "vector.h"
#include "matrix.h"

namespace ugm {

vec4 vec4::operator*(const Matrix4& m) const {
	return vec4(this->x * m.a1 + this->y * m.a2 + this->z * m.a3 + this->w * m.a4,
		this->x * m.b1 + this->y * m.b2 + this->z * m.b3 + this->w * m.b4,
		this->x * m.c1 + this->y * m.c2 + this->z * m.c3 + this->w * m.c4,
		this->x * m.d1 + this->y * m.d2 + this->z * m.d3 + this->w * m.d4);
}

inline float vertex_angle(const vec3& v1, const vec3& v2) {
  return (float)acos(dot(v1, v2) / v1.length() / v2.length());
}

}