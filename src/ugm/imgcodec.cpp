///////////////////////////////////////////////////////////////////////////////
//  unvell Common Graphics Module (libugm.a)
//  Common classes for cross-platform C++ 2D/3D graphics application.
//
//  MIT License
//  Copyright 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "imgcodec.h"

#include <vector>
#include "ucm/types.h"

extern "C" {
#include "jpeglib.h"
#include "png.h"
}

namespace ugm {

bool getImageFormatByExtension(const string& path, ImageCodecFormat* format) {
	if (path.endsWith(".jpg", StringComparingFlags::SCF_CASE_INSENSITIVE)
			|| path.endsWith(".jpeg", StringComparingFlags::SCF_CASE_INSENSITIVE)) {
		*format = ImageCodecFormat::ICF_JPEG;
		return true;
	} else if (path.endsWith(".png", StringComparingFlags::SCF_CASE_INSENSITIVE)) {
		*format = ImageCodecFormat::ICF_PNG;
		return true;
	} else if (path.endsWith(".gif", StringComparingFlags::SCF_CASE_INSENSITIVE)) {
		*format = ImageCodecFormat::ICF_GIF;
		return true;
	} else if (path.endsWith(".bmp", StringComparingFlags::SCF_CASE_INSENSITIVE)) {
		*format = ImageCodecFormat::ICF_BMP;
		return true;
	}
	
	return false;
}

void loadImage(Image& image, const string& path, ImageCodecFormat format) {
	
	if (format == ImageCodecFormat::ICF_AUTO) {
		getImageFormatByExtension(path, &format);
	}
	
	FileStream fs(path);
	fs.openRead();
	
	loadImage(image, fs, format);
	
	fs.close();
}

void loadImage(Image& image, Stream& stream, ImageCodecFormat format) {
	if (format == ImageCodecFormat::ICF_AUTO) {
		throw "must specify the image format";
	}
	
	switch (format) {
		default:
		case ImageCodecFormat::ICF_JPEG:
			readJPEG(image, stream);
			break;
			
		case ImageCodecFormat::ICF_PNG:
			readPNG(image, stream);
			break;
	}
}

bool loadImage(Image& image, Archive& archive, const uint uid, ImageCodecFormat format) {
	uint fmtcode = 0;

	switch (format) {
		case ImageCodecFormat::ICF_JPEG: fmtcode = FORMAT_TAG_JPEG; break;
		case ImageCodecFormat::ICF_PNG: fmtcode = FORMAT_TAG_PNG; break;
		case ImageCodecFormat::ICF_GIF: fmtcode = FORMAT_TAG_GIF; break;
		case ImageCodecFormat::ICF_BMP: fmtcode = FORMAT_TAG_BMP; break;
			
		default: break;
			
		case ImageCodecFormat::ICF_AUTO:
			if (loadImage(image, archive, uid, ImageCodecFormat::ICF_JPEG)) return true;
			if (loadImage(image, archive, uid, ImageCodecFormat::ICF_PNG)) return true;
			if (loadImage(image, archive, uid, ImageCodecFormat::ICF_BMP)) return true;
			if (loadImage(image, archive, uid, ImageCodecFormat::ICF_GIF)) return true;
			break;
	}
	
	bool success = false;

	ChunkEntry* entry = archive.openChunk(uid, fmtcode);
	if (entry != NULL) {

		if (entry->stream->getLength() > 0) {
			loadImage(image, *entry->stream, format);
			success = true;
		}
		
		archive.closeChunk(entry);
	}
	
	return success;
}

void readJPEG(Image& image, FILE* file) {

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, file);
	jpeg_read_header(&cinfo, true);

	jpeg_start_decompress(&cinfo);

	int row_stride = cinfo.output_width * cinfo.output_components;

