#include <algorithm>
#include <cmath>
#include <valarray>

#include "ImageComparer.h"

const double PI = 3.14159265359;

inline double deg2rad(double angle) {
	return (angle * PI) / 180.0;
}

std::pair<double, double> compareInRange(const Image& original, const Image& rotated, double start, double end, double step, double crop) {
	crop = std::max(0.0, std::min(1.0, crop));
	std::vector<std::pair<double, double>> results;
	for (double ang = start; ang < end; ang += step) {
		ImageRotateView rotatedView(rotated, deg2rad(ang));
		results.push_back(std::make_pair(compareImages(original, rotatedView, crop), ang));
	}
	auto min = std::min_element(results.cbegin(), results.cend());
	return *min;
}

double compareImages(const Image& img1, const Image& img2, double crop) {
	crop = std::max(0.0, std::min(1.0, crop));
	size_t widthMax = std::max(img1.width(), img2.width());
	size_t heightMax = std::max(img1.height(), img2.height());

	size_t width = static_cast<size_t>(crop * widthMax);
	size_t widthOffset = (widthMax - width) / 2;

	size_t height = static_cast<size_t>(crop * heightMax);
	size_t heightOffset = (heightMax - height) / 2;

	std::valarray<double> vec(width * height);
	size_t index = 0;
	Image::PointF pt;

	for (size_t i = heightOffset; i < height; ++i) {
		for (size_t j = widthOffset; j < width; ++j) {
			pt.first = i;
			pt.second = j;
			double outValue = 0.0;
			auto val1 = img1(pt);
			auto val2 = img2(pt);
			if (val1 != Image::BAD_VALUE && val2 != Image::BAD_VALUE) {
				outValue = std::abs(std::abs((double)val1) - std::abs((double)val2));
			}
			vec[index++] = outValue;
		}
	}
	return std::sqrt((vec * vec).sum());
}
