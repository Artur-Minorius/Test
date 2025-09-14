#include "Shape.h"
namespace Shapes {
	int Shape::GetAlpha() {
		return isHidden ? 0 : 255;
	}
	void Shape::SetZIndex(int newZIndex) {
		this->zIndex = newZIndex;
	}
	int Shape::GetZIndex()
	{
		return zIndex;
	}

	void Shape::Hide()
	{
		isHidden = true;
	}
	void Shape::Show()
	{
		isHidden = false;
	}

	void Shape::Move(Point mousePosition)
	{
		if (selectedControlPointIndex < 0)
			return;
		auto pts = GatherControlPoints();
		Point delta = mousePosition - pts[selectedControlPointIndex];
		origin = origin + delta;
		for (int i = 0; i < points.size(); i++)
			points[i] = points[i] + delta;
	}
	void Shape::Rotate(Point mousePosition)
	{
		Point center = GetGeometricalCenter();
		if (selectedControlPointIndex < 0)
			return;
		Point delta = mousePosition - center;
		//Delta is a vector (technically) and angle between line or vector and OX is tan of this line/vector https://en.wikipedia.org/wiki/Atan2
		double currentAngle = Point::Angle(delta);
		/*if (currentAngle > 180) currentAngle -= 360;
		if (currentAngle < -180) currentAngle += 360;*/
		rotation = currentAngle;
	}
	void Shape::Scale(Point mousePosition)
	{
		Point center = GetGeometricalCenter();
		if (selectedControlPointIndex < 0)
			return;
		auto pts = GatherControlPoints();
		auto selectedPoint = pts[selectedControlPointIndex];
		Point oldVec = selectedPoint.Rotate(center, -rotation) - center;
		Point newVec = mousePosition.Rotate(center, -rotation) - center;

		if (std::abs(oldVec.x) > 1e-6)
			scaleX *= (newVec.x / oldVec.x);
		else
			scaleX += 1e-4; //Close to zero imperfection

		if (std::abs(oldVec.y) > 1e-6)
			scaleY *= (newVec.y / oldVec.y);
		else
			scaleY += 1e-4; //Close to zero imperfection
	}
	void Shape::SetPosition(Shapes::Point newPosition)
	{
		Point center = GetGeometricalCenter();
		Point delta = newPosition - center;
		origin = origin + delta;
		for (int i = 0; i < points.size(); i++)
			points[i] = points[i] + delta;
	}
	void Shape::ChangePosition(Point delta)
	{
		origin = origin + delta;
		for (int i = 0; i < points.size(); i++)
			points[i] = points[i] + delta;
	}
	Shapes::Point* Shape::GetPosition()
	{
		Point center = GetGeometricalCenter();
		return new Shapes::Point(center.x, center.y);
	}

	double Shape::GetRotation()
	{
		return rotation;
	}

	void Shape::SetRotation(double newRotation)
	{
		rotation = newRotation;
	}

	Point Shape::GetScale()
	{
		return Point(scaleX, scaleY);
	}

	void Shape::SetScale(Point newScale)
	{
		scaleX = newScale.x;
		scaleY = newScale.y;
	}

	void Shape::UpdatePreviewPoint(int x, int y)
	{
		previewPoint = new Point(x, y);
	}

	bool Shape::AddPoint(int ex, int ey) {
		if (points.size() >= pointsNeeded)
			return true;
		points.push_back(Point(ex, ey));
		if (points.size() >= pointsNeeded)
		{
			isFilled = true;
			previewPoint = nullptr;
		}
		return points.size() >= pointsNeeded;
	}

	void Shape::PlacePoints(Gdiplus::Graphics& g) {
		Gdiplus::SolidBrush controlBrush(Gdiplus::Color(255, 0, 0, 0));
		if (previewPoint)
			g.FillEllipse(&controlBrush, previewPoint->x - controlPointRadius, previewPoint->y - controlPointRadius, 2 * controlPointRadius, 2 * controlPointRadius);
		else
		{
			auto gC = GetGeometricalCenter();
			g.FillEllipse(&controlBrush, gC.x - controlPointRadius, gC.y - controlPointRadius, 2 * controlPointRadius, 2 * controlPointRadius);
		}
		List<Point> pts = GatherPoints();
		pts.ForEach([&](Point p) {g.FillEllipse(&controlBrush, p.x - controlPointRadius, p.y - controlPointRadius, 2 * controlPointRadius, 2 * controlPointRadius); });
	}

	void Shape::DrawOutline(Gdiplus::Graphics& g)
	{
		List<Point> pts = GatherPoints();
		Point center = origin;
		if (isFilled)
			center = GetGeometricalCenter();
		pen.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		pen.SetDashStyle(Gdiplus::DashStyleDash);
		pen.SetWidth(3);
		g.DrawPolygon(&pen, pts.ToArray<Gdiplus::PointF>(), pts.size());
		PlacePoints(g);
	}

	void Shape::SetSelectedPointIndex(int index)
	{
		selectedControlPointIndex = index;
	}

	int Shape::GetSelectedPointIndex()
	{
		return selectedControlPointIndex;
	}

	Point Shape::GetGeometricalCenter()
	{
		//Centroid of a closed polygon https://en.wikipedia.org/wiki/Centroid
		auto points = GatherPurePoints();
		int n = points.size();
		if (n < 3) {
			// Degenerate case: just average vertices
			double sx = 0, sy = 0;
			for (int i = 0; i < n; i++) {
				sx += points[i].x;
				sy += points[i].y;
			}
			return Shapes::Point(sx / n, sy / n);
		}

		double area = 0.0;
		double cx = 0.0, cy = 0.0;

		for (int i = 0; i < n; i++) {
			const Shapes::Point& p0 = points[i];
			const Shapes::Point& p1 = points[(i + 1) % n];

			double cross = p0.x * p1.y - p1.x * p0.y;
			area += cross;
			cx += (p0.x + p1.x) * cross;
			cy += (p0.y + p1.y) * cross;
		}

		area *= 0.5;
		//Computational error fallback
		if (std::abs(area) < 1e-9) {
			double sx = 0, sy = 0;
			for (int i = 0; i < n; i++) {
				sx += points[i].x;
				sy += points[i].y;
			}
			return Shapes::Point(sx / n, sy / n);
		}

		cx /= (6.0 * area);
		cy /= (6.0 * area);

		return Shapes::Point(cx, cy);
	}

	List<Point> Shape::GatherPoints()
	{
		List<Point> allPoints(points.size() + 1);
		Point center = GetGeometricalCenter();
		auto applyTransform = [&](Point p) {
			double sx = (p.x - center.x) * scaleX;
			double sy = (p.y - center.y) * scaleY;
			Point scaled(center.x + sx, center.y + sy);
			return scaled.Rotate(center, rotation);
			};
		allPoints.push_back(applyTransform(origin));
		points.ForEach([&](Point p) { allPoints.push_back(applyTransform(p)); });

		return allPoints;
	}
	List<Point> Shape::GatherControlPoints()
	{
		List<Point> allPoints = GatherPoints();
		allPoints.push_back(GetGeometricalCenter());
		return allPoints;
	}
	List<Point> Shape::GatherPurePoints()
	{
		List<Point> allPoints(points.size() + 1);
		allPoints.push_back(origin);
		points.ForEach([&](Point p) {allPoints.push_back(p); });
		return allPoints;
	}
}