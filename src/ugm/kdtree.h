///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __KDTREE_H__
#define __KDTREE_H__

#include <vector>
#include <functional>
#include "types2d.h"
#include "types3d.h"
#include "functions.h"

namespace ugm {

template<typename T>
class KDNode {
public:
	BoundingBox bbox;
	KDNode* left = NULL;
	KDNode* right = NULL;

	std::vector<T> items;

	KDNode() {
	}

	~KDNode() {
		reset();
	}

	inline BoundingBox boundingBoxFromItems(T* items, size_t itemCount) {
		BoundingBox bbox = items[0]->bbox;
		for (size_t i = 1; i < itemCount; i++) {
			bbox.expandTo(items[i]->bbox);
		}
		bbox.finalize();
		return bbox;
	}
	
	void reset() {
		if (this->left != NULL) {
			delete this->left;
			this->left = NULL;
		}
		
		if (this->right != NULL) {
			delete this->right;
			this->right = NULL;
		}
		
		this->items.clear();
	}

    void build(T* items, size_t itemCount, int depth = 0) {
        if (itemCount <= 2 || depth >= 32) {
            this->items.assign(items, items + itemCount);
            this->bbox = boundingBoxFromItems(items, itemCount);
            return;
        }

        // 全体バウンディングボックス
        BoundingBox nodeBox = boundingBoxFromItems(items, itemCount);

        // 最良の分割コスト
        float bestCost = std::numeric_limits<float>::max();
        int bestAxis = -1;
        float bestSplit = 0.0f;

        // 各軸ごとに評価
        for (int axis = 0; axis < 3; ++axis) {
            // ソート用構造体
            std::vector<std::pair<float, T>> sorted;
            for (size_t i = 0; i < itemCount; ++i) {
                float center = items[i]->bbox.origin[axis];
                sorted.emplace_back(center, items[i]);
            }
            std::sort(sorted.begin(), sorted.end(),
                [](const auto& a, const auto& b) { return a.first < b.first; });

            std::vector<BoundingBox> leftBoxes(itemCount);
            std::vector<BoundingBox> rightBoxes(itemCount);

            BoundingBox leftBox, rightBox;
            for (size_t i = 0; i < itemCount; ++i) {
                leftBox.expandTo(sorted[i].second->bbox);
                leftBoxes[i] = leftBox;
            }
            for (int i = (int)itemCount - 1; i >= 0; --i) {
                rightBox.expandTo(sorted[i].second->bbox);
                rightBoxes[i] = rightBox;
            }

            // 各分割点のコストを評価
            for (size_t i = 1; i < itemCount; ++i) {
                float SA = nodeBox.surfaceArea();
                float SAL = leftBoxes[i - 1].surfaceArea();
                float SAR = rightBoxes[i].surfaceArea();

                int NL = (int)i;
                int NR = (int)(itemCount - i);
                float cost = (SAL / SA) * NL + (SAR / SA) * NR;

                if (cost < bestCost) {
                    bestCost = cost;
                    bestAxis = axis;
                    bestSplit = 0.5f * (sorted[i - 1].first + sorted[i].first);
                }
            }
        }

        // 分割失敗 → リーフ
        if (bestAxis == -1) {
            this->items.assign(items, items + itemCount);
            this->bbox = nodeBox;
            return;
        }

        // 左右に振り分け
        std::vector<T> ltris, rtris;
        for (size_t i = 0; i < itemCount; ++i) {
            const T& item = items[i];
            if (item->bbox.origin[bestAxis] < bestSplit) {
                ltris.push_back(item);
            } else {
                rtris.push_back(item);
            }
        }

        // リーフ条件
        if (ltris.size() == itemCount || rtris.size() == itemCount) {
            this->items.assign(items, items + itemCount);
            this->bbox = nodeBox;
            return;
        }

        // 再帰構築
        this->left = new KDNode();
        this->left->build(ltris.data(), ltris.size(), depth + 1);

        this->right = new KDNode();
        this->right->build(rtris.data(), rtris.size(), depth + 1);

        this->bbox = nodeBox;
    }

