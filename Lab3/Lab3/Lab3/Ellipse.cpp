#include "Ellipse.h"

namespace Shapes {
	Ellipse::Ellipse(Point start, COLORREF Color):Shape(start, Color){
		pointsNeeded = 1;
	}
	Ellipse::Ellipse(Point start, Point end, COLORREF Color) : Shape(start, Color) {
		pointsNeeded = 1;
		points.push_back(end);
	}

	void Ellipse::Draw(Gdiplus::Graphics& g) {
		brush.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		List<Point> pts = GatherPoints();
		Point origin = pts[0];
		Point end = pts[1];
		int left = min(origin.x, end.x);
		int top = min(origin.y, end.y);
		int width = abs(end.x - origin.x);
		int height = abs(end.y - origin.y);
		g.FillEllipse(&brush, left, top, width, height);
		if (isOutlineAlwaysVisible)
			DrawOutline(g);
	}
	void Ellipse::DrawHidden(Gdiplus::Graphics& g)
	{
		pen.SetColor(Gdiplus::Color(255, GetRValue(color), GetGValue(color), GetBValue(color)));
		List<Point> pts = GatherPoints();
		Point origin = pts[0];
		Point end = pts[1];
		int left = min(origin.x, end.x);
		int top = min(origin.y, end.y);
		int width = abs(end.x - origin.x);
		int height = abs(end.y - origin.y);
		g.DrawEllipse(&pen, left, top, width, height);
	}
	void Ellipse::DrawPreview(Gdiplus::Graphics& g)
	{
		pen.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		pen.SetDashStyle(Gdiplus::DashStyleDash);
		if (previewPoint) {
			int left = min(origin.x, previewPoint->x);
			int top = min(origin.y, previewPoint->y);
			int width = abs(previewPoint->x - origin.x);
			int height = abs(previewPoint->y - origin.y);
			g.DrawEllipse(&pen, left, top, width, height);
		}
		// For control
		PlacePoints(g);
	}
	bool Ellipse::HitTest(Point localPos)
	{
		List<Point> pts = GatherPoints();
		Point origin = pts[0];
		Point end = pts[1];
		Point ellipseCenter((origin.x + end.x) / 2, (origin.y + end.y) / 2);
		Point ellipseDiagonals(abs(origin.x - end.x) / 2, abs(origin.y - end.y) / 2);
		float p = (pow((localPos.x - ellipseCenter.x), 2) / pow(ellipseDiagonals.x, 2))
			+ (pow((localPos.y - ellipseCenter.y), 2) / pow(ellipseDiagonals.y, 2));

		return p <= 1;
	}
	void Ellipse::DrawOutline(Gdiplus::Graphics& g) {
		List<Point> pts = GatherPoints();
		Point origin = pts[0];
		Point end = pts[1];
		pen.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		pen.SetDashStyle(Gdiplus::DashStyleDash);
		pen.SetWidth(3);
		int left = min(origin.x, end.x);
		int top = min(origin.y, end.y);
		int width = abs(end.x - origin.x);
		int height = abs(end.y - origin.y);
		g.DrawEllipse(&pen, left, top, width, height);
		PlacePoints(g);
	}
	Shape* Ellipse::Clone()
	{
		Shape* newShape = new Ellipse(origin, points[0], color);
		newShape->SetRotation(rotation);
		newShape->SetScale(Point(scaleX, scaleY));
		newShape->isHidden = isHidden;
		return newShape;
	}
}

