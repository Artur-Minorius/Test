#include "Line.h"

namespace Shapes {
	Line::Line(Point start, Point end, COLORREF Color) : Shape(start, Color) {
		pointsNeeded = 1;
		points.push_back(end);
	}
	Line::Line(Point start, COLORREF Color) : Shape(start, Color) { pointsNeeded = 1; }

	void Line::Draw(Gdiplus::Graphics& g) {
		pen.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		List<Point> pts = GatherPoints();
		Point origin = pts[0];
		Point end = pts[1];
		g.DrawLine(&pen, (Gdiplus::PointF)origin, (Gdiplus::PointF)end);
		if (isOutlineAlwaysVisible)
			DrawOutline(g);
	}
	void Line::DrawHidden(Gdiplus::Graphics& g)
	{
		pen.SetColor(Gdiplus::Color(255, GetRValue(color), GetGValue(color), GetBValue(color)));
		List<Point> pts = GatherPoints();
		Point origin = pts[0];
		Point end = pts[1];
		g.DrawLine(&pen, (Gdiplus::PointF)origin, (Gdiplus::PointF)end);
	}
	void Line::DrawPreview(Gdiplus::Graphics& g)
	{
		pen.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		pen.SetDashStyle(Gdiplus::DashStyleDash);
		if(previewPoint)
			g.DrawLine(&pen, (Gdiplus::PointF)origin, (Gdiplus::PointF)*previewPoint);

		// For control
		PlacePoints(g);
	}
	bool Line::HitTest(Point localPos)
	{
		Point end = points[0];
		int threshold = 3;
		float lineLength = (float)Point::Distance(origin, end); //https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
		float distance = abs((end.y - origin.y) * localPos.x - (end.x - origin.x) * localPos.y + end.x * origin.y - end.y * origin.x) / lineLength;
		return distance <= threshold;
	}
	void Line::DrawOutline(Gdiplus::Graphics& g)
	{
		PlacePoints(g);
	}
	Shape* Line::Clone()
	{
		Shape* newShape = new Line(origin, points[0], color);
		newShape->SetRotation(rotation);
		newShape->SetScale(Point(scaleX, scaleY));
		newShape->isHidden = isHidden;
		return newShape;
	}
}