#include <fstream>
#include <string>
#include <stdexcept>

#include <png.h>

#include "PngReader.h"

// A bit of magic for reading PNG files. 
// Based on http://www.piko3d.com/tutorials/libpng-tutorial-loading-png-files-from-streams

#define PNGSIGSIZE 8

void validate(std::istream& source) {
    png_byte pngsig[PNGSIGSIZE];
    int is_png = 0;
    source.read((char*)pngsig, PNGSIGSIZE);
    if (!source.good())
		throw std::runtime_error("Cannot read the source");

    is_png = png_sig_cmp(pngsig, 0, PNGSIGSIZE);
    if (is_png != 0)
		throw std::runtime_error("Source is not proper PNG file");
}

void userReadData(png_structp pngPtr, png_bytep data, png_size_t length) {
    png_voidp a = png_get_io_ptr(pngPtr);
    ((std::istream*)a)->read((char*)data, length);
}

template <typename T>
class ArrayDeleter {
public:
	void operator () (T* d) const { delete[] d; }
};

RealImage read(const std::string& fname) {
	std::ifstream source(fname.c_str(), std::ios::binary);
	if (!source.good())
		throw std::runtime_error("Cannot open file " + fname + " for read");

	validate(source);

	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngPtr)
		throw std::runtime_error("Couldn't initialize PNG read struct");

	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr) {
		png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
		throw std::runtime_error("Couldn't initialize PNG info struct");
	}

	png_set_read_fn(pngPtr,(png_voidp)&source, userReadData);

    png_set_sig_bytes(pngPtr, PNGSIGSIZE);
    png_read_info(pngPtr, infoPtr);

	// image size
	png_uint_32 imgWidth = png_get_image_width(pngPtr, infoPtr);
    png_uint_32 imgHeight = png_get_image_height(pngPtr, infoPtr);

    // bits per CHANNEL! note: not per pixel!
    png_uint_32 bitdepth = png_get_bit_depth(pngPtr, infoPtr);
    
	// number of channels
    png_uint_32 channels = png_get_channels(pngPtr, infoPtr);
    
	// color type (RGB, RGBA, Luminance, luminance alpha... palette... etc.)
    png_uint_32 color_type = png_get_color_type(pngPtr, infoPtr);

	switch (color_type) {
	case PNG_COLOR_TYPE_RGB:
	case PNG_COLOR_TYPE_RGBA:
		png_set_rgb_to_gray(pngPtr, 1, 0.212671, 0.715160);
        channels = 1;
        break;

    case PNG_COLOR_TYPE_GRAY:
        if (bitdepth < 8)
			png_set_expand_gray_1_2_4_to_8(pngPtr);
        bitdepth = 8;
        break;

	default:
		throw std::runtime_error("Unsupported PNG file type");
    }

    if (bitdepth == 16)
        png_set_strip_16(pngPtr);

	png_bytepp rowPtrs = new png_bytep[imgHeight];

    char* data = new char[imgWidth * imgHeight * bitdepth * channels / 8];
    const size_t stride = imgWidth * bitdepth * channels / 8;

    for (size_t i = 0; i < imgHeight; ++i) {
        png_uint_32 q = (imgHeight - i - 1) * stride;
		rowPtrs[i] = (png_bytep)data + q;
    }

    png_read_image(pngPtr, rowPtrs);

	std::vector<std::vector<Image::PixelValue>> vecData(imgHeight, std::vector<Image::PixelValue>(imgWidth));
	for (size_t i = 0; i < imgHeight; ++i)
		for (size_t j = 0; j < imgWidth; ++j)
			vecData[i][j] = rowPtrs[i][j];

	delete[] data;
	delete[] rowPtrs;
	source.close();

	return RealImage(vecData);
}
