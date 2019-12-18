///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "imgfilter.h"
#include "functions.h"

#define BLUR_GAUSS_KERNEL_SIZE 5

namespace ugm {
namespace img {
	
	void gaussBlur(Image& img, const float* kernel, const uint kernelSize);
	
	void blur(Image& img) {
		
		static float gaussKernel[BLUR_GAUSS_KERNEL_SIZE * BLUR_GAUSS_KERNEL_SIZE] = {
			0.01f, 0.02f, 0.04f, 0.02f, 0.01f,
			0.02f, 0.04f, 0.08f, 0.04f, 0.02f,
			0.04f, 0.08f, 0.16f, 0.08f, 0.04f,
			0.02f, 0.04f, 0.08f, 0.04f, 0.02f,
			0.01f, 0.02f, 0.04f, 0.02f, 0.01f,
		};
		
		gaussBlur(img, gaussKernel, BLUR_GAUSS_KERNEL_SIZE);
	}
	
	void gaussBlur(Image& img, const uint range) {
		float* kernel = new float[range * range];
		gaussianDistributionGenKernel(kernel, range);
		
		gaussBlur(img, kernel, range);
		
		delete [] kernel;
	}
	
	void gaussBlur(Image& img, const float* kernel, const uint kernelSize) {
		
		const int w = img.width(), h = img.height();
		
		Image newimg(PixelDataFormat::PDF_RGB, 32);
		newimg.createEmpty(w, h);
		
		const int halfKernelSize = kernelSize / 2;

		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {

				color4f sample;
				
				for (int ky = -halfKernelSize, qy = 0; qy < (int)kernelSize; ky++, qy++) {
					for (int kx = -halfKernelSize, qx = 0; qx < (int)kernelSize; kx++, qx++) {
						
						int mx = x + kx, my = y + ky;

						if (mx < 0) { mx = 0; }
						else if (mx >= w) { mx = w - 1; }

						if (my < 0) { my = 0; }
						else if (my >= h) { my = h - 1; }
						
						sample += img.getPixel(mx, my) * kernel[qy * kernelSize + qx];
					}
				}
				
				newimg.setPixel(x, y, sample);
			}
		}
		
		Image::copy(newimg, img);
	}
	
	void gamma(Image& img, const double gamma) {
		const float delta = (float)(1.0 / gamma);
		
		for (uint y = 0; y < img.height(); y++) {
			for (uint x = 0; x < img.width(); x++) {
				
				const color4f c = img.getPixel(x, y);
				img.setPixel(x, y, pow(c, delta));
			}
		}
	}
	
	void flipImageHorizontally(Image& image) {
		Image tmpImage(image.getPixelDataFormat(), image.getBitDepth());
		tmpImage.createEmpty(image.width(), image.height());
		
		const int width = image.width();
		const int height = image.height();
		
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				const color4f& c = image.getPixel(x, y);
				tmpImage.setPixel(width - x - 1, y, c);
			}
		}
		
		image.copyBuffer(tmpImage.getBuffer());
	}
	
	void flipImageVertically(Image& image) {
		Image tmpImage(image.getPixelDataFormat(), image.getBitDepth());
		tmpImage.createEmpty(image.width(), image.height());
		
		const int width = image.width();
		const int height = image.height();
		
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				const color4f& c = image.getPixel(x, y);
				tmpImage.setPixel(x, height - y - 1, c);
			}
		}
		
		image.copyBuffer(tmpImage.getBuffer());
	}

	void calc(Image& imga, Image& imgb, const CalcMethods method, const float factor) {
		const int width = imga.width();
		const int height = imga.height();
		
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				const color4f c1 = imga.getPixel(x, y);
				const color4f c2 = imgb.getPixel(x, y);
				color4f oc;
				
				switch (method) {
					default:
						oc = c1;
						break;

					case CalcMethods::Lighter:
					{
						color4f diff = c2 - c1;
						if (diff.r < 0) diff.r = 0;
						if (diff.g < 0) diff.g = 0;
						if (diff.b < 0) diff.b = 0;
						diff.a = 0;
						
						oc = c1 + diff * factor;
					}
					break;
				}
				
				imga.setPixel(x, y, oc);
			}
		}
	}

}
}