	int buffer_height = 1;
	JSAMPARRAY buffer = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * buffer_height);
	buffer[0] = (JSAMPROW)malloc(sizeof(JSAMPLE) * row_stride);

	const int bufferLength = cinfo.output_width * cinfo.output_height * cinfo.output_components;
	byte* imageBuffer = new byte[bufferLength];
	byte* writeBufferPointer = imageBuffer;

	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, buffer, 1);
		memcpy(writeBufferPointer, buffer[0], row_stride);
		writeBufferPointer += row_stride;
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	image.setPixelDataFormat(PixelDataFormat::PDF_RGB, 8);
	image.createEmpty(cinfo.output_width, cinfo.output_height);
	
	if (cinfo.out_color_space == JCS_RGB) {
		image.copyBuffer(imageBuffer);
	}
	else if (cinfo.out_color_space == JCS_GRAYSCALE && cinfo.output_components == 1) {
		for (JDIMENSION y = 0; y < cinfo.output_height; y++) {
			for (JDIMENSION x = 0; x < cinfo.output_width; x++) {
				const float gray = imageBuffer[y * cinfo.output_width + x];
				image.setPixel(x, y, color4f(gray / 255.0f));
			}
		}
	}
	
	delete[] imageBuffer;
	imageBuffer = NULL;
	
	for (int i = 0; i < buffer_height; i++) {
		free(buffer[i]);
	}
	free(buffer);
}

// FIXME: multiple thread support
struct my_source_mgr {
	struct jpeg_source_mgr pub;
	Stream* is;
	JOCTET* buffer;
};

const static size_t JPEG_BUF_SIZE = 16384;

static std::vector<unsigned char> my_buffer;
#define BLOCK_SIZE 40960

static void my_init_source(j_decompress_ptr cinfo) {
}

static int my_fill_input_buffer(j_decompress_ptr cinfo) {
	my_source_mgr* src = (my_source_mgr*)cinfo->src;
	
	int readBytes = src->is->read((char*)src->buffer, JPEG_BUF_SIZE);

	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = readBytes;

	return TRUE;
}

static void my_skip_input_data (j_decompress_ptr cinfo, long count) {
	my_source_mgr* src = (my_source_mgr*)cinfo->src;
	if (count > 0) {
		while (count > (long)src->pub.bytes_in_buffer) {
			count -= (long)src->pub.bytes_in_buffer;
			my_fill_input_buffer(cinfo);
		}
		src->pub.next_input_byte += count;
		src->pub.bytes_in_buffer -= count;
	}
}

static void my_term_source(j_decompress_ptr cinfo) {
}

void readJPEG(Image& image, Stream& stream) {
	
	if (stream.getLength() > 2) {
		size_t pos = stream.getPosition();

		byte pngTag[2];
		stream.read(pngTag, 2);

		stream.setPosition(pos);
		
		if (pngTag[0] == 0x89 && pngTag[1] == 0x50) {
			readPNG(image, stream);
			return;
		}
	}
	
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	
	my_source_mgr* src;

	cinfo.src = (struct jpeg_source_mgr *)(*cinfo.mem->alloc_small)
		((j_common_ptr) &cinfo, JPOOL_PERMANENT, sizeof(my_source_mgr));
		
	src = (my_source_mgr*) cinfo.src;
	src->buffer = (JOCTET *)(*cinfo.mem->alloc_small)
		((j_common_ptr) &cinfo, JPOOL_PERMANENT, JPEG_BUF_SIZE * sizeof(JOCTET));
	
	src->is = &stream;
	src->pub.init_source = my_init_source;
	src->pub.fill_input_buffer = my_fill_input_buffer;
	src->pub.skip_input_data = my_skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
	src->pub.term_source = my_term_source;
	src->pub.bytes_in_buffer = 0;
	src->pub.next_input_byte = 0;

	jpeg_read_header(&cinfo, true);
	
	jpeg_start_decompress(&cinfo);
	
	int row_stride = cinfo.output_width * cinfo.output_components;
	
	int buffer_height = 1;
	JSAMPARRAY buffer = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * buffer_height);
	buffer[0] = (JSAMPROW)malloc(sizeof(JSAMPLE) * row_stride);
	
	const int bufferLength = cinfo.output_width * cinfo.output_height * cinfo.output_components;
	byte* imageBuffer = new byte[bufferLength];
	byte* writeBufferPointer = imageBuffer;
	
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, buffer, 1);
		memcpy(writeBufferPointer, buffer[0], row_stride);
		writeBufferPointer += row_stride;
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	
	image.setPixelDataFormat(PixelDataFormat::PDF_RGB, 8);
	image.createEmpty(cinfo.output_width, cinfo.output_height);
	
	if (cinfo.out_color_space == JCS_RGB) {
		image.copyBuffer(imageBuffer);
	}
	else if (cinfo.out_color_space == JCS_GRAYSCALE && cinfo.output_components == 1) {
		for (JDIMENSION y = 0; y < cinfo.output_height; y++) {
			for (JDIMENSION x = 0; x < cinfo.output_width; x++) {
				const float gray = imageBuffer[y * cinfo.output_width + x];
				image.setPixel(x, y, color4f(gray / 255.0f));
			}
		}
	}
	
	delete[] imageBuffer;
	imageBuffer = NULL;
	
	for (int i = 0; i < buffer_height; i++) {
		free(buffer[i]);
	}
	free(buffer);
}

