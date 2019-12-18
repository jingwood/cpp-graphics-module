///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "types2d.h"

namespace ugm {

BBox2D::BBox2D() {
	
}
	
BBox2D::BBox2D(const vec2& min, const vec2& max)
: min(min), max(max) {
}
	
void BBox2D::initAt(const vec2& v) {
	this->min = v;
	this->max = v;
}
	
void BBox2D::initAt(const vec2& v1, const vec2& v2) {
	this->initAt(v1);
	this->expandTo(v2);
}
	
 void BBox2D::expandTo(const vec2& v) {
	if (this->min.x > v.x) this->min.x = v.x;
	if (this->min.y > v.y) this->min.y = v.y;
	if (this->max.x < v.x) this->max.x = v.x;
	if (this->max.y < v.y) this->max.y = v.y;
}
	
void BBox2D::expandTo(const vec2& v1, const vec2& v2) {
	this->expandTo(v1);
	this->expandTo(v2);
}
	
 void BBox2D::expandTo(const BBox2D& box) {
	this->expandTo(box.min);
	this->expandTo(box.max);
}
	
float BBox2D::getWidth() const {
	return max.x - min.x;
}
	
void BBox2D::setWidth(const float value) {
	max.x = min.x + value;
}
	
float BBox2D::getHeight() const {
	return max.y - min.y;
}
	
void BBox2D::setHeight(const float value) {
	max.y = min.y + value;
}
	
vec2 BBox2D::getSize() const {
	return vec2(getWidth(), getHeight());
}
	
vec2 BBox2D::getOrigin() const {
	return this->min + this->getSize() * 0.5;
}
	
void BBox2D::offset(const vec2& offset) {
	this->min += offset;
	this->max += offset;
}
	
void BBox2D::offset(const float offsetX, const float offsetY) {
	this->offset(vec2(offsetX, offsetY));
}
	
void BBox2D::inflate(const float v) {
	this->inflate(vec2(v, v));
}
	
void BBox2D::inflate(const vec2& v) {
	this->min -= v;
	this->max += v;
}
	
 bool BBox2D::contains(const vec2& p) const {
	return (p.x > min.x && p.x < max.x && p.y > min.y && p.y < max.y);
}
	
bool BBox2D::contains(const BBox2D& box) const {
	return this->min.x <= box.min.x && this->min.y <= box.min.y
	&& this->max.x >= box.max.x && this->max.y >= box.max.y;
}
	
bool BBox2D::intersects(const BBox2D& b) const {
	return intersects(*this, b);
}
	
bool BBox2D::intersects(const BBox2D& a, const BBox2D& b) {
	return intersects(a.min, a.max, b.min, b.max);
}
	
bool BBox2D::intersects(const vec2& amin, const vec2& amax, const vec2& bmin, const vec2& bmax) {
	if (amax.x < bmin.x) return false;
	if (amin.x > bmax.x) return false;
	if (amax.y < bmin.y) return false;
	if (amin.y > bmax.y) return false;
	
	return true;
}
	
BBox2D BBox2D::fromTwoPoints(const vec2& v1, const vec2& v2) {
	vec2 min, max;
	
	min.x = fminf(v1.x, v2.x);
	min.y = fminf(v1.y, v2.y);
	max.x = fmaxf(v1.x, v2.x);
	max.y = fmaxf(v1.y, v2.y);
	
	return BBox2D(min, max);
}
	
//	inline static BBox2D fromTriangle(const Triangle2D& t) {
//		return BBox2D::fromTriangle(t.v1, t.v2, t.v3);
//	}
	
BBox2D BBox2D::fromTriangle(const vec2& v1, const vec2& v2, const vec2& v3) {
	BBox2D box = BBox2D::fromTwoPoints(v1, v2);
	box.expandTo(v3);
	return box;
}
	
BBox2D BBox2D::operator*=(float s) {
	this->min *= s;
	this->max *= s;
	return *this;
}
	
BBox2D BBox2D::operator*(const vec2& v) const {
	return BBox2D(this->min * v, this->max * v);
}
	
BBox2D BBox2D::operator*=(const vec2& v) {
	this->min *= v;
	this->max *= v;
	return *this;
}

}