#include "Animation.h"
namespace Shapes {
	Animation::Animation(Shapes::Shape* shape, double duration, double speed, COLORREF color) :
		shape(shape), speed(speed), duration(duration),
		Shape(shape->GetPosition(), color)
	{
		AddPoint(shape->GetGeometricalCenter()); //Start of every animation is it's geometrical center
	}

	void Animation::AddPoint(Shapes::Point p) {
		points.push_back(p);
	}
	void Animation::Start() {
		if (points.size() < 2) return;
		playing = true;
		currentSegment = 0;
		t = 0.0;
		lastUpdate = GetTickCount64();
		shape->SetPosition(points[0]);
	}
	void Animation::Update() {
		if (!playing || currentSegment >= points.size() - 1)
			return;

		ULONGLONG now = GetTickCount64();
		double deltaTime = (now - lastUpdate) / 1000.0;
		lastUpdate = now;

		Shapes::Point p0 = points[currentSegment];
		Shapes::Point p1 = points[currentSegment + 1];

		int dist = Shapes::Point::Distance(p0, p1);
		double segmentDuration = dist / speed;

		t += deltaTime / segmentDuration;

		if (t >= 1.0) {
			//move to next segment
			shape->SetPosition(p1);
			currentSegment++;
			t = 0.0;
			if (currentSegment >= points.size() - 1)
			{
				playing = false; //finished
				isEnded = true;
			}
		}
		else {
			//interpolate position
			//This in line a "segment" so position is defined by (end - start) * progress (point on a segment) and + start
			Shapes::Point pos = p0 + (p1 - p0) * t;
			shape->SetPosition(pos);
		}
	}
	bool Animation::IsPlaying() { return playing; }

	void Animation::Draw(Gdiplus::Graphics& g)
	{
		pen.SetColor(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
		pen.SetWidth(2);
		for (int i = 0; i < points.size() - 1; i++)
		{
			Shapes::Point p0 = points[i];
			Shapes::Point p1 = points[i + 1];
			g.DrawLine(&pen, (Gdiplus::PointF)p0, (Gdiplus::PointF)p1);
		}
		PlacePoints(g);
	}

	void Animation::DrawPreview(Gdiplus::Graphics& g)
	{
		pen.SetColor(Gdiplus::Color(255, GetRValue(color), GetGValue(color), GetBValue(color)));
		pen.SetDashStyle(Gdiplus::DashStyleDash);
		pen.SetWidth(2);
		for (int i = 0; i < points.size() - 1; i++)
		{
			Shapes::Point p0 = points[i];
			Shapes::Point p1 = points[i + 1];
			g.DrawLine(&pen, (Gdiplus::PointF)p0, (Gdiplus::PointF)p1);
		}
		if (previewPoint)
			g.DrawLine(&pen, (Gdiplus::PointF)points[points.size() - 1], (Gdiplus::PointF)*previewPoint);
	}

	bool Animation::HitTest(Shapes::Point localPos)
	{
		if (!playing || currentSegment >= points.size() - 1)
			return false;
		int threshold = 3;
		for (int i = 0; i < points.size() - 1; i++)
		{
			Shapes::Point p0 = points[i];
			Shapes::Point p1 = points[i + 1];
			float lineLength = (float)Shapes::Point::Distance(p0, p1); //https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line

			float distance = abs((p1.y - p0.y) * localPos.x - (p1.x - p0.x) * localPos.y + p1.x * p0.y - p1.y * p0.x) / lineLength;
			if (distance <= threshold)
				return true;
		}

		return false;
	}

	void Animation::PlacePoints(Gdiplus::Graphics& g)
	{
		Gdiplus::SolidBrush controlBrush(Gdiplus::Color(255, 0, 0, 0));
		if (previewPoint)
			g.FillEllipse(&controlBrush, previewPoint->x - controlPointRadius, previewPoint->y - controlPointRadius, 2 * controlPointRadius, 2 * controlPointRadius);
		points.ForEach([&](Shapes::Point p) {
			g.FillEllipse(&controlBrush, p.x - controlPointRadius, p.y - controlPointRadius, 2 * controlPointRadius, 2 * controlPointRadius);
			});
	}

	void Animation::SetSelectedPointIndex(int index)
	{
		selectedControlPointIndex = index;
	}

	int Animation::GetSelectedPointIndex()
	{
		return selectedControlPointIndex;
	}

	void Animation::EndCreating()
	{
		previewPoint = nullptr;
	}
	bool Animation::IsEnded()
	{
		return isEnded;
	}
	List<Shapes::Point> Animation::GetPoints()
	{
		return points;
	}

	void Animation::UpdatePreviewPoint(int x, int y)
	{
		delete previewPoint;
		previewPoint = new Shapes::Point(x, y);
	}

}