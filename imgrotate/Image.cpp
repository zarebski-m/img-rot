#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdint>

#include "Image.h"

RealImage::RealImage(const std::vector<std::vector<PixelValue>>& data) : data(data) {}

Image::PixelValue RealImage::operator()(const PointF& pt) const {
	if (pt.first < 1 || pt.first >= height() - 1)
		return Image::BAD_VALUE;
	if (pt.second < 1 || pt.second >= width() - 1)
		return Image::BAD_VALUE;

	size_t left = static_cast<size_t>(pt.first);
	size_t right = left + 1;
	double leftWeight = pt.first - left;

	size_t top = static_cast<size_t>(pt.second);
	size_t bottom = top + 1;
	double topWeight = pt.second - top;

	double b1 = data[left][top];
	double b2 = data[right][top] - b1;
	double b3 = data[left][bottom] - b1;
	double b4 = -b2 - data[left][bottom] + data[right][bottom];

	return static_cast<PixelValue>(b1 + b2 * leftWeight + b3 * topWeight + b4 * leftWeight * topWeight);
}
	
size_t RealImage::width() const { return data[0].size(); }
size_t RealImage::height() const { return data.size(); }

ImageRotateView::ImageRotateView(const Image& img, double angle) : orig(img), angle(angle) {}

Image::PixelValue ImageRotateView::operator()(const PointF& pt) const {
	PointF res = rotate(pt);
	return orig(res);
}

size_t ImageRotateView::width() const { return orig.width(); }
size_t ImageRotateView::height() const { return orig.height(); }

Image::PointF ImageRotateView::rotate(const PointF& pt) const {
	double x = pt.first;
	double y = pt.second;
	double sinA = std::sin(angle);
	double cosA = std::cos(angle);
	double xOffset = orig.width() / 2.0;
	double yOffset = orig.height() / 2.0;

	x -= xOffset;
	y -= yOffset;

	double xnew = x * cosA + y * sinA;
	double ynew = y * cosA - x * sinA;

	return std::make_pair(xnew + xOffset, ynew + yOffset);
}