void writeJPEG(const Image& image, FILE* file) {
	
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr       jerr;
 
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, file);
	
	cinfo.image_width      = image.width();
	cinfo.image_height     = image.height();
	cinfo.input_components = 3;
	cinfo.in_color_space   = JCS_RGB;
	
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, 90, true);
	jpeg_start_compress(&cinfo, true);
	
	const byte* imageBuffer = image.getBuffer();
	
	while (cinfo.next_scanline < cinfo.image_height) {
		JSAMPROW row_pointer = (JSAMPROW)&(imageBuffer[cinfo.next_scanline * cinfo.input_components * cinfo.image_width]);
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}
	
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
}

// FIXME: multiple thread support
struct my_destination_mgr {
	struct jpeg_destination_mgr pub; /* public fields */
	Stream* os; /* target stream */
	JOCTET * buffer;	   /* start of buffer */
};

static void my_init_destination(j_compress_ptr cinfo) {
	my_buffer.resize(BLOCK_SIZE);
	cinfo->dest->next_output_byte = &my_buffer[0];
	cinfo->dest->free_in_buffer = my_buffer.size();
}

static int my_empty_output_buffer(j_compress_ptr cinfo) {
	size_t oldsize = my_buffer.size();
	my_buffer.resize(oldsize + BLOCK_SIZE);
	cinfo->dest->next_output_byte = &my_buffer[oldsize];
	cinfo->dest->free_in_buffer = my_buffer.size() - oldsize;
	return true;
}

static void my_term_destination(j_compress_ptr cinfo) {
	my_buffer.resize(my_buffer.size() - cinfo->dest->free_in_buffer);
}

void writeJPEG(const Image& image, Stream& stream) {
	
#ifdef DEBUG
	assert(image.width() > 0);
	assert(image.height() > 0);
	assert(image.width() < 65500);
	assert(image.height() < 65500);
#endif /* DEBUG */
	
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr       jerr;
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	
	jpeg_destination_mgr dest;
	cinfo.dest = &dest;
	
	cinfo.dest->init_destination 		= &my_init_destination;
	cinfo.dest->empty_output_buffer = &my_empty_output_buffer;
	cinfo.dest->term_destination 		= &my_term_destination;

	cinfo.image_width      = image.width();
	cinfo.image_height     = image.height();
	cinfo.input_components = 3;
	cinfo.in_color_space   = JCS_RGB;
	
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, 90, true);
	jpeg_start_compress(&cinfo, true);
	
	const byte* imageBuffer = image.getBuffer();
	
	while (cinfo.next_scanline < cinfo.image_height) {
		JSAMPROW row_pointer = (JSAMPROW)&(imageBuffer[cinfo.next_scanline * cinfo.input_components * cinfo.image_width]);
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}
	
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	
	stream.write(my_buffer.data(), (uint)my_buffer.size());
}