    bool iterate(const Ray& ray, std::function<bool(T)> iterator) const {
        for (const auto& t : this->items) {
            const bool res = iterator(t);
            if (!res) return false;
        }

        if (this->left != NULL && this->left->bbox.intersects(ray)) {
            const bool res = this->left->iterate(ray, iterator);
            if (!res) return false;
        }

        if (this->right != NULL && this->right->bbox.intersects(ray)) {
            const bool res = this->right->iterate(ray, iterator);
            if (!res) return false;
        }

        return true;
    }

//    bool iterate(const Ray& ray, std::function<bool(T)> iterator) const {
//        // 現在のノード内のアイテムに対して処理
//        for (const auto& t : this->items) {
//            if (!iterator(t)) return false;
//        }
//
//        // 左右ノードの距離計算
//        float tminLeft = 0.0f, tmaxLeft = 0.0f;
//        float tminRight = 0.0f, tmaxRight = 0.0f;
//        bool hitLeft = this->left && this->left->bbox.intersects(ray, tminLeft, tmaxLeft);
//        bool hitRight = this->right && this->right->bbox.intersects(ray, tminRight, tmaxRight);
//
//        if (hitLeft && hitRight) {
//            // 距離の近い方から探索
//            if (tminLeft < tminRight) {
//                if (!this->left->iterate(ray, iterator)) return false;
//                if (!this->right->iterate(ray, iterator)) return false;
//            } else {
//                if (!this->right->iterate(ray, iterator)) return false;
//                if (!this->left->iterate(ray, iterator)) return false;
//            }
//        }
//        else if (hitLeft) {
//            if (!this->left->iterate(ray, iterator)) return false;
//        }
//        else if (hitRight) {
//            if (!this->right->iterate(ray, iterator)) return false;
//        }
//
//        return true;
//    }
	
};

template<typename T>
class KDNode2D {
public:
	BBox2D bbox;
	KDNode2D* left = NULL;
	KDNode2D* right = NULL;
	
	std::vector<T> items;
	
	KDNode2D() {
	}
	
	~KDNode2D() {
		reset();
	}
	
	inline BBox2D boundingBoxFromItems(T* items, size_t itemCount) {
		BBox2D bbox = items[0].bbox;
		for (size_t i = 1; i < itemCount; i++) {
			bbox.expandTo(items[i].bbox);
		}
		return bbox;
	}
	
	void reset() {
		if (this->left != NULL) {
			delete this->left;
			this->left = NULL;
		}
		
		if (this->right != NULL) {
			delete this->right;
			this->right = NULL;
		}
		
		this->items.clear();
	}
	
	void build(T* items, size_t itemCount, int depth = 0) {
		if (itemCount <= 0) {
			return;
		}
		else if (itemCount <= 1) {
			this->bbox = items[0].bbox;
			this->items.push_back(items[0]);
			return;
		}
		
		BBox2D bbox = boundingBoxFromItems(items, itemCount);
		
		if (itemCount <= 3) {
			this->bbox = bbox;
			for (size_t i = 0; i < itemCount; i++) {
				this->items.push_back(items[i]);
			}
			return;
		}
		
		KDNode2D* left = NULL;
		KDNode2D* right = NULL;
		BBox2D lbox, rbox;
		std::vector<T> ltris, rtris;
		
		const vec2 boxSize = bbox.getSize();
		const vec2 splitPoint = bbox.min + boxSize * 0.5f;
		
		if (boxSize.x > boxSize.y) {
			lbox = BBox2D(bbox.min, vec2(splitPoint.x, bbox.max.y));
			rbox = BBox2D(vec2(splitPoint.x, bbox.min.y), bbox.max);
		} else {
			lbox = BBox2D(bbox.min, vec2(bbox.max.x, splitPoint.y));
			rbox = BBox2D(vec2(bbox.min.x, splitPoint.y), bbox.max);
		}
		
		for (size_t i = 0; i < itemCount; i++/*, pitem++*/) {
			const T& item = items[i];
			
			const auto& bbox = item.bbox;
			
			if (lbox.contains(bbox)) {
				ltris.push_back(item);
			}
			else if (rbox.contains(bbox)) {
				rtris.push_back(item);
			}
			else {
				this->items.push_back(item);
			}
		}
		
		if (ltris.size() > 0) {
			left = new KDNode2D();
			left->build(ltris.data(), ltris.size(), depth + 1);
			this->left = left;
		}
		
		if (rtris.size() > 0) {
			right = new KDNode2D();
			right->build(rtris.data(), rtris.size(), depth + 1);
			this->right = right;
		}
		
		if (this->items.size() > 0) {
			
			bbox = boundingBoxFromItems(this->items.data(), this->items.size());
			
			if (left != NULL) bbox.expandTo(left->bbox);
			if (right != NULL) bbox.expandTo(right->bbox);
			
			this->bbox = bbox;
		}
		else {
			if (left != NULL && right == NULL) {
				this->bbox = left->bbox;
			}
			else if (left == NULL && right != NULL) {
				this->bbox = right->bbox;
			}
			else {
				this->bbox = BBox2D(left->bbox.min, right->bbox.max);
			}
		}
	}
	
