///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "types3d.h"
#include "functions.h"

namespace ugm {

void BoundingBox::initTo(const vec3& v) {
	this->min = v;
	this->max = v;
}

void BoundingBox::expandTo(const vec3& v) {
	if (minX > v.x) minX = v.x;
	if (minY > v.y) minY = v.y;
	if (minZ > v.z) minZ = v.z;
	
	if (maxX < v.x) maxX = v.x;
	if (maxY < v.y) maxY = v.y;
	if (maxZ < v.z) maxZ = v.z;
}

void BoundingBox::expandTo(const BoundingBox& bbox) {
	this->expandTo(bbox.min);
	this->expandTo(bbox.max);
}

void BoundingBox::expandTo(const Triangle& t) {
	this->expandTo(t.v1, t.v2, t.v3);
}

void BoundingBox::expandTo(const vec3& v1, const vec3& v2, const vec3& v3) {
	this->expandTo(v1);
	this->expandTo(v2);
	this->expandTo(v3);
}

void BoundingBox::finalize() {
	this->size = this->max - this->min;
	this->origin = this->min + this->size * 0.5f;
}

bool BoundingBox::contains(const vec3& p) const {
	return p.x >= this->min.x && p.y >= this->min.y && p.z >= this->min.z
				 && p.x <= this->max.x && p.y <= this->max.y && p.z <= this->max.z;
}

bool BoundingBox::contains(const Triangle& t) const {
	return this->contains(t.v1) && this->contains(t.v2) && this->contains(t.v3);
}

bool BoundingBox::contains(const BoundingBox& bbox) const {
	return this->contains(bbox.min) && this->contains(bbox.max);
}

BoundingBox BoundingBox::fromTriangle(const Triangle& t) {
	return BoundingBox::fromTriangle(t.v1, t.v2, t.v3);
}

BoundingBox BoundingBox::fromTriangle(const vec3& v1, const vec3& v2, const vec3& v3) {
	BoundingBox bbox;

	bbox.initTo(v1);
	bbox.expandTo(v2);
	bbox.expandTo(v3);
	bbox.finalize();

	return bbox;
}

BoundingBox BoundingBox::fromTriangles(const Triangle* t, const uint triangleCount) {
	if (triangleCount <= 0) {
		return BoundingBox();
	}
	else if (triangleCount <= 1) {
		return BoundingBox::fromTriangle(t[0]);
	}

	BoundingBox bbox = BoundingBox::fromTriangle(t[0]);

	for (size_t i = 1; i < triangleCount; i++) {
		bbox.expandTo(t[i]);
	}

	bbox.finalize();

	return bbox;
}

BoundingBox BoundingBox::fromBoundingBoxes(const BoundingBox* boxes, const uint boxCount) {
	if (boxCount <= 0) {
		return BoundingBox();
	}
	else if (boxCount <= 1) {
		return boxes[0];
	}

	BoundingBox bbox = boxes[0];

	for (size_t i = 1; i < boxCount; i++) {
		bbox.expandTo(boxes[i]);
	}

	bbox.finalize();

	return bbox;
}

bool BoundingBox::intersects(const Ray& ray) const {
	return rayIntersectBox(ray, *this);
}

bool BoundingBox::intersects(const BoundingBox& bbox) const {
	return BoundingBox::intersects(*this, bbox);
}

bool BoundingBox::intersects(const BoundingBox& b1, const BoundingBox& b2) {
	return b1.max.x > b2.min.x && b1.min.x < b2.max.x &&
		b1.max.y > b2.min.y && b1.min.y < b2.max.y &&
		b1.max.z > b2.min.z && b1.min.z < b2.max.z;
}

BoundingBox BoundingBox::operator+ (const vec3& offset) const {
	return BoundingBox(this->min + offset, this->max + offset);
}

BoundingBox BoundingBox::operator+=(const vec3& offset) {
	this->min += offset;
	this->max += offset;
	this->finalize();
	return *this;
}

BoundingBox BoundingBox::operator- (const vec3& offset) const {
	return BoundingBox(this->min - offset, this->max - offset);
}

BoundingBox BoundingBox::operator-=(const vec3& offset) {
	this->min -= offset;
	this->max -= offset;
	this->finalize();
	return *this;
}

BoundingBox BoundingBox::operator* (const vec3& scale) const {
	return BoundingBox(this->min * scale, this->max * scale);
}

BoundingBox BoundingBox::operator*(const Matrix4 &mat) const {
	const BoundingBox& bbox = *this;
	
	vec3 v1 = (vec4(bbox.max.x, bbox.max.y, bbox.max.z, 1) * mat).xyz;
	vec3 v2 = (vec4(bbox.max.x, bbox.max.y, bbox.min.z, 1) * mat).xyz;
	vec3 v3 = (vec4(bbox.max.x, bbox.min.y, bbox.max.z, 1) * mat).xyz;
	vec3 v4 = (vec4(bbox.max.x, bbox.min.y, bbox.min.z, 1) * mat).xyz;
	vec3 v5 = (vec4(bbox.min.x, bbox.max.y, bbox.max.z, 1) * mat).xyz;
	vec3 v6 = (vec4(bbox.min.x, bbox.max.y, bbox.min.z, 1) * mat).xyz;
	vec3 v7 = (vec4(bbox.min.x, bbox.min.y, bbox.max.z, 1) * mat).xyz;
	vec3 v8 = (vec4(bbox.min.x, bbox.min.y, bbox.min.z, 1) * mat).xyz;
	
	BoundingBox tb;
	tb.initTo(v1);
	tb.expandTo(v2); tb.expandTo(v3); tb.expandTo(v4); tb.expandTo(v5);
	tb.expandTo(v6); tb.expandTo(v7); tb.expandTo(v8);
	tb.finalize();
	
	return tb;
}

BoundingBox BoundingBox::operator*= (const Matrix4& mat) {
	BoundingBox& bbox = *this;
	bbox = bbox * mat;
	return bbox;
}

}