void readPNG_readDataFromStream(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead) {
	png_voidp io_ptr = png_get_io_ptr(png_ptr);
	if (io_ptr == NULL) return;
	
	Stream& stream = *(Stream*)io_ptr;
	stream.read((byte*)outBytes, (uint)byteCountToRead);
}

bool readPNG(Image& image, Stream& stream) {
	byte sig[8];
	
	stream.read(sig, 8);
	if (!png_check_sig(sig, 8))
		return false;   /* bad signature */
	
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) return false;
	
	png_infop info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;   /* out of memory */
	}
	
	if (setjmp(png_jmpbuf(png_ptr)))
		return false;  // "[read_png_file] Error during init_io");
	
	png_set_read_fn(png_ptr, &stream, readPNG_readDataFromStream);

	png_set_sig_bytes(png_ptr, 8);
	
	png_read_info(png_ptr, info_ptr);
	
	uint width = png_get_image_width(png_ptr, info_ptr);
	uint height = png_get_image_height(png_ptr, info_ptr);
	byte color_type = png_get_color_type(png_ptr, info_ptr);
	byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	
	//int number_of_passes =
	png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);
	
	/* read file */
	if (setjmp(png_jmpbuf(png_ptr)))
		return false; //"[read_png_file] Error during read_image")
	
	
	png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (uint y = 0; y < height; y++) {
		row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
	}

	png_read_image(png_ptr, row_pointers);

	byte* buffer;
	
	switch (color_type) {
		case PNG_COLOR_TYPE_RGB:
			image.setPixelDataFormat(PixelDataFormat::PDF_RGB, bit_depth);
			image.createEmpty(width, height);
			buffer = (byte*)image.getBuffer();
			for (uint y = 0; y < height; y++) {
				memcpy(buffer + y * width * 3, row_pointers[y], width * image.getPixelByteLength());
			}
			break;
			
		case PNG_COLOR_TYPE_RGBA:
			image.setPixelDataFormat(PixelDataFormat::PDF_RGBA, bit_depth);
			image.createEmpty(width, height);
			buffer = (byte*)image.getBuffer();
			for (uint y = 0; y < height; y++) {
				memcpy(buffer + y * width * 4, row_pointers[y], width * image.getPixelByteLength());
			}
			break;
	}

	for (uint y = 0; y < height; y++) {
		free(row_pointers[y]);
	}
	free(row_pointers);
	
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);

	return true;
}

void writePNG_writeDataIntoStream(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead) {
	png_voidp io_ptr = png_get_io_ptr(png_ptr);
	if (io_ptr == NULL) return;
	
	Stream& stream = *(Stream*)io_ptr;
	stream.write((byte*)outBytes, (uint)byteCountToRead);
}

void writePNG_flushDataIntoStream(png_structp png_ptr) {
	png_voidp io_ptr = png_get_io_ptr(png_ptr);
	if (io_ptr == NULL) return;
	
	Stream& stream = *(Stream*)io_ptr;
	stream.flush();
}

