///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef __vector_h__
#define __vector_h__

#include <stdio.h>
#include <math.h>

#include "ucm/types.h"

namespace ugm {

using namespace ucm;

class Matrix4;

template<typename T>
struct _vec2 {
  union {
    struct {
      T x, y;
    };
    
    struct {
      T u, v;
    };
    
    struct {
      T s, t;
    };
    
    T arr[2];
  };
  
  _vec2() : x(0), y(0) { }
  _vec2(const T x, const T y) : x(x), y(y) { }
	
	static _vec2<T> zero, one;
	
  inline operator T*() {
    return this->arr;
  }
	
	inline bool operator ==(const _vec2<T>& v2) const {
		return this->x == v2.x && this->y == v2.y;
	}
	
	inline bool operator !=(const _vec2<T>& v2) const {
		return this->x != v2.x || this->y != v2.y;
	}

	inline bool equals(T x, T y) const {
		return this->x == x && this->y == y;
	}
	
	inline bool equals(const _vec2<T>& v2) const {
		return this->x == v2.x && this->y == v2.y;
	}
	
  inline _vec2<T> operator +(const T v) const {
    return _vec2<T>(this->x + v, this->y + v);
  }
  
  inline _vec2<T> operator +(const _vec2<T>& v2) const {
    return _vec2<T>(this->x + v2.x, this->y + v2.y);
  }
  
  inline _vec2<T> operator -(const _vec2<T>& v2) const {
    return _vec2<T>(this->x - v2.x, this->y - v2.y);
  }
	
	inline _vec2<T> operator *(const T s) const {
		return _vec2<T>(this->x * s, this->y * s);
	}
	
	inline _vec2<T> operator *(const _vec2<T>& s) const {
		return _vec2<T>(this->x * s.x, this->y * s.y);
	}

	inline _vec2<T> operator /(const T s) const {
		return _vec2<T>(this->x / s, this->y / s);
	}

	inline _vec2<T> operator /(const _vec2<T>& s) const {
		return _vec2<T>(this->x / s.x, this->y / s.y);
	}
	
	inline T area() const {
		return this->x * this->y;
	}
	
	inline T aspectRate() const {
		return this->x == 0 ? 1 : (this->y / this->x);
	}

	inline T length() const {
		return (T)sqrt(this->x * this->x + this->y * this->y);
	}
	
	inline _vec2<T> normalize() const {
		const T lenInv = 1.0 / this->length();
		return _vec2<T>(this->x * lenInv, this->y * lenInv);
	}
	
	_vec2<T> operator -() const {
		return _vec2<T>(-this->x, -this->y);
	}
	
  _vec2<T> operator +=(const T v) {
    this->x += v; this->y += v;
    return *this;
  }
	
	_vec2<T> operator +=(const _vec2<T>& v) {
		this->x += v.x; this->y += v.y;
		return *this;
	}
	
  _vec2<T> operator -=(const T v) {
    this->x -= v; this->y -= v;
    return *this;
  }
	
	_vec2<T> operator -=(const _vec2<T>& v) {
		this->x -= v.x; this->y -= v.y;
		return *this;
	}
	
  _vec2<T> operator *=(const T s) {
    this->x *= s; this->y *= s;
    return *this;
  }

	_vec2<T> operator *=(const _vec2<T>& s) {
		this->x *= s.x; this->y *= s.y;
		return *this;
	}
	
  _vec2<T> operator /=(const T s) {
    this->x /= s; this->y /= s;
    return *this;
  }
};

template<typename T> _vec2<T> _vec2<T>::zero(0, 0);
template<typename T> _vec2<T> _vec2<T>::one(1, 1);

typedef _vec2<float> vec2, vec2f;
typedef _vec2<byte> vec2b;
typedef _vec2<int> vec2i;

template<typename T>
_vec2<T> floor(const _vec2<T>& v, const T p = 1)
{
	const T d = pow(10, p);
	return _vec2<T>((T)(floor(v.x * d) / d), (T)(floor(v.y * d) / d));
}

template<typename T>
_vec2<T> ceiling(const _vec2<T>& v, const T p = 1)
{
	const T d = pow(10.0f, p);
	return _vec2<T>((T)(ceil(v.x * d) / d), (T)(ceil(v.y * d) / d));
}

template<typename T>
_vec2<T> round(const _vec2<T>& v, const T p = 1)
{
	const T d = pow(10, p);
	return _vec2<T>((T)(round(v.x * d) / d), (T)(round(v.y * d) / d));
}

template<typename T>
struct _vec3
{
  union
  {
    struct {
      T x, y, z;
    };
    
