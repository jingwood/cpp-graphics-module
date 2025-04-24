///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef imgfilter_h
#define imgfilter_h

#include <stdio.h>

#include "image.h"

namespace ugm {

namespace img {
	enum CalcMethods {
        Add,
        Sub,
		Lighter,
	};
	
	void blur(Image& img);
	void gaussBlur(Image& img, const uint range);
    void threshold(Image& img, float thresholdValue);
    void thresholdSoft(Image& img, float thresholdValue, float curvePower = 3.5 /* 2 ~ 5 */);
	void gamma(Image& img, const double gamma);

	void flipImageHorizontally(Image& image);
	void flipImageVertically(Image& image);

	void calc(Image& imga, Image& imgb,
						const CalcMethods method = CalcMethods::Lighter,
						const float factor = 1.0f);

}
}


#endif /* imgfilter_h */