	void split(int depth = 0, int maxDepth = 5) {
		if (depth > maxDepth) return;
		
		const vec2 boxSize = this->bbox.getSize();
		const vec2 splitPoint = this->bbox.min + boxSize * 0.5f;
		
		if (this->left == NULL) this->left = new KDNode2D();
		if (this->right == NULL) this->right = new KDNode2D();
		
		if (boxSize.x > boxSize.y) {
			this->left->bbox = BBox2D(bbox.min, vec2(splitPoint.x, bbox.max.y));
			this->right->bbox = BBox2D(vec2(splitPoint.x, bbox.min.y), bbox.max);
		} else {
			this->left->bbox = BBox2D(bbox.min, vec2(bbox.max.x, splitPoint.y));
			this->left->bbox = BBox2D(vec2(bbox.min.x, splitPoint.y), bbox.max);
		}
		
		this->left->split(depth + 1, maxDepth);
		this->right->split(depth + 1, maxDepth);
	}
	
	void addItem(T& item) {
		this->items.push_back(item);
		
		if (this->left != NULL && this->left->bbox.contains(item.bbox)) {
			this->left->addItem(item);
			return;
		} else if (this->right != NULL && this->right->bbox.contains(item.bbox)) {
			this->right->addItem(item);
			return;
		}
		
		if (this->items.size() < 5) {
			this->items.push_back(item);
			this->bbox.expandTo(item.bbox);
			return;
		}
	}
	
	bool iterate(const vec2& p, std::function<bool(T&)> iterator) const {
		for (const auto& t : this->items) {
			if (t.bbox.contains(p)) {
				const bool res = iterator(t);
				if (!res) return false;
			}
		}
		
		if (this->left != NULL && this->left->bbox.contains(p)) {
			const bool res = this->left->iterate(p, iterator);
			if (!res) return false;
		}
		
		if (this->right != NULL && this->right->bbox.contains(p)) {
			const bool res = this->right->iterate(p, iterator);
			if (!res) return false;
		}
		
		return true;
	}
	
	bool iterate(const BBox2D& box, std::function<bool(T&)> iterator) const {
		for (const auto& i : this->items) {
			if (i.bbox.intersects(bbox)) {
				const bool res = iterator(box);
				if (!res) return false;
			}
		}
		
		if (this->left != NULL && this->left->bbox.contains(box)) {
			const bool res = this->left->iterate(box, iterator);
			if (!res) return false;
		}
		
		if (this->right != NULL && this->right->bbox.contains(box)) {
			const bool res = this->right->iterate(box, iterator);
			if (!res) return false;
		}
		
		return true;
	}
	
	bool hitAny(const vec2& p) const {
		for (const auto& t : this->items) {
			if (t.bbox.contains(p) && pointInTriangle2D(p, t)) {
				return true;
			}
		}
		
		if (this->left != NULL && this->left->bbox.contains(p)) {
			const bool res = this->left->hitAny(p);
			if (res) return true;
		}
		
		if (this->right != NULL && this->right->bbox.contains(p)) {
			const bool res = this->right->hitAny(p);
			if (res) return true;
		}
		
		return false;
	}
	
	bool hitAny(const BBox2D& box) const {
		for (const auto& t : this->items) {
			if (t.bbox.intersects(box)) {
				return true;
			}
		}
		
		if (this->left != NULL && this->left->bbox.contains(box)) {
			const bool res = this->left->hitAny(box);
			if (res) return true;
		}
		
		if (this->right != NULL && this->right->bbox.contains(box)) {
			const bool res = this->right->hitAny(box);
			if (res) return true;
		}
		
		return false;
	}
};

}

#endif /* __KDTREE_H__ */
