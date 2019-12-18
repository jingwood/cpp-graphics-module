///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef functions_h
#define functions_h

#include <math.h>

#include "types2d.h"
#include "types3d.h"

#if defined(_WIN32)
#define M_PI       3.14159265f
#define M_PI_2     1.57079632f
#define M_PI_4     0.78539816f
#define M_E        2.71828183f
#endif /* _WIN32 */

#define ANGLE_TO_DEGREE(a) (a * M_PI / 180.0f)
#define DEGREE_TO_ANGLE(a) (a * 180.0f / M_PI)

#define EPSILON_F 0.00001f

namespace ugm {

inline bool extremelyClose(const vec3& v1, const vec3& v2) {
	return ::fabsf(v2.x - v1.x) < EPSILON_F && ::fabsf(v2.y - v1.y) < EPSILON_F && ::fabsf(v2.z - v1.z) < EPSILON_F;
}

inline bool extremelyClose(const vec2& v1, const vec2& v2) {
	return ::fabsf(v2.x - v1.x) < EPSILON_F && ::fabsf(v2.y - v1.y) < EPSILON_F;
}

vec3 eulerAnglesFromVectors(const vec3& v1, const vec3& v2);

vec3 calcTriangleInterpolation(const Triangle& t1, const vec3& v, const Triangle& t);

bool rayIntersectTriangle(const Ray& ray, const Triangle& tri, 
													const float maxt, float& dist, vec3& hit);

bool rayIntersectTriangle2(const Ray& ray, const Triangle& tri, const float maxt, float& t, vec3& hit);

bool pointInTriangle3D(const vec3& hitPoint, const vec3& n, const Triangle& t);

bool pointInTriangle2D(const vec2& p, const Triangle2D& t);
bool pointInTriangle2D(const vec2& p, const vec2& v1, const vec2& v2, const vec2& v3);

float pointLineDistance(const vec2& p, const vec2& lstart, const vec2& lend);

bool lineLineIntersection(const vec2& p1, const vec2& p2, const vec2& p3, const vec2& p4, vec2* ip);

vec3 randomRayInHemisphere(const vec3& n);

vec3 randomPointInTriangle(const Triangle& t);

inline bool pointInBox(const vec3& p, const vec3& min, const vec3& max) {
		return (p.x >= min.x && p.x <= max.x) 
			&& (p.y >= min.y && p.y <= max.y) 
			&& (p.z >= min.z && p.z <= max.z);
}

bool rayIntersectBox(const Ray& ray, const BoundingBox& bbox, float* t = NULL);

bool rayIntersectBox(const Ray& ray, const vec3& lb, const vec3& rt, float* t = NULL);

bool triangleIntersectBox(const vec3& boxcenter, const vec3& boxhalfsize, const Triangle& t);

//int triBoxOverlap(float boxcenter[3], float boxhalfsize[3], float triverts[3][3]);

float gaussianDistribution(const float x, const float sigma = 0.2f);

void gaussianDistributionGenKernel(float* kernel, const int kernelSize, const float sigma = 0.2f);

float randomValue();

}

#endif /* functions_h */

