#pragma once
#include "Windows.h"
#include "gdiplus.h"

namespace Shapes {
	class Point
	{
	public:
		double x, y;
		Point() : x(0), y(0) {};
		Point(double x, double y) : x(x), y(y) {};
		explicit operator Gdiplus::PointF() const { return Gdiplus::PointF((Gdiplus::REAL)x, (Gdiplus::REAL)y); }
		Point operator+(const Point&);
		Point operator-(const Point&);
		Point operator*(double c);
		static double Distance(const Point&, const Point& = Point(0, 0));
		static Point* Rotate(const Point& origin, const Point&, double);
		Point Rotate(const Point&, double);
		static double Angle(const Point& p);
	};
}

