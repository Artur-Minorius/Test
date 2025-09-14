#define _USE_MATH_DEFINES // M_PI

#include "Point.h"
#include <cmath>
namespace Shapes {
	Point Shapes::Point::operator+(const Point& point)
	{
		return Point(this->x + point.x, this->y + point.y);
	}
	Point Shapes::Point::operator-(const Point& point)
	{
		return Point(this->x - point.x, this->y - point.y);
	}

	Point Point::operator*(double c)
	{
		return Point(c * this->x, c * this->y);
	}

	double Point::Distance(const Point& p1, const Point& p2)
	{
		return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	}

	Point* Point::Rotate(const Point& origin, const Point& point, double angle)
	{
		double rad = angle * M_PI / 180.0;
		double s = sin(rad);
		double c = cos(rad);

		double x = point.x - origin.x;
		double y = point.y - origin.y;

		double xNew = x * c - y * s;
		double yNew = x * s + y * c;

		return new Point(xNew + origin.x, yNew + origin.y);
	}

	Point Point::Rotate(const Point& origin, double angle)
	{
		double rad = angle * M_PI / 180.0;
		double s = sin(rad);
		double c = cos(rad);

		double x = this->x - origin.x;
		double y = this->y - origin.y;

		double xNew = x * c - y * s;
		double yNew = x * s + y * c;
		return Point(xNew + origin.x, yNew + origin.y);
	}

	double Point::Angle(const Point& p)
	{
		double angleRad = std::atan2(p.y, p.x);
		double angleDeg = angleRad * (180.0 / M_PI);

		return angleDeg;
	}

}
