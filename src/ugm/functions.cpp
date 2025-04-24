///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "functions.h"
//#include <random>
#include <stdio.h>

namespace ugm {

vec3 eulerAnglesFromVectors(const vec3& v1, const vec3& v2) {
  const vec3 l = v1 - v2;
    
  const float padj = (float)sqrt(::pow(l.x, 2.f) + ::pow(l.z, 2.f));
  
  return vec3(DEGREE_TO_RADIAN(atan2f(l.x, l.z)),
              90.f - DEGREE_TO_RADIAN(atan2f(padj, l.y)),
              0.0f);
}

vec3 calcTriangleInterpolation(const Triangle& t1, const vec3& v, const Triangle& t) {
	// https://stackoverflow.com/questions/19654251/random-point-inside-triangle-inside-java
	
	const vec3 f1 = t1.v1 - v;
	const vec3 f2 = t1.v2 - v;
	const vec3 f3 = t1.v3 - v;
	
	const float a = 1.0f / cross(t1.v1 - t1.v2, t1.v1 - t1.v3).length();
	const float a1 = cross(f2, f3).length() * a;
	const float a2 = cross(f3, f1).length() * a;
	const float a3 = cross(f1, f2).length() * a;
	
	return vec3(t.v1 * a1 + t.v2 * a2 + t.v3 * a3);
}

bool rayIntersectTriangle(const Ray& ray,
                          const Triangle& tri, const float maxt, float& t, vec3& hit) {
	vec3 pd = (tri.v2 - tri.v1) * (tri.v3 - tri.v2);
	float len = pd.length();
	vec4 l = vec4(pd.x, pd.y, pd.z, dot(-pd, tri.v1)) * (1.0f / len);
	const float dist = -dot(l, vec4(ray.origin, 1.0f)) / dot(l, vec4(ray.dir, 0.0f));

	if (dist < 0 || isnan(dist) || dist > maxt) {
		return false;
	}

	t = dist;
	hit = ray.origin + ray.dir * dist;

	vec3 C;

	C = cross(tri.v2 - tri.v1, hit - tri.v1);
	if (dot(pd, C) < 0) return false; // P is on the right side 

	C = cross(tri.v3 - tri.v2, hit - tri.v2);
	if (dot(pd, C) < 0)  return false; // P is on the right side 

	C = cross(tri.v1 - tri.v3, hit - tri.v3);
	if (dot(pd, C) < 0) return false; // P is on the right side;

	return true;
}

bool pointInTriangle3D(const vec3& hit, const vec3& n, const Triangle& t) {
	vec2 cv1, cv2, cv3, cp;

	const float nx = (float)fabs(n.x);
	const float ny = (float)fabs(n.y);
	const float nz = (float)fabs(n.z);

	float max = fmaxf(nx, fmaxf(ny, nz));

	if (max == nx) {
		cv1 = vec2(t.v1.y, t.v1.z);
		cv2 = vec2(t.v2.y, t.v2.z);
		cv3 = vec2(t.v3.y, t.v3.z);
		cp = vec2(hit.y, hit.z);
	} else if (max == ny) {
		cv1 = vec2(t.v1.x, t.v1.z);
		cv2 = vec2(t.v2.x, t.v2.z);
		cv3 = vec2(t.v3.x, t.v3.z);
		cp = vec2(hit.x, hit.z);
	} else {
		cv1 = vec2(t.v1.x, t.v1.y);
		cv2 = vec2(t.v2.x, t.v2.y);
		cv3 = vec2(t.v3.x, t.v3.y);
		cp = vec2(hit.x, hit.y);
	}

	return pointInTriangle2D(cp, cv1, cv2, cv3);
}

bool pointInTriangle2D(const vec2& p, const Triangle2D& t) {
	return pointInTriangle2D(p, t.v1, t.v2, t.v3);
}

bool pointInTriangle2D(const vec2& p, const vec2& v1, const vec2& v2, const vec2& v3) {
	float s = v1.y * v3.x - v1.x * v3.y + (v3.y - v1.y) * p.x + (v1.x - v3.x) * p.y;
	float t = v1.x * v2.y - v1.y * v2.x + (v1.y - v2.y) * p.x + (v2.x - v1.x) * p.y;

	if ((s < 0) != (t < 0))
		return false;

	float area = -v2.y * v3.x + v1.y * (v3.x - v2.x) + v1.x * (v2.y - v3.y) + v2.x * v3.y;
	
	if (area < 0.0) {
		s = -s;
		t = -t;
		area = -area;
	}

	return s > 0 && t > 0 && (s + t) <= area;
}

float pointLineDistance(const vec2& p, const vec2& lstart, const vec2& lend) {
	float a = lend.y - lstart.y;
	float b = lstart.x - lend.x;
	float c = lend.x * lstart.y - lstart.x * lend.y;
	return ::fabsf(a * p.x + b * p.y + c) / ::sqrt(a * a + b * b);
}

vec2 lineLineIntersectionPoint(const vec2& p1, const vec2& p2,
															 const vec2& p3, const vec2& p4) {
	float x1df = p1.x - p2.x;
	float x2df = p3.x - p4.x;
	float y1df = p1.y - p2.y;
	float y2df = p3.y - p4.y;
	
	float d1 = (p1.x * p2.y - p1.y * p2.x);
	float d2 = (p3.x * p4.y - p3.y * p4.x);
	float len = (x1df * y2df - y1df * x2df);
	
	float x = (d1 * x2df - x1df * d2) / len;
	float y = (d1 * y2df - y1df * d2) / len;
	
	return vec2(x, y);
}

bool lineLineIntersection(const vec2& p1, const vec2& p2, const vec2& p3, const vec2& p4, vec2* ip) {
	float x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
	float y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;
	
	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

	if (d == 0) return false;
	
	float pre  = (x1 * y2 - y1 * x2);
	float post = (x3 * y4 - y3 * x4);
	
	vec2 hit;
	hit.x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
	hit.y = (pre * (y3 - y4) - (y1 - y2) * post) / d;
	
	*ip = hit;
	
//	// Check if the x and y coordinates are within both lines
//	if ( x < min(x1, x2) || x > max(x1, x2) ||
//			x < min(x3, x4) || x > max(x3, x4) ) return NULL;
//	if ( y < min(y1, y2) || y > max(y1, y2) ||
//			y < min(y3, y4) || y > max(y3, y4) ) return NULL;
	
	return true;
}

#define kEpsilon 0.0000001

bool rayIntersectTriangle2(const Ray& ray, const Triangle& tri, const float maxt, float& t, vec3& hit) {
	const vec3 N = cross(tri.v2 - tri.v1, tri.v3 - tri.v1); // N

	// check if ray and plane are parallel ?
	float NdotRayDirection = dot(N, ray.dir);
	if (::fabsf(NdotRayDirection) < kEpsilon) // almost 0 
		return false; 

	const float dist = ((dot(N, ray.origin) + dot(N, tri.v1))) / NdotRayDirection;

	if (dist > maxt || dist < 0 || isnan(dist)) return false; // the triangle is behind 

	t = dist;
	hit = ray.origin + ray.dir * dist;

	vec3 C;

	C = cross(tri.v2 - tri.v1, hit - tri.v1);
	if (dot(N, C) < 0) return false; // P is on the right side 

	C = cross(tri.v3 - tri.v2, hit - tri.v2);
	if (dot(N, C) < 0)  return false; // P is on the right side 

	C = cross(tri.v1 - tri.v3, hit - tri.v3);
	if (dot(N, C) < 0) return false; // P is on the right side; 

	return true; // this ray hits the triangle 
}

static inline unsigned int rand() {
	static unsigned next1 = 1151752134u, next2 = 2070363486u;
	next1 = next1 * 1701532575u + 571550083u;
	next2 = next2 * 3145804233u + 4178903934u;
	return (next1 << 16) ^ next2;
}

float randomValue() {
//	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	return float(rand() % 32767) / 32767.0f;
//	return float(rand() % 100000) / 100000.0f;
}

static inline vec3 randomPointOnSphere() {
	const float t2 = 2.0f * M_PI * randomValue();
	const float p2 = acosf(1.0f - 2.0f * randomValue());
	const float sp2 = sinf(p2);
	return vec3(sp2 * cosf(t2), sp2 * sinf(t2), cosf(p2));
}

vec3 randomRayInHemisphere(const vec3& n) {
	vec3 result = randomPointOnSphere();

	if (dot(result, n) < 0) {
		result = -result;
	}

	return result;
}

//vec3 randomPointInTriangle1(const Triangle& t) {
//  const float a1 = randomValue();
//  const float a2 = randomValue();
//
//  const vec3 vv1 = (t.v1 - t.v2);
//  const vec3 vv2 = (t.v3 - t.v2);
//
//	return t.v2 + (vv1 * a1 + vv2 * (1.0f - a1) * a2);
//	//return t.v2 + (vv1 * a1 + vv2 * a2);
//}

vec3 randomPointInTriangle(const Triangle& tri) {
//	const float r1 = randomValue();
//	const float r2 = randomValue();
//	
//	const float sq1 = sqrtf(r1);
//	const float i = 1.0f - sq1;
//	const float j = sq1 * (1.0f - r2);
//	const float k = sq1 * r2;
//	
//	const float x = i * tri.v1.x + j * tri.v2.x + k * tri.v3.x;
//	const float y = i * tri.v1.y + j * tri.v2.y + k * tri.v3.y;
//	const float z = i * tri.v1.z + j * tri.v2.z + k * tri.v3.z;
//
//	return vec3(x, y, z);
    float u = randomValue();
    float v = randomValue();

    if (u + v > 1.0f) {
        u = 1.0f - u;
        v = 1.0f - v;
    }

    return tri.v1 + (tri.v2 - tri.v1) * u + (tri.v3 - tri.v1) * v;
}

//bool rayBoxIntersect(const Ray& r, float t0, float t1) const {
//  float tmin, tmax, tymin, tymax, tzmin, tzmax;
//  
//  tmin = (parameters[r.sign[0]].x() - r.origin.x()) * r.inv_direction.x();
//  tmax = (parameters[1-r.sign[0]].x() - r.origin.x()) * r.inv_direction.x();
//  tymin = (parameters[r.sign[1]].y() - r.origin.y()) * r.inv_direction.y();
//  tymax = (parameters[1-r.sign[1]].y() - r.origin.y()) * r.inv_direction.y();
//  if ( (tmin > tymax) || (tymin > tmax) )
//    return false;
//  if (tymin > tmin)
//    tmin = tymin;
//  if (tymax < tmax)
//    tmax = tymax;
//  tzmin = (parameters[r.sign[2]].z() - r.origin.z()) * r.inv_direction.z();
//  tzmax = (parameters[1-r.sign[2]].z() - r.origin.z()) * r.inv_direction.z();
//  if ( (tmin > tzmax) || (tzmin > tmax) )
//    return false;
//  if (tzmin > tmin)
//    tmin = tzmin;
//  if (tzmax < tmax)
//    tmax = tzmax;
//  return ( (tmin < t1) && (tmax > t0) );
//}

bool rayIntersectBox(const Ray& ray, const BoundingBox& bbox, float* t) {
	return rayIntersectBox(ray, bbox.min, bbox.max, t);
}

bool rayIntersectBox(const Ray& ray, const vec3& lb, const vec3& rt, float* t) {
	// r.dir is unit direction vector of ray
	vec3 dirfrac;

	dirfrac.x = 1.0f / ray.dir.x;
	dirfrac.y = 1.0f / ray.dir.y;
	dirfrac.z = 1.0f / ray.dir.z;

	// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
	// r.org is origin of ray
	float t1 = (lb.x - ray.origin.x) * dirfrac.x;
	float t2 = (rt.x - ray.origin.x) * dirfrac.x;
	float t3 = (lb.y - ray.origin.y) * dirfrac.y;
	float t4 = (rt.y - ray.origin.y) * dirfrac.y;
	float t5 = (lb.z - ray.origin.z) * dirfrac.z;
	float t6 = (rt.z - ray.origin.z) * dirfrac.z;

	float tmax = fminf(fminf(fmaxf(t1, t2), fmaxf(t3, t4)), fmaxf(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
	if (tmax < 0) {
		if (t != NULL) *t = tmax;
		return false;
	}

	float tmin = fmax(fmax(fminf(t1, t2), fminf(t3, t4)), fminf(t5, t6));

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax) {
		if (t != NULL) *t = tmax;
		return false;
	}

	if (t != NULL) *t = tmin;
	return true;
}

int planeBoxOverlap(const vec3& normal, const vec3& vert, const vec3& maxbox) {
  vec3 vmin, vmax;
  
  // x
  if (normal.x > 0.0f) {
    vmin.x = -maxbox.x - vert.x;
    vmax.x =  maxbox.x - vert.x;
  } else {
    vmin.x =  maxbox.x - vert.x;
    vmax.x = -maxbox.x - vert.x;
  }
  
  // y
  if (normal.y > 0.0f) {
    vmin.y = -maxbox.y - vert.y;
    vmax.y =  maxbox.y - vert.y;
  } else {
    vmin.y =  maxbox.y - vert.y;
    vmax.y = -maxbox.y - vert.y;
  }

  // z
  if (normal.z > 0.0f) {
    vmin.z = -maxbox.z - vert.z;
    vmax.z =  maxbox.z - vert.z;
  } else {
    vmin.z =  maxbox.z - vert.z;
    vmax.z = -maxbox.z - vert.z;
  }
  
	if (dot(normal, vmin) > 0.0f) {
    return false;
	}

  return (dot(normal, vmax) >= 0.0f);
}


/*======================== X-tests ========================*/

#define AXISTEST_X01(a, b, fa, fb)			   \
  p0 = a * v0.y - b * v0.z;			       	   \
  p2 = a * v2.y - b * v2.z;			       	   \
  if (p0 < p2) { min = p0; max = p2; } else { min = p2; max = p0; } \
  rad = fa * boxHalfSize.y + fb * boxHalfSize.z;   \
  if (min > rad || max < -rad) return false;

#define AXISTEST_X2(a, b, fa, fb)			     \
  p0 = a * v0.y - b * v0.z;			           \
  p1 = a * v1.y - b * v1.z;			       	   \
  if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; } \
  rad = fa * boxHalfSize.y + fb * boxHalfSize.z;   \
  if (min > rad || max < -rad) return false;

/*======================== Y-tests ========================*/

#define AXISTEST_Y02(a, b, fa, fb)			   \
  p0 = -a * v0.x + b * v0.z;		      	   \
  p2 = -a * v2.x + b * v2.z;	       	       	   \
  if (p0<p2) { min=p0; max=p2;} else { min=p2; max=p0; } \
  rad = fa * boxHalfSize.x + fb * boxHalfSize.z;   \
  if (min>rad || max<-rad) return false;

#define AXISTEST_Y1(a, b, fa, fb)			   \
  p0 = -a * v0.x + b * v0.z;		      	   \
  p1 = -a * v1.x + b * v1.z;	     	       	   \
  if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0;} \
  rad = fa * boxHalfSize.x + fb * boxHalfSize.z;   \
  if (min > rad || max < -rad) return false;