    T arr[3];
		_vec2<T> xy;
  };
  
  _vec3() : x(0), y(0), z(0) { }
	_vec3(const T v) : x(v), y(v), z(v) { }
	_vec3(const _vec2<T>& v, const T z) : x(v.x), y(v.y), z(z) { }
	_vec3(const T x, const T y, const T z) : x(x), y(y), z(z) { }
	
	static _vec3<T> zero, one;
	static _vec3<T> up, down, left, right, forward, back;
	
  inline operator const T*() const
  {
    return this->arr;
  }
  
  inline _vec3<T> operator+(const _vec3<T>& v2) const
  {
    return _vec3<T>(this->x + v2.x, this->y + v2.y, this->z + v2.z);
  }
  
  inline _vec3<T> operator+=(const _vec3<T>& v2)
  {
    this->x += v2.x; this->y += v2.y; this->z += v2.z;
    return *this;
  }
  
  inline _vec3<T> operator-(const _vec3<T>& v2) const
  {
    return _vec3(this->x - v2.x, this->y - v2.y, this->z - v2.z);
  }
  
  inline _vec3<T> operator-=(const _vec3<T>& v2)
  {
    this->x -= v2.x; this->y -= v2.y; this->z -= v2.z;
    return *this;
  }

	inline _vec3<T> operator -() const {
		return _vec3<T>(-this->x, -this->y, -this->z);
	}

  inline _vec3<T> operator *(const T s) const {
    return _vec3<T>(this->x * s, this->y * s, this->z * s);
  }
	
	inline _vec3<T> operator*(const _vec3<T>& v2) const {
		return _vec3<T>(this->x * v2.x, this->y * v2.y, this->z * v2.z);
	}
	
  inline _vec3<T> operator *=(const T s) {
    this->x *= s; this->y *= s; this->z *= s;
    return *this;
  }
	
//  inline _vec3<T> operator*(const _vec3<T>& v2) const {
//    return _vec3<T>(this->y * v2.z - this->z * v2.y,
//                -(this->x  * v2.z - this->z * v2.x),
//                this->x * v2.y - this->y * v2.x);
//  }
//
//  inline _vec3<T> operator*=(const _vec3<T>& v2) {
//		return *this = vec3(this->y * v2.z - this->z * v2.y,
//												-(this->x  * v2.z - this->z * v2.x),
//												this->x * v2.y - this->y * v2.x);
//  }
	
//	inline _vec3<T> operator*=(const Matrix4& m) {
//		return *this = (_vec4<T>(*this, 1.0) * m).xyz;
//	}
  
  inline _vec3<T> operator /(const T s) const {
    return _vec3<T>(this->x / s, this->y / s, this->z / s);
  }

	inline bool operator ==(const _vec3<T>& v2) const {
		return this->x == v2.x && this->y == v2.y && this->z == v2.z;
	}

	inline bool operator !=(const _vec3<T>& v2) const {
		return this->x != v2.x || this->y != v2.y || this->z != v2.z;
	}
	
	inline bool equals(const T x, const T y, const T z) const {
		return this->x == x && this->y == y && this->z == z;
	}
	
	inline bool equals(const _vec3<T>& v2) const {
		return this->x == v2.x && this->y == v2.y && this->z == v2.z;
	}

  inline T length() const {
    return (T)sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
  }
  
  inline _vec3<T> normalize() const {
    const T len = this->length();
		return _vec3<T>(this->x / len, this->y / len, this->z / len);
  }
};

template<typename T> _vec3<T> _vec3<T>::zero(0, 0, 0);
template<typename T> _vec3<T> _vec3<T>::one(1, 1, 1);

template<typename T> _vec3<T> _vec3<T>::up(0, 1, 0);
template<typename T> _vec3<T> _vec3<T>::down(0, -1, 0);
template<typename T> _vec3<T> _vec3<T>::left(-1, 0, 0);
template<typename T> _vec3<T> _vec3<T>::right(1, 0, 0);
template<typename T> _vec3<T> _vec3<T>::forward(0, 0, -1);
template<typename T> _vec3<T> _vec3<T>::back(0, 0, 1);

typedef _vec3<float> vec3, vec3f;
typedef _vec3<double> vec3d;

struct vec4
{
  union
  {
    struct {
      float x, y, z, w;
    };
    
    struct {
      float r, g, b, a;
    };
    
    float arr[4];
    vec3 xyz;
  };
  
