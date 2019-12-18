///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "matrix.h"
#include "functions.h"

namespace ugm {

const float Matrix4::identity[] =
{
  1.f, 0.f, 0.f, 0.f,
  0.f, 1.f, 0.f, 0.f,
  0.f, 0.f, 1.f, 0.f,
  0.f, 0.f, 0.f, 1.f,
};

Matrix4& Matrix4::rotate(const vec3& angles) {
	this->rotateX(angles.x);
	this->rotateY(angles.y);
	this->rotateZ(angles.z);

	return *this;
}

Matrix4& Matrix4::rotate(const float x, const float y, const float z) {
  this->rotateX(x);
  this->rotateY(y);
  this->rotateZ(z);
	
	return *this;
}

void Matrix4::rotateX(const float angle)
{
  const float d = angle * M_PI / 180.F;
  
  const float s = (float)sin(d);
  const float c = (float)cos(d);
  
  const float m2b2 = c, m2c2 = s;
  const float m2b3 = -s, m2c3 = c;
  
  // post
  const float a2 = this->a2 * m2b2 + this->a3 * m2c2;
  const float b2 = this->b2 * m2b2 + this->b3 * m2c2;
  const float c2 = this->c2 * m2b2 + this->c3 * m2c2;
  const float d2 = this->d2 * m2b2 + this->d3 * m2c2;
  
  const float a3 = this->a2 * m2b3 + this->a3 * m2c3;
  const float b3 = this->b2 * m2b3 + this->b3 * m2c3;
  const float c3 = this->c2 * m2b3 + this->c3 * m2c3;
  const float d3 = this->d2 * m2b3 + this->d3 * m2c3;
  
  this->a2 = a2; this->b2 = b2; this->c2 = c2; this->d2 = d2;
  this->a3 = a3; this->b3 = b3; this->c3 = c3; this->d3 = d3;
}

void Matrix4::rotateY(const float angle)
{
  const float d = angle * M_PI / 180.0F;
  
  const float s = (float)sin(d);
  const float c = (float)cos(d);
  
  const float m2a1 = c, m2c1 = -s;
  const float m2a3 = s, m2c3 = c;

  const float a1 = this->a1 * m2a1 + this->a3 * m2c1;
  const float b1 = this->b1 * m2a1 + this->b3 * m2c1;
  const float c1 = this->c1 * m2a1 + this->c3 * m2c1;
  const float d1 = this->d1 * m2a1 + this->d3 * m2c1;
  
  const float a3 = this->a1 * m2a3 + this->a3 * m2c3;
  const float b3 = this->b1 * m2a3 + this->b3 * m2c3;
  const float c3 = this->c1 * m2a3 + this->c3 * m2c3;
  const float d3 = this->d1 * m2a3 + this->d3 * m2c3;
  
  this->a1 = a1; this->b1 = b1; this->c1 = c1; this->d1 = d1;
  this->a3 = a3; this->b3 = b3; this->c3 = c3; this->d3 = d3;
}

void Matrix4::rotateZ(const float angle)
{
  const float d = angle * M_PI / 180.0F;
  
  const float s = (float)sin(d);
  const float c = (float)cos(d);
  
  const float m2a1 = c, m2b1 = s;
  const float m2a2 = -s, m2b2 = c;
  
  const float a1 = this->a1 * m2a1 + this->a2 * m2b1;
  const float b1 = this->b1 * m2a1 + this->b2 * m2b1;
  const float c1 = this->c1 * m2a1 + this->c2 * m2b1;
  const float d1 = this->d1 * m2a1 + this->d2 * m2b1;
  
  const float a2 = this->a1 * m2a2 + this->a2 * m2b2;
  const float b2 = this->b1 * m2a2 + this->b2 * m2b2;
  const float c2 = this->c1 * m2a2 + this->c2 * m2b2;
  const float d2 = this->d1 * m2a2 + this->d2 * m2b2;
  
  this->a1 = a1; this->b1 = b1; this->c1 = c1; this->d1 = d1;
  this->a2 = a2; this->b2 = b2; this->c2 = c2; this->d2 = d2;
}

void Matrix4::scale(const vec3& s) {
	this->scale(s.x, s.y, s.z);
}

void Matrix4::scale(const float x, const float y, const float z) {
  if (x == 1 && y == 1 && z == 1) return;
  
  this->a1 *= x; this->b1 *= x; this->c1 *= x; this->d1 *= x;
  this->a2 *= y; this->b2 *= y; this->c2 *= y; this->d2 *= y;
  this->a3 *= z; this->b3 *= z; this->c3 *= z; this->d3 *= z;
}

Matrix4& Matrix4::translate(const float x, const float y, const float z)
{
	if (x != 0 || y != 0 || z != 0) {
		this->a4 += this->a1 * x + this->a2 * y + this->a3 * z;
		this->b4 += this->b1 * x + this->b2 * y + this->b3 * z;
		this->c4 += this->c1 * x + this->c2 * y + this->c3 * z;
		this->d4 += this->d1 * x + this->d2 * y + this->d3 * z;
	}
	
	return *this;
}

float Matrix4::determinant() const
{
  const float
		a = a1, b = b1, c = c1, d = d1,
		e = a2, f = b2, g = c2, h = d2,
		i = a3, j = b3, k = c3, l = d3,
		m = a4, n = b4, o = c4, p = d4;
  
  const float q = f * k * p + j * o * h + n * g * l
		- f * l * o - g * j * p - h * k * n;
  
  const float r = e * k * p + i * o * h + m * g * l
		- e * l * o - g * i * p - h * k * m;
  
  const float s = e * j * p + i * n * h + m * f * l
		- e * l * n - f * i * p - h * j * m;
  
  const float t = e * j * o + i * n * g + m * f * k
		- e * k * n - f * i * o - g * j * m;
  
  return (a * q - b * r + c * s - d * t);
}

bool Matrix4::canInverse() const
{
  return (this->determinant() != 0);
}

void Matrix4::inverse()
{
  const float
		a = a1, b = b1, c = c1, d = d1,
		e = a2, f = b2, g = c2, h = d2,
		i = a3, j = b3, k = c3, l = d3,
		m = a4, n = b4, o = c4, p = d4;
  
  const float q = f * k * p + j * o * h + n * g * l
		- f * l * o - g * j * p - h * k * n;
  
  const float r = e * k * p + i * o * h + m * g * l
		- e * l * o - g * i * p - h * k * m;
  
  const float s = e * j * p + i * n * h + m * f * l
		- e * l * n - f * i * p - h * j * m;
  
  const float t = e * j * o + i * n * g + m * f * k
		- e * k * n - f * i * o - g * j * m;
  
  const float delta = (a * q - b * r + c * s - d * t);
  
  if (delta == 0) return;
  
  float detM = 1 / delta;
  
  // adj
  float m2a1 = q, m2b1 = r, m2c1 = s, m2d1 = t;
  float m2a2 = b * k * p + j * o * d + n * c * l - b * l * o - c * j * p - d * k * n;
  float m2b2 = a * k * p + i * o * d + m * c * l - a * l * o - c * i * p - d * k * m;
  float m2c2 = a * j * p + i * n * d + m * b * l - a * l * n - b * i * p - d * j * m;
  float m2d2 = a * j * o + i * n * c + m * b * k - a * k * n - b * i * o - c * j * m;
  float m2a3 = b * g * p + f * o * d + n * c * h - b * h * o - c * f * p - d * g * n;
  float m2b3 = a * g * p + e * o * d + m * c * h - a * h * o - c * e * p - d * g * m;
  float m2c3 = a * f * p + e * n * d + m * b * h - a * h * n - b * e * p - d * f * m;
  float m2d3 = a * f * o + e * n * c + m * b * g - a * g * n - b * e * o - c * f * m;
  float m2a4 = b * g * l + f * k * d + j * c * h - b * h * k - c * f * l - d * g * j;
  float m2b4 = a * g * l + e * k * d + i * c * h - a * h * k - c * e * l - d * g * i;
  float m2c4 = a * f * l + e * j * d + i * b * h - a * h * j - b * e * l - d * f * i;
  float m2d4 = a * f * k + e * j * c + i * b * g - a * g * j - b * e * k - c * f * i;
  
  m2b1 = -m2b1; m2d1 = -m2d1;
  m2a2 = -m2a2; m2c2 = -m2c2;
  m2b3 = -m2b3; m2d3 = -m2d3;
  m2a4 = -m2a4; m2c4 = -m2c4;
  
  // transpose
  float m3a1 = m2a1, m3b1 = m2a2, m3c1 = m2a3, m3d1 = m2a4;
  float m3a2 = m2b1, m3b2 = m2b2, m3c2 = m2b3, m3d2 = m2b4;
  float m3a3 = m2c1, m3b3 = m2c2, m3c3 = m2c3, m3d3 = m2c4;
  float m3a4 = m2d1, m3b4 = m2d2, m3c4 = m2d3, m3d4 = m2d4;
  
  this->a1 = m3a1 * detM; this->b1 = m3b1 * detM; this->c1 = m3c1 * detM; this->d1 = m3d1 * detM;
  this->a2 = m3a2 * detM; this->b2 = m3b2 * detM; this->c2 = m3c2 * detM; this->d2 = m3d2 * detM;
  this->a3 = m3a3 * detM; this->b3 = m3b3 * detM; this->c3 = m3c3 * detM; this->d3 = m3d3 * detM;
  this->a4 = m3a4 * detM; this->b4 = m3b4 * detM; this->c4 = m3c4 * detM; this->d4 = m3d4 * detM;
}

void Matrix4::transpose()
{
  const float a2 = this->b1;
  const float a3 = this->c1;
  const float a4 = this->d1;
  
  const float b1 = this->a2;
  const float b3 = this->c2;
  const float b4 = this->d2;
  
  const float c1 = this->a3;
  const float c2 = this->b3;
  const float c4 = this->d3;
  
  const float d1 = this->a4;
  const float d2 = this->b4;
  const float d3 = this->c4;
  
  this->b1 = b1; this->c1 = c1; this->d1 = d1;
  this->a2 = a2; this->c2 = c2; this->d2 = d2;
  this->a3 = a3; this->b3 = b3; this->d3 = d3;
  this->a4 = a4; this->b4 = b4; this->c4 = c4;
}

Matrix4 Matrix4::operator*(const Matrix4& m2) const
{
//  const float m[] = {
//    this->a1 * m2.a1 + this->b1 * m2.a2 + this->c1 * m2.a3 + this->d1 * m2.a4,
//    this->a1 * m2.b1 + this->b1 * m2.b2 + this->c1 * m2.b3 + this->d1 * m2.b4,
//    this->a1 * m2.c1 + this->b1 * m2.c2 + this->c1 * m2.c3 + this->d1 * m2.c4,
//    this->a1 * m2.d1 + this->b1 * m2.d2 + this->c1 * m2.d3 + this->d1 * m2.d4,
//    this->a2 * m2.a1 + this->b2 * m2.a2 + this->c2 * m2.a3 + this->d2 * m2.a4,
//    this->a2 * m2.b1 + this->b2 * m2.b2 + this->c2 * m2.b3 + this->d2 * m2.b4,
//    this->a2 * m2.c1 + this->b2 * m2.c2 + this->c2 * m2.c3 + this->d2 * m2.c4,
//    this->a2 * m2.d1 + this->b2 * m2.d2 + this->c2 * m2.d3 + this->d2 * m2.d4,
//    this->a3 * m2.a1 + this->b3 * m2.a2 + this->c3 * m2.a3 + this->d3 * m2.a4,
//    this->a3 * m2.b1 + this->b3 * m2.b2 + this->c3 * m2.b3 + this->d3 * m2.b4,
//    this->a3 * m2.c1 + this->b3 * m2.c2 + this->c3 * m2.c3 + this->d3 * m2.c4,
//    this->a3 * m2.d1 + this->b3 * m2.d2 + this->c3 * m2.d3 + this->d3 * m2.d4,
//    this->a4 * m2.a1 + this->b4 * m2.a2 + this->c4 * m2.a3 + this->d4 * m2.a4,
//    this->a4 * m2.b1 + this->b4 * m2.b2 + this->c4 * m2.b3 + this->d4 * m2.b4,
//    this->a4 * m2.c1 + this->b4 * m2.c2 + this->c4 * m2.c3 + this->d4 * m2.c4,
//    this->a4 * m2.d1 + this->b4 * m2.d2 + this->c4 * m2.d3 + this->d4 * m2.d4,
//  };
	
	const float m[] = {
		this->a1 * m2.a1 + this->a2 * m2.b1 + this->a3 * m2.c1 + this->a4 * m2.d1,
		this->b1 * m2.a1 + this->b2 * m2.b1 + this->b3 * m2.c1 + this->b4 * m2.d1,
		this->c1 * m2.a1 + this->c2 * m2.b1 + this->c3 * m2.c1 + this->c4 * m2.d1,
		this->d1 * m2.a1 + this->d2 * m2.b1 + this->d3 * m2.c1 + this->d4 * m2.d1,
		this->a1 * m2.a2 + this->a2 * m2.b2 + this->a3 * m2.c2 + this->a4 * m2.d2,
		this->b1 * m2.a2 + this->b2 * m2.b2 + this->b3 * m2.c2 + this->b4 * m2.d2,
		this->c1 * m2.a2 + this->c2 * m2.b2 + this->c3 * m2.c2 + this->c4 * m2.d2,
		this->d1 * m2.a2 + this->d2 * m2.b2 + this->d3 * m2.c2 + this->d4 * m2.d2,
		this->a1 * m2.a3 + this->a2 * m2.b3 + this->a3 * m2.c3 + this->a4 * m2.d3,
		this->b1 * m2.a3 + this->b2 * m2.b3 + this->b3 * m2.c3 + this->b4 * m2.d3,
		this->c1 * m2.a3 + this->c2 * m2.b3 + this->c3 * m2.c3 + this->c4 * m2.d3,
		this->d1 * m2.a3 + this->d2 * m2.b3 + this->d3 * m2.c3 + this->d4 * m2.d3,
		this->a1 * m2.a4 + this->a2 * m2.b4 + this->a3 * m2.c4 + this->a4 * m2.d4,
		this->b1 * m2.a4 + this->b2 * m2.b4 + this->b3 * m2.c4 + this->b4 * m2.d4,
		this->c1 * m2.a4 + this->c2 * m2.b4 + this->c3 * m2.c4 + this->c4 * m2.d4,
		this->d1 * m2.a4 + this->d2 * m2.b4 + this->d3 * m2.c4 + this->d4 * m2.d4,
	};
	
  return Matrix4(m);
}

vec4 Matrix4::operator*(const vec4& v) const {
	return vec4(this->a1 * v.x + this->a2 * v.y + this->a3 * v.z + this->a4 * v.w,
							this->b1 * v.x + this->b2 * v.y + this->b3 * v.z + this->b4 * v.w,
							this->c1 * v.x + this->c2 * v.y + this->c3 * v.z + this->c4 * v.w,
							this->d1 * v.x + this->d2 * v.y + this->d3 * v.z + this->d4 * v.w);
}

void Matrix4::frustum(const float left, const float right, const float bottom, const float top,
                      const float near, const float far) {
  
  float x = right - left, y = bottom - top, z = far - near;
  
  this->a1 = near * 2 / x; this->b1 = 0; this->c1 = 0; this->d1 = 0;
  this->a2 = 0; this->b2 = near * 2 / y; this->c2 = 0; this->d2 = 0;
  this->a3 = (right + left) / x; this->b3 = (bottom + top) / y; this->c3 = -(far + near) / z; this->d3 = -1;
  this->a4 = 0; this->b4 = 0; this->c4 = -(far * near * 2) / z; this->d4 = 0;
}

void Matrix4::perspective(const float angle, const float widthAspect, const float near, const float far) {
  const float heightRate = (float)(near * tan(angle * M_PI / 360.0f));
  const float rate = heightRate * widthAspect;
  this->frustum(-rate, rate, heightRate, -heightRate, near, far);
};

void Matrix4::ortho(const float left, const float right, const float top, const float bottom,
                    const float near, const float far) {
  const float x = right - left, y = bottom - top, z = far - near;
  
  this->a1 = 2 / x; this->b1 = 0; this->c1 = 0; this->d1 = 0;
  this->a2 = 0; this->b2 = 2 / y; this->c2 = 0; this->d2 = 0;
  this->a3 = 0; this->b3 = 0; this->c3 = -2 / z; this->d3 = 0;
  this->a4 = -(left + right) / x; this->b4 = -(bottom + top) / y; this->c4 = -(far + near) / z; this->d4 = 1;
};

Matrix4& Matrix4::lookAt(const vec3& eye, const vec3& target, const vec3& up) {
  const vec3 zaxis = normalize(eye - target);         // forward
  const vec3 xaxis = normalize(cross(up, zaxis));     // right
  const vec3 yaxis = cross(zaxis, xaxis);             // up
  
  this->a1 = xaxis.x; this->b1 = yaxis.x; this->c1 = zaxis.x; this->d1 = 0;
  this->a2 = xaxis.y; this->b2 = yaxis.y; this->c2 = zaxis.y; this->d2 = 0;
  this->a3 = xaxis.z; this->b3 = yaxis.z; this->c3 = zaxis.z; this->d3 = 0;
  //this->a4 = -dot(xaxis, eye); this->b4 = -dot(yaxis, eye); this->c4 = -dot(zaxis, eye); this->d4 = 1;
	this->a4 = 0; this->b4 = 0; this->c4 = 0; this->d4 = 0;
	
	return *this;
};

vec3 Matrix4::extractEulerAngles() const {
	const float
		m11 = this->a1, m13 = this->c1,
		m21 = this->a2, m22 = this->b2, m23 = this->c2,
		m31 = this->a3, m33 = this->c3;
	
	float x, y, z;
	
	if (m21 > 0.99999f || m21 < -0.99999f) {
		x = 0.0f;
		y = atan2f(m13, m33);
		z = -M_PI * 0.5f;
	} else {
		x = atan2f(m23, m22);
		y = atan2f(m31, m11);
		z = asinf(-m21);
	}
	
	return vec3(DEGREE_TO_ANGLE(x), DEGREE_TO_ANGLE(y), DEGREE_TO_ANGLE(z));
}

void Matrix4::extractLookAtVectors(vec3 *dir, vec3 *up) const {
	*dir = normalize(vec3(this->c1, this->c2, -this->c3));
	*up  = normalize(vec3(this->b1, this->b2, -this->b3));
}

const float Matrix3::identity[] = {
  1, 0, 0,
  0, 1, 0,
  0, 0, 1,
};

void Matrix3::rotate(const float x, const float y) {
	this->rotateX(x);
	this->rotateY(y);
}

void Matrix3::rotateX(const float angle)
{
	const float d = angle * M_PI / 180.F;

	const float s = (float)sin(d);
	const float c = (float)cos(d);

	const float m2b2 = c, m2c2 = s;
	const float m2b3 = -s, m2c3 = c;

	// post
	const float a2 = this->a2 * m2b2 + this->a3 * m2c2;
	const float b2 = this->b2 * m2b2 + this->b3 * m2c2;
	const float c2 = this->c2 * m2b2 + this->c3 * m2c2;

	const float a3 = this->a2 * m2b3 + this->a3 * m2c3;
	const float b3 = this->b2 * m2b3 + this->b3 * m2c3;
	const float c3 = this->c2 * m2b3 + this->c3 * m2c3;

	this->a2 = a2; this->b2 = b2; this->c2 = c2;
	this->a3 = a3; this->b3 = b3; this->c3 = c3;
}

void Matrix3::rotateY(const float angle)
{
	const float d = angle * M_PI / 180.0F;

	const float s = (float)sin(d);
	const float c = (float)cos(d);

	const float m2a1 = c, m2c1 = -s;
	const float m2a3 = s, m2c3 = c;

	const float a1 = this->a1 * m2a1 + this->a3 * m2c1;
	const float b1 = this->b1 * m2a1 + this->b3 * m2c1;
	const float c1 = this->c1 * m2a1 + this->c3 * m2c1;

	const float a3 = this->a1 * m2a3 + this->a3 * m2c3;
	const float b3 = this->b1 * m2a3 + this->b3 * m2c3;
	const float c3 = this->c1 * m2a3 + this->c3 * m2c3;

	this->a1 = a1; this->b1 = b1; this->c1 = c1;
	this->a3 = a3; this->b3 = b3; this->c3 = c3;
}

void Matrix3::rotateZ(const float angle)
{
	const float d = angle * M_PI / 180.0F;

	const float s = (float)sin(d);
	const float c = (float)cos(d);

	const float m2a1 = c, m2b1 = s;
	const float m2a2 = -s, m2b2 = c;

	const float a1 = this->a1 * m2a1 + this->a2 * m2b1;
	const float b1 = this->b1 * m2a1 + this->b2 * m2b1;
	const float c1 = this->c1 * m2a1 + this->c2 * m2b1;

	const float a2 = this->a1 * m2a2 + this->a2 * m2b2;
	const float b2 = this->b1 * m2a2 + this->b2 * m2b2;
	const float c2 = this->c1 * m2a2 + this->c2 * m2b2;

	this->a1 = a1; this->b1 = b1; this->c1 = c1;
	this->a2 = a2; this->b2 = b2; this->c2 = c2;
}


void Matrix3::scale(const float x, const float y)
{
	if (x == 1 && y == 1) return;

	this->a1 *= x; this->b1 *= x; this->c1 *= x;
	this->a2 *= y; this->b2 *= y; this->c2 *= y;
}

void Matrix3::translate(const float x, const float y)
{
	if (x == 0 && y == 0) return;

	this->a3 += this->a1 * x + this->a2 * y;
	this->b3 += this->b1 * x + this->b2 * y;
	this->c3 += this->c1 * x + this->c2 * y;
}

void Matrix3::transpose()
{
	const float a2 = this->b1;
	const float a3 = this->c1;

	const float b1 = this->a2;
	const float b3 = this->c2;

	const float c1 = this->a3;
	const float c2 = this->b3;

	this->b1 = b1; this->c1 = c1;
	this->a2 = a2; this->c2 = c2;
	this->a3 = a3; this->b3 = b3;
}

Matrix3 Matrix3::operator*(const Matrix3& m2) const
{
	float m[] = {
		this->a1 * m2.a1 + this->b1 * m2.a2 + this->c1 * m2.a3,
		this->a1 * m2.b1 + this->b1 * m2.b2 + this->c1 * m2.b3,
		this->a1 * m2.c1 + this->b1 * m2.c2 + this->c1 * m2.c3,
		this->a2 * m2.a1 + this->b2 * m2.a2 + this->c2 * m2.a3,
		this->a2 * m2.b1 + this->b2 * m2.b2 + this->c2 * m2.b3,
		this->a2 * m2.c1 + this->b2 * m2.c2 + this->c2 * m2.c3,
		this->a3 * m2.a1 + this->b3 * m2.a2 + this->c3 * m2.a3,
		this->a3 * m2.b1 + this->b3 * m2.b2 + this->c3 * m2.b3,
		this->a3 * m2.c1 + this->b3 * m2.c2 + this->c3 * m2.c3,
	};

	return Matrix3(m);
}

vec3 Matrix3::operator*(const vec3& v) const {
	return vec3(this->a1 * v.x + this->a2 * v.y + this->a3 * v.z,
		this->b1 * v.x + this->b2 * v.y + this->b3 * v.z,
		this->c1 * v.x + this->c2 * v.y + this->c3 * v.z);
}

}