/*======================== Z-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)			   \
  p1 = a * v1.x - b * v1.y;			           \
  p2 = a * v2.x - b * v2.y;			       	   \
  if (p2 < p1) { min = p2; max = p1; } else {min = p1; max = p2;} \
  rad = fa * boxHalfSize.x + fb * boxHalfSize.y;   \
  if (min > rad || max < -rad) return false;

#define AXISTEST_Z0(a, b, fa, fb)			   \
  p0 = a * v0.x - b * v0.y;				   \
  p1 = a * v1.x - b * v1.y;			           \
  if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; } \
  rad = fa * boxHalfSize.x + fb * boxHalfSize.y;   \
  if (min > rad || max < -rad) return false;

#define FINDMINMAX(x0, x1, x2, min, max) \
  min = max = x0;    \
  if(x1<min) min=x1; \
  if(x1>max) max=x1; \
  if(x2<min) min=x2; \
  if(x2>max) max=x2;

bool triangleIntersectBox(const vec3& boxcenter, const vec3& boxHalfSize, const Triangle& t) {
  /*    use separating axis theorem to test overlap between triangle and box */
  /*    need to test for overlap in these directions: */
  /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
  /*       we do not even need to test these) */
  /*    2) normal of the triangle */
  /*    3) crossproduct(edge from tri, {x,y,z}-directin) */
  /*       this gives 3x3=9 more tests */
  vec3 v0, v1, v2;

  //   float axis[3];
  float min, max, p0, p1, p2, rad;
  vec3 normal, fe, e0, e1, e2;
  
  /* This is the fastest branch on Sun */
  /* move everything so that the boxcenter is in (0,0,0) */
  v0 = t.v1 - boxcenter;
  v1 = t.v2 - boxcenter;
  v2 = t.v3 - boxcenter;
  
  /* compute triangle edges */
  e0 = v1 - v0;      /* tri edge 0 */
  e1 = v2 - v1;      /* tri edge 1 */
  e2 = v0 - v2;      /* tri edge 2 */
  
  /* Bullet 3:  */
  fe = fabsf(e0);
  
  AXISTEST_X01(e0.z, e0.y, fe.z, fe.y);
  AXISTEST_Y02(e0.z, e0.x, fe.z, fe.x);
  AXISTEST_Z12(e0.y, e0.x, fe.y, fe.x);
  
  fe = fabsf(e1);
  
  AXISTEST_X01(e1.z, e1.y, fe.z, fe.y);
  AXISTEST_Y02(e1.z, e1.x, fe.z, fe.x);
  AXISTEST_Z0(e1.y, e1.x, fe.y, fe.x);
  
  fe = fabsf(e2);
  
  AXISTEST_X2(e2.z, e2.y, fe.z, fe.y);
  AXISTEST_Y1(e2.z, e2.x, fe.z, fe.x);
  AXISTEST_Z12(e2.y, e2.x, fe.y, fe.x);
  
  /* Bullet 1: */
  /*  first test overlap in the {x,y,z}-directions */
  /*  find min, max of the triangle each direction, and test for overlap in */
  /*  that direction -- this is equivalent to testing a minimal AABB around */
  /*  the triangle against the AABB */
  
  /* test in X-direction */
  FINDMINMAX(v0.x, v1.x, v2.x, min, max);
  if (min > boxHalfSize.x || max < -boxHalfSize.x) return 0;
  
  /* test in Y-direction */
  FINDMINMAX(v0.y, v1.y, v2.y, min, max);
  if (min > boxHalfSize.y || max < -boxHalfSize.y) return 0;
  
  /* test in Z-direction */
  FINDMINMAX(v0.z, v1.z, v2.z, min, max);
  if (min > boxHalfSize.z || max < -boxHalfSize.z) return 0;
  
  /* Bullet 2: */
  /*  test if the box intersects the plane of the triangle */
  /*  compute plane equation of triangle: normal*x+d=0 */
  normal = cross(e0, e1);
  
  // -NJMP- (line removed here)
  if (!planeBoxOverlap(normal, v0, boxHalfSize)) return false;
  
  return true;   /* box and triangle overlaps */
}

float gaussianDistribution(const float x, const float sigma) {
	return (1.0f / sqrtf(2.0f * M_PI * sigma)) * ::pow(M_E, -(x * x / (2.0f * sigma)));
}

void gaussianDistributionGenKernel(float* kernel, const int kernelSize, const float sigma) {
	float sum = 0;
	
	if (kernelSize == 1) {
		kernel[0] = 1;
		return;
	} else if (kernelSize < 1) {
		return;
	}
	
	for (int y = 0; y < kernelSize; y++) {
		for (int x = 0; x < kernelSize; x++) {
			
			const float dy = gaussianDistribution((float)y / (kernelSize - 1) - 0.5f, sigma);
			const float dx = gaussianDistribution((float)x / (kernelSize - 1) - 0.5f, sigma);
			const float d = dx * dy;
			
			kernel[y * kernelSize + x] = d;
			sum += d;
		}
	}
	
	for (int y = 0; y < kernelSize; y++) {
		for (int x = 0; x < kernelSize; x++) {
			kernel[y * kernelSize + x] /= sum;
		}
	}
}

}
