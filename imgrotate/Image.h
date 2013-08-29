#pragma once

#include <vector>
#include <cstdint>

class Image {
public:
	typedef uint16_t PixelValue;
	typedef std::pair<double, double> PointF;

	virtual PixelValue operator()(const PointF& pt) const = 0;

	virtual size_t width() const = 0;
	virtual size_t height() const = 0;

	static const PixelValue BAD_VALUE = (PixelValue)-1;
};

class RealImage final : public Image {
public:
	RealImage(const std::vector<std::vector<PixelValue>>& data);

	PixelValue operator()(const PointF& pt) const override;
	
	size_t width() const override;
	size_t height() const override;
private:
	std::vector<std::vector<PixelValue>> data;
};

class ImageRotateView final : public Image {
public:
	ImageRotateView(const Image& img, double angle);

	PixelValue operator()(const PointF& pt) const override;

	size_t width() const override;
	size_t height() const override;

private:
	const Image& orig;
	double angle;

	PointF rotate(const PointF& pt) const;
};
