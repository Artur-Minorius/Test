#include "Rectangle.h"
namespace Shapes {
	Rectangle::Rectangle(Point start, Point end, COLORREF Color) : Shape(start, Color) {
		pointsNeeded = 3;
		//Origin set in constructor of Shape
		//First corner
		points.push_back(Point(end.x, start.y));
		//Second corner
		points.push_back(end);
		//Third corner
		points.push_back(Point(start.x, end.y));
	}
	Rectangle::Rectangle(Point start, COLORREF Color) : Shape(start, Color) {
		pointsNeeded = 3;
	}

	void Rectangle::Draw(Gdiplus::Graphics& g) {
		brush.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		g.FillPolygon(&brush, GatherPoints().ToArray<Gdiplus::PointF>(), points.size() + 1);
		if (isOutlineAlwaysVisible)
			DrawOutline(g);
	}
	void Rectangle::DrawHidden(Gdiplus::Graphics& g)
	{
		pen.SetColor(Gdiplus::Color(255, GetRValue(color), GetGValue(color), GetBValue(color)));
		g.DrawPolygon(&pen, GatherPoints().ToArray<Gdiplus::PointF>(), points.size() + 1);
	}
	void Rectangle::DrawPreview(Gdiplus::Graphics& g)
	{
		pen.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		pen.SetDashStyle(Gdiplus::DashStyleDash);
		if(previewPoint)
		{
			List<Point> tempPoints(4);
			tempPoints.push_back(origin);
			//First corner
			tempPoints.push_back(Point(previewPoint->x, origin.y));
			//Second corner
			tempPoints.push_back(Point(previewPoint->x, previewPoint->y));
			//Third corner
			tempPoints.push_back(Point(origin.x, previewPoint->y));
			g.DrawPolygon(&pen, tempPoints.ToArray<Gdiplus::PointF>(), tempPoints.size());
		}
		// For control
		PlacePoints(g);
	}
	bool Rectangle::AddPoint(int ex, int ey)
	{
		if (points.size() >= pointsNeeded)
			return true;
		//First corner
		points.push_back(Point(ex, origin.y));
		//Second corner
		points.push_back(Point(ex, ey));
		//Third corner
		points.push_back(Point(origin.x, ey));
		if (points.size() >= pointsNeeded)
		{
			previewPoint = nullptr;
			isFilled = true;
		}
		return true;
	}
	bool Rectangle::HitTest(Point localPos)
	{
		List<Point> poly = GatherPoints();
		int n = (int)poly.size();
		auto pt = poly.ToArray<Gdiplus::PointF>();
		bool inside = false;
		for (int i = 0, j = n - 1; i < n; j = i++) {
			float xi = pt[i].X, yi = pt[i].Y;
			float xj = pt[j].X, yj = pt[j].Y;
			bool intersect = ((yi > localPos.y) != (yj > localPos.y)) &&
				(localPos.x < (xj - xi) * (localPos.y - yi) / (yj - yi + 1e-9f) + xi); // line function got from two points (1e-9f is for vertical line)
			if (intersect) inside = !inside; // Intersect once - inside, twice - outside
		}
		delete[] pt;
		return inside;
	}
	Shape* Rectangle::Clone()
	{
		Shape* newShape = new Rectangle(origin, points[1], color);
		newShape->SetRotation(rotation);
		newShape->SetScale(Point(scaleX, scaleY));
		newShape->isHidden = isHidden;
		return newShape;
	}
}