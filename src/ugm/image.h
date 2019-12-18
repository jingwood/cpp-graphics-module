///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <cstring>
#include <algorithm>

#include "ucm/types.h"
#include "ucm/exception.h"
#include "types2d.h"
#include "color.h"

#define IMAGE_TYPE_RGBA_BYTE
#define DEFAULT_COLOR_BIT_DEPTH 32

namespace ugm {

using namespace unvell_common_class;

enum PixelDataFormat {
	PDF_RGB,
	PDF_BGR,
  PDF_RGBA,
  PDF_BGRA,
};

class Image {
private:
	PixelDataFormat pixelDataFormat = PixelDataFormat::PDF_RGBA;
	byte bitDepth = DEFAULT_COLOR_BIT_DEPTH;
	byte components = 4;
	byte componentByteLength = 4;
	byte pixelByteLength = 16;
	uint rowPixelByteLength = 0;
	
  byte* buffer = NULL;
	size_t bufferLength = 0;
  sizei size;
   
public:
 
	Image(const PixelDataFormat pixelDataFormat = PDF_RGBA, const uint bitDepth = DEFAULT_COLOR_BIT_DEPTH,
				const uint width = 0, const uint height = 0);
	~Image();

	void setPixelDataFormat(const PixelDataFormat format, const byte bitDepth);
	
	inline PixelDataFormat getPixelDataFormat() const { return this->pixelDataFormat; }
	inline byte getBitDepth() const { return this->bitDepth; }
	inline byte getPixelByteLength() const { return this->pixelByteLength; }
	inline uint getPixelRowByteLength() const { return this->rowPixelByteLength; }
	inline const byte getColorComponents() const { return this->components; }

	inline const sizei& getSize() const { return this->size; }
	inline const uint width() const { return (uint)this->size.width; }
	inline const uint height() const { return (uint)this->size.height; }
	
	inline int getPixelCount() const {
		return this->width() * this->height();
	}

	void resize(const int width, const int height);
  inline void resize(const sizei& size) {
		this->resize(size.width, size.height);
  }

	void createEmpty(const sizei& size) { this->createEmpty(size.width, size.height); }
	void createEmpty(const int width, const int height);

  inline void copyBuffer(const byte* buffer, const size_t length = -1) {
		memcpy(this->buffer, buffer, length == -1 ? this->bufferLength : length);
  }
  
	inline byte* getBuffer() const { return this->buffer; }
	inline size_t getBufferLength() const { return this->bufferLength; }
	
	void clear();
	
	void setPixel(const int x, const int y, const color4f& color);

	color4f getPixel(const pointi& p) const;
	color4f getPixel(int width, int height) const;

	void fillRect(const recti& rect, const color4f& c);
	void fillRect(const int x, const int y, const int width, const int height, const color4f& c);

	static void copy(const Image& imgsrc, Image& imgdest) {
		if (imgdest.width() != imgsrc.width() || imgdest.height() != imgsrc.height()) {
			imgdest.createEmpty(imgsrc.width(), imgsrc.height());
		}
		copyRect(imgsrc, imgdest);
	}
	
	static void copyRect(const Image& imgsrc, Image& imgdest) {
		if (imgsrc.size == imgdest.size
				&& imgsrc.pixelDataFormat == imgdest.pixelDataFormat
				&& imgsrc.bitDepth == imgdest.bitDepth) {
			imgdest.copyBuffer(imgsrc.getBuffer());
		} else {
			copyRect(imgsrc, 0, 0, imgdest, 0, 0);
		}
	}
	
	static void copyRect(const Image& imgsrc, const int srcX, const int srcY,
											 Image& imgdest, const int destX, const int destY) {
		const int minWidth = std::min(imgsrc.width(), imgdest.width());
		const int minHeight = std::min(imgsrc.height(), imgdest.height());
		
		copyRect(imgsrc, srcX, srcY, minWidth, minHeight, imgdest, destX, destY);
	}
	
	static void copyRect(const Image& imgsrc, const uint srcX, const uint srcY, const uint srcWidth, const uint srcHeight,
											 Image& imgdest, const uint destX, const uint destY);
	
	static void clone(const Image& src, Image& dest);
};

typedef Image Image4f, Image3f, Image4b, Image3b;

class NotSupportPixelColorTypeException : public Exception {
	
};

}

#endif /* __IMAGE_H_ */
