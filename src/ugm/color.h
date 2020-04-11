///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef __COLOR_H__
#define __COLOR_H__

#include "basefun.h"

namespace ugm {

using namespace ucm;

template<typename T>
struct _color4;

template<typename T>
struct _color3 {
  union {
    struct {
      T r, g, b;
    };
    T arr[3];
  };
	
	static _color3<T> zero;
	static _color3<T> one;
	
  _color3() : r(0), g(0), b(0) { }
	_color3(const T v) : r(v), g(v), b(v) { }
  _color3(const T r, const T g, const T b) : r(r), g(g), b(b) { }
	
  inline operator _color4<T>() const {
    return _color4<T>(this->r, this->g, this->b, 1.0);
  }
  
  inline operator T*() {
    return this->arr;
  }

	inline operator const T*() const {
		return this->arr;
	}

	inline _color3<T> operator+(const T& c) const {
		return _color3<T>(this->r + c, this->g + c, this->b + c);
	}

  inline _color3<T> operator+(const _color3<T>& c) const {
    return _color3<T>(this->r + c.r, this->g + c.g, this->b + c.b);
  }

	inline _color3<T> operator+=(const _color3<T>& c) {
		return _color3<T>(this->r += c.r, this->g += c.g, this->b += c.b);
	}
  
  inline _color3<T> operator-(const float c) const {
    return _color3<T>(this->r - c, this->g - c, this->b - c);
  }
  
  inline _color3<T> operator-(const _color3<T>& c) const {
    return _color3<T>(this->r - c.r, this->g - c.g, this->b - c.b);
  }

  inline _color3<T> operator-=(const float c) const {
    return _color3<T>(this->r -= c, this->g -= c, this->b -= c);
  }

	template<typename T2>
	inline _color3<T> operator*(const T2 s) const {
		return _color3((T2)this->r * s, (T2)this->g * s, (T2)this->b * s);
	}

	inline _color3<T> operator*(const _color3<T>& c) const {
		return _color3(this->r * c.r, this->g * c.g, this->b * c.b);
	}

	template<typename T2>
	inline _color3<T> operator*=(const T2 c) {
		this->r *= c; this->g *= c, this->b *= c;
		return *this;
	}
	
	inline _color3<T> operator*=(const _color3<T>& c) {
		return _color3<T>(this->r *= c.r, this->g *= c.g, this->b *= c.b);
	}

	inline _color3<T> operator/(const float s) {
		return _color3<T>(this->r / s, this->g / s, this->b / s);
	}
  
  inline _color3<T> operator/=(const float s) {
    return _color3<T>(this->r /= s, this->g /= s, this->b /= s);
  }
  
	inline bool operator==(_color3<T>& c) const {
		return this->r == c.r && this->g == c.g && this->b == c.b;
	}

	inline bool operator!=(_color3<T>& c) const {
		return this->r != c.r || this->g != c.g || this->b != c.b;
	}

	inline bool operator!=(const _color3<T>& c) const {
		return this->r != c.r || this->g != c.g || this->b != c.b;
	}

};

template<typename T> _color3<T> _color3<T>::zero;
template<typename T> _color3<T> _color3<T>::one(1, 1, 1);

typedef _color3<float> color3, color3f;
typedef _color3<byte> color3b;

template<typename T>
struct _color4 {
  union {
    struct {
      T r, g, b, a;
    };
    _color3<T> rgb;
    T arr[4];
  };
	
	static _color4<T> one;
  static _color4<T> zero;
	
  _color4()
  : r(0.0), g(0.0), b(0.0), a(1.0)
  { }
	
	_color4(const T c, const T a = 1.0)
	: r(c), g(c), b(c), a(a)
	{ }
  
  _color4(const _color3<T>& c, const T a)
  : r(c.r), g(c.g), b(c.b), a(a)
  { }

  _color4(const T r, const T g, const T b, const T a = 1.0)
  : r(r), g(g), b(b), a(a)
  { }
  
  inline operator T*() {
    return this->arr;
  }
  
  inline operator const T*() const {
    return this->arr;
  }
  
  inline operator color3() const {
    return this->rgb;
  }
  
  //inline _color4<T> operator+(_color4<T>& c) {
  //  return _color4<T>(this->r + c.r, this->g + c.g, this->b + c.b, this->a + c.a);
  //}

	inline _color4<T> operator+(const _color4<T>& c) {
		return _color4<T>(this->r + c.r, this->g + c.g, this->b + c.b, this->a + c.a);
	}

	inline _color4<T> operator-(const _color4<T>& c) {
		return _color4<T>(this->r - c.r, this->g - c.g, this->b - c.b, this->a - c.a);
	}

	inline const _color4<T> operator+(const _color4<T>& c) const {
		return _color4<T>(this->r + c.r, this->g + c.g, this->b + c.b, this->a + c.a);
	}

	inline _color4<T> operator+=(const _color4<T>& c) {
		return _color4<T>(this->r += c.r, this->g += c.g, this->b += c.b, this->a += c.a);
	}

	inline _color4<T> operator*(const float s) {
		return _color4<T>(this->r * s, this->g * s, this->b * s, this->a * s);
	}