bool writePNG(const Image& image, Stream& stream) {
	
	/* initialize stuff */
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if (!png_ptr)
		return false; //abort_("[write_png_file] png_create_write_struct failed");
	
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		return false; // abort_("[write_png_file] png_create_info_struct failed");
	
	if (setjmp(png_jmpbuf(png_ptr)))
		return false; //abort_("[write_png_file] Error during init_io");
	
	//png_init_io(png_ptr, fp);
	png_set_write_fn(png_ptr, &stream, writePNG_writeDataIntoStream, writePNG_flushDataIntoStream);
	
	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
		return false; // abort_("[write_png_file] Error during writing header");
	
	const uint width = image.width(), height = image.height();
	const byte bitDepth = image.getBitDepth();
	
	int colorType = 0;
	
	switch (image.getPixelDataFormat()) {
		case PixelDataFormat::PDF_RGB:
			colorType = PNG_COLOR_TYPE_RGB;
			break;

		case PixelDataFormat::PDF_RGBA:
			colorType = PNG_COLOR_TYPE_RGBA;
			break;

		default:
		case PixelDataFormat::PDF_BGR:
		case PixelDataFormat::PDF_BGRA:
			throw NotSupportImageCodecException();
			break;
	}
	
	png_set_IHDR(png_ptr, info_ptr, width, height,
							 bitDepth, colorType, PNG_INTERLACE_NONE,
							 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	
	png_write_info(png_ptr, info_ptr);
	
	
	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
		return false; // abort_("[write_png_file] Error during writing bytes");
	
	const byte* imageBuffer = image.getBuffer();
	const uint rowByteLength = image.getPixelRowByteLength();
	
	png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (uint y = 0; y < height; y++) {
		row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
		memcpy(row_pointers[y], imageBuffer + y * rowByteLength, rowByteLength);
	}
	
	png_write_image(png_ptr, row_pointers);
	
	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
		return false; //abort_("[write_png_file] Error during end of write");
	
	png_write_end(png_ptr, NULL);
	
	/* cleanup heap allocation */
	for (uint y = 0; y < height; y++) {
		free(row_pointers[y]);
	}
	free(row_pointers);
	
	png_destroy_write_struct(&png_ptr, &info_ptr);

	return true;
}

void saveImage(const Image& image, const string& path, ImageCodecFormat format) {
	
	if (format == ImageCodecFormat::ICF_AUTO) {
		getImageFormatByExtension(path, &format);
	}
	
	FileStream fs(path);
	fs.openWrite();
	
	Image image3b(PixelDataFormat::PDF_RGB, 8), image4b(PixelDataFormat::PDF_RGBA, 8);
	const Image* saveImage = &image;
	
	switch (format) {
		case ImageCodecFormat::ICF_JPEG:
			if (image.getColorComponents() > 3 || image.getBitDepth() != 8) {
				Image::copy(image, image3b);
				saveImage = &image3b;
			}
			writeJPEG(*saveImage, fs.getHandler());
			break;
		
		case ImageCodecFormat::ICF_PNG:
			if (image.getBitDepth() != 8) {
				Image::copy(image, image4b);
				saveImage = &image4b;
			}
			writePNG(*saveImage, fs);
			break;
		
		default:
			break;
	}
	
	fs.close();
}

void saveImage(const Image& image, Stream& stream, ImageCodecFormat format) {
	switch (format) {
		default:
			throw NotSupportImageCodecException();
			
		case ImageCodecFormat::ICF_AUTO:
		{
			const PixelDataFormat pdf = image.getPixelDataFormat();
			if (pdf == PixelDataFormat::PDF_RGBA || pdf == PixelDataFormat::PDF_BGRA) {
				saveImage(image, stream, ImageCodecFormat::ICF_PNG);
			} else {
				saveImage(image, stream, ImageCodecFormat::ICF_JPEG);
			}
		}
			break;
			
		case ImageCodecFormat::ICF_JPEG:
			
			if (image.getPixelDataFormat() == PixelDataFormat::PDF_RGBA) {
				Image imgtmp(PixelDataFormat::PDF_RGB, image.getBitDepth());
				Image::copy(image, imgtmp);
				writeJPEG(imgtmp, stream);
			} else {
				writeJPEG(image, stream);
			}
			break;
		
		case ImageCodecFormat::ICF_PNG:
			writePNG(image, stream);
			break;
	}
}

uint saveImage(const Image& img, Archive& archive, ImageCodecFormat format) {
	return saveImage(img, archive, 0, format);
}

uint saveImage(const Image& img, Archive& archive, uint formatTag, ImageCodecFormat format) {
	auto* chunk = archive.newChunk(formatTag);
	const uint uid = chunk->uid;
	chunk->isCompressed = false;
	saveImage(img, *chunk->stream, format);
	archive.updateAndCloseChunk(chunk);
	return uid;
}

}
