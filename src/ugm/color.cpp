///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "color.h"

namespace ugm {

color3b tocolor3b(const color3f& c) {
	const color3f cc = clamp(c);
	return color3b((byte)(cc.r * 255.0f), (byte)(cc.g * 255.0f), (byte)(cc.b * 255.0f));
}

color3f tocolor3f(const color3b& c) {
	return color3f(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f);
}

color4b tocolor4b(const color4f& c) {
	const color4f cc = clamp(c);
	return color4b((byte)(cc.r * 255.0f), (byte)(cc.g * 255.0f), (byte)(cc.b * 255.0f), (byte)(cc.a * 255.0f));
}

color4f tocolor4f(const color4b& c) {
	return color4f(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f);
}

color3f tocolor3f(const int value) {
	return color3f(
		(float)((value >> 16) & 0xff) / 255.0f,
		(float)((value >> 8) & 0xff) / 255.0f,
		(float)((value) & 0xff) / 255.0f);
}

color3b tocolor3b(const int value) {
	return color3b(
		((value >> 16) & 0xff),
		((value >> 8) & 0xff),
		((value) & 0xff));
}

color4f tocolor4f(const int value) {
	return color4f((float)((value >> 24) & 0xff) / 255.0f,
		(float)((value >> 16) & 0xff) / 255.0f,
		(float)((value >> 8) & 0xff) / 255.0f,
		(float)((value) & 0xff) / 255.0f);
}

color4b tocolor4b(const int value) {
	return color4b(((value >> 24) & 0xff),
		((value >> 16) & 0xff),
		((value >> 8) & 0xff),
		((value) & 0xff));
}

}