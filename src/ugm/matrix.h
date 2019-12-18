///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef matrix_h
#define matrix_h

#include <stdio.h>
#include <math.h>
#include <memory.h>

#include "vector.h"

namespace ugm {

class Matrix4
{
public:
  union {
    struct {
      float a1, b1, c1, d1;
      float a2, b2, c2, d2;
      float a3, b3, c3, d3;
      float a4, b4, c4, d4;
    };
    
    float arr[16];
  };
  
  Matrix4() {
    loadIdentity();
  }
  
  Matrix4(const Matrix4& m) {
    this->copyFrom(m);
  }

  Matrix4(const float m[]) {
    this->copyFrom(m);
  }
	
	Matrix4(const float m11, const float m12, const float m13, const float m14,
					const float m21, const float m22, const float m23, const float m24,
					const float m31, const float m32, const float m33, const float m34,
					const float m41, const float m42, const float m43, const float m44) {
		const float arr[16] = { m11, m12, m13, m14, m21, m22, m23, m24,
			m31, m32, m33, m34, m41, m42, m43, m44 };
		this->copyFrom(arr);
	}
  
  inline void copyFrom(const Matrix4& m) {
    this->copyFrom(m.arr);
  }
  
  inline void copyFrom(const float m[]) {
    memcpy(arr, m, sizeof(float) * 16);
  }
  
  static const float identity[];
  
  inline void loadIdentity() {
    memcpy(this->arr, Matrix4::identity, sizeof(float) * 16);
  }
	
	inline bool isIdentity() const {
		return memcmp(this->arr, Matrix4::identity, sizeof(float) * 16) == 0;
	}
  
  void rotateX(const float angle);
  void rotateY(const float angle);
  void rotateZ(const float angle);
	Matrix4& rotate(const vec3& angles);
  Matrix4& rotate(const float x, const float y, const float z);
  
  inline void scale(const float all) {
    this->scale(all, all, all);
  }
	void scale(const vec3& s);
  void scale(const float x, const float y, const float z);

	inline Matrix4& translate(const vec3& t) { return this->translate(t.x, t.y, t.z); }
  Matrix4& translate(const float x, const float y, const float z);
	
  float determinant() const;
  bool canInverse() const;
  void inverse();
  void transpose();
  
	void ortho(const float left, const float right, const float top, const float bottom,
						 const float near, const float far);
  void frustum(const float left, const float right, const float top, const float bottom,
               const float near, const float far);
  void perspective(const float angle, const float widthRate, const float near, const float far);

	Matrix4& lookAt(const vec3& eye, const vec3& target, const vec3& up);
   
  inline operator float*() { return this->arr; }
	inline operator const float*() const { return this->arr; }
	
	bool operator ==(const Matrix4& mat) const;
	bool operator !=(const Matrix4& mat) const;

	Matrix4 operator*(const Matrix4& m2) const;
  vec4 operator*(const vec4& v) const;
	
	vec3 extractEulerAngles() const;
	void extractLookAtVectors(vec3* dir, vec3* up) const;
	
	static Matrix4 createTranslate(const float x, const float y, const float z) {
		Matrix4 mat(0, 0, 0, 0,
								0, 0, 0, 0,
								0, 0, 0, 0,
								x, y, z, 1);
		return mat;
	}
	
	static Matrix4 createRotate(const float x, const float y, const float z) {
		Matrix4 mat;
		mat.rotate(x, y, z);
		return mat;
	}

	static Matrix4 createRotate(const vec3& v) {
		Matrix4 mat;
		mat.rotate(v);
		return mat;
	}

	static Matrix4 createScale(const float x, const float y, const float z) {
		Matrix4 mat(x, 0, 0, 0,
								0, y, 0, 0,
								0, 0, z, 0,
								0, 0, 0, 1);
		return mat;
	}
};

class Matrix3
{
public:
  
  union
  {
    struct {
      float a1, b1, c1;
      float a2, b2, c2;
      float a3, b3, c3;
    };
    
    float arr[9];
  };
  
  static const float identity[];
	
  inline void loadIdentity()
  {
    memcpy(this->arr, identity, sizeof(float) * 9);
  }
  
  Matrix3()
  {
    this->loadIdentity();
  }

	Matrix3(const float arr[9]) {
		memcpy(this->arr, arr, sizeof(float) * 9);
	}

	void rotate(const float x, const float y);
	void rotateX(const float angle);
	void rotateY(const float angle);
	void rotateZ(const float angle);

	void scale(const float x, const float y);
	void translate(const float x, const float y);
	void transpose();
	
	Matrix3 operator*(const Matrix3& m2) const;
	vec3 operator*(const vec3& v) const;
};

//vec3 gaussEliminate(const Matrix3& m, const vec3& v);

}

#endif /* matrix_h */
