///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef __SPACETREE_H__
#define __SPACETREE_H__

#include <vector>

#include "types3d.h"
#include "functions.h"

namespace ugm {

enum SplitDir
{
  ST_SplitX,
  ST_SplitY,
  ST_SplitZ,
};

template<typename T>
class SpaceTreeNode
{
public:
  vec3 origin, size;
  vec3 halfSize;
	vec3 minpos, maxpos;
  
  SpaceTreeNode() { }
  SpaceTreeNode(const vec3& origin, const vec3& size)
  : origin(origin), size(size), halfSize(size / 2.0),
		minpos(origin - halfSize), maxpos(origin + halfSize)
  { }
  
  bool splitted = false;
  SplitDir splitNextDir = ST_SplitX;
	SpaceTreeNode* left = NULL;
	SpaceTreeNode* right = NULL;

	std::vector<T> list;
  
  inline bool intersectTriangle(const Triangle& t) const {
    return triangleIntersectBox(this->origin, this->halfSize, t);
  }
  
  inline bool intersectRay(const Ray& r) const {
    return rayIntersectBox(r, this->minpos, this->maxpos);
  }
  
  void split(const int depth = 0, const int maxDepth = 3)
  {
    switch (this->splitNextDir)
    {
      case ST_SplitX:
        {
          const float halfHalfSize = this->halfSize.x / 2;
					//if (halfHalfSize < 0.1) {
					//	return;
					//}

          const vec3 splittdBoxSize = vec3(this->halfSize.x, this->size.y, this->size.z);
          
          // (-2.5, 0, 0) (5, 10, 10)
          this->left = new SpaceTreeNode(vec3(origin.x - halfHalfSize,
                                              origin.y,
                                              origin.z), splittdBoxSize);

          // ( 2.5, 0, 0) (5, 10, 10)
          this->right = new SpaceTreeNode(vec3(origin.x + halfHalfSize,
                                               origin.y,
                                               origin.z), splittdBoxSize);
          
          this->left->splitNextDir = ST_SplitY;
          this->right->splitNextDir = ST_SplitY;
        }
        break;
        
      case ST_SplitY:
        {
          const float halfHalfSize = this->halfSize.y / 2;
          const vec3 splittdBoxSize = vec3(this->size.x, this->halfSize.y, this->size.z);
          
          // (-10, -10, 0) (10, 10, 20)
          this->left = new SpaceTreeNode(vec3(origin.x,
                                              origin.y - halfHalfSize,
                                              origin.z), splittdBoxSize);
          
          // ( 10,  10, 0) (10, 10, 20)
          this->right = new SpaceTreeNode(vec3(origin.x,
                                               origin.y + halfHalfSize,
                                               origin.z), splittdBoxSize);
          
          this->left->splitNextDir = ST_SplitZ;
          this->right->splitNextDir = ST_SplitZ;
        }
        break;
        
      case ST_SplitZ:
        {
          const float halfHalfSize = this->halfSize.z / 2;
          const vec3 splittdBoxSize = vec3(this->size.x, this->size.y, this->halfSize.z);
          
          // (-10, -10, -10) (10, 10, 10)
          this->left = new SpaceTreeNode(vec3(origin.x,
                                              origin.y,
                                              origin.z - halfHalfSize), splittdBoxSize);
          
          // ( 10,  10,  10) (10, 10, 10)
          this->right = new SpaceTreeNode(vec3(origin.x,
                                               origin.y,
                                               origin.z + halfHalfSize), splittdBoxSize);
          
          this->left->splitNextDir = ST_SplitX;
          this->right->splitNextDir = ST_SplitX;
        }
        break;
    }
    
		this->splitted = true;

		if (depth < maxDepth)
		{
			this->left->split(depth + 1);
			this->right->split(depth + 1);
		}
	}

	~SpaceTreeNode() {
		this->release();
	}

	void release()
	{
		this->list.clear();

		if (this->left != NULL) {
			this->left->release();
			delete this->left;
			this->left = NULL;
		}

		if (this->right != NULL) {
			this->right->release();
			delete this->right;
			this->right = NULL;
		}
	}
};

template<typename T>
class SpaceTree
{
public:
	SpaceTreeNode<T> root;

  void initSpace(const BoundingBox& box, const int maxDepth = 3) {
    this->root.origin = box.origin;
		this->root.size = box.size;
    this->root.halfSize = box.size / 2.0f;

		this->root.split(0, maxDepth);
  }

	~SpaceTree() {
		this->root.release();
	}
};

}

#endif /* __SPACETREE_H__ */
