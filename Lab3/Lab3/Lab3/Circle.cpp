#include "Circle.h"
namespace Shapes {
	Circle::Circle(Point start, COLORREF Color) :Shape(start, Color) { pointsNeeded = 1; }
	Circle::Circle(Point start, Point end, COLORREF Color) : Shape(start, Color) {
		pointsNeeded = 1;
		points.push_back(end);
	}


	void Circle::Draw(Gdiplus::Graphics& g) {
		brush.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		List<Point> pts = GatherPoints();
		Point end = pts[1];
		int radius = Point::Distance(origin, end);

		int left = origin.x - radius;
		int top = origin.y - radius;
		int width = radius * 2;
		int height = radius * 2;
		g.FillEllipse(&brush, left, top, width, height);
		if (isOutlineAlwaysVisible)
			DrawOutline(g);
	}
	void Circle::DrawHidden(Gdiplus::Graphics& g)
	{
		pen.SetColor(Gdiplus::Color(255, GetRValue(color), GetGValue(color), GetBValue(color)));
		List<Point> pts = GatherPoints();
		Point end = pts[1];
		int radius = Point::Distance(origin, end);

		int left = origin.x - radius;
		int top = origin.y - radius;
		int width = radius * 2;
		int height = radius * 2;
		g.DrawEllipse(&pen, left, top, width, height);
	}
	void Circle::DrawPreview(Gdiplus::Graphics& g)
	{
		pen.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		pen.SetDashStyle(Gdiplus::DashStyleDash);
		if(previewPoint)
		{
			int radius = Point::Distance(origin, *previewPoint);

			int left = origin.x - radius;
			int top = origin.y - radius;
			int width = radius * 2;
			int height = radius * 2;
			g.DrawEllipse(&pen, left, top, width, height);
		}
		// For control
		PlacePoints(g);
	}
	void Circle::DrawOutline(Gdiplus::Graphics& g) {
		List<Point> pts = GatherPoints();
		Point end = pts[1];
		pen.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		pen.SetDashStyle(Gdiplus::DashStyleDash);
		pen.SetWidth(3);
		int radius = Point::Distance(origin, end);
		int left = origin.x - radius;
		int top = origin.y - radius;
		int width = radius * 2;
		int height = radius * 2;
		g.DrawEllipse(&pen, left, top, width, height);
		PlacePoints(g);
	}
	bool Circle::HitTest(Point localPos) {
		List<Point> pts = GatherPoints();
		Point end = pts[1];
		return Point::Distance(origin, end) >= Point::Distance(origin, localPos);
	}
	Point Circle::GetGeometricalCenter()
	{
		return origin;
	}
	Shape* Circle::Clone()
	{
		Shape* newShape = new Circle(origin, points[0], color);
		newShape->SetRotation(rotation);
		newShape->SetScale(Point(scaleX, scaleY));
		newShape->isHidden = isHidden;
		return newShape;
	}
}

