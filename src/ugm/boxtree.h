///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef boxtree_h
#define boxtree_h

#include <stdio.h>
#include <vector>

#include "types3d.h"

namespace ugm {

template<typename T>
class BoxTreeNode {
public:
	BoundingBox box;
	std::vector<BoxTreeNode*> children;
	
	void split() {
		
	}
};

template<typename T>
class BoxTree {
private:
	struct Item {
		BoundingBox box;
		const T* item;
	};
	BoxTreeNode<T> root;
	bool firstAdd = true;
	
	std::vector<Item> items;
	std::vector<BoxTreeNode<T>*> nodes;
	
public:
	BoxTree();
	
	void add(BoundingBox box, const T* item) {
		items.push_back({ box, item });
		
		if (this->firstAdd) {
			root.box.initTo(box.min);
			root.box.expandTo(box.max);
			firstAdd = false;
		} else {
			root.box.expandTo(box.min);
			root.box.expandTo(box.max);
		}
	}
	
	void construct() {
		root.split();
	}
	
};

}

#endif /* boxtree_h */
