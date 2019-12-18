///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef basefun_h
#define basefun_h

#include "ucm/types.h"

namespace ugm {

template<typename T>
inline T clamp(T v, const T& min, const T& max) {
	if (v < min)
		return min;
	else if (v > max)
		return max;
	else
		return v;
}

template<typename T>
inline T smoothstep(T edge0, T edge1, T x) {
	T t = clamp<T>((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	return (T)(t * t * (3.0 - 2.0 * t));
}

inline int absi(int m) {
	return m > 0 ? m : -m;
}

inline int modulo(const int m, const int n) {
	if (n == 0) return m;
	return m >= 0 ? m % n : (n - absi(m % n)) % n;
}

}

#endif /* basefun_h */

