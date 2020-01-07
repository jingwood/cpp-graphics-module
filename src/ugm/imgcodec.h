///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef imgcodec_h
#define imgcodec_h

#include <stdio.h>

#include "ucm/string.h"
#include "ucm/file.h"
#include "ucm/archive.h"
#include "image.h"

#define FORMAT_TAG_JPEG 0x6765706a
#define FORMAT_TAG_PNG  0x20676e70
#define FORMAT_TAG_BMP  0x20706d62
#define FORMAT_TAG_GIF  0x20666967
#define FORMAT_TAG_TIFF 0x66666974

namespace ugm {

using namespace ucm;

enum ImageCodecFormat {
	ICF_AUTO,
	ICF_JPEG,
	ICF_PNG,
	ICF_GIF,
	ICF_BMP,
	ICF_TIFF,
};

bool readPNG(Image& image, Stream& stream);
void readJPEG(Image& image, Stream& stream);
void readJPEG(Image& image, FILE* file);

bool writePNG(const Image& image, Stream& stream);
void writeJPEG(const Image& image, Stream& stream);
void writeJPEG(const Image& image, FILE* file);

bool getImageFormatByExtension(const string& path, ImageCodecFormat* format);

void loadImage(Image& image, const string& path, ImageCodecFormat format = ICF_AUTO);
void loadImage(Image& image, Stream& stream, ImageCodecFormat format);
bool loadImage(Image& image, Archive& archive, const uint uid, ImageCodecFormat format = ICF_AUTO);

void saveImage(const Image& image, const string& path, ImageCodecFormat format = ICF_AUTO);
void saveImage(const Image& image, Stream& stream, ImageCodecFormat format);
uint saveImage(const Image& image, Archive& archive, ImageCodecFormat format);
uint saveImage(const Image& image, Archive& archive, uint formatTag, ImageCodecFormat format);

class NotSupportImageCodecException : public Exception { };

}

#endif /* imgcodec_h */