	inline const _color4<T> operator*(const float s) const {
		return _color4<T>(this->r * s, this->g * s, this->b * s, this->a * s);
	}

	inline _color4<T> operator/(const float s) {
		return _color4<T>(this->r / s, this->g / s, this->b / s, this->a / s);
	}

	inline const _color4<T> operator/(const float s) const {
		return _color4<T>(this->r / s, this->g / s, this->b / s, this->a / s);
	}
	
	inline _color4<T> operator/=(const float s) {
		return _color4<T>(this->r /= s, this->g /= s, this->b /= s, this->a /= s);
	}

	inline _color4<T> operator*(const _color4<T>& c) const {
		return _color4(this->r * c.r, this->g * c.g, this->b * c.b, this->a * c.a);
	}

	inline const _color4<T> operator*=(const float s) {
		this->r *= s; this->g *= s; this->b *= s; this->a *= s;
		return *this;
	}
	
	inline bool operator==(_color4<T>& c) {
		return this->r == c.r && this->g == c.g && this->b == c.b && this->a == c.a;
	}

	inline bool operator!=(_color4<T>& c) {
		return this->r != c.r || this->g != c.g || this->b != c.b || this->a != c.a;
	}

	inline const bool operator!=(const _color4<T>& c) const {
		return this->r != c.r || this->g != c.g || this->b != c.b || this->a != c.a;
	}
};

typedef _color4<float> color4, color4f;
typedef _color4<byte> color4b;

template<typename T> _color4<T> _color4<T>::zero;
template<typename T> _color4<T> _color4<T>::one(1.0, 1.0, 1.0);

template<typename T>
inline _color3<T> normalize(const _color3<T>& c) {
  T len = c.r * c.r + c.g * c.g + c.b * c.b;
  return _color3<T>(c.r / len, c.g / len, c.b / len);
}

template<typename T>
inline _color3<T> clamp(const _color3<T>& c, const T min = 0.0, const T max = 1.0) {
  return _color3<T>(clamp(c.r, min, max),
                    clamp(c.g, min, max),
                    clamp(c.b, min, max));
}

template<typename T>
inline _color4<T> clamp(const _color4<T>& c, const T min = 0.0, const T max = 1.0) {
  return _color4<T>(clamp(c.r, min, max),
                 clamp(c.g, min, max),
                 clamp(c.b, min, max),
                 clamp(c.a, min, max));
}

inline color4b clamp(const color4b& c, const byte min = 0, const byte max = 255) {
  return clamp<byte>(c, min, max);
}

template<typename T>
inline _color3<T> max(const _color3<T>& c, const T maxValue) {
	return _color3<T>((T)fmax(c.r, maxValue), (T)fmax(c.g, maxValue), (T)fmax(c.b, maxValue));
}

template<typename T>
inline _color3<T> min(const _color3<T>& c, const T minValue) {
	return _color3<T>((T)fmin(c.r, minValue), (T)fmin(c.g, minValue), (T)fmin(c.b, minValue));
}

template<typename T>
inline _color3<T> abs(const _color3<T>& c) {
	return _color3<T>((T)fabs(c.r), (T)fabs(c.g), (T)fabs(c.b));
}

template<typename T>
inline _color3<T> pow(const _color3<T>& c, const T t) {
	return _color3<T>((T)powf(c.r, t), (T)powf(c.g, t), (T)powf(c.b, t));
}

template<typename T>
inline _color4<T> pow(const _color4<T>& c, const T t) {
	return _color4<T>((T)powf(c.r, t), (T)powf(c.g, t), (T)powf(c.b, t), c.a);
}

color3b tocolor3b(const color3f& c);
color3f tocolor3f(const color3b& c);
color4b tocolor4b(const color4f& c);
color4f tocolor4f(const color4b& c);

color3 tocolor3f(const int value);
color3b tocolor3b(const int value);
color4 tocolor4f(const int value);
color4b tocolor4b(const int value);

namespace colors
{
	const color4f transparent   = color4f(0.0f, 0.0f, 0.0f, 0.0f);

	const color3f white         = color3f(1.0f, 1.0f, 1.0f);
	const color3f whitesmoke		= color3f(0.9f, 0.9f, 0.9f);
	const color3f silver				= color3f(0.7f, 0.7f, 0.7f);
	const color3f gray					= color3f(0.5f, 0.5f, 0.5f);
	const color3f dimgray       = color3f(0.35f, 0.35f, 0.35f);
	const color3f darkgray			= color3f(0.2f, 0.2f, 0.2f);
	const color3f black         = color3f(0.0f, 0.0f, 0.0f);

	const color3f red           = color3f(1.0f, 0.0f, 0.0f);
	const color3f green         = color3f(0.0f, 1.0f, 0.0f);
	const color3f blue					= color3f(0.0f, 0.0f, 1.0f);
	const color3f yellow				= color3f(1.0f, 1.0f, 0.0f);
	const color3f cyan					= color3f(0.0f, 1.0f, 1.0f);
	const color3f pink					= color3f(1.0f, 0.0f, 1.0f);
}

}

#endif // __COLOR_H__
