///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef types3d_h
#define types3d_h

#include <stdio.h>

#include "vector.h"
#include "color.h"
#include "matrix.h"

namespace ugm {

struct Ray {
	vec3 origin;
	vec3 dir;

	Ray() { }

	Ray(const vec3& origin, const vec3& dir)
		: origin(origin), dir(dir)
	{ }
};

struct BoundingBox;

struct Triangle {
	vec3 v1, v2, v3;

	//// todo: replace with bbox property
	//inline BoundingBox getBoundingBox() const {
	//	return BoundingBox::fromTriangle(this->v1, this->v2, this->v3);
	//}
};

struct TriangleNUV {
	union {
		Triangle t;
		struct {
			vec3 v1, v2, v3;
			vec3 n1, n2, n3;
			vec2 uv1, uv2, uv3;
		};
	};
	
	TriangleNUV() : t() { }
};

struct TriangleNUV2TBC {
	union {
		Triangle t;
		struct {
			union {
				struct { vec3 v1, v2, v3; };
				vec3 vs[3];
			};
			union {
				struct { vec3 n1, n2, n3; };
				vec3 ns[3];
			};
			union {
				struct { vec2 uv1, uv2, uv3, uv4, uv5, uv6; };
				vec2 uvs[6];
			};
			union {
				struct { vec3 t1, t2, t3, b1, b2, b3; };
				vec3 tbs[6];
			};
			union {
				struct { color3 c1, c2, c3; };
				color3 cs[3];
			};
		};
	};
	
	TriangleNUV2TBC() : t() { }
};

struct BoundingBox {
	union {
		struct {
			float minX;
			float minY;
			float minZ;
		};
		vec3 min;
	};

	union {
		struct {
			float maxX;
			float maxY;
			float maxZ;
		};
		vec3 max;
	};
	
	vec3 origin;
	vec3 size;
	
public:
	BoundingBox() : BoundingBox(vec3(), vec3()) { }
	
	BoundingBox(const vec3& min, const vec3& max) : min(min), max(max) {
		this->finalize();
	}
	
	void initTo(const vec3& v);
	void expandTo(const vec3& v);
	void expandTo(const BoundingBox& bbox);
	void expandTo(const Triangle& t);
	void expandTo(const vec3& v1, const vec3& v2, const vec3& v3);

	void finalize();
    float surfaceArea() const;
	
	bool contains(const vec3& p) const;
	bool contains(const BoundingBox& bbox) const;
	bool contains(const Triangle& t) const;

	static BoundingBox fromTriangle(const Triangle& t);
	static BoundingBox fromTriangle(const vec3& v1, const vec3& v2, const vec3& v3);
	static BoundingBox fromTriangles(const Triangle* t, const uint triangleCount);

	static BoundingBox fromBoundingBoxes(const BoundingBox* boxes, const uint boxCount);

	bool intersects(const Ray& ray) const;
    bool intersects(const Ray& ray, float& tmin, float& tmax) const;
	bool intersects(const BoundingBox& bbox) const;
	
	static bool intersects(const BoundingBox& b1, const BoundingBox& b2);
	
	BoundingBox operator+ (const vec3& offset) const;
	BoundingBox operator+=(const vec3& offset);
	BoundingBox operator- (const vec3& offset) const;
	BoundingBox operator-=(const vec3& offset);
	BoundingBox operator* (const vec3& scale) const;
	BoundingBox operator* (const Matrix4& mat) const;
	BoundingBox operator*= (const Matrix4& mat);
};

}

#endif /* types3d_h */
