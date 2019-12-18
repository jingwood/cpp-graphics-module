///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef __TYPES2D_H__
#define __TYPES2D_H__

#include <stdio.h>
#include "vector.h"

namespace ugm {

template<typename T>
using _point = _vec2<T>;

typedef _point<float> pointf;
typedef _point<int> pointi;

template<typename T>
struct _size
{
  T width, height;
  
  _size() : width(0), height(0) { }
  
  _size(const T width, const T height)
  : width(width), height(height) {
  }
	
	inline operator _size<float>() {
		return _size<float>((float)this->width, (float)this->height);
	}
	
	inline bool operator==(const _size<T>& size) const {
		return this->width == size.width && this->height == size.height;
	}
};

typedef _size<float> sizef;
typedef _size<int> sizei;
typedef _size<uint> sizeui;

template<typename T>
struct _rect
{
  union {
    struct {
      T x, y;
    };
    _point<T> location;
  };
  
  union {
    struct {
      T width, height;
    };
    _point<T> size;
  };
  
  _rect() : x(0), y(0), width(0), height(0) { }
  
  _rect(const T x, const T y, const T width, const T height)
  : x(x), y(y), width(width), height(height)
  { }
	
	_rect(const _point<T> p, const _size<T> s)
	: x(p.x), y(p.y), width(s.width), height(s.height)
	{ }
	
	inline T right() { return this->x + this->width; };
	inline T bottom() { return this->y + this->height; };
	
	inline bool contains(const vec2& p) {
		return p.x >= this->x && p.x <= this->right()
			&& p.y >= this->y && p.y <= this->bottom();
	}
};

typedef _rect<float> rectf;
typedef _rect<int> recti;

struct BBox2D {
	vec2 min, max;
	
	BBox2D();
	BBox2D(const vec2& min, const vec2& max);
	
	void initAt(const vec2& v);
	void initAt(const vec2& v1, const vec2& v2);
	
	void expandTo(const vec2& v);
	void expandTo(const vec2& v1, const vec2& v2);
	void expandTo(const BBox2D& box);
	
	float getWidth() const;
	void setWidth(const float value);
	
	float getHeight() const;
	void setHeight(const float value);
	
	vec2 getSize() const;
	vec2 getOrigin() const;
	
	void offset(const vec2& offset);
	void offset(const float offsetX, const float offsetY);
	
	void inflate(const float v);
	void inflate(const vec2& v);
	
	bool contains(const vec2& p) const;
	bool contains(const BBox2D& box) const;
	
	bool intersects(const BBox2D& b) const;
	static bool intersects(const BBox2D& a, const BBox2D& b);
	static bool intersects(const vec2& amin, const vec2& amax, const vec2& bmin, const vec2& bmax);

	static BBox2D fromTwoPoints(const vec2& v1, const vec2& v2);
	
//	inline static BBox2D fromTriangle(const Triangle2D& t) {
//		return BBox2D::fromTriangle(t.v1, t.v2, t.v3);
//	}
	
	static BBox2D fromTriangle(const vec2& v1, const vec2& v2, const vec2& v3);
	
	BBox2D operator*=(float s);
	BBox2D operator*(const vec2& v) const;
	BBox2D operator*=(const vec2& v);

};

struct Triangle2D {
	vec2 v1, v2, v3;
	BBox2D bbox;
	
	inline bool operator ==(const Triangle2D& t2) const {
		return this->v1 == t2.v1 && this->v2 == t2.v2 && this->v3 == t2.v3;
	}

	inline Triangle2D operator*(const vec2& v) const {
		Triangle2D t;
		t.v1 = this->v1 * v;
		t.v2 = this->v2 * v;
		t.v3 = this->v3 * v;
		return *this;
	}

	inline Triangle2D operator*=(const vec2& v) {
		this->v1 *= v;
		this->v2 *= v;
		this->v3 *= v;
		return *this;
	}
	
	inline Triangle2D operator+(const vec2& v) {
		this->v1 += v;
		this->v2 += v;
		this->v3 += v;
		return *this;
	}
	
	inline Triangle2D operator-(const vec2& v) {
		this->v1 -= v;
		this->v2 -= v;
		this->v3 -= v;
		return *this;
	}
};

}

#endif /* __TYPES2D_H__ */
