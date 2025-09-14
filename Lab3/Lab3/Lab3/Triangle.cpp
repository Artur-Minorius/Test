#include "Triangle.h"
namespace Shapes {
	Triangle::Triangle(Point start, Point secondPoint, Point thirdPoint, COLORREF Color) : Shape(start, Color)
	{
		pointsNeeded = 2;
		points.push_back(secondPoint);
		points.push_back(thirdPoint);
	}

	Triangle::Triangle(Point start, COLORREF Color) : Shape(start, Color) {
		pointsNeeded = 2;
	}

	void Triangle::Draw(Gdiplus::Graphics& g) {
		brush.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		g.FillPolygon(&brush, GatherPoints().ToArray<Gdiplus::PointF>(), points.size() + 1);
		if (isOutlineAlwaysVisible)
			DrawOutline(g);
	}
	void Triangle::DrawHidden(Gdiplus::Graphics& g)
	{
		pen.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		g.DrawPolygon(&pen, GatherPoints().ToArray<Gdiplus::PointF>(), points.size() + 1);
	}
	void Triangle::DrawPreview(Gdiplus::Graphics& g)
	{
		pen.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		pen.SetDashStyle(Gdiplus::DashStyleDash);
		if (previewPoint)
		{
			List<Point> tempPoints(points.size() + 1 + 1);
			tempPoints.push_back(origin);
			points.ForEach([&](Point p) {tempPoints.push_back(p); });
			tempPoints.push_back(*previewPoint);
			g.DrawPolygon(&pen, tempPoints.ToArray<Gdiplus::PointF>(), tempPoints.size());
		}
		// For control
		PlacePoints(g);
	}
	bool Triangle::HitTest(Point localPos)
	{
		List<Point> poly = GatherPoints();
		int n = (int)poly.size();
		auto pt = poly.ToArray<Gdiplus::PointF>();
		bool inside = false;
		for (int i = 0, j = n - 1; i < n; j = i++) {
			float xi = pt[i].X, yi = pt[i].Y;
			float xj = pt[j].X, yj = pt[j].Y;
			bool intersect = ((yi > localPos.y) != (yj > localPos.y)) &&
				(localPos.x < (xj - xi) * (localPos.y - yi) / (yj - yi + 1e-9f) + xi);
			if (intersect) inside = !inside;
		}
		delete[] pt;
		return inside;
	}
	Shape* Triangle::Clone()
	{
		Shape* newShape = new Triangle(origin, points[0], points[1], color);
		newShape->SetRotation(rotation);
		newShape->SetScale(Point(scaleX, scaleY));
		newShape->isHidden = isHidden;
		return newShape;
	}
}