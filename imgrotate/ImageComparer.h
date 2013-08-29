#pragma once

#include "Image.h"

double compareImages(const Image& img1, const Image& img2, double crop);

std::pair<double, double> compareInRange(const Image& original, const Image& rotated, double start, double end, double step, double crop);