  vec4() : x(0), y(0), z(0), w(0) { }
  vec4(float x, float y, float z) : x(x), y(y), z(z), w(1) { }
  vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
  vec4(float v) : x(v), y(v), z(v), w(v) { }
  vec4(vec2 v, float z, float w) : x(v.x), y(v.y), z(z), w(w) { }
  vec4(vec3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) { }

  inline operator const float*() const {
    return &this->x;
  }
  
  inline vec4 operator+(const vec4& v2) const {
    return vec4(this->x + v2.x, this->y + v2.y, this->z + v2.z, this->w + v2.w);
  }
  
  inline vec4 operator-(const vec4& v2) const {
    return vec4(this->x - v2.x, this->y - v2.y, this->z - v2.z, this->w - v2.w);
  }
  
  inline vec4 operator*(const float s) const {
    return vec4(this->x * s, this->y * s, this->z * s, this->w * s);
  }

	vec4 operator*(const Matrix4& m) const;
    
  inline float length() const {
    return (float)sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
  }
  
};

inline float distance(const vec3& v1, const vec3& v2) {
  float x = v2.x - v1.x;
  float y = v2.y - v1.y;
  float z = v2.z - v1.z;
  return (float)sqrt(x*x + y*y + z*z);
}

inline float distance(const vec4& v1, const vec4& v2) {
  float x = v2.x - v1.x;
  float y = v2.y - v1.y;
  float z = v2.z - v1.z;
  float w = v2.w - v1.w;
  return (float)sqrt(x*x + y*y + z*z + w*w);
}

template<typename T>
inline T length(const _vec2<T>& v) {
	return v.length();
}

template<typename T>
inline T length(const _vec3<T>& v) {
  return v.length();
}

inline float length(const vec4& v) {
	return v.length();
}

inline vec2 abs(const vec2& v) {
	return vec2(fabsf(v.x), fabsf(v.y));
}

inline vec3 abs(const vec3& v) {
	return vec3(fabsf(v.x), fabsf(v.y), fabsf(v.z));
}

template<typename T>
inline _vec3<T> fabsf(const _vec3<T>& v) {
  return _vec3<T>(::fabs(v.x), ::fabs(v.y), ::fabs(v.z));
}

template<typename T>
inline _vec2<T> normalize(const _vec2<T>& v)
{
	const T len = length(v);
	
	if (len == 0) {
		return _vec2<T>();
	}
	
	const float invl = 1.0f / len;
	return _vec2<T>(v.x * invl, v.y * invl);
}

template<typename T>
inline _vec3<T> normalize(const _vec3<T>& v)
{
  const T len = length(v);
	
	if (len == 0) {
		return _vec3<T>();
	}
	
	const float invl = 1.0f / len;
  return _vec3<T>(v.x * invl, v.y * invl, v.z * invl);
}

inline vec4 normalize(const vec4& v)
{
  const float len = length(v);
	
	if (len == 0) {
		return vec4();
	}
	
	const float invl = 1.0f / len;
  return vec4(v.x * invl, v.y * invl, v.z * invl, v.w * invl);
}

template<typename T>
inline T dot(const _vec3<T>& v1, const _vec3<T>& v2)
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float dot(const vec4& v1, const vec4& v2)
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

template<typename T>
inline T cross(const _vec2<T>& v1, const _vec2<T>& v2) {
	return v1.x * v2.y + (-v1.y * v2.x);
}

template<typename T>
inline _vec3<T> cross(const _vec3<T>& v1, const _vec3<T>& v2) {
  return _vec3<T>(v1.y * v2.z - v1.z * v2.y,
								-(v1.x  * v2.z - v1.z * v2.x),
								v1.x * v2.y - v1.y * v2.x);
}

template<typename T>
inline T dot(const _vec2<T>& v1, const _vec2<T>& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

//template<typename T>
//inline T _vec2<T>(const _vec2<T>& v1, const _vec2<T>& v2) {
//	return vec2(fminf(v1.x, v2.x), fminf(v1.y, v2.y));
//}

//inline vec3 vmin(const vec3& v1, const float minValue) {
//  return vec3((float)fmin(v1.x, minValue), (float)fmin(v1.y, minValue), (float)fmin(v1.z, minValue));
//}
//
template<typename T>
inline _vec3<T> max(const _vec3<T>& v, const T maxValue) {
  return _vec3<T>((T)fmax(v.x, maxValue), (T)fmax(v.y, maxValue), (T)fmax(v.z, maxValue));
}

template<typename T>
inline _vec3<T> pow(const _vec3<T>& v, const T p) {
	return _vec3<T>((T)powf(v.x, p), (T)powf(v.y, p), (T)powf(v.z, p));
}

}

#endif /* __vector_h__ */
