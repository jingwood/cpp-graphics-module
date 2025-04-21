///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "image.h"
#include "functions.h"

namespace ugm {

Image::Image(const PixelDataFormat pixelDataFormat, const uint bitDepth, const uint width, const uint height) {
	this->setPixelDataFormat(pixelDataFormat, bitDepth);
	if (width > 0 && height > 0) {
		this->createEmpty(width, height);
	}
}

void Image::setPixelDataFormat(PixelDataFormat format, const byte bitDepth) {
	if (this->pixelDataFormat != format || this->bitDepth != bitDepth) {
		this->pixelDataFormat = format;
		this->bitDepth = bitDepth;
		
		if (this->buffer != NULL && this->width() > 0 && this->height() > 0) {
			this->createEmpty(this->width(), this->height());
		}
	}
}

Image::~Image() {
	if (this->buffer != NULL) {
		delete[] this->buffer;
	}
	
	this->buffer = NULL;
	this->bufferLength = 0;
	
	this->size = sizei(0, 0);
}

void Image::resize(const int newWidth, const int newHeight) {
	if (this->size.width == newWidth && this->size.height == newHeight) {
		return;
	}
	
	if (this->buffer == NULL) {
		this->size = sizei(newWidth, newHeight);
		return;
	}
	
	Image orgimg;
	Image::clone(*this, orgimg);
	
	this->createEmpty(newWidth, newHeight);
	
	const float sx = (float)orgimg.width() / newWidth, sy = (float)orgimg.height() / newHeight;

	for (float y = 0; y < newHeight; y++) {
		for (float x = 0; x < newWidth; x++) {
			float ox = x * sx, oy = y * sy;

			const int px = (int)ox, py = (int)oy;
			color4f c1 = orgimg.getPixel(px, py);
			color4f c2 = py + 1 < (int)orgimg.height() ? orgimg.getPixel(px, py + 1) : orgimg.getPixel(px, py);
			color4f c3 = px + 1 < (int)orgimg.width() ? orgimg.getPixel(px + 1, py) : orgimg.getPixel(px, py);
			color4f c4 = px + 1 < (int)orgimg.width() && py + 1 < (int)orgimg.height() ? orgimg.getPixel(px + 1, py + 1) : orgimg.getPixel(px, py);

			const float xr = ox - px, yr = oy - py;
			color4f d = c1 * (1.0f - xr) + c3 * xr;
			color4f e = c2 * (1.0f - xr) + c4 * xr;
			color4f o = d * (1.0f - yr) + e * yr;
			
			this->setPixel((int)x, (int)y, o);
		}
	}
}

void Image::createEmpty(const int width, const int height) {
	this->size = sizei(width, height);
	
	if (width > 0 && height > 0) {
		
		switch (this->pixelDataFormat) {
			default:
			case PDF_RGBA:
			case PDF_BGRA:
				this->components = 4;
				break;
			
			case PDF_RGB:
			case PDF_BGR:
				this->components = 3;
				break;
		}
		
		this->componentByteLength = this->bitDepth / 8;
		this->pixelByteLength = this->components * this->componentByteLength;
		this->rowPixelByteLength = this->width() * this->pixelByteLength;
		
		const size_t bufferLength = this->size.height * this->rowPixelByteLength;
		
		if (this->bufferLength != bufferLength) {
			if (this->buffer != NULL) {
				delete[] this->buffer;
			}
			
			this->buffer = new byte[bufferLength];
			this->bufferLength = bufferLength;
		}
		
		memset(this->buffer, 0, this->bufferLength);
	}
}

void Image::clear() {
	memset(this->buffer, 0, this->bufferLength);
}

void Image::setPixel(const int x, const int y, const color4f& color) {
	if (this->buffer == NULL) {
		throw BufferUnavailableException();
	}
	
	if (x < 0 || x >= this->width() || y < 0 || y >= this->height()) {
		throw ArgumentOutOfRangeException();
	}

	const int index = y * width() + x;
	
	if (this->components == 3) {
		if (this->bitDepth == 8) {
			*((color3b*)this->buffer + index) = tocolor3b(color);
		} else if (this->bitDepth == 32) {
			*((color3f*)this->buffer + index) = color.rgb;
		} else {
			throw NotSupportPixelColorTypeException();
		}
	} else if (this->components == 4) {
		if (this->bitDepth == 8) {
			*((color4b*)this->buffer + index) = tocolor4b(color);
		} else if (this->bitDepth == 32) {
			*((color4f*)this->buffer + index) = color;
		} else {
			throw NotSupportPixelColorTypeException();
		}
	} else {
		throw NotSupportPixelColorTypeException();
	}
}

color4f Image::getPixel(const pointi& p) const {
	return this->getPixel(p.x, p.y);
}

color4f Image::getPixel(int x, int y) const {
	if (x < 0 || x >= this->width() || y < 0 || y >= this->height()) {
		throw ArgumentOutOfRangeException();
	}
	
	const uint index = y * this->width() + x;
	
	if (this->components == 3) {
		if (this->bitDepth == 8) {
			return tocolor4f(*((color3b*)this->buffer + index));
		} else if (this->bitDepth == 32) {
			return *((color3f*)this->buffer + index);
		} else {
			throw NotSupportPixelColorTypeException();
		}
	} else if (this->components == 4) {
		if (this->bitDepth == 8) {
			return tocolor4f(*((color4b*)this->buffer + index));
		} else if (this->bitDepth == 32) {
			return *((color4f*)this->buffer + index);
		} else {
			throw NotSupportPixelColorTypeException();
		}
	} else {
		throw NotSupportPixelColorTypeException();
	}
}

void Image::fillRect(const recti& rect, const color4f& c) {
	this->fillRect(rect.x, rect.y, rect.width, rect.height, c);
}

void Image::fillRect(const int x, const int y, const int width, const int height, const color4f& c) {
	const int x2 = x + width;
	const int y2 = y + height;
	
	for (int iy = y; iy < y2; iy++) {
		for (int ix = x; ix < x2; ix++) {
			setPixel(ix, iy, c);
		}
	}
}

void Image::copyRect(const Image& imgsrc, const uint srcX, const uint srcY, const uint srcWidth, const uint srcHeight,
										 Image& imgdest, const uint destX, const uint destY) {
	uint endX = srcX + srcWidth;
	uint endY = srcY + srcHeight;
	
	if (endX > imgsrc.width() || endY > imgsrc.height()) {
		throw "source position or size out of range";
	}
	
	uint destEndX = destX + srcWidth;
	uint destEndY = destY + srcHeight;
	
	if (destEndX > imgdest.width() || destEndY > imgdest.height()) {
		throw "destination position or size out of range";
	}
	
	for (uint y = srcY, dy = destY; y < endY; y++, dy++) {
		for (uint x = srcX, dx = destX; x < endX; x++, dx++) {
			const color4f& c = imgsrc.getPixel(x, y);
			imgdest.setPixel(dx, dy, c);
		}
	}
}

void Image::clone(const Image& src, Image& dest) {
	dest.setPixelDataFormat(src.pixelDataFormat, src.bitDepth);
	dest.createEmpty(src.width(), src.height());
	dest.copyBuffer(src.buffer);
}